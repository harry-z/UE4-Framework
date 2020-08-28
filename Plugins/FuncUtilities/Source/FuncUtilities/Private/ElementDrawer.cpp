#include "ElementDrawer.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"

namespace FuncUtilities
{
    const FElementDrawer::LineHandle FElementDrawer::S_InvalidHandle = nullptr;

    FElementDrawer::FElementDrawer()
    {
        M_LineInfoFreeList = MakeShareable(new LineInfoFreeList(S_BlockSize));
        M_PolylineInfoFreeList = MakeShareable(new PolylineInfoFreeList(S_BlockSize));
    }

    FElementDrawer::~FElementDrawer()
    {
        for (FLineElementInfo::TIterator It(M_LineInfoList); It; It.Next())
        {
            FLineElementInfo *pLineElemInfo = &(*It);
            pLineElemInfo->~FLineElementInfo();
            M_LineInfoFreeList->Free((reinterpret_cast<uint8*>(pLineElemInfo)) - sizeof(UPTRINT));
        }
        M_LineInfoFreeList.Reset();
        for (FPolylineElementInfo::TIterator It(M_PolylineInfoList); It; It.Next())
        {
            FPolylineElementInfo *pPolylineElemInfo = &(*It);
            pPolylineElemInfo->~FPolylineElementInfo();
            M_PolylineInfoFreeList->Free((reinterpret_cast<uint8*>(pPolylineElemInfo)) - sizeof(UPTRINT));
        }
        M_PolylineInfoFreeList.Reset();
        
        M_LineInfoList = nullptr;
        M_LineInfoListTail = nullptr;
        M_PolylineInfoList = nullptr;
        M_PolylineInfoListTail = nullptr;
    }

    void FElementDrawer::Draw(const FSceneView *View, FPrimitiveDrawInterface *PDI)
    {
        // 顺序绘制，当有像素重叠时，后绘制的会覆盖先绘制的
        if (M_LineInfoList != nullptr)
        {
            for (FLineElementInfo::TIterator It(M_LineInfoList); It; It.Next())
            {
                auto &LineElement = (*It);
                if (FMath::IsNearlyZero(LineElement.M_DashSize))
                {
                    PDI->DrawLine(LineElement.M_Start, 
                        LineElement.M_End, 
                        FLinearColor(LineElement.M_BaseInfo.M_Color), 
                        SDPG_Foreground, 
                        LineElement.M_Thickness, 
                        0.0f, 
                        LineElement.M_BaseInfo.M_bScreenSpace);
                }
                else
                {
                    DrawDashedLine(PDI, LineElement.M_Start, 
                        LineElement.M_End, 
                        FLinearColor(LineElement.M_BaseInfo.M_Color),
                        LineElement.M_DashSize,
                        SDPG_Foreground, 
                        0.0f);
                }
                
            }
        }
        if (M_PolylineInfoList != nullptr)
        {
            for (FPolylineElementInfo::TIterator It(M_PolylineInfoList); It; It.Next())
            {
                auto &PolylineElement = (*It);
                typename Math::FloatingPointList::SizeType NumPoints = PolylineElement.M_Points.Num();
                typename Math::FloatingPointList::SizeType NumLines = PolylineElement.M_bClosed ? PolylineElement.M_Points.Num() : PolylineElement.M_Points.Num() - 1;
                if (FMath::IsNearlyZero(PolylineElement.M_DashSize))
                {
                    // 内存预分配
                    PDI->AddReserveLines(SDPG_Foreground, NumLines, false, !FMath::IsNearlyZero(PolylineElement.M_Thickness));
                    for (typename Math::FloatingPointList::SizeType i = 0; i < NumLines; ++i)
                    {
                        PDI->DrawLine(PolylineElement.M_Points[i], PolylineElement.M_Points[(i + 1) % NumPoints], 
                            FLinearColor(PolylineElement.M_BaseInfo.M_Color),
                            SDPG_Foreground,
                            PolylineElement.M_Thickness,
                            0.0f, 
                            PolylineElement.M_BaseInfo.M_bScreenSpace);
                    }
                }
                else
                {
                    for (typename Math::FloatingPointList::SizeType i = 0; i < NumLines; ++i)
                    {
                        DrawDashedLine(PDI, PolylineElement.M_Points[i], PolylineElement.M_Points[(i + 1) % NumLines], 
                            FLinearColor(PolylineElement.M_BaseInfo.M_Color), 
                            PolylineElement.M_DashSize,
                            SDPG_Foreground,
                            0.0f);
                    }
                }
            }
        }
    }

