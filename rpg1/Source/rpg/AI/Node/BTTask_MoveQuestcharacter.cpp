// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveQuestcharacter.h"
#include "rpg/AI/C_QuestKnight.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "rpg/AI/C_AIController.h"
#include "Kismet/KismetSystemLibrary.h"
UBTTask_MoveQuestcharacter::UBTTask_MoveQuestcharacter()
{
	NodeName = TEXT("MoveQuestcharacter");
}

EBTNodeResult::Type UBTTask_MoveQuestcharacter::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const Teleportlocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

	if (auto const controler = OwnerComp.GetAIOwner()) {
		if (auto* const AIcharacter = Cast <AC_QuestKnight>(controler->GetPawn())) {

			AIcharacter->Teleport(Teleportlocation);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
