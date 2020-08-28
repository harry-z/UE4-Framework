#include "PlayerControllerPanning2D.h"
#include "PawnPanning2D.h"
#include "KernelDefines.h"
#include "KernelDelegates.h"
#include "Kismet/GameplayStatics.h"

APlayerControllerPanning2D::APlayerControllerPanning2D()
{
	Kernel::FKernelDelegates::S_OnViewportResized.AddLambda(
		[this](FVector2D NewSize) {
		APawnPanning2D* Panning2D = GetPawn<APawnPanning2D>();
		if (Panning2D)
			Panning2D->NotifyViewportResized(NewSize);
		CalculateOnePixelUnit();
	}
	);
}

bool APlayerControllerPanning2D::InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad)
{
	if (EventType == IE_Pressed)
	{
		if (Key == EKeys::MiddleMouseButton)
		{
			BIT_ADD(M_MouseButtonFlag, EMiddleMouseButton);
		}
	}
	else if (EventType == IE_Released)
	{
		if (Key == EKeys::MiddleMouseButton)
		{
			BIT_REMOVE(M_MouseButtonFlag, EMiddleMouseButton);
		}
	}
	
	return Super::InputKey(Key, EventType, AmountDepressed, bGamepad);
}

bool APlayerControllerPanning2D::InputAxis(FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	if (Key == EKeys::MouseX)
	{
		if (BIT_CHECK(M_MouseButtonFlag, EMiddleMouseButton))
		{
			GetPawn<APawnPanning2D>()->MoveCamera(FVector2D(-Delta * M_OnePixelUnit.X, 0.0f));
		}
	}
	else if (Key == EKeys::MouseY)
	{
		if (BIT_CHECK(M_MouseButtonFlag, EMiddleMouseButton))
		{
			GetPawn<APawnPanning2D>()->MoveCamera(FVector2D(0.0f, Delta * M_OnePixelUnit.Y));
		}
	}
	else if (Key == EKeys::MouseWheelAxis)
	{
		GetPawn<APawnPanning2D>()->ZoomCamera(Delta);
		CalculateOnePixelUnit();
	}

	return Super::InputAxis(Key, Delta, DeltaTime, NumSamples, bGamepad);
}

void APlayerControllerPanning2D::CalculateOnePixelUnit()
{
	UpdateCameraManager(0.0f);
	FVector WorldOrigin1, WorldDir1;
	UGameplayStatics::DeprojectScreenToWorld(this, FVector2D(0.0f, 0.0f), WorldOrigin1, WorldDir1);
	FVector WorldOrigin2, WorldDir2;
	UGameplayStatics::DeprojectScreenToWorld(this, FVector2D(1.0f, 1.0f), WorldOrigin2, WorldDir2);
	M_OnePixelUnit.Set(FMath::Abs(WorldOrigin1.X - WorldOrigin2.X), FMath::Abs(WorldOrigin1.Y - WorldOrigin2.Y));
}