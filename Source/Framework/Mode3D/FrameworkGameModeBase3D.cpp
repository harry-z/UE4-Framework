// Copyright Epic Games, Inc. All Rights Reserved.


#include "FrameworkGameModeBase3D.h"
#include "PawnFreeLook.h"
#include "PlayerControllerFreeLook.h"

AFrameworkGameModeBase3D::AFrameworkGameModeBase3D()
{
	DefaultPawnClass = APawnFreeLook::StaticClass();
	PlayerControllerClass = APlayerControllerFreeLook::StaticClass();
}
