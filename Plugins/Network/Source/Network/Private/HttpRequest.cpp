#include "HttpRequest.h"
#include "Network.h"

namespace Network
{
	FHttpRequest::FHttpRequest(const FString& InUrl)
	{
		TSharedRef<IHttpRequest> HttpRequestImpl = FHttpModule::Get().CreateRequest();
		HttpRequestImpl->SetURL(InUrl);
		M_RequestImpl = HttpRequestImpl;
	}

	void FHttpRequest::ProcessRequest()
	{
		if (GetRequestState() == ERequestState::ERequestState_WaitingForProcess)
		{
			auto RequestImpl = M_RequestImpl.Pin();
			RequestImpl->OnProcessRequestComplete().BindLambda(
				[this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful) {
				if (bSuccessful && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
				{
					M_RequestState = FHttpRequestBase::ERequestState_Finished;
					M_FinishedDelegate.ExecuteIfBound(Request, Response);
					UE_LOG(LogNetwork, Verbose, TEXT("FHttpRequest: Http request %s finished."), *(Request->GetURL()));
				}
				else if (Response.IsValid())
				{
					M_RequestState = FHttpRequestBase::ERequestState_Failed;
					M_FailedDelegate.ExecuteIfBound(Response->GetResponseCode());
					UE_LOG(LogNetwork, Display, TEXT("FHttpRequest: Http request %s failed: %d."), *(Request->GetURL()), Response->GetResponseCode());
				}
				else
				{
					M_RequestState = FHttpRequestBase::ERequestState_Failed;
					M_FailedDelegate.ExecuteIfBound(-1);
					UE_LOG(LogNetwork, Display, TEXT("FHttpRequest: Http request failed: No http reponse."));
				}
				RemoveFromList();
			}
			);
			M_RequestState = ERequestState::ERequestState_Processing;
			RequestImpl->ProcessRequest();
		}
		else
		{
			UE_LOG(LogNetwork, Warning, TEXT("FHttpRequest: Try to process a request that is not waiting for process."));
		}
	}

	void FHttpRequest::CancelRequest()
	{
		M_RequestState = ERequestState::ERequestState_Canceled;
		M_RequestImpl.Pin()->CancelRequest();
		RemoveFromList();
	}
}
