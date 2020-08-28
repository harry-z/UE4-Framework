#pragma once
#include "HttpRequestBase.h"

namespace Network
{
	class NETWORK_API FSequentialHttpRequestSet : public FHttpRequestSetBase
	{
	public:
		enum EProcessType {
			// 允许请求集中的某个请求失败，失败后继续进行后面的请求
			AllowFailure = 0,
			// 不允许请求失败，如果有任意一个请求失败，认为整个请求集失败
			DonotAllowFailure
		};

		virtual void ProcessRequest() override;
		virtual void CancelRequest() override;
		virtual ERequestType GetRequestType() const override { return ERequestType_SequentialSet; }
		void AddRequest(const TSharedRef<FHttpRequestBase>& InRequest);

		friend TSharedRef<FSequentialHttpRequestSet> CreateSequentialHttpRequestSet(FSequentialHttpRequestSet::EProcessType InProcessType);

	private:
		FSequentialHttpRequestSet(EProcessType InProcessType) : M_ProcessType(InProcessType) {}
		FSequentialHttpRequestSet(const FSequentialHttpRequestSet& Other) = default;
		FSequentialHttpRequestSet(FSequentialHttpRequestSet&& Other) = default;
		FSequentialHttpRequestSet& operator= (const FSequentialHttpRequestSet& Other) = default;
		FSequentialHttpRequestSet& operator= (FSequentialHttpRequestSet&& Other) = default;

		void SubRequestFinished();
		void SubRequestFailed();

	private:
		typename TArray<TWeakPtr<FHttpRequestBase>>::SizeType M_CurrentRequest = INDEX_NONE;
		EProcessType M_ProcessType = DonotAllowFailure;
	};

	inline TSharedRef<FSequentialHttpRequestSet> CreateSequentialHttpRequestSet(FSequentialHttpRequestSet::EProcessType InProcessType) {
		return MakeShareable(new FSequentialHttpRequestSet(InProcessType));
	}
}
