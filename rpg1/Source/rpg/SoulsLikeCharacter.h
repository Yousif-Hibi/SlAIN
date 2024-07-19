// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once


#include "Enums/E_DamageType.h"
#include "Enums/E_MovementSpeedMode.h"
#include "Enums/E_CharacterAction.h"
#include "Enums/E_ChartacterState.h"
#include "GameplayTagContainer.h"
#include "Interfaces/Combat_CI.h"
#include "Interfaces/Interactive_CI.h"
#include "GameFramework/Actor.h"
#include "Interfaces/AnimInstance_CI.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SoulsLikeCharacter.generated.h"


class AC_BaseWeapon;
class UC_CombatComponent;
class AActor;
class UC_StatsComponent;
class UManger;
class UC_TargetingComponent;


UCLASS(config=Game)
class ASoulsLikeCharacter : public ACharacter, public  IAnimInstance_CI ,public  ICombat_CI
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ASoulsLikeCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual void Tick(float DeltaTime) override;
	

	

protected:
	// Called when the game starts or when spawned
	 void BeginPlay() override;
	 //UFUNCTION
public:
	UFUNCTION(BlueprintCallable)
	void ToggleCombat();
	UFUNCTION(BlueprintCallable)
	void ToggleLockOn();

	UFUNCTION(BlueprintCallable)
	void Intract();

	UFUNCTION(BlueprintCallable)
	void LightAttack();
	UFUNCTION(BlueprintCallable)
	void Pause();
	UFUNCTION(BlueprintCallable)
	void Dodge();
	UFUNCTION(BlueprintCallable)
	void ToggleWalk();
	UFUNCTION(BlueprintCallable)
	void Sprint();
	UFUNCTION(BlueprintCallable)
	void StopSprint();
	UFUNCTION(BlueprintCallable)
	void HeavyAttack();
	UFUNCTION(BlueprintCallable)
	void ChargedAttack(FKey Key);
	UFUNCTION(BlueprintCallable)
	void ChargedAttackEvent();
	UFUNCTION(BlueprintCallable)
	void ChargedAttackTimer();
	UFUNCTION(BlueprintCallable)
	void TrackChargedAttack(FKey Key );
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
	void PerformAttack(TEnumAsByte< EChartacterAction> AttackType ,int32 AttackIndex, bool bRandomIndex);

	UFUNCTION(BlueprintCallable)
	void  PreformAction(UAnimMontage* ActionMontage,EChartacterAction Action,EChartacterState state,int32 MontageIndex, bool bRandomIndex);
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
	virtual float TakeDamage(float DamageAmount,struct FDamageEvent const& DamageEvent,class AController* EventInstigator,AActor* DamageCauser	) override;
	virtual bool CanRecieveDamage()override;
	virtual void Jump() override;
	virtual void SetCanMove(bool isCanMove)  override;
	virtual bool ReserveAttakTokken(int32 Amount)  override;
	virtual void ReturnAttackTokken(int32 Amount)  override;

	
	UFUNCTION()
	bool IsValueInRange(float Value, float Min, float Max, bool InclusiveMin, bool InclusiveMax);


	UFUNCTION()
	void OnStateBegin( EChartacterState CharacterState);
	UFUNCTION()
	void OnStateEnd(EChartacterState CharacterState);
	
	UFUNCTION()
	void OnActionBegin(EChartacterAction CharacterAction);
	UFUNCTION()
	void OnActionEnd(EChartacterAction CharacterAction);

	UFUNCTION()
	void SetMovmentSpeedMode(EMovementSpeedMode  NewMovmetSpeed);
	UFUNCTION()
	TEnumAsByte < EMovementSpeedMode> GetMovmentSpeedMode();
	UFUNCTION()
	void SprintStaminaCost();
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

	void SetUpSyimulusSource();

	UFUNCTION()
	void SaveGame();
	UFUNCTION()
	void LoadSaveGame();

	UFUNCTION()
	void Respawn();
	UFUNCTION()
	void SetRespawnPoint(FTransform NewRespawnPoint);
	


	//UPROPERTY
private:
	UPROPERTY(EditAnywhere)
	bool bCanMove =true;
	UPROPERTY(EditAnywhere)
	bool bHitFront = true;

	FTransform RespawnPoint;
	FTimerHandle RespawnTimerHandle;

	UPROPERTY(EditAnywhere)
	bool bHeavyAttack;
	UPROPERTY(EditAnywhere)
	bool bChargedAttack;
	 UPROPERTY(EditAnywhere)
	 bool bCombatEnabled;
	 UPROPERTY(EditAnywhere)
	 float WeaponDamege=30.0f;
	 
	UPROPERTY(EditAnywhere)
	UAnimMontage* TempMontage;
	UPROPERTY(EditAnywhere)
	UAnimMontage* DeathMontage;
	UPROPERTY(EditAnywhere)
	FName PelvisBoneName  ;



	
	 


public:

	UPROPERTY()
	FVector spawnLocation;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> pauseWidgetClass;

	UUserWidget* WidgetInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bisGettingTargeted = false;
	UPROPERTY(EditAnywhere)
	float Health;
	UPROPERTY(EditAnywhere)
	float AttackHeldTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	 UC_CombatComponent* CombatComponent;
	 UPROPERTY(EditAnywhere, BlueprintReadWrite)
	 UC_StatsComponent* StatsComponents;
	 UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	 UManger *manger;
	 UPROPERTY(EditAnywhere)
	 bool bisMagic=false;
	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovmentSpeed")
	 float WalkingSpeed = 300.0f;
	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovmentSpeed")
	 float JoggingSpeed = 500.0f;
	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovmentSpeed")
	 float SprintingSpeed = 700.0f;

	 UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USoundCue *HitSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UParticleSystem* hitParticals;
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECombatType> combatType;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDamageType> damageType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* HitReaction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* backHitReaction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* KnockdownReaction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* GeettingUpFromKnockdownReaction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UC_TargetingComponent* TargetingComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AC_SpawnPoints> SpawnPointClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte < EMovementSpeedMode>  MovmentSpeedMode;
	private:
		UPROPERTY(EditAnywhere)
		FKey TempKey;
		UPROPERTY(EditAnywhere)
		UUserWidget* MainWidget;
		FTimerHandle SprintTimerHandle;
		class UAIPerceptionStimuliSourceComponent* stimuliSource;
		
};

