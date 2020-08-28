#include "Test2D.h"
#include "../Framework.h"
#include "KernelDefines.h"
#include "Subsystems/InputDelegateSubsystem.h"
#include "Subsystems/ElementDrawerSubsystem.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"

FTest2D::FTest2D(UWorld* InWorld)
{
	if (InWorld != nullptr)
	{
		Kernel::FKernelInputDelegates* InputDelegates = InWorld->GetSubsystem<UFuncUtilInputDelegateSubsystem>()->GetInputDelegates();
		InputDelegates->M_OnMouseMove.AddRaw(this, &FTest2D::OnMouseMove);
		InputDelegates->M_OnMouseWheel.AddRaw(this, &FTest2D::OnMouseWheel);
		InputDelegates->M_OnLMouseButtonUp.AddRaw(this, &FTest2D::OnLMouseButtonUp);
		InputDelegates->M_OnRMouseButtonUp.AddRaw(this, &FTest2D::OnRMouseButtonUp);
		InputDelegates->M_OnKeyUp.AddRaw(this, &FTest2D::OnKeyUp);
		SetViewportMinMaxNeedUpdate(true);
	}
}

void FTest2D::OnMouseMove(FViewport *InViewport, FIntPoint MouseRelToViewport, FIntPoint MouseRelToWindow, int32 ModifierKeyFlag)
{
	typename Math::FixedPointList2D::SizeType NumPoints = M_CachedFixedPoints.Num();
	if (NumPoints > 0) 
	{
		Math::Fixed FixedTolerance = Math::FixedFromFloat(2.0f);

		APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GWorld, 0);
		if (IsViewportMinMaxNeedUpdate())
			CalculateViewportMinMax(PlayerController, InViewport);

		FVector WorldOrigin, WorldDir;
		UGameplayStatics::DeprojectScreenToWorld(PlayerController, FVector2D(MouseRelToWindow.X, MouseRelToWindow.Y), WorldOrigin, WorldDir);
		Math::FFixedVector2D WorldFixedVector2D = Math::FromFloatingVector2D(FVector2D(WorldOrigin));
		const auto &LastFixedPoint = M_CachedFixedPoints[NumPoints - 1];

		switch (M_TestCase)
		{
			case 2:
			{
				if (BIT_CHECK(ModifierKeyFlag, Kernel::EModifierKey_Ctrl)) // Horizontal line
					WorldFixedVector2D.Y = LastFixedPoint.Y;
				else if (BIT_CHECK(ModifierKeyFlag, Kernel::EModifierKey_Shift)) // Vertical line
					WorldFixedVector2D.X = LastFixedPoint.X;
				break;
			}
			default:
				break;
		}

		CreateOrUpdateRubberLine(LastFixedPoint, WorldFixedVector2D);

		if (FMath::Abs(WorldFixedVector2D.X - LastFixedPoint.X) <= FixedTolerance)
		{
			CreateOrUpdateAssistantLine(Math::FFixedVector2D(LastFixedPoint.X, M_Min.Y), Math::FFixedVector2D(LastFixedPoint.X, M_Max.Y));
		}
		else if (FMath::Abs(WorldFixedVector2D.Y - LastFixedPoint.Y) <= FixedTolerance)
		{
			CreateOrUpdateAssistantLine(Math::FFixedVector2D(M_Min.X, LastFixedPoint.Y), Math::FFixedVector2D(M_Max.X, LastFixedPoint.Y));
		}
		else
		{
			if (M_AssistantLineHandle != FuncUtilities::FElementDrawer::S_InvalidHandle)
			{
				GWorld->GetSubsystem<UElementDrawerSubsystem>()->GetElementDrawer()->RemoveLine(M_AssistantLineHandle);
				M_AssistantLineHandle = FuncUtilities::FElementDrawer::S_InvalidHandle;
			}
		}
	}	
}

void FTest2D::OnMouseWheel(FViewport *InViewport, float Delta, int32 ModifierKeyFlag)
{
	SetViewportMinMaxNeedUpdate(true);
}

