#include "HttpRequestBase.h"

namespace Network
{
	void FHttpRequestBase::EmptyRequestList() {
		S_RequestList.Empty();
	}

	TDoubleLinkedList<TSharedPtr<FHttpRequestBase>> FHttpRequestBase::S_RequestList;
	FCriticalSection FHttpRequestBase::S_SyncRequestList;
}