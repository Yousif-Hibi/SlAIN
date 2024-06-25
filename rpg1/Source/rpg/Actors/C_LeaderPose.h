// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_BaseEquippable.h"
#include "C_LeaderPose.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AC_LeaderPose : public AC_BaseEquippable
{
	GENERATED_BODY()
	

	//Function
public : 
	
	void AttachActor(FName SocketName);
	
	 void OnEquip();
	void OnUnequipped();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float ArmarValue = 25.0f;

};