void FTest2D::OnLMouseButtonUp(FViewport *InViewport, FIntPoint MouseRelToViewport, FIntPoint MouseRelToWindow, int32 ModifierKeyFlag)
{
	APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GWorld, 0);
	FVector WorldOrigin, WorldDir;
	UGameplayStatics::DeprojectScreenToWorld(PlayerController, FVector2D(MouseRelToWindow.X, MouseRelToWindow.Y), WorldOrigin, WorldDir);
	if (M_CachedPoints.Num() > 0)
	{
		switch (M_TestCase)
		{
			case 2:
			{
				if (BIT_CHECK(ModifierKeyFlag, Kernel::EModifierKey_Ctrl)) // Horizontal line
					WorldOrigin.Y = M_CachedPoints[M_CachedPoints.Num() - 1].Y;
				else if (BIT_CHECK(ModifierKeyFlag, Kernel::EModifierKey_Shift)) // Vertical line
					WorldOrigin.X = M_CachedPoints[M_CachedPoints.Num() - 1].X;
				break;
			}
		}
	}
	M_CachedPoints.Emplace(FVector2D(WorldOrigin));
	M_CachedFixedPoints.Emplace(Math::FromFloatingVector2D(FVector2D(WorldOrigin)));
	typename Math::FloatingPointList2D::SizeType NumPoints = M_CachedPoints.Num();
	if (NumPoints > 1)
	{
		switch (M_TestCase)
		{
			case 1:
			{
				CreateOrUpdateOutline();

				FVector2D FloatingVector = M_CachedPoints[NumPoints - 1] - M_CachedPoints[NumPoints - 2];
				float FloatingLength = FloatingVector.Size();
				Math::FFixedVector2D FixedVector = M_CachedFixedPoints[NumPoints - 1] - M_CachedFixedPoints[NumPoints - 2];
				Math::Fixed FixedLength = Math::FixedVector2DLength(FixedVector);

				// Vector length test
				// Integer values are equal
				GEngine->AddOnScreenDebugMessage(0, 3600.0f, FColor(255,48,16), 
					FString::Printf(TEXT("Floating Length: %d"), FMath::RoundToInt(FloatingLength * 10.0f)));
				GEngine->AddOnScreenDebugMessage(1, 3600.0f, FColor(255,48,16),
					FString::Printf(TEXT("Fixed Length: %d"), FMath::RoundToInt(Math::FixedToFloat(FixedLength * 10))));

				FloatingVector.Normalize();
				Math::FixedVector2DNormalizeInplace(FixedVector);
				// Vector normalize test
				// Values are equal at first 3 digits 
				GEngine->AddOnScreenDebugMessage(2, 3600.0f, FColor(255,48,16), 
					FString::Printf(TEXT("Floating Vector Dir: X: %f, Y: %f"), FloatingVector.X, FloatingVector.Y));
				GEngine->AddOnScreenDebugMessage(3, 3600.0f, FColor(255,48,16),
					FString::Printf(TEXT("Fixed Vector Dir: X: %f, Y: %f"), Math::FixedToFloat(FixedVector.X), Math::FixedToFloat(FixedVector.Y)));

				// Dot product test
				// Radians are equal at first 2 digits, integer part of degrees are equal
				FVector2D LastVector = NumPoints > 2 ? (M_CachedPoints[NumPoints - 2] - M_CachedPoints[NumPoints - 3]) : FVector2D(1.0f, 0.0f);
				LastVector.Normalize();
				Math::FFixedVector2D LastFixedVector = NumPoints > 2 ? (M_CachedFixedPoints[NumPoints - 2] - M_CachedFixedPoints[NumPoints - 3]) : Math::FFixedVector2D(Math::C_FixedConst1, 0);
				Math::FixedVector2DNormalizeInplace(LastFixedVector);
				float FloatingCosine = FloatingVector | LastVector;
				float FixedCosine = Math::FixedToFloat(Math::FixedVector2DDotProduct(FixedVector, LastFixedVector));
				GEngine->AddOnScreenDebugMessage(4, 3600.0f, FColor(255,48,16), 
					FString::Printf(TEXT("Angle Between Floating Vectors: Cosine: %f, Radian: %f, Degree: %f"), FloatingCosine, FMath::Acos(FloatingCosine), FMath::Acos(FloatingCosine) / PI * 180.0f));
				GEngine->AddOnScreenDebugMessage(5, 3600.0f, FColor(255,48,16),
					FString::Printf(TEXT("Angle Between Fixed Vectors: Cosine: %f, Radian: %f, Degree: %f"), FixedCosine, FMath::Acos(FixedCosine), FMath::Acos(FixedCosine) / PI * 180.0f));

				// Cross product test
				// Values are equal at first 3 digits
				FVector Cross = FVector::UpVector ^ FVector(FloatingVector, 0.0f); Cross.Normalize();
				Math::FFixedVector FixedCross = Math::FixedVectorCrossProduct(Math::C_FixedVectorUnitZ, Math::Fixed2DTo3D(FixedVector)); Math::FixedVectorNormalizeInplace(FixedCross);
				GEngine->AddOnScreenDebugMessage(6, 3600.0f, FColor(255,48,16), 
					FString::Printf(TEXT("Floating Vector Cross Product: X: %f, Y: %f, Z: %f"), Cross.X, Cross.Y, Cross.Z));
				GEngine->AddOnScreenDebugMessage(7, 3600.0f, FColor(255,48,16),
					FString::Printf(TEXT("Fixed Vector Cross Product: X: %f, Y: %f, Z: %f"), Math::FixedToFloat(FixedCross.X), Math::FixedToFloat(FixedCross.Y), Math::FixedToFloat(FixedCross.Z)));

				FQuat qRot(FVector::UpVector, PI * 0.5f);
				FVector FloatingRot = qRot.RotateVector(FVector(FloatingVector, 0.0f));
				Math::FFixedVector FixedRot = Math::FromFloatingVector(qRot.RotateVector(FVector(FloatingVector, 0.0f)));
				GEngine->AddOnScreenDebugMessage(8, 3600.0f, FColor(255,48,16), 
					FString::Printf(TEXT("Floating Vector Rotation: X: %f, Y: %f, Z: %f"), FloatingRot.X, FloatingRot.Y, FloatingRot.Z));
				GEngine->AddOnScreenDebugMessage(9, 3600.0f, FColor(255,48,16),
					FString::Printf(TEXT("Fixed Vector Rotation: X: %f, Y: %f, Z: %f"), Math::FixedToFloat(FixedRot.X), Math::FixedToFloat(FixedRot.Y), Math::FixedToFloat(FixedRot.Z)));
				break;
			}
			case 2:
			{
				auto StartPoint = M_CachedFixedPoints[NumPoints - 2];

				Math::FFixedVector2D Dir = M_CachedFixedPoints[NumPoints - 1] - M_CachedFixedPoints[NumPoints - 2];
				Math::Fixed Length = Math::FixedVector2DLength(Dir);
				Math::FixedVector2DNormalizeInplace(Dir);

				Math::FFixedVector2D New1(Math::C_FixedVector2DZero);
				if (BIT_CHECK(ModifierKeyFlag, Kernel::EModifierKey_Ctrl)) // Horizontal line
				{
					Dir.X = -Dir.X;
					New1 = M_CachedFixedPoints[NumPoints - 1] + Math::FixedVector2DMul(Math::C_FixedVector2DUnitY, Math::FixedFromFloat(10.0f));
				}
				else if (BIT_CHECK(ModifierKeyFlag, Kernel::EModifierKey_Shift)) // Vertical line
				{
					Dir.Y = -Dir.Y;
					New1 = M_CachedFixedPoints[NumPoints - 1] + Math::FixedVector2DMul(Math::C_FixedVector2DUnitX, Math::FixedFromFloat(10.0f));
				}
				M_CachedFixedPoints.Emplace(New1);
				Math::FFixedVector2D New2 = New1 + Math::FixedVector2DMul(Dir, Length);
				M_CachedFixedPoints.Emplace(New2);

				CreateOrUpdateOutline();
				M_CachedPoints.Reset();
				M_CachedFixedPoints.Reset();

				GEngine->AddOnScreenDebugMessage(6, 3600.0f, FColor(255,48,16), 
					FString::Printf(TEXT("First Point: X: %f, Y: %f"), Math::FixedToFloat(StartPoint.X), Math::FixedToFloat(StartPoint.Y)));
				GEngine->AddOnScreenDebugMessage(7, 3600.0f, FColor(255,48,16),
					FString::Printf(TEXT("Last Point: X: %f, Y: %f"), Math::FixedToFloat(New2.X), Math::FixedToFloat(New2.Y)));
				break;
			}
		}
	}
}

