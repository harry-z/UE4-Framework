#pragma once
#include "HttpRequestBase.h"
#include "Tickable.h"

namespace Network
{
	class NETWORK_API FParallelHttpRequestSet : public FHttpRequestSetBase
	{
	public:
		virtual void ProcessRequest() override;
		virtual void CancelRequest() override;
		virtual ERequestType GetRequestType() const override { return ERequestType_ParallelSet; }
		void AddRequest(const TSharedRef<FHttpRequestBase>& InRequest);

		friend TSharedRef<FParallelHttpRequestSet> CreateParallelHttpRequestSet(bool bBlocking);

	private:
		class FZYParallelHttpRequestSetTickable final : public FTickableGameObject
		{
		public:
			FZYParallelHttpRequestSetTickable(const TSharedRef<FParallelHttpRequestSet>& InRequestSet) : M_RequestSet(InRequestSet) {}
			virtual void Tick(float DeltaTime) override;
			virtual bool IsTickable() const;
			virtual TStatId GetStatId() const override;
			virtual UWorld* GetTickableGameObjectWorld() const;
		private:
			TWeakPtr<FParallelHttpRequestSet> M_RequestSet;
		};
		friend class FZYParallelHttpRequestSetTickable;
		TSharedPtr<FZYParallelHttpRequestSetTickable> M_Tickable;

	private:
		FParallelHttpRequestSet(bool bBlocking) : M_Blocking(bBlocking) {}
		FParallelHttpRequestSet(const FParallelHttpRequestSet& Other) = default;
		FParallelHttpRequestSet(FParallelHttpRequestSet&& Other) = default;
		FParallelHttpRequestSet& operator= (const FParallelHttpRequestSet& Other) = default;
		FParallelHttpRequestSet& operator= (FParallelHttpRequestSet&& Other) = default;
		void NotifyNonBlockingRequestSetFinished();

	private:
		bool M_Blocking = false;
		typename TArray<TWeakPtr<FHttpRequestBase>>::SizeType M_RequestCounter;
	};

	inline TSharedRef<FParallelHttpRequestSet> CreateParallelHttpRequestSet(bool bBlocking) {
		return MakeShareable(new FParallelHttpRequestSet(bBlocking));
	}
}
