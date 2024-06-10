// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ClearFouces.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UBTTask_ClearFouces : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	explicit  UBTTask_ClearFouces(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
