// Copyright Epic Games, Inc. All Rights Reserved.

#include "UFOCPPGameMode.h"
#include "UFOCPPPawn.h"

AUFOCPPGameMode::AUFOCPPGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = AUFOCPPPawn::StaticClass();
}

