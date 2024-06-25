// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_MasterAI.h"
#include "C_HumanoidEnemy.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AC_HumanoidEnemy : public AC_MasterAI
{
	GENERATED_BODY()
	

	public:
		
		// Called every frame
		virtual void Tick(float DeltaTime) override;




		
		UFUNCTION(BlueprintImplementableEvent)
		void ChangeHealth();










};
