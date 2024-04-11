// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_FocusTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include <rpg/AI/C_AIController.h>
#include <Kismet/GameplayStatics.h>
#include "BehaviorTree/BehaviorTreeTypes.h"
UMyBTTask_FocusTarget::UMyBTTask_FocusTarget(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("FocusPlayer");
}

EBTNodeResult::Type UMyBTTask_FocusTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const AIControlar = Cast<AC_AIController>(OwnerComp.GetAIOwner())) {
		
		if (auto* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey())))
		{
			// Set focus on the specified actor
			AIControlar->SetFocus(TargetActor);


			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;

		}
	}

	return EBTNodeResult::Failed;
}
