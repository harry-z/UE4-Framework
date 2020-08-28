#pragma once
#include "Containers/AllocatorFixedSizeFreeList.h"
#include "Containers/List.h"
#include "SceneManagement.h"
#include "MathTypes.h"
#include "Subsystems/WorldSubsystem.h"

namespace FuncUtilities
{
    class FUNCUTILITIES_API FElementDrawer : public FViewElementDrawer
    {
    public:
        FElementDrawer();
        virtual ~FElementDrawer();
        virtual void Draw(const FSceneView *View, FPrimitiveDrawInterface *PDI) override;

        using LineHandle = void*;
        
        LineHandle AddLine(const FVector& Start,
            const FVector& End,
            const FColor& Color,
            float Thickness = 0.0f,
            float DashSize = 0.0f,
            bool bScreenSpace = false);
        void UpdateLine(LineHandle InHandle, const FVector& Start, const FVector& End);
        void RemoveLine(LineHandle InHandle);

        LineHandle AddPolyline(const Math::FloatingPointList &InPoints, 
            const FColor &Color, 
            bool bClosed = false, 
            float Thickness = 0.0f, 
            float DashSize = 0.0f,
            bool bScreenSpace = false);
        void UpdatePolyline(LineHandle InHandle, const Math::FloatingPointList &InPoints);
        void RemovePolyline(LineHandle InHandle);

        static const LineHandle S_InvalidHandle;

    private:
        enum EElementType : uint8
        {
            EType_Line = 0x01,
            EType_Polyline
        };

        uint8 MakeElementFlag(uint8 InElementType) { return InElementType; }
        
        static const uint32 S_BlockSize = 64;

        struct FBaseElementInfo {
            uint8 M_Flag;
            bool M_bScreenSpace;
            FColor M_Color;
            FBaseElementInfo(uint8 InFlag, bool bScreenSpace, const FColor &InColor)
            : M_Flag(InFlag)
            , M_bScreenSpace(bScreenSpace)
            , M_Color(InColor)
            {}
        };

        struct FLineElementInfo : public TIntrusiveLinkedList<FLineElementInfo> {
            FBaseElementInfo M_BaseInfo;
            FLineElementInfo *M_Prev = nullptr;
            float M_Thickness;
            float M_DashSize;
            FVector M_Start, M_End;

            FLineElementInfo(const FVector &InStart, 
                const FVector &InEnd,
                const FColor &InColor, 
                float Thickness, 
                float DashSize,
                bool bScreenSpace)
            : M_BaseInfo(EType_Line, bScreenSpace, InColor)
            , M_Thickness(Thickness)
            , M_DashSize(DashSize)
            , M_Start(InStart)
            , M_End(InEnd)
            {}
            ~FLineElementInfo() {}
        };

        // Simple memory pool
        // sizeof(UPTRINT) for NextFreeAllocation. See TAllocatorFixedSizeFreeList::FreeListNode
        using LineInfoFreeList = TAllocatorFixedSizeFreeList<sizeof(FLineElementInfo) + sizeof(UPTRINT), S_BlockSize>;
        TSharedPtr<LineInfoFreeList> M_LineInfoFreeList;
        // Linked list for line elements
        FLineElementInfo *M_LineInfoList = nullptr;
        // Tracing tail node for linked list
        FLineElementInfo *M_LineInfoListTail = nullptr;

        struct FPolylineElementInfo : public TIntrusiveLinkedList<FPolylineElementInfo> {
            bool M_bClosed;
            FBaseElementInfo M_BaseInfo;
            FPolylineElementInfo *M_Prev = nullptr;
            float M_Thickness;
            float M_DashSize;
            Math::FloatingPointList M_Points;      

            FPolylineElementInfo(const Math::FloatingPointList &InPoints, const FColor &InColor, float Thickness, float DashSize, bool bScreenSpace, bool bClosed)
            : M_bClosed(bClosed)
            , M_BaseInfo(EType_Polyline, bScreenSpace, InColor)
            , M_Thickness(Thickness)
            , M_DashSize(DashSize)
            , M_Points(InPoints)
            {}
            ~FPolylineElementInfo() {}
        };

        // Simple memory pool
        // sizeof(UPTRINT) for NextFreeAllocation. See TAllocatorFixedSizeFreeList::FreeListNode
        using PolylineInfoFreeList = TAllocatorFixedSizeFreeList<sizeof(FPolylineElementInfo) + sizeof(UPTRINT), S_BlockSize>;
        TSharedPtr<PolylineInfoFreeList> M_PolylineInfoFreeList;
        // Linked list for line elements
        FPolylineElementInfo *M_PolylineInfoList = nullptr;
        // Tracing tail node for linked list
        FPolylineElementInfo *M_PolylineInfoListTail = nullptr;
    };
}