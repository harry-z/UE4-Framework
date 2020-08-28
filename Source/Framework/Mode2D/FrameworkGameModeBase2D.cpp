// Copyright Epic Games, Inc. All Rights Reserved.


#include "FrameworkGameModeBase2D.h"
#include "PawnPanning2D.h"
#include "PlayerControllerPanning2D.h"

AFrameworkGameModeBase2D::AFrameworkGameModeBase2D()
{
	DefaultPawnClass = APawnPanning2D::StaticClass();
	PlayerControllerClass = APlayerControllerPanning2D::StaticClass();
}