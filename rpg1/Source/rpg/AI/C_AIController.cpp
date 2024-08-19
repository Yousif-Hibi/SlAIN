// Fill out your copyright notice in the Description page of Project Settings.


#include "C_AIController.h"
#include "C_MasterAI.h"
#include <BehaviorTree/BehaviorTree.h>
#include <Perception/AIPerceptionTypes.h>
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h" 
#include <rpg/SoulsLikeCharacter.h>
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "rpg/Component/C_StatsComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include <Kismet/GameplayStatics.h>
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseEvent.h"
#include "Navigation/CrowdFollowingComponent.h"


void AC_AIController::BeginPlay()
{
	Super::BeginPlay();


//	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AC_AIController::OnTargetPerceptionForgotten, 2.f, true);
}

AC_AIController::AC_AIController(FObjectInitializer const& ObjectInitializer) 
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
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
    // Create and configure the perception component only once
    UAIPerceptionComponent* PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));
    SetPerceptionComponent(*PerceptionComp);

    if (PerceptionComp)
    {
        // Create and configure sense configs
        sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
        hearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
        otherhearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Other Hearing Config"));
        DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage Config"));

        if (sightConfig)
        {
            sightConfig->SightRadius = 2000.0f;
            sightConfig->LoseSightRadius = sightConfig->SightRadius + 500.0f;
            sightConfig->PeripheralVisionAngleDegrees = 140.0f;
            sightConfig->SetMaxAge(5.0f);
            sightConfig->AutoSuccessRangeFromLastSeenLocation = 5.0f;
            sightConfig->DetectionByAffiliation.bDetectEnemies = true;
            sightConfig->DetectionByAffiliation.bDetectFriendlies = true;
            sightConfig->DetectionByAffiliation.bDetectNeutrals = true;

            PerceptionComp->ConfigureSense(*sightConfig);
        }

        if (hearingConfig)
        {
            hearingConfig->HearingRange = 2000.0f;
            hearingConfig->SetMaxAge(5.0f);
            hearingConfig->DetectionByAffiliation.bDetectEnemies = true;
            hearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
            hearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

            PerceptionComp->ConfigureSense(*hearingConfig);
        }

        if (otherhearingConfig)
        {
            otherhearingConfig->HearingRange = 1000.0f;
            otherhearingConfig->SetMaxAge(5.0f);
            otherhearingConfig->DetectionByAffiliation.bDetectEnemies = true;
            otherhearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
            otherhearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

            PerceptionComp->ConfigureSense(*otherhearingConfig);
        }

        if (DamageConfig)
        {
            PerceptionComp->ConfigureSense(*DamageConfig);
        }

        // Set the dominant sense once
        PerceptionComp->SetDominantSense(*sightConfig->GetSenseImplementation());

        // Add delegates only once
        PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AC_AIController::OnTargetDetected);
        PerceptionComp->OnTargetPerceptionForgotten.AddDynamic(this, &AC_AIController::OnTargetPerceptionForgotten);

        // Use a different name for the local variable to avoid hiding class members
        TArray<AActor*> PerceivedActors;
        PerceptionComp->GetKnownPerceivedActors(PerceptionComp->GetDominantSense(), PerceivedActors);
    }
}





void AC_AIController::OnTargetDetected(AActor* Actor, FAIStimulus const stimulus)
{
	playerCharacter = Cast<ASoulsLikeCharacter>(Actor);
	if ( playerCharacter ) {
		UBlackboardComponent* BlackboardComp = GetBlackboardComponent();

		if (BlackboardComp)
		{
			bool bWasSuccessfullySensed = stimulus.WasSuccessfullySensed();
			BlackboardComp->SetValueAsBool("CanSeePlayer", bWasSuccessfullySensed);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Blackboard component is null"));
		}
	}
	if (stimulus.GetAge()==4.0f) {
		UE_LOG(LogTemp, Warning, TEXT("forgot"));
	}
	if(!stimulus.IsActive()) {
		
		
	
	}
}

void AC_AIController::OnTargetPerceptionForgotten(AActor* Actor)
{
	//if (KnownPerceivedActors.Num() ==0 ) {
		UE_LOG(LogTemp, Warning, TEXT("forgot"));
	//}
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

