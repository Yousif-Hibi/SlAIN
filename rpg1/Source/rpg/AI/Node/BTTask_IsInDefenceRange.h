// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_IsInDefenceRange.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UBTTask_IsInDefenceRange : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	explicit  UBTTask_IsInDefenceRange(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float DeffenceRange = 350.f;
};
