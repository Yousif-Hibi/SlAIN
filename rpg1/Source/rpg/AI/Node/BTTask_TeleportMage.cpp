// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TeleportMage.h"
#include "rpg/AI/C_mageAI.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "rpg/AI/C_AIController.h"
#include "Kismet/KismetSystemLibrary.h"

UBTTask_TeleportMage::UBTTask_TeleportMage()
{
	NodeName = TEXT("TeleportMage");
}

EBTNodeResult::Type UBTTask_TeleportMage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const Teleportlocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
	
	if (auto const controler = OwnerComp.GetAIOwner()) {
		if (auto* const AIcharacter = Cast <AC_mageAI>(controler->GetPawn())) {

			AIcharacter->Teleport(Teleportlocation);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}
