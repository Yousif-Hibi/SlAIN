// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RegenHealth.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UBTTask_RegenHealth : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	explicit  UBTTask_RegenHealth(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	void RegenerateHealthStep();
	void FinishTask();

	UBehaviorTreeComponent* OwnerCompRef;
	int32 RegenerationSteps;
	FTimerHandle RegenTimerHandle;
	class   UParticleSystemComponent* RegenParticleSystemComponent;
};
