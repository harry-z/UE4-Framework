#include "PawnFreeLook.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/PlayerController.h"

APawnFreeLook::APawnFreeLook()
{
    PrimaryActorTick.bCanEverTick = true;
    bFindCameraComponentWhenViewTarget = false;
    bUseControllerRotationPitch = true;
    bUseControllerRotationRoll = true;
    bUseControllerRotationYaw = true;
    BaseEyeHeight = 0.0f;

    RootComponent = CreateDefaultSubobject<USceneComponent>("FreeLookRoot");

    AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void APawnFreeLook::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (IsNeedUpdate())
    {
        FVector InputVector = ConsumeMovementInputVector();
        SetActorLocation(GetActorLocation() + InputVector);
        NeedUpdate(false);
    }
}

void APawnFreeLook::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    APlayerController* PlayerController = GetController<APlayerController>();
    if (PlayerController != nullptr)
    {
        UPlayerInput* PlayerInput = PlayerController->PlayerInput;
        PlayerInput->AddAxisMapping(FInputAxisKeyMapping("MoveForward", EKeys::W, 1.0f));
        PlayerInput->AddAxisMapping(FInputAxisKeyMapping("MoveBackward", EKeys::S, -1.0f));
        PlayerInput->AddAxisMapping(FInputAxisKeyMapping("MoveRight", EKeys::D, 1.0f));
        PlayerInput->AddAxisMapping(FInputAxisKeyMapping("MoveLeft", EKeys::A, -1.0f));
        PlayerInput->AddAxisMapping(FInputAxisKeyMapping("MoveUpward", EKeys::E, 1.0f));
        PlayerInput->AddAxisMapping(FInputAxisKeyMapping("MoveDownward", EKeys::Q, -1.0f));
    }

    PlayerInputComponent->BindKey<APawnFreeLook>(EKeys::LeftMouseButton, IE_Pressed, this, &APawnFreeLook::Callback_LMouseButtonDown);
    PlayerInputComponent->BindKey<APawnFreeLook>(EKeys::LeftMouseButton, IE_Released, this, &APawnFreeLook::Callback_LMouseButtonUp);
    PlayerInputComponent->BindKey<APawnFreeLook>(EKeys::RightMouseButton, IE_Pressed, this, &APawnFreeLook::Callback_RMouseButtonDown);
    PlayerInputComponent->BindKey<APawnFreeLook>(EKeys::RightMouseButton, IE_Released, this, &APawnFreeLook::Callback_RMouseButtonUp);
    PlayerInputComponent->BindAxis("MoveForward", this, &APawnFreeLook::Callback_MoveForward);
    PlayerInputComponent->BindAxis("MoveBackward", this, &APawnFreeLook::Callback_MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &APawnFreeLook::Callback_MoveRight);
    PlayerInputComponent->BindAxis("MoveLeft", this, &APawnFreeLook::Callback_MoveRight);
    PlayerInputComponent->BindAxis("MoveUpward", this, &APawnFreeLook::Callback_MoveUp);
    PlayerInputComponent->BindAxis("MoveDownward", this, &APawnFreeLook::Callback_MoveUp);
    PlayerInputComponent->BindAxisKey<APawnFreeLook>(EKeys::MouseX, this, &APawnFreeLook::Callback_MouseAxisX);
    PlayerInputComponent->BindAxisKey<APawnFreeLook>(EKeys::MouseY, this, &APawnFreeLook::Callback_MouseAxisY);
    PlayerInputComponent->BindAxisKey<APawnFreeLook>(EKeys::MouseWheelAxis, this, &APawnFreeLook::Callback_MouseWheelAxis);
    PlayerInputComponent->BindKey<APawnFreeLook>(EKeys::Equals, IE_Released, this, &APawnFreeLook::Callback_IncMovementStep);
    PlayerInputComponent->BindKey<APawnFreeLook>(EKeys::Subtract, IE_Released, this, &APawnFreeLook::Callback_DecMovementStep);
}

void APawnFreeLook::Callback_MoveForward(float InAxisValue)
{
    FRotator const ControlSpaceRot = Controller->GetControlRotation();
    AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::X), InAxisValue * M_MovementStep);
    NeedUpdate(true);
}

void APawnFreeLook::Callback_MoveRight(float InAxisValue)
{
    FRotator const ControlSpaceRot = Controller->GetControlRotation();
    AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::Y), InAxisValue * M_MovementStep);
    NeedUpdate(true);
}

void APawnFreeLook::Callback_MoveUp(float InAxisValue)
{
    AddMovementInput(FVector::UpVector, InAxisValue * M_MovementStep);
    NeedUpdate(true);
}

void APawnFreeLook::Callback_MouseAxisX(float InAxisValue)
{
    if (BIT_CHECK(M_Flag, EState_MouseLButton) || BIT_CHECK(M_Flag, EState_MouseRButton))
        AddControllerYawInput(InAxisValue);
}

void APawnFreeLook::Callback_MouseAxisY(float InAxisValue)
{
    if (BIT_CHECK(M_Flag, EState_MouseLButton))
    {
        FRotator const ControlSpaceRot = Controller->GetControlRotation();
        FVector ScaledAxis = FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::X);
        ScaledAxis.Z = 0.0f; ScaledAxis.Normalize();
        AddMovementInput(ScaledAxis, InAxisValue * M_MovementStep);
        NeedUpdate(true);
    }
    else if (BIT_CHECK(M_Flag, EState_MouseRButton))
        AddControllerPitchInput(-InAxisValue);
}

void APawnFreeLook::Callback_MouseWheelAxis(float InAxisValue)
{
    if (!FMath::IsNearlyZero(InAxisValue))
        Callback_MoveForward(InAxisValue * M_MovementStep);
}