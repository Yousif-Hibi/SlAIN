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
	auto const OutOfRange = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
	if (OutOfRange) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	if (auto const controler = OwnerComp.GetAIOwner()) {
		if (auto* const AIcharacter = Cast <AC_MasterAI>(controler->GetPawn())) {
			if (auto* const icombat = Cast<ICombat_CI>(AIcharacter)) {
				if (MontageHasFinished(AIcharacter)) {
					icombat->Execute_AIAttack(AIcharacter);
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;

				}
			}
			
		}
	}
	return EBTNodeResult::Failed;
}

bool UBTTask_MeleeAttack::MontageHasFinished(AC_MasterAI* const AICharacter)
{

	return AICharacter->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(AICharacter->GetAIAttackMontage());
}
