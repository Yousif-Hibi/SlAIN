// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CanAttackPlayer.h"
#include "rpg/AI/C_MasterAI.h"
#include "rpg/AI/C_AIController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "rpg/Component/C_StatsComponent.h"
#include "rpg/SoulsLikeCharacter.h"
UBTService_CanAttackPlayer::UBTService_CanAttackPlayer()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Can Attack  Player ckeck");
}

void UBTService_CanAttackPlayer::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto const controler = OwnerComp.GetAIOwner()) {
		if (auto* const AIcharacter = Cast <AC_MasterAI>(controler->GetPawn())) {
			if (auto* const PlayerCharacter = Cast <ASoulsLikeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))) {
				
					
					OwnerComp.GetBlackboardComponent()->SetValueAsBool(
						GetSelectedBlackboardKey(),
						PlayerCharacter->ReputationIndex>=10);
				

			}
		}
	}
}
