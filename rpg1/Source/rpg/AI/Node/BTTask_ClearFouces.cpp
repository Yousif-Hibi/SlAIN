// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ClearFouces.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include <rpg/AI/C_AIController.h>
#include <Kismet/GameplayStatics.h>
#include "BehaviorTree/BehaviorTreeTypes.h"

UBTTask_ClearFouces::UBTTask_ClearFouces(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("ClearFocusPlayer");
}

EBTNodeResult::Type UBTTask_ClearFouces::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const AIControlar = Cast<AC_AIController>(OwnerComp.GetAIOwner())) {

		if (auto* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey())))
		{
			// Set clear focus
			AIControlar->ClearFocus(EAIFocusPriority::Gameplay);


			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;

		}
	}

	return EBTNodeResult::Failed;
}
