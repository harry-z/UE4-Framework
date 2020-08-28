#include "PawnPanning2D.h"
#include "Camera/CameraComponent.h"

APawnPanning2D::APawnPanning2D()
{
	bFindCameraComponentWhenViewTarget = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>("Panning2DPawnRoot");
    M_CameraComponent = CreateDefaultSubobject<UCameraComponent>("Panning2DCameraComponent");
    M_CameraComponent->SetProjectionMode(ECameraProjectionMode::Orthographic);
    M_CameraComponent->SetWorldLocation(FVector::UpVector);
    M_CameraComponent->SetWorldRotation(FRotator(-90.0f, 0.0f, -90.0f));
    M_CameraComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

    AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void APawnPanning2D::NotifyViewportResized(const FVector2D& InNewSize)
{
    M_CameraComponent->SetOrthoWidth(InNewSize.X);
    M_CameraComponent->SetAspectRatio(InNewSize.X / InNewSize.Y);
}

void APawnPanning2D::MoveCamera(const FVector2D& InDelta)
{
    M_CameraComponent->AddWorldOffset(FVector(InDelta, 0.0f));
}

void APawnPanning2D::ZoomCamera(float InWheelValue)
{
    M_CameraComponent->SetOrthoWidth(M_CameraComponent->OrthoWidth - InWheelValue * 4.0f);
}