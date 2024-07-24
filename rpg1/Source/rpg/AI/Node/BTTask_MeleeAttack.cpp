// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MeleeAttack.h"
#include "rpg/AI/C_MasterAI.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "rpg/AI/C_AIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/LatentActionManager.h"
#include "Animation/AnimMontage.h"
#include "rpg/Interfaces/Combat_CI.h"
#include "Animation/AnimInstance.h"
#include "rpg/Component/C_StatsComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "rpg/SoulsLikeCharacter.h"


UBTTask_MeleeAttack::UBTTask_MeleeAttack()
{
	NodeName = TEXT("MeleeAttack");
}

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Ensure the blackboard component is valid
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
  
    // Check if the AI is out of range
    bool OutOfRange = !BlackboardComp->GetValueAsBool(GetSelectedBlackboardKey());

    // Ensure the controller is valid
    AAIController* Controller = OwnerComp.GetAIOwner();
    if (!Controller)
    {
        UE_LOG(LogTemp, Warning, TEXT("AIController is null!"));
        return EBTNodeResult::Failed;
    }

    // Ensure the AI character is valid
    AC_MasterAI* AICharacter = Cast<AC_MasterAI>(Controller->GetPawn());
    if (!AICharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("AICharacter is null!"));
        return EBTNodeResult::Failed;
    }

    // Ensure the AI character implements ICombat_CI
    ICombat_CI* CombatInterface = Cast<ICombat_CI>(AICharacter);
    if (!CombatInterface)
    {
        UE_LOG(LogTemp, Warning, TEXT("AICharacter does not implement ICombat_CI!"));
        return EBTNodeResult::Failed;
    }

    // Check if montage has finished
    if (MontageHasFinished(AICharacter))
    {
        CombatInterface->Execute_AIAttack(AICharacter);
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}

bool UBTTask_MeleeAttack::MontageHasFinished(AC_MasterAI* const AICharacter)
{
    // Ensure the mesh and anim instance are valid
    UAnimInstance* AnimInstance = AICharacter->GetMesh()->GetAnimInstance();
    if (!AnimInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("AnimInstance is null for AICharacter: %s"), *AICharacter->GetName());
        return true; // Return true to avoid blocking other actions if anim instance is null
    }

    return AnimInstance->Montage_GetIsStopped(AICharacter->GetAIAttackMontage());
}
