// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Targeting_CI.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTargeting_CI : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPG_API ITargeting_CI
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual bool CanBeTarget() { return false; }


};
