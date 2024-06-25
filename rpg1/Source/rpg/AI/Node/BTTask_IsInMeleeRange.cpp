// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_IsInMeleeRange.h"
#include "rpg/AI/C_MasterAI.h"
#include "rpg/AI/C_AIController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"



UBTTask_IsInMeleeRange::UBTTask_IsInMeleeRange(FObjectInitializer const& ObjectInitializer)
{
	
	NodeName = TEXT("is in Range");
}

EBTNodeResult::Type UBTTask_IsInMeleeRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto const controler = OwnerComp.GetAIOwner()) {
		if (auto* const AIcharacter = Cast <AC_MasterAI>(controler->GetPawn())) {
			if (auto* const PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) {
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(
					GetSelectedBlackboardKey(),
					AIcharacter->GetDistanceTo(PlayerCharacter) <= meleeRange);
				UE_LOG(LogTemp, Warning, TEXT("asdas %f"), AIcharacter->GetDistanceTo(PlayerCharacter));
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
