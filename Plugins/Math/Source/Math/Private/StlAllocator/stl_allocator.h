/*
	Custom allocator for stl containers using UE4 FMalloc
*/
#pragma once
#include "HAL/UnrealMemory.h"
#include "Templates/IsConst.h"
#include "Templates/UnrealTemplate.h"
#include <vector>
#include <list>

template <class _Ty>
class FUnrealMallocAllocator {
public:
	static_assert(!TIsConst<_Ty>::Value, "The C++ Standard forbids containers of const elements "
                                   "because allocator<const T> is ill-formed.");

    using value_type = _Ty;

	template <class _Other>
    struct _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS rebind {
        using other = FUnrealMallocAllocator<_Other>;
    };
    _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS _NODISCARD _Ty* address(_Ty& _Val) const noexcept {
        return _STD addressof(_Val);
    }
    _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS _NODISCARD const _Ty* address(const _Ty& _Val) const noexcept {
        return _STD addressof(_Val);
    }

    constexpr FUnrealMallocAllocator() noexcept {}

    constexpr FUnrealMallocAllocator(const FUnrealMallocAllocator&) noexcept = default;
    template <class _Other>
    constexpr FUnrealMallocAllocator(const FUnrealMallocAllocator<_Other>&) noexcept {}

    void deallocate(_Ty* const _Ptr, const size_t _Count) {
        GMalloc->Free(_Ptr);
    }

    _NODISCARD _DECLSPEC_ALLOCATOR _Ty* allocate(_CRT_GUARDOVERFLOW const size_t _Count) {
        return static_cast<_Ty*>(GMalloc->Malloc(_STD _Get_size_of_n<sizeof(_Ty)>(_Count)));
    }

    _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS _NODISCARD _DECLSPEC_ALLOCATOR _Ty* allocate(
        _CRT_GUARDOVERFLOW const size_t _Count, const void*) {
        return allocate(_Count);
    }

    template <class _Objty, class... _Types>
    _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS void construct(_Objty* const _Ptr, _Types&&... _Args) {
        ::new (const_cast<void*>(static_cast<const volatile void*>(_Ptr))) _Objty(Forward<_Types>(_Args)...);
    }

    template <class _Uty>
    _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS void destroy(_Uty* const _Ptr) {
        _Ptr->~_Uty();
    }

    _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS _NODISCARD size_t max_size() const noexcept {
        return static_cast<size_t>(-1) / sizeof(_Ty);
    }
};

#if !FORCE_ANSI_ALLOCATOR

template <class _Ty>
using TAllocator = FUnrealMallocAllocator<_Ty>;

template <class _Ty>
using ZYSTDVector = std::vector<_Ty, TAllocator<_Ty>>;

template <class _Ty>
using ZYSTDList = std::list<_Ty, TAllocator<_Ty>>;

#else

template <class _Ty>
using TAllocator = std::allocator<_Ty>;

template <class _Ty>
using ZYSTDVector = std::vector<_Ty>;

template <class _Ty>
using ZYSTDList = std::list<_Ty>;

#endif