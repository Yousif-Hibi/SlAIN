// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_HealthCheck.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UBTService_HealthCheck : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:

	UBTService_HealthCheck();
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	class UC_StatsComponent* statsComponent;

};
