// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_IsInMeleeRange.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UBTTask_IsInMeleeRange : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	explicit  UBTTask_IsInMeleeRange(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float meleeRange = 150.f;

};
