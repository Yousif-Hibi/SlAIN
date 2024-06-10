// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "rpg/Enums/E_DamageType.h"
#include "rpg/Enums/E_MovementSpeedMode.h"
#include "rpg/Enums/E_CharacterAction.h"
#include "rpg/Enums/E_ChartacterState.h"
#include "GameplayTagContainer.h"
#include "rpg/Interfaces/Combat_CI.h"
#include "rpg/Interfaces/Interactive_CI.h"
#include "rpg/Interfaces/Targeting_CI.h"
#include "GameFramework/Actor.h"
#include "rpg/Interfaces/AnimInstance_CI.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C_MasterAI.generated.h"




class AC_BaseWeapon;
class UC_CombatComponent;
class AActor;
class UC_StatsComponent;
class UManger;


UCLASS()
class RPG_API AC_MasterAI : public ACharacter, public  IAnimInstance_CI, public  ICombat_CI,public ITargeting_CI
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AC_MasterAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	






	//UFUNCTION
public:
	UFUNCTION(BlueprintCallable)
	void ToggleCombat();
	
	UFUNCTION(BlueprintCallable)
	bool CanPreformToggleCombat();

	UFUNCTION(BlueprintCallable)
	bool CanPreFormAttack();

	UFUNCTION(BlueprintCallable)
	bool CanPreformDodge();
	UFUNCTION(BlueprintCallable)
	bool CanPreformJump();

	UFUNCTION(BlueprintCallable)
	bool CanRecieveHitreaction();

	UFUNCTION(BlueprintCallable)
	void  PreformDodge(int32 MontageIndex, bool bRandomIndex);
	UFUNCTION()
	void AttackEvent();
	UFUNCTION()
	void PerformAttack(TEnumAsByte< EChartacterAction> AttackType, int32 AttackIndex, bool bRandomIndex);

	UFUNCTION(BlueprintCallable)
	void  PreformAction(UAnimMontage* ActionMontage, EChartacterAction Action, EChartacterState state, int32 MontageIndex, bool bRandomIndex);
	UFUNCTION()
	void DamegeTaken(float inDamage);
	UFUNCTION()
	void EnableRagdoll();
	UFUNCTION()
	void ApplyHitReactionPhysicsVelocity(float initialSpeed);
	UFUNCTION()
	void PerformDeath();
	UFUNCTION()
	void PerformDeathAfterDelay();

	virtual  void ContinueAttack() override;
	virtual  void RestAttack() override;
	virtual FRotator  GetDesiredRotation() override;
	virtual  void RestCombat() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual bool CanRecieveDamage()override;
	virtual void Jump() override;
	virtual void SetCanMove(bool isCanMove)  override;
	virtual bool CanBeTarget() override;

	virtual bool ReserveAttakTokken(int32 Amount)  override;
	virtual void ReturnAttackTokken(int32 Amount)  override;

	UFUNCTION()
	bool IsValueInRange(float Value, float Min, float Max, bool InclusiveMin, bool InclusiveMax);
	UFUNCTION()
	TEnumAsByte < EMovementSpeedMode> GetMovmentSpeedMode();

	UFUNCTION()
	void OnStateBegin(EChartacterState CharacterState);
	UFUNCTION()
	void OnStateEnd(EChartacterState CharacterState);

	UFUNCTION()
	void OnActionBegin(EChartacterAction CharacterAction);
	UFUNCTION()
	void OnActionEnd(EChartacterAction CharacterAction);

	UFUNCTION()
	void SetMovmentSpeedMode(EMovementSpeedMode  NewMovmetSpeed);
	UFUNCTION(BlueprintImplementableEvent)
	void Track();
	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool IsTrack();
	  

	UFUNCTION()
	void DisableSprint();
	UFUNCTION(BlueprintCallable)
	void Fire();
	UFUNCTION()
	void ApplyHitReaction(EDamageType DamageType);
	UFUNCTION()
	void PerfformHitStun();
	UFUNCTION()
	void PerfformKnockdown();
	UFUNCTION()
	void SetIsMagic(bool isMagic);

	UFUNCTION()
	void returnTokken();
	UFUNCTION()
	UBehaviorTree* GetBehaviorTree();

	UFUNCTION()
	class APatrolPath* GetPatrolPath();

	UAnimMontage* GetAIAttackMontage();

	 int AIAttack_Implementation() override;
	//UPROPERTY
private:
	UPROPERTY(EditAnywhere)
	bool bCanMove = true;
	UPROPERTY(EditAnywhere)
	bool bHitFront = true;





	UPROPERTY(EditAnywhere)
	bool bCombatEnabled;
	UPROPERTY(EditAnywhere)
	float WeaponDamege = 20.0f;

	UPROPERTY(EditAnywhere)
	UAnimMontage* TempMontage;

	UPROPERTY(EditAnywhere)
	FName PelvisBoneName;





public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bisGettingTargeted = false;
	UPROPERTY(EditAnywhere)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AC_BaseWeapon> ToSpawn;

	UPROPERTY(EditAnywhere)
	AC_BaseWeapon* mainWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UC_CombatComponent* CombatComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UC_StatsComponent* StatsComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UManger* manger;
	UPROPERTY(EditAnywhere)
	bool bisMagic = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovmentSpeed")
	float WalkingSpeed = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovmentSpeed")
	float JoggingSpeed = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovmentSpeed")
	float SprintingSpeed = 700.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USoundCue* HitSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UParticleSystem* hitParticals;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	 UParticleSystem * healParticals;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECombatType> combatType;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDamageType> damageType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* LightAttack ;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* HitReaction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* backHitReaction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* KnockdownReaction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* GeettingUpFromKnockdownReaction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte < EMovementSpeedMode>  MovmentSpeedMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBehaviorTree * BehaviorTree ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APatrolPath* PatrolPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UC_CollisionCombonent * CollisionCombonent;


	UPROPERTY(EditAnywhere)
	bool IshasTokken=true;







private:
	UPROPERTY(EditAnywhere)
	FKey TempKey;

	UPROPERTY(EditAnywhere)
	UUserWidget* MainWidget;


	



	FTimerHandle tokkenTimerHandle;
	FTimerHandle SprintTimerHandle;

};


