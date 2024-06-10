// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "rpg/Enums/EState.h" 
#include "CoreMinimal.h"
#include "rpg/Structs/FBaseStat.h"

#include "Components/ActorComponent.h"
#include "C_StatsComponent.generated.h"




UCLASS(Blueprintable)
class RPG_API UC_StatsComponent : public UActorComponent
{
	GENERATED_BODY()



private:
	Estat temp;
	bool checkRegen;


public:	
	// Sets default values for this component's properties
	UC_StatsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
	
	UPROPERTY(EditAnywhere, Category = "StateRegen")
	float StaminaRegenRate = 2.0f;
	
	UPROPERTY(EditAnywhere)
	int32 tokkenAmount= 1;

	UPROPERTY(EditAnywhere)
	float LocalDamage = 0;

	UPROPERTY(EditAnywhere)
	float DelayCount= 0;
	UPROPERTY(EditAnywhere, Category = "BaseState")
	TMap<TEnumAsByte< Estat>,FBaseStat > BaseStat;

	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte< Estat>, float > CurrentState;


	UFUNCTION (BlueprintCallable, BlueprintPure, Category = "BaseState")
	float GetBaseStateValue(Estat stat );

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "BaseState")
	float GetMaxStateValue(Estat stat);


	UFUNCTION(BlueprintCallable, Category = "BaseState")
	void SetBaseStateValue(Estat stat,float Value);

	UFUNCTION(BlueprintCallable, Category = "BaseState")
	void SetMaxStateValue(Estat stat, float Value);

	UFUNCTION(BlueprintCallable )
	void SetCurrentStateValue(Estat stat, float Value);


	UFUNCTION(BlueprintCallable,  BlueprintPure)
	float GetCurrentStateValue(Estat stat);


	

	UFUNCTION(BlueprintCallable)
	void InitialState();

	UFUNCTION(BlueprintCallable)
	void ModifyCurrentStatValue(Estat stat,float Value, bool bShouldRegenrate);

	UFUNCTION(BlueprintCallable)
	void TakeDamage(float  InDamage);

	UFUNCTION(BlueprintCallable)
	void RegenStamina();
	UFUNCTION(BlueprintCallable)
	void RegenHealth();

	UFUNCTION(BlueprintCallable)
	void StartRegen(Estat stat);
	
	UFUNCTION(BlueprintCallable)
	void dealyRegen();

	
	UFUNCTION(BlueprintCallable)
	bool ReserveToken(int32 Amount);

	UFUNCTION(BlueprintCallable)
	void ReturnToken(int32 Amount);
















};
