#pragma once
#include "GameFramework/Pawn.h"
#include "PawnPanning2D.generated.h"

class UCameraComponent;

UCLASS()
class FUNCUTILITIES_API APawnPanning2D : public APawn
{
	GENERATED_BODY()

public:
	APawnPanning2D();
	void NotifyViewportResized(const FVector2D& InNewSize);
	void MoveCamera(const FVector2D& InDelta);
	void ZoomCamera(float InWheelValue);

private:
	UCameraComponent* M_CameraComponent;
};