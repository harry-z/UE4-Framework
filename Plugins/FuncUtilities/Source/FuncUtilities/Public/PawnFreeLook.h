#pragma once
#include "GameFramework/Pawn.h"
#include "KernelDefines.h"
#include "PawnFreeLook.generated.h"

UCLASS()
class FUNCUTILITIES_API APawnFreeLook : public APawn
{
	GENERATED_BODY()

public:
    APawnFreeLook();
    virtual void Tick(float DeltaTime) override;

protected:
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
    void Callback_LMouseButtonDown() { BIT_ADD(M_Flag, EState_MouseLButton); }
    void Callback_LMouseButtonUp() { BIT_REMOVE(M_Flag, EState_MouseLButton); }
    void Callback_RMouseButtonDown() { BIT_ADD(M_Flag, EState_MouseRButton); }
    void Callback_RMouseButtonUp() { BIT_REMOVE(M_Flag, EState_MouseRButton); }
    void Callback_MoveForward(float InAxisValue);
    void Callback_MoveRight(float InAxisValue);
    void Callback_MoveUp(float InAxisValue);
    void Callback_MouseAxisX(float InAxisValue);
    void Callback_MouseAxisY(float InAxisValue);
    void Callback_MouseWheelAxis(float InAxisValue);
    void Callback_IncMovementStep() { ++M_MovementStep; }
    void Callback_DecMovementStep() { M_MovementStep = FMath::Max(1, M_MovementStep - 1); }

    void NeedUpdate(bool bNeedUpdate) {
        if (bNeedUpdate)
            BIT_ADD(M_Flag, EState_Dirty);
        else
            BIT_REMOVE(M_Flag, EState_Dirty);
    }
    bool IsNeedUpdate() const { return BIT_CHECK(M_Flag, EState_Dirty); }

private:
    int32               M_MovementStep = 1;
    enum EState : uint16 {
        EState_Dirty = 0x0001,
        EState_MouseLButton = 0x0100,
        EState_MouseRButton = 0x0200
    };
    uint16              M_Flag = 0;
};