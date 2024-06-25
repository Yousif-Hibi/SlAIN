// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPathPoint.h"
#include "rpg/AI/C_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <rpg/AI/C_HumanoidEnemy.h>
#include "rpg/AI/PatrolPath.h"

UBTTask_FindPathPoint::UBTTask_FindPathPoint(FObjectInitializer const& ObjectInitializer) :
    UBTTask_BlackboardBase{ ObjectInitializer }
{
    NodeName = TEXT("FindPathPoint");
}

EBTNodeResult::Type UBTTask_FindPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (auto* const controller = Cast<AC_AIController>(OwnerComp.GetAIOwner())) {

        if (auto* const BlackBored = OwnerComp.GetBlackboardComponent()) {//get black bored 

            auto const index = BlackBored->GetValueAsInt((GetSelectedBlackboardKey()));//get patrol path index
            
            if (auto* character = Cast<AC_MasterAI>(controller->GetPawn())) {
               auto const point = character->GetPatrolPath()->GetPatrolPoint(index);//get path
               auto const GlobalPoint = character->GetPatrolPath()->GetActorTransform().TransformPosition(point);//covet to global point 
               BlackBored->SetValueAsVector(PatrolPathVectorKey.SelectedKeyName, GlobalPoint);


               FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
               return EBTNodeResult::Succeeded;
            }
        }
    }
    return EBTNodeResult::Failed;
}
