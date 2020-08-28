#include "SequentialHttpRequestSet.h"
#include "HttpRequest.h"
#include "Network.h"

namespace Network
{
	void FSequentialHttpRequestSet::ProcessRequest()
	{
		if (GetRequestState() == ERequestState::ERequestState_WaitingForProcess)
		{
			if (M_Requests.Num() > 0)
			{
				M_CurrentRequest = 0;
				M_RequestState = ERequestState::ERequestState_Processing;
				M_Requests[0]->ProcessRequest();
			}
			else
			{
				UE_LOG(LogNetwork, Warning, TEXT("FSequentialHttpRequestSet: Trying to process an empty request set."));
			}
		}
		else
		{
			UE_LOG(LogNetwork, Warning, TEXT("FSequentialHttpRequestSet: Trying to process a request set that is not waiting for process."));
		}
	}

	void FSequentialHttpRequestSet::CancelRequest()
	{
		if (M_Requests.IsValidIndex(M_CurrentRequest))
		{
			M_RequestState = ERequestState::ERequestState_Canceled;
			M_Requests[M_CurrentRequest]->CancelRequest();
			RemoveFromList();
		}
	}

	void FSequentialHttpRequestSet::AddRequest(const TSharedRef<FHttpRequestBase>& InRequest)
	{
		if (GetRequestState() != ERequestState::ERequestState_WaitingForProcess)
		{
			UE_LOG(LogNetwork, Warning, TEXT("FSequentialHttpRequestSet: Request can only be added to a request set that is waiting for process."));
			return;
		}
		if (InRequest->GetRequestState() != ERequestState::ERequestState_WaitingForProcess)
		{
			UE_LOG(LogNetwork, Warning, TEXT("FSequentialHttpRequestSet: Can not add a request that is not waiting for process."));
			return;
		}

		ERequestType RequestType = InRequest->GetRequestType();
		if (IsNormalRequest(RequestType))
		{
			TSharedRef<FHttpRequest> NormalRequest = StaticCastSharedRef<FHttpRequest>(InRequest);
			auto FinishedDelegate = NormalRequest->GetRequestFinished();
			auto FailedDelegate = NormalRequest->GetRequestFailed();
			NormalRequest->SetOnRequestFinished(
				FHttpRequestFinishedDelegate::CreateLambda(
					[this, FinishedDelegate](FHttpRequestPtr Request, FHttpResponsePtr Response) {
				FinishedDelegate.ExecuteIfBound(Request, Response);
				SubRequestFinished();
			}
				)
			);
			NormalRequest->SetOnRequestFailed(
				FHttpRequestFailedDelegate::CreateLambda(
					[this, FailedDelegate](int32 RetCode) {
				FailedDelegate.ExecuteIfBound(RetCode);
				SubRequestFailed();
			}
				)
			);
			M_Requests.Emplace(InRequest);
		}
		else if (IsSetRequest(RequestType))
		{
			TSharedRef<FHttpRequestSetBase> RequestSet = StaticCastSharedRef<FHttpRequestSetBase>(InRequest);
			auto FinishedDelegate = RequestSet->GetRequestFinished();
			auto FailedDelegate = RequestSet->GetRequestFailed();
			RequestSet->SetOnRequestFinished(
				FSimpleDelegate::CreateLambda(
					[this, FinishedDelegate]() {
				FinishedDelegate.ExecuteIfBound();
				SubRequestFinished();
			}
				)
			);
			RequestSet->SetOnRequestFailed(
				FSimpleDelegate::CreateLambda(
					[this, FailedDelegate]() {
				FailedDelegate.ExecuteIfBound();
				SubRequestFailed();
			}
				)
			);
			M_Requests.Emplace(InRequest);
		}
	}

	void FSequentialHttpRequestSet::SubRequestFinished()
	{
		++M_CurrentRequest;
		if (M_CurrentRequest == M_Requests.Num()) // 所有的请求都已被处理
		{
			M_RequestState = ERequestState::ERequestState_Finished;
			M_FinishedDelegate.ExecuteIfBound();
			RemoveFromList();
		}
		else // 处理下一个请求
			M_Requests[M_CurrentRequest]->ProcessRequest();
	}

	void FSequentialHttpRequestSet::SubRequestFailed()
	{
		if (M_ProcessType == DonotAllowFailure)
		{
			M_RequestState = ERequestState::ERequestState_Failed;
			M_FailedDelegate.ExecuteIfBound();
			RemoveFromList();
			UE_LOG(LogNetwork, Display, TEXT("FSequentialHttpRequestSet: Request set failed."));
		}
		else
		{
			SubRequestFinished();
		}
	}
}
