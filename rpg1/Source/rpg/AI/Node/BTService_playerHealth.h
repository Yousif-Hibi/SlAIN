// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_playerHealth.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UBTService_playerHealth : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:

	UBTService_playerHealth();
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	class UC_StatsComponent* statsComponent;
};
