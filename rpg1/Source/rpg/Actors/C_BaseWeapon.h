// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "rpg/Enums/E_DamageType.h"
#include "rpg/Enums/E_CollisionPart.h"
#include "rpg/Enums/E_CombatType.h"
#include "rpg/Interfaces/AnimInstance_CI.h"
#include "rpg/Enums/E_CharacterAction.h"
#include "CoreMinimal.h"
#include "C_BaseEquippable.h"
#include "C_BaseWeapon.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AC_BaseWeapon : public AC_BaseEquippable
{
	GENERATED_BODY()
public:
	AC_BaseWeapon();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//Functions
public:
	void OnEquipped() override;
	void OnAIEquipped() override;
	UFUNCTION(BlueprintCallable) 
	void OnHit(FHitResult hit);
	UFUNCTION(BlueprintCallable)
	void setDmg(int32 dmg);

	UFUNCTION(BlueprintCallable)
	void SimulateWeaponPhysics();
	UFUNCTION(BlueprintCallable)
	void StopSimulateWeaponPhysics();
	UFUNCTION(BlueprintCallable)
	TArray<UAnimMontage*> GetActionMontages(EChartacterAction ChartacterAction);
	UFUNCTION(BlueprintCallable)
	float GetStatCostForAction();
	UFUNCTION(BlueprintCallable)
	int32 GetDamage(bool isAI);
	UFUNCTION(BlueprintCallable)
	void ActivateCollision(ECollisionPart collisionPart);
	UFUNCTION(BlueprintCallable)
	void DeactivateCollision(ECollisionPart collisionPart);

	UFUNCTION(BlueprintCallable)
	void ToggleCombat(bool bEnableCombat);
	//Proprtys
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Initialization")
	FName HandSocketName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Initialization")
	bool bIsAI = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Initialization")
	bool bIsAttachedToHand=false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Initialization")
	bool bCombat = false;
	UPROPERTY()
	int32 Damege;
	UPROPERTY()
	int32 weapDamege;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* EquipWeapenMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* UnEquipWeapenMontage;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TEnumAsByte< ECombatType> CombatType;
	UPROPERTY(EditAnywhere)
	class UC_CombatComponent* CombatComponent;

	UPROPERTY(EditAnywhere)
	TArray<UAnimMontage*> AttackMontage;
	UPROPERTY(EditAnywhere)
	TArray<UAnimMontage*> SprintAttackMontage;
	UPROPERTY(EditAnywhere)
	TArray<UAnimMontage*> HeavyAttackMontage;

	UPROPERTY(EditAnywhere)
	TArray<UAnimMontage*> ChargedAttackMontage;

	UPROPERTY(EditAnywhere)
	TArray<UAnimMontage*> FallingAttackMontage;

	UPROPERTY(EditAnywhere)
	TArray<UAnimMontage*> EnterCombatMontage;

	UPROPERTY(EditAnywhere)
	TArray<UAnimMontage*> ExitCombatMontage;

	UPROPERTY(EditAnywhere)
	TArray<UAnimMontage*> DodgeMontage;
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDamageType> DamageType;
	
	UPROPERTY(EditAnywhere)
	class UC_CollisionCombonent* CollisionCombonent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Stats")
	TMap<TEnumAsByte<EChartacterAction>, float> ActionStatCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TMap<TEnumAsByte<EChartacterAction>, float> ActionDamageMultiplyer;
	UPROPERTY(EditAnywhere)
	class  UManger* manger;
	UPROPERTY(EditAnywhere)
	UManger* aimanger;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UDamageType> AttackDamageType ;


};
