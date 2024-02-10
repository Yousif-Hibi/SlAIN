// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "CoreMinimal.h"
#include "rpg/Enums/E_CombatType.h"

#include "UObject/Interface.h"
#include "AnimInstance_CI.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAnimInstance_CI : public UInterface
{
	GENERATED_BODY()


	 

};

/**
 * 
 */
class  IAnimInstance_CI
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	

	virtual void UpdateCombatType(TEnumAsByte< ECombatType> combatType){}

	virtual  void  UpdateCombatEnabled(bool bAttachToHand) {}
};
 