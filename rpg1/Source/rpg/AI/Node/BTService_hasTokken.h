// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_hasTokken.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UBTService_hasTokken : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_hasTokken();
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
