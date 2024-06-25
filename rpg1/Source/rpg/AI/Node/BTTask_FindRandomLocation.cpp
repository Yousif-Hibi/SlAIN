// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindRandomLocation.h"
#include <rpg/AI/C_AIController.h>
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer) :
	UBTTask_BlackboardBase{ ObjectInitializer }
{
	NodeName = "FindRandomLocation";
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	if (auto* const  controller = Cast<AC_AIController>(OwnerComp.GetAIOwner())) {
		if (auto* const AICharecter = controller->GetPawn()) {
			auto const OriginVector = AICharecter->GetActorLocation();
			if (auto* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld())) {//get navigation system and make random location
				FNavLocation navLocation;
				if (NavSystem->GetRandomPointInNavigableRadius(OriginVector, SearchRadius, navLocation)) {
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(),navLocation.Location);
				}
				 
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}








	return EBTNodeResult::Failed;
}
