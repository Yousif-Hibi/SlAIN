// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AxeSpin.h"
#include "rpg/AI/C_HumanoidEnemy.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "rpg/AI/C_AIController.h"
#include "Kismet/KismetSystemLibrary.h"


UBTTask_AxeSpin::UBTTask_AxeSpin()
{
	NodeName = TEXT("spine");
}

EBTNodeResult::Type UBTTask_AxeSpin::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const Newlocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

	if (auto const controler = OwnerComp.GetAIOwner()) {
		if (auto* const AIcharacter = Cast <AC_HumanoidEnemy>(controler->GetPawn())) {
			
			 
			controler->MoveToLocation(Newlocation);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
