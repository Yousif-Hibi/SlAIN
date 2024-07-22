// Copyright Epic Games, Inc. All Rights Reserved.

#include "SoulsLikeGameMode.h"
#include "SoulsLikeCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASoulsLikeGameMode::ASoulsLikeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Script/Engine.Blueprint'/Game/Dynamic/CombatSystem/Blueprints/BP_CombatCharcter.BP_CombatCharcter'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
