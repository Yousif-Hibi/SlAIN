// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_IncrementPathIndex.h"
#include <rpg/AI/C_AIController.h>
#include "rpg/AI/C_MasterAI.h"
#include <rpg/AI/PatrolPath.h>
#include "BehaviorTree/BlackboardComponent.h"
UBTTask_IncrementPathIndex::UBTTask_IncrementPathIndex(FObjectInitializer const& ObjectInitializer):
	UBTTask_BlackboardBase{ObjectInitializer}
{
	NodeName = TEXT("Increment Path Index"); 
}

EBTNodeResult::Type UBTTask_IncrementPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	if (auto* const  controller = Cast<AC_AIController>(OwnerComp.GetAIOwner())) {
		if (auto* const Character = Cast<AC_MasterAI>(controller->GetPawn())) {

			if (auto* const BlackBored = OwnerComp.GetBlackboardComponent()) {//get black bored 
				auto const path = Character->GetPatrolPath();
				auto const NoOfPoints = path->num();
				auto  const  minIndex = 0;
				auto const MaxIndex = NoOfPoints - 1;
				auto  Index = BlackBored->GetValueAsInt(GetSelectedBlackboardKey());

				if (bBiDiractional) {
					if (Index >= MaxIndex && (Direction == EDirectionType::Forward)) {
						Direction = EDirectionType::Reverse;

					}
					else if (Index <= minIndex && (Direction == EDirectionType::Reverse)) {

						Direction = EDirectionType::Forward;
					}
				}

				BlackBored->SetValueAsInt(GetSelectedBlackboardKey(),
					(Direction == EDirectionType::Forward ? ++Index : --Index) % NoOfPoints);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
		return EBTNodeResult::Failed;
}
