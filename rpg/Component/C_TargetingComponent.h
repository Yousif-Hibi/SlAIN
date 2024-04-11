// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "rpg/Enums/Enums.h"
#include "Engine/EngineTypes.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_TargetingComponent.generated.h"



UCLASS(Blueprintable,ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UC_TargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UC_TargetingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	


public:	
	// Called every frame

	UPROPERTY(EditAnywhere)
	bool bIsTargeting = false;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere)
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere,Category="Initialization")
	float TargetingDistance = 900;

	UPROPERTY(EditAnywhere,Category = "Initialization")
	float TargetRadius = 50;

	UPROPERTY(EditAnywhere,Category = "Initialization")
	TEnumAsByte<ERotationMode> DefaultRotationMode;

	UPROPERTY(EditAnywhere, Category = "Initialization")
	TArray<TEnumAsByte<EObjectTypeQuery>> TargetObjectTypes;

	UPROPERTY(EditAnywhere)
	class ACharacter* OwnerCharacter;

	UPROPERTY(EditAnywhere)
	class AController * OwnerController;

	UPROPERTY(EditAnywhere)
	class AActor* TargetActor;

	UPROPERTY(EditAnywhere)
	float TargetInterpSpeed = 9;

	UPROPERTY(EditAnywhere)
	class UC_CombatComponent * combatComponent;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ERotationMode> CurrentRotationMode;

	UFUNCTION( BlueprintCallable)
	void SetIsTargeting(bool bEnableTargeting);


	UFUNCTION( BlueprintCallable)
	bool GetIsTargeting();

	UFUNCTION(BlueprintCallable)
	void SetTargetActor(AActor* NewTargetActor);


	UFUNCTION(BlueprintCallable)
	AActor* GetTargetActor();

	UFUNCTION(BlueprintCallable)
	AActor* FindTarget();

	UFUNCTION(BlueprintCallable)
	void UpdateTargetingControlRotation();

	UFUNCTION(BlueprintCallable)
	void SetRotationMode(TEnumAsByte<ERotationMode> NewRotationMode);

	UFUNCTION(BlueprintCallable)
	ERotationMode GetRotationMode() const;

	UFUNCTION(BlueprintCallable)
	void EnableLockon();

	UFUNCTION(BlueprintCallable)
	void disableLockOn();

	UFUNCTION(BlueprintCallable)
	void ToggleLockOn();

	



};
