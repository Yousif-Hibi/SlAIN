// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FIndPlayerLocation.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include <NavigationSystem.h>
#include "BehaviorTree/BlackboardComponent.h"
UBTTask_FIndPlayerLocation::UBTTask_FIndPlayerLocation(FObjectInitializer const& ObjectInitializer) :
	UBTTask_BlackboardBase{ ObjectInitializer }
{
	NodeName = "FindPlayerLocation";
}

EBTNodeResult::Type UBTTask_FIndPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) {

		auto const characterLocation = character->GetActorLocation();
		if (SearchRandom) {
			FNavLocation navLocation;
			if (auto* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld())) {//get navigation system and make random location
				
				if (NavSystem->GetRandomPointInNavigableRadius(characterLocation, SearchRadius, navLocation)) {
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), navLocation.Location);
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
		}
		else {//follow player location
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), characterLocation);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}


	}
		



	return EBTNodeResult::Failed;
}
