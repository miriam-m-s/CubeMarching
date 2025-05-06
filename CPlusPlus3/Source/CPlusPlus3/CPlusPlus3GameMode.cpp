// Copyright Epic Games, Inc. All Rights Reserved.

#include "CPlusPlus3GameMode.h"
#include "CPlusPlus3Character.h"
#include "UObject/ConstructorHelpers.h"

ACPlusPlus3GameMode::ACPlusPlus3GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
