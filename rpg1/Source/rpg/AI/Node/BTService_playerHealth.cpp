// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_playerHealth.h"
#include "rpg/SoulsLikeCharacter.h"
#include "rpg/C_PlayerController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "rpg/Component/C_StatsComponent.h"


UBTService_playerHealth::UBTService_playerHealth()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Healch ckeck");
}

void UBTService_playerHealth::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto const controler = OwnerComp.GetOwner()) {
		
			if (auto* const PlayerCharacter =Cast<ASoulsLikeCharacter>( UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))) {
				
					
					OwnerComp.GetBlackboardComponent()->SetValueAsFloat(
						GetSelectedBlackboardKey(),
						PlayerCharacter->StatsComponents->GetCurrentStateValue(Estat::Health));
				

			}
		
	}
}