// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Combat_CI.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombat_CI : public UInterface
{
	GENERATED_BODY() 
};

/**
 * 
 */
class  ICombat_CI
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:


	virtual  void ContinueAttack() {}

	virtual  void RestAttack() {}
	virtual  void RestCombat() {}
	virtual FRotator  GetDesiredRotation() { return FRotator(); }
	virtual bool CanRecieveDamage() { return false; }
	virtual  void SetCanMove(bool isCanMove) { }



	virtual bool ReserveAttakTokken(int32 Amount) { return false; }
	virtual void ReturnAttackTokken(int32 Amount) { }

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	int AIAttack();
};
