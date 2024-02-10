// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoulsLikeCharacter.h"
#include "DummyCharacter.generated.h"

/**
 * 
 */

class AC_BaseEquippable;

UCLASS()
class RPG_API ADummyCharacter : public ASoulsLikeCharacter
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public :
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<TSubclassOf<  AC_BaseEquippable>> StartingEquipmeent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf <AC_BaseEquippable> Equip;
	
};
