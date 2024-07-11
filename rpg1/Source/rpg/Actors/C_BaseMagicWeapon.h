// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_BaseWeapon.h"
#include "C_BaseMagicWeapon.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AC_BaseMagicWeapon : public AC_BaseWeapon
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AC_BaseMagicWeapon();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public : 
	UPROPERTY(EditAnywhere)
	class UC_CollisionCombonent* ProjectialCollisionCombonent;
	
	UPROPERTY(EditAnywhere)
	USceneComponent* ProjectileSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystemComponent* Particales;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* StartParticales;

	UPROPERTY()
	float  magicDameges ;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	 TSubclassOf< class AC_magicSpell > magicSpell;
	 UPROPERTY(EditAnywhere, BlueprintReadWrite)
	  AC_magicSpell *  Spell;
	
	 UPROPERTY(EditAnywhere)
	 bool CombatEnabled=true;

	 void ToggleCombat(bool bEnableCombate);

	 UFUNCTION()
	 void fire();
	 UFUNCTION()
	 void MagicDamegeSet(float temp);



};
