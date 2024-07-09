// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_MasterAI.h"
#include "C_mageAI.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AC_mageAI : public AC_MasterAI
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AC_mageAI(); 

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	   



	UFUNCTION(BlueprintImplementableEvent)
	void ChangeHealth();

	UFUNCTION()
	void Teleport(FVector Location);

	UFUNCTION()
	void EndTeleport();
	void BeginPlay();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystem* TeleportParticalTrial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	 UParticleSystem* TeleportParticalImpact;
	 APatrolPath* GetPatrolPath() const;

	 // Set the patrol path
	 void SetPatrolPath(APatrolPath* NewPatrolPath);


	 FTimerHandle TimerHandle;


};