    FElementDrawer::LineHandle FElementDrawer::AddLine(const FVector& Start,
        const FVector& End,
        const FColor& Color,
        float Thickness /* = 0.0f */,
        float DashSize /* = 0.0f */,
        bool bScreenSpace /* = false */)
    {
        uint8 *pMem = static_cast<uint8*>(M_LineInfoFreeList->Allocate());
        FLineElementInfo *pLineElemInfo = new (pMem + sizeof(UPTRINT)) FLineElementInfo(Start, End, Color, Thickness, DashSize, bScreenSpace);
        pLineElemInfo->M_BaseInfo.M_Flag = MakeElementFlag(pLineElemInfo->M_BaseInfo.M_Flag);
        if (M_LineInfoList == nullptr) // 链表为空
            pLineElemInfo->LinkHead(M_LineInfoList);
        else
        {
            // 链接到Tail
            checkSlow(M_LineInfoListTail != nullptr);
            pLineElemInfo->LinkAfter(M_LineInfoListTail);
        }
        // 更新Tail
        pLineElemInfo->M_Prev = M_LineInfoListTail;
        M_LineInfoListTail = pLineElemInfo;
        return (LineHandle)(pLineElemInfo);
    }

    void FElementDrawer::UpdateLine(LineHandle InHandle, const FVector &Start, const FVector &End)
    {
        FLineElementInfo *pLineElemInfo = static_cast<FLineElementInfo*>(InHandle);
        checkSlow(pLineElemInfo->M_BaseInfo.M_Flag == MakeElementFlag((uint8)EType_Line));
        pLineElemInfo->M_Start = Start;
        pLineElemInfo->M_End = End;
    }

    void FElementDrawer::RemoveLine(LineHandle InHandle)
    {
        FLineElementInfo *pLineElemInfo = static_cast<FLineElementInfo*>(InHandle);
        checkSlow(pLineElemInfo->M_BaseInfo.M_Flag == MakeElementFlag((uint8)EType_Line));
        // 要删除的对象就是Tail，移除后需要更新Tail指针
        if (M_LineInfoListTail == pLineElemInfo)
            M_LineInfoListTail = (M_LineInfoListTail != M_LineInfoList) ? pLineElemInfo->M_Prev : nullptr;
        pLineElemInfo->Unlink();
        pLineElemInfo->M_Prev = nullptr;
        pLineElemInfo->~FLineElementInfo();
        M_LineInfoFreeList->Free((reinterpret_cast<uint8*>(pLineElemInfo)) - sizeof(UPTRINT));
    }

    FElementDrawer::LineHandle FElementDrawer::AddPolyline(const Math::FloatingPointList &InPoints,
        const FColor &Color,
        bool bClosed,
        float Thickness /* = 0.0f */,
        float DashSize /* = 0.0f */,
        bool bScreenSpace /* = false */)
    {
        uint8 *pMem = static_cast<uint8*>(M_PolylineInfoFreeList->Allocate());
        FPolylineElementInfo *pPolylineElemInfo = new (pMem + sizeof(UPTRINT)) FPolylineElementInfo(InPoints, Color, Thickness, DashSize, bScreenSpace, bClosed);
        pPolylineElemInfo->M_BaseInfo.M_Flag = MakeElementFlag(pPolylineElemInfo->M_BaseInfo.M_Flag);
        if (M_PolylineInfoList == nullptr)
            pPolylineElemInfo->LinkHead(M_PolylineInfoList);
        else
        {
            checkSlow(M_PolylineInfoListTail != nullptr);
            pPolylineElemInfo->LinkAfter(M_PolylineInfoListTail);
        }
        pPolylineElemInfo->M_Prev = M_PolylineInfoListTail;
        M_PolylineInfoListTail = pPolylineElemInfo;
        return (LineHandle)(pPolylineElemInfo);
    }

    void FElementDrawer::UpdatePolyline(LineHandle InHandle, const Math::FloatingPointList &InPoints)
    {
        FPolylineElementInfo *pPolylineElemInfo = static_cast<FPolylineElementInfo*>(InHandle);
        checkSlow(pPolylineElemInfo->M_BaseInfo.M_Flag == MakeElementFlag((uint8)EType_Polyline));
        pPolylineElemInfo->M_Points = InPoints;
    }

    void FElementDrawer::RemovePolyline(LineHandle InHandle)
    {
        FPolylineElementInfo *pPolylineElemInfo = static_cast<FPolylineElementInfo*>(InHandle);
        checkSlow(pPolylineElemInfo->M_BaseInfo.M_Flag == MakeElementFlag((uint8)EType_Polyline));
        if (M_PolylineInfoListTail == pPolylineElemInfo)
            M_PolylineInfoListTail = (M_PolylineInfoListTail != M_PolylineInfoList) ? pPolylineElemInfo->M_Prev : nullptr;
        pPolylineElemInfo->Unlink();
        pPolylineElemInfo->M_Prev = nullptr;
        pPolylineElemInfo->~FPolylineElementInfo();
        M_PolylineInfoFreeList->Free((reinterpret_cast<uint8*>(pPolylineElemInfo)) - sizeof(UPTRINT));
    }
}
