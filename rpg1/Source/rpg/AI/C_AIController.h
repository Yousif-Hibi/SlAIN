// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Perception/AIPerceptionTypes.h>
#include "CoreMinimal.h"
#include "AIController.h"
#include "C_AIController.generated.h"


/**
 * 
 */
UCLASS()
class RPG_API AC_AIController : public AAIController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
public:

	explicit AC_AIController(FObjectInitializer const& ObjectInitializer);
 
	UPROPERTY(EditAnywhere)
	class UEnvQuery* FindingEQS;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	 class ASoulsLikeCharacter* playerCharacter;
	 UFUNCTION(BlueprintPure,BlueprintCallable)
	 ASoulsLikeCharacter* GetCharacter();
protected:
	virtual void OnPossess(APawn* InPawn) override;
private:
	class UAISenseConfig_Sight* sightConfig;
	class UAISenseConfig_Hearing* hearingConfig;
	UAISenseConfig_Hearing* otherhearingConfig;
	class UAISenseConfig_Damage* DamageConfig;
	void SetUpPerceptionSystem();
	class UBehaviorTree* tree;
	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const stimulus);

	UFUNCTION()
	void OnTargetPerceptionForgotten(AActor* Actor);
	


	FTimerHandle TimerHandle;
	void SetPlayerTarget();

	
	TArray<AActor*> KnownPerceivedActors;

};
