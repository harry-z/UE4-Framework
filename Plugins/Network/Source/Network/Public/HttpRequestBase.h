#pragma once
#include "HttpModule.h"
#include "HttpManager.h"
#include "Interfaces/IHttpResponse.h"
#include "Containers/List.h"
#include "HAL/CriticalSection.h"
#include "Misc/ScopeLock.h"

namespace Network
{
	class FHttpRequestBase : public TSharedFromThis<FHttpRequestBase>
	{
	public:
		enum ERequestState {
			// 等待处理
			ERequestState_WaitingForProcess = 0,
			// 正在处理
			ERequestState_Processing,
			// 处理完毕
			ERequestState_Finished,
			// 处理失败
			ERequestState_Failed,
			// 请求被取消
			ERequestState_Canceled
		};

		enum ERequestType {
			// 未知请求类型
			ERequestType_Unknown = 0x0000,
			// 普通请求类型
			ERequestType_Normal = 0x1000,
			// 请求集合类型
			ERequestType_Set = 0x2000,
			// 顺序执行请求集合类型
			ERequestType_SequentialSet = 0x2001,
			// 并行执行请求集合类型
			ERequestType_ParallelSet = 0x2002
		};

		FHttpRequestBase() : M_RequestState(ERequestState_WaitingForProcess) { AddToList(); }
		virtual ~FHttpRequestBase() {}
		virtual void ProcessRequest() = 0;
		virtual void CancelRequest() = 0;
		virtual ERequestType GetRequestType() const { return ERequestType_Unknown; }
		FORCEINLINE ERequestState GetRequestState() const { return M_RequestState; }

	protected:
		// 线程安全
		void AddToList() {
			FScopeLock sl(&S_SyncRequestList);
			S_RequestList.AddTail(SharedThis(this));
		}
		// 线程安全
		void RemoveFromList() {
			FScopeLock sl(&S_SyncRequestList);
			S_RequestList.RemoveNode(SharedThis(this));
		}

	protected:
		ERequestState M_RequestState;

	public:
		static void EmptyRequestList();

	protected:
		static TDoubleLinkedList<TSharedPtr<FHttpRequestBase>> S_RequestList;
		static FCriticalSection S_SyncRequestList;
	};

	class FHttpRequestSetBase : public FHttpRequestBase
	{
	public:
		virtual ~FHttpRequestSetBase() { M_Requests.Empty(); }
		virtual ERequestType GetRequestType() const override { return ERequestType_Set; }
		void SetOnRequestFinished(const FSimpleDelegate& InFinished) {}
		void SetOnRequestFailed(const FSimpleDelegate& InFailed) {}
		FSimpleDelegate& GetRequestFinished() { return M_FinishedDelegate; }
		FSimpleDelegate& GetRequestFailed() { return M_FailedDelegate; }

	protected:
		FSimpleDelegate M_FinishedDelegate;
		FSimpleDelegate M_FailedDelegate;

		TArray<TSharedPtr<FHttpRequestBase>> M_Requests;
	};

	FORCEINLINE bool IsNormalRequest(FHttpRequestBase::ERequestType InType) {
		return ((int32)InType & (int32)FHttpRequestBase::ERequestType_Normal) != 0;
	}

	FORCEINLINE bool IsSetRequest(FHttpRequestBase::ERequestType InType) {
		return ((int32)InType & (int32)FHttpRequestBase::ERequestType_Set) != 0;
	}
}

