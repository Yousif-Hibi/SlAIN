// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_HealthCheck.h"
#include "rpg/AI/C_MasterAI.h"
#include "rpg/AI/C_AIController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "rpg/Component/C_StatsComponent.h"


UBTService_HealthCheck::UBTService_HealthCheck()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Healch ckeck");
}

void UBTService_HealthCheck::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto const controler = OwnerComp.GetAIOwner()) {
		if (auto* const AIcharacter = Cast <AC_MasterAI>(controler->GetPawn())) {
			if (auto* const PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) {
				
					
					OwnerComp.GetBlackboardComponent()->SetValueAsFloat(
						GetSelectedBlackboardKey(),
						AIcharacter->StatsComponent->GetCurrentStateValue(Estat::Health));
				

			}
		}
	}
}