void FTest2D::OnRMouseButtonUp(FViewport *InViewport, FIntPoint MouseRelToViewport, FIntPoint MouseRelToWindow, int32 ModifierKeyFlag)
{
	ClearScene();
}

void FTest2D::OnKeyUp(FViewport *InViewport, FKey InKey, int32 ModifierKeyFlag)
{
	uint8 GotoCase = 0;
	if (InKey == EKeys::One)
		GotoCase = 1;
	else if (InKey == EKeys::Two)
		GotoCase = 2;

	if (GotoCase != 0)
	{
		if (GotoCase != M_TestCase)
			ClearScene();
		M_TestCase = GotoCase;
	}
}

void FTest2D::CreateOrUpdateOutline()
{
	Math::FloatingPointList TempPointList; TempPointList.SetNum(M_CachedFixedPoints.Num());
	for (typename Math::FixedPointList2D::SizeType i = 0, iEnd = M_CachedFixedPoints.Num(); i < iEnd; ++i)
	{
		TempPointList[i] = FVector(Math::ToFloatingVector2D(M_CachedFixedPoints[i]), 0.0f);
	}

	FuncUtilities::FElementDrawer* ElementDrawer = GWorld->GetSubsystem<UElementDrawerSubsystem>()->GetElementDrawer();
	if (M_OutlineHandle == FuncUtilities::FElementDrawer::S_InvalidHandle)
	{
		M_OutlineHandle = ElementDrawer->AddPolyline(TempPointList, FColor(255, 255, 255), false, 1.0f, 0.0f, true);
	}
	else
	{
		ElementDrawer->UpdatePolyline(M_OutlineHandle, TempPointList);
	}
}

void FTest2D::CreateOrUpdateRubberLine(const Math::FFixedVector2D &InStart, const Math::FFixedVector2D &InEnd)
{
	FuncUtilities::FElementDrawer* ElementDrawer = GWorld->GetSubsystem<UElementDrawerSubsystem>()->GetElementDrawer();
	if (M_RubberLineHandle == FuncUtilities::FElementDrawer::S_InvalidHandle)
	{
		M_RubberLineHandle = ElementDrawer->AddLine(FVector(Math::ToFloatingVector2D(InStart), 0.0f),
			FVector(Math::ToFloatingVector2D(InEnd), 0.0f),
			FColor(255, 255, 255), 
			1.0f, 
			0.0f,
			true);
	}
	else
	{
		ElementDrawer->UpdateLine(M_RubberLineHandle, 
			FVector(Math::ToFloatingVector2D(InStart), 0.0f),
			FVector(Math::ToFloatingVector2D(InEnd), 0.0f));
	}
}

void FTest2D::CreateOrUpdateAssistantLine(const Math::FFixedVector2D &InStart, const Math::FFixedVector2D &InEnd)
{
	FuncUtilities::FElementDrawer* ElementDrawer = GWorld->GetSubsystem<UElementDrawerSubsystem>()->GetElementDrawer();
	if (M_AssistantLineHandle == FuncUtilities::FElementDrawer::S_InvalidHandle)
	{
		M_AssistantLineHandle = ElementDrawer->AddLine(FVector(Math::ToFloatingVector2D(InStart), 0.0f),
			FVector(Math::ToFloatingVector2D(InEnd), 0.0f),
			FColor(0, 255, 255), 
			5.0f, 
			2.0f,
			true);
	}
	else
	{
		ElementDrawer->UpdateLine(M_AssistantLineHandle, 
			FVector(Math::ToFloatingVector2D(InStart), 0.0f),
			FVector(Math::ToFloatingVector2D(InEnd), 0.0f));
	}
}

void FTest2D::CalculateViewportMinMax(APlayerController *InPC, FViewport *InViewport)
{
	FIntPoint SizeXY = InViewport->GetSizeXY();
	FVector World, Dir;
	UGameplayStatics::DeprojectScreenToWorld(InPC, FVector2D(0.0f, 0.0f), World, Dir);
	M_Min = Math::FromFloatingVector2D(FVector2D(World));
	UGameplayStatics::DeprojectScreenToWorld(InPC, FVector2D(SizeXY.X, SizeXY.Y), World, Dir);
	M_Max = Math::FromFloatingVector2D(FVector2D(World));
	SetViewportMinMaxNeedUpdate(false);
}

void FTest2D::ClearScene()
{
	M_CachedPoints.Reset();
	M_CachedFixedPoints.Reset();

	FuncUtilities::FElementDrawer* ElementDrawer = GWorld->GetSubsystem<UElementDrawerSubsystem>()->GetElementDrawer();
	if (M_RubberLineHandle != FuncUtilities::FElementDrawer::S_InvalidHandle)
	{
		ElementDrawer->RemoveLine(M_RubberLineHandle);
		M_RubberLineHandle = FuncUtilities::FElementDrawer::S_InvalidHandle;
	}
	if (M_OutlineHandle != FuncUtilities::FElementDrawer::S_InvalidHandle)
	{
		ElementDrawer->RemovePolyline(M_OutlineHandle);
		M_OutlineHandle = FuncUtilities::FElementDrawer::S_InvalidHandle;
	}
	if (M_AssistantLineHandle != FuncUtilities::FElementDrawer::S_InvalidHandle)
	{
		ElementDrawer->RemoveLine(M_AssistantLineHandle);
		M_AssistantLineHandle = FuncUtilities::FElementDrawer::S_InvalidHandle;
	}
}