#include "ParallelHttpRequestSet.h"
#include "HttpRequest.h"
#include "Network.h"
#include "HAL/PlatformTime.h"
#include "HAL/PlatformProcess.h"
#include "Engine/Engine.h"

namespace Network
{
	void FParallelHttpRequestSet::ProcessRequest()
	{
		if (GetRequestState() == ERequestState::ERequestState_WaitingForProcess)
		{
			if (M_Requests.Num() > 0)
			{
				M_RequestCounter = M_Requests.Num();
				M_RequestState = ERequestState::ERequestState_Processing;
				for (const auto& Request : M_Requests)
					Request->ProcessRequest();
				if (M_Blocking)
				{
					// 阻塞调用线程直到所有的请求处理完毕
					double LastRealTime, CurrentRealTime;
					LastRealTime = FPlatformTime::Seconds();
					do {
						// 让调用线程休眠500ms
						FPlatformProcess::Sleep(0.5f);
						CurrentRealTime = FPlatformTime::Seconds();
						// 线程重新唤起后进行HttpManager的Tick，检查请求完成情况
						FHttpModule::Get().GetHttpManager().Tick(CurrentRealTime - LastRealTime);
						LastRealTime = CurrentRealTime;
					} while (M_RequestCounter > 0);
					M_RequestState = ERequestState::ERequestState_Finished;
					M_FinishedDelegate.ExecuteIfBound();
					// 请求执行完毕，将自己从列表中移除，引用计数完全由调用线程来维护，如果此请求在调用线程
					// 作用域结束，则会被销毁
					RemoveFromList();
				}
				else
				{
					// 非阻塞，创建一个TickableGameObject
					M_Tickable = MakeShareable(new FZYParallelHttpRequestSetTickable(StaticCastSharedRef<FParallelHttpRequestSet>(AsShared())));
				}
			}
			else
			{
				UE_LOG(LogNetwork, Warning, TEXT("FParallelHttpRequestSet: Trying to process an empty request set."));
			}
		}
		else
		{
			UE_LOG(LogNetwork, Warning, TEXT("FParallelHttpRequestSet: Trying to process a request set that is not waiting for process."));
		}
	}

	void FParallelHttpRequestSet::CancelRequest()
	{
		if (M_Requests.Num() > 0)
		{
			M_RequestState = ERequestState::ERequestState_Canceled;
			for (const auto& Request : M_Requests)
				Request->CancelRequest();
			RemoveFromList();
		}
	}

	void FParallelHttpRequestSet::AddRequest(const TSharedRef<FHttpRequestBase>& InRequest)
	{
		if (GetRequestState() != ERequestState::ERequestState_WaitingForProcess)
		{
			UE_LOG(LogNetwork, Warning, TEXT("FParallelHttpRequestSet: Request can only be added to a request set that is waiting for process."));
			return;
		}
		if (InRequest->GetRequestState() != ERequestState::ERequestState_WaitingForProcess)
		{
			UE_LOG(LogNetwork, Warning, TEXT("FSequentialHttpRequestSet: Can not add a request that is not waiting for process."));
			return;
		}

		ERequestType RequestType = GetRequestType();
		if (IsNormalRequest(RequestType))
		{
			TSharedRef<FHttpRequest> NormalRequest = StaticCastSharedRef<FHttpRequest>(InRequest);
			auto FinishedDelegate = NormalRequest->GetRequestFinished();
			auto FailedDelegate = NormalRequest->GetRequestFailed();
			NormalRequest->SetOnRequestFinished(
				FHttpRequestFinishedDelegate::CreateLambda(
					[this, FinishedDelegate](FHttpRequestPtr Request, FHttpResponsePtr Response) {
				FinishedDelegate.ExecuteIfBound(Request, Response);
				// 完成了一个请求，将请求计数减1
				--M_RequestCounter;
			}
				)
			);
			NormalRequest->SetOnRequestFailed(
				FHttpRequestFailedDelegate::CreateLambda(
					[this, FailedDelegate](int32 RetCode) {
				FailedDelegate.ExecuteIfBound(RetCode);
				// 完成了一个请求，将请求计数减1
				--M_RequestCounter;
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
				// 完成了一个请求，将请求计数减1
				--M_RequestCounter;
			}
				)
			);
			RequestSet->SetOnRequestFailed(
				FSimpleDelegate::CreateLambda(
					[this, FailedDelegate]() {
				FailedDelegate.ExecuteIfBound();
				// 完成了一个请求，将请求计数减1
				--M_RequestCounter;
			}
				)
			);
			M_Requests.Emplace(InRequest);
		}
	}

	void FParallelHttpRequestSet::NotifyNonBlockingRequestSetFinished()
	{
		M_FinishedDelegate.ExecuteIfBound();
		RemoveFromList();
	}

	void FParallelHttpRequestSet::FZYParallelHttpRequestSetTickable::Tick(float DeltaTime)
	{
		auto RequestSetPtr = M_RequestSet.Pin();
		if (RequestSetPtr->M_RequestCounter == 0)
			RequestSetPtr->NotifyNonBlockingRequestSetFinished();
	}

	bool FParallelHttpRequestSet::FZYParallelHttpRequestSetTickable::IsTickable() const
	{
		auto RequestSetPtr = M_RequestSet.Pin();
		return !RequestSetPtr->M_Blocking && RequestSetPtr->GetRequestState() == FHttpRequestBase::ERequestState_Processing;
	}

	TStatId FParallelHttpRequestSet::FZYParallelHttpRequestSetTickable::GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FZYParallelHttpRequestSetTickable, STATGROUP_Tickables);
	}

	UWorld* FParallelHttpRequestSet::FZYParallelHttpRequestSetTickable::GetTickableGameObjectWorld() const
	{
		const TIndirectArray<FWorldContext>& WorldList = GEngine->GetWorldContexts();
		if (WorldList.Num() > 0)
		{
			const FWorldContext** ppWorldContent = WorldList.GetData();
			for (int32 i = 0, iEnd = WorldList.Num(); i < iEnd; ++i, ++ppWorldContent)
			{
				if ((*ppWorldContent)->World()->ShouldTick())
					return (*ppWorldContent)->World();
			}
		}
		return nullptr;
	}
}
