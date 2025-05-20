// Copyright Epic Games, Inc. All Rights Reserved.

#include "proyectoGameMode.h"
#include "proyectoCharacter.h"
#include "UObject/ConstructorHelpers.h"

AproyectoGameMode::AproyectoGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
