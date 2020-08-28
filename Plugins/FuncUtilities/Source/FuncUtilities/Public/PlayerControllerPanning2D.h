#pragma once
#include "GameFramework/PlayerController.h"
#include "PlayerControllerPanning2D.generated.h"

UCLASS()
class FUNCUTILITIES_API APlayerControllerPanning2D : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerControllerPanning2D();
	virtual bool InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad) override;
	virtual bool InputAxis(FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad) override;

private:
	void CalculateOnePixelUnit();

private:
	enum {
		ELeftMouseButton = 0x01,
		ERightMouseButton = 0x02,
		EMiddleMouseButton = 0x04
	};
	uint8 M_MouseButtonFlag = 0;
	FVector2D M_OnePixelUnit;
	FVector2D M_MouseLocationWhenPressed;
};