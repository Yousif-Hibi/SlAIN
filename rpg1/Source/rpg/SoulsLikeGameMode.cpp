// Copyright Epic Games, Inc. All Rights Reserved.

#include "SoulsLikeGameMode.h"
#include "SoulsLikeCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASoulsLikeGameMode::ASoulsLikeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/CombatSystem/Blueprints/BP_CombatCharacter.BP_CombatCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
