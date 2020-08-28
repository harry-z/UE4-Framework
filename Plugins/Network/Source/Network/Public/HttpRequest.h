#pragma once
#include "HttpRequestBase.h"

namespace Network
{
	// 请求完成代理
	DECLARE_DELEGATE_TwoParams(FHttpRequestFinishedDelegate, FHttpRequestPtr, FHttpResponsePtr)
	// 请求失败代理，代理回传错误码
	DECLARE_DELEGATE_OneParam(FHttpRequestFailedDelegate, int32)

	/*
		普通Http请求
	*/
	class NETWORK_API FHttpRequest : public FHttpRequestBase
	{
	public:
		virtual void ProcessRequest() override;
		virtual void CancelRequest() override;
		virtual ERequestType GetRequestType() const override { return ERequestType_Normal; }
		void SetOnRequestFinished(const FHttpRequestFinishedDelegate& InFinished) { M_FinishedDelegate = InFinished; }
		void SetOnRequestFailed(const FHttpRequestFailedDelegate& InFailed) { M_FailedDelegate = InFailed; }
		FHttpRequestFinishedDelegate& GetRequestFinished() { return M_FinishedDelegate; }
		FHttpRequestFailedDelegate& GetRequestFailed() { return M_FailedDelegate; }

		template <class HttpType>
		friend TSharedRef<FHttpRequest> CreateHttpRequest(const FString& InUrl, const HttpType& InHttpType);

	private:
		FHttpRequest(const FString& InUrl);
		FHttpRequest(const FHttpRequest& Other) = default;
		FHttpRequest(FHttpRequest&& Other) = default;
		FHttpRequest& operator= (const FHttpRequest& Other) = default;
		FHttpRequest& operator= (FHttpRequest&& Other) = default;

	protected:
		FHttpRequestFinishedDelegate M_FinishedDelegate;
		FHttpRequestFailedDelegate M_FailedDelegate;

		// IHttpRequest的引用计数维护在HTTP模块的FHttpManager里
		TWeakPtr<IHttpRequest> M_RequestImpl;
	};

	struct FGetJsonType
	{
		void SetupRequest(const TSharedPtr<IHttpRequest>& InRequest) {
			InRequest->SetVerb("GET");
			InRequest->SetHeader("Content-Type", "application/json; charset=utf-8");
		}
	};

	struct FPutJsonType
	{
		FPutJsonType(const FString& InJsonStr)
			: M_Content(&InJsonStr) {}
		void SetupRequest(const TSharedPtr<IHttpRequest>& InRequest) const {
			InRequest->SetVerb("PUT");
			InRequest->SetHeader("Content_Type", "application/json; charset=utf-8");
			InRequest->SetContentAsString(*M_Content);
		}
		const FString* M_Content;
	};

	struct FPostJsonType
	{
		FPostJsonType(const FString& InJsonStr)
			: M_Content(&InJsonStr) {}
		void SetupRequest(const TSharedPtr<IHttpRequest>& InRequest) const {
			InRequest->SetVerb("POST");
			InRequest->SetHeader("Content_Type", "application/json; charset=utf-8");
			InRequest->SetContentAsString(*M_Content);
		}
		const FString* M_Content;
	};

	template <class HttpType>
	TSharedRef<FHttpRequest> CreateHttpRequest(const FString& InUrl, const HttpType& InHttpType) {
		TSharedRef<FHttpRequest> NewHttpRequest = MakeShareable(new FHttpRequest(InUrl));
		InHttpType.SetupRequest(NewHttpRequest->M_RequestImpl);
		return NewHttpRequest;
	}
}
