#include "BTTask_RegenHealth.h"
#include "rpg/AI/C_MasterAI.h"
#include "rpg/AI/C_AIController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "rpg/Component/C_StatsComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"



UBTTask_RegenHealth::UBTTask_RegenHealth(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("RegenHealth");
}

EBTNodeResult::Type UBTTask_RegenHealth::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerCompRef = &OwnerComp;
	RegenerationSteps = 10;  // Total number of regeneration steps

	// Start the particle effect
	if (auto const Controller = OwnerComp.GetAIOwner())
	{
		if (auto* const AICharacter = Cast<AC_MasterAI>(Controller->GetPawn()))
		{
			if (AICharacter->healParticals)
			{
				// Attach to the root bone or a specific socket like "foot_l" or "foot_r"
				RegenParticleSystemComponent = UGameplayStatics::SpawnEmitterAttached(
					AICharacter->healParticals,
					AICharacter->GetMesh(),  // Attach to the character's mesh
					NAME_None,  // You can replace NAME_None with the specific socket name if needed
					FVector::ZeroVector,  // Offset if needed
					FRotator::ZeroRotator,
					EAttachLocation::SnapToTargetIncludingScale,
					true  // Auto destroy
				);
			}
		}
	}

	// Start the first step of health regeneration
	RegenerateHealthStep();

	return EBTNodeResult::InProgress;
}

void UBTTask_RegenHealth::RegenerateHealthStep()
{
	if (auto const Controller = OwnerCompRef->GetAIOwner())
	{
		if (auto* const AICharacter = Cast<AC_MasterAI>(Controller->GetPawn()))
		{
			if (AICharacter->StatsComponent)
			{
				AICharacter->StatsComponent->RegenHealth();
				RegenerationSteps--;

				if (RegenerationSteps > 0)
				{
					// Set a timer to call this function again after 2 seconds
					FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &UBTTask_RegenHealth::RegenerateHealthStep);
					AICharacter->GetWorldTimerManager().SetTimer(RegenTimerHandle, TimerDelegate, 2.0f, false);
				}
				else
				{
					// Finish the task after all steps are completed
					FinishTask();
				}
			}
		}
	}
}

void UBTTask_RegenHealth::FinishTask()
{
	// Destroy the particle effect
	if (RegenParticleSystemComponent)
	{
		RegenParticleSystemComponent->DestroyComponent();
		RegenParticleSystemComponent = nullptr;
	}

	FinishLatentTask(*OwnerCompRef, EBTNodeResult::Succeeded);
}

void UBTTask_RegenHealth::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	// Ensure the particle effect is destroyed if the task is aborted
	if (RegenParticleSystemComponent)
	{
		RegenParticleSystemComponent->DestroyComponent();
		RegenParticleSystemComponent = nullptr;
	}
}
