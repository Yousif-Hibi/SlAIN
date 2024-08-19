// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CanAttackPlayer.generated.h"

/**
 * 
 */
UCLASS()
class  UBTService_CanAttackPlayer : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_CanAttackPlayer();
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
