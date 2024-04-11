// Fill out your copyright notice in the Description page of Project Settings.


#include "C_AIController.h"
#include "C_MasterAI.h"
#include <BehaviorTree/BehaviorTree.h>
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h" 
#include <rpg/SoulsLikeCharacter.h>
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "rpg/Component/C_StatsComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include <Kismet/GameplayStatics.h>

AC_AIController::AC_AIController(FObjectInitializer const& ObjectInitializer)
{
	SetUpPerceptionSystem();

}

ASoulsLikeCharacter* AC_AIController::GetCharacter()
{
	playerCharacter = Cast<ASoulsLikeCharacter> (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	return playerCharacter;
}

void AC_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (auto* AICharcter = Cast<AC_MasterAI>(InPawn)) {
		tree = AICharcter->GetBehaviorTree();
		if (tree) {

			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AC_AIController::SetPlayerTarget, 2.f, false);

		}
	}
}

void AC_AIController::SetUpPerceptionSystem()
{
	sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	hearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	otherhearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("other Hearing Config"));
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage Config"));
		if (sightConfig && hearingConfig && DamageConfig) {
			SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception sight Component")));
			
			sightConfig->SightRadius=1000.0f;
			sightConfig->LoseSightRadius = sightConfig->SightRadius + 20.0f;
			sightConfig->PeripheralVisionAngleDegrees = 90.0f;
			sightConfig->SetMaxAge(5.0f);
			sightConfig->AutoSuccessRangeFromLastSeenLocation = 20.0f;
			sightConfig->DetectionByAffiliation.bDetectEnemies = true;
			sightConfig->DetectionByAffiliation.bDetectFriendlies = true;
			sightConfig->DetectionByAffiliation.bDetectNeutrals = true;

			GetPerceptionComponent()->SetDominantSense(*sightConfig->GetSenseImplementation());
			GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AC_AIController::OnTargetDetected);
			GetPerceptionComponent()->ConfigureSense(*sightConfig);
		
			
			hearingConfig->HearingRange = 500;
			hearingConfig->SetMaxAge(5.0f);

			hearingConfig->DetectionByAffiliation.bDetectEnemies = true;
			hearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
			hearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

			GetPerceptionComponent()->SetDominantSense(*hearingConfig->GetSenseImplementation());
			GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AC_AIController::OnTargetDetected);
			GetPerceptionComponent()->ConfigureSense(*hearingConfig);
			otherhearingConfig->HearingRange = 1000;
			otherhearingConfig->SetMaxAge(5.0f);

			otherhearingConfig->DetectionByAffiliation.bDetectEnemies = true;
			otherhearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
			otherhearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

			GetPerceptionComponent()->SetDominantSense(*otherhearingConfig->GetSenseImplementation());
			GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AC_AIController::OnTargetDetected);
			GetPerceptionComponent()->ConfigureSense(*otherhearingConfig);
			
			GetPerceptionComponent()->ConfigureSense(*DamageConfig);
		}



}



void AC_AIController::OnTargetDetected(AActor* Actor, FAIStimulus const stimulus)
{
	playerCharacter = Cast<ASoulsLikeCharacter>(Actor);
	if ( playerCharacter ) {
		GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", stimulus.WasSuccessfullySensed());
	}
}

void AC_AIController::SetPlayerTarget()
{
	
		UBlackboardComponent* blackBoard;

		UseBlackboard(tree->BlackboardAsset, blackBoard);
		Blackboard = blackBoard;
		RunBehaviorTree(tree);
		auto* character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		if (character)
		{

			Blackboard->SetValueAsObject(TEXT("player"), character);

		}
	}

