// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_CombatComponent.generated.h"
   

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UC_CombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UC_CombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
//Ufnctions
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	


	UFUNCTION(BlueprintCallable)
	void RestAttack();

	UFUNCTION(BlueprintCallable)
	void SetCombatEnabled(bool isEnabled);
	UFUNCTION(BlueprintCallable)
	void SetAICombatEnabled(bool isEnabled);
	UFUNCTION(BlueprintCallable)
	bool IsCombatEnabled();

	

	UFUNCTION(BlueprintCallable)
	void  SetAttackCount(int Count);

	UFUNCTION(BlueprintCallable)
	void  SetAttackSaved(bool attackSaved);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool  GetAttackSaved();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32  GetAttackCount();

	UFUNCTION(BlueprintCallable)
	void SetMainWeapon(AC_BaseWeapon* Weapon);
	UFUNCTION(BlueprintCallable)
	AC_BaseWeapon* GetMainWeapon();

	UPROPERTY(EditAnywhere)
	bool bmaigcenabled = false;
	//UPROPERTYS
private:


	UPROPERTY(EditAnywhere)
	int32 attackCount = 0;

	UPROPERTY(EditAnywhere)
	bool bCombatEnabled = false;
	
	UPROPERTY(EditAnywhere)
	bool bAttackSaved = false;

	

	UPROPERTY(EditAnywhere)
	class AC_BaseWeapon* MainWeapon;


};
