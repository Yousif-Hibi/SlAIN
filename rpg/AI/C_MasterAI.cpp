// Fill out your copyright notice in the Description page of Project Settings.


#include "C_MasterAI.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "rpg/Actors/C_BaseWeapon.h"
#include <Kismet/KismetSystemLibrary.h>
#include "rpg/Enums/E_CombatType.h"
#include "rpg/Component/C_CombatComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include <Sound/SoundCue.h>
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayTagsManager.h"
#include "rpg/Component/Manger.h"
#include "GameFramework/PlayerController.h"
#include "rpg/Component/C_StatsComponent.h"
#include "Blueprint/UserWidget.h"
#include "rpg/C_PlayerController.h"
#include <BehaviorTree/BehaviorTree.h>
#include "rpg/Actors/C_BaseMagicWeapon.h"
#include "rpg/Actors/C_magicSpell.h"
#include "Particles/ParticleSystemComponent.h"
#include "rpg/Enums/E_DamageType.h"
#include "rpg/Actors/C_BaseDualWeapon.h"
#include "PatrolPath.h"
#include "rpg/Component/C_CollisionCombonent.h"
#include <Perception/AIPerceptionListenerInterface.h>
#include <AIController.h>
#include <Perception/AISense_Damage.h>







//////////////////////////////////////////////////////////////////////////
#define Intractable ECC_GameTraceChannel1
// AC_MasterAI

AC_MasterAI::AC_MasterAI()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);


	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	


	CombatComponent = CreateDefaultSubobject<UC_CombatComponent>(TEXT("combat"));
	manger = CreateDefaultSubobject<UManger>(TEXT("Manger"));
	
	StatsComponent = CreateDefaultSubobject<UC_StatsComponent>(TEXT("StatsComponent"));


	manger->stateBegin.AddDynamic(this, &AC_MasterAI::OnStateBegin);
	manger->stateEnd.AddDynamic(this, &AC_MasterAI::OnStateEnd);
	manger->CharacterActionBegin.AddDynamic(this, &AC_MasterAI::OnActionBegin);
	manger->CharacterActionEnd.AddDynamic(this, &AC_MasterAI::OnActionEnd);
	
	CollisionCombonent= CreateDefaultSubobject<UC_CollisionCombonent>(TEXT("CollisionCombonent"));

}




void AC_MasterAI::ToggleCombat()
{


	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (IsValid(CombatComponent->GetMainWeapon())) {
		float playrate = 1.0f;
		if (CanPreformToggleCombat()) {

			if (!CombatComponent->IsCombatEnabled()) {
				UE_LOG(LogTemp, Warning, TEXT("xxxx %d"), CombatComponent->IsCombatEnabled());
				if (animInstance) {
					TempMontage = CombatComponent->GetMainWeapon()->GetActionMontages(EChartacterAction::EnterCombat)[0];
					manger->SetState(EChartacterState::GeneralActionState);
					manger->SetAction(EChartacterAction::EnterCombat);
					PlayAnimMontage(TempMontage, 1.0f, FName("Default"));

				}
			}

			else {
				UE_LOG(LogTemp, Warning, TEXT("yyyyyy %d"), CombatComponent->IsCombatEnabled());
				if (animInstance) {
					TempMontage = CombatComponent->GetMainWeapon()->GetActionMontages(EChartacterAction::ExitCombat)[0];
					manger->SetState(EChartacterState::Disabled);
					manger->SetAction(EChartacterAction::ExitCombat);
					PlayAnimMontage(TempMontage, 1.0f, FName("Default"));


				}
			}
		}
	}

}


bool AC_MasterAI::CanPreformToggleCombat()
{

	TArray<TEnumAsByte< EChartacterState>> temp = { EChartacterState::Attacking, EChartacterState::Dead, EChartacterState::Disabled, EChartacterState::Dodging, EChartacterState::GeneralActionState };
	if (!manger->IsCurrentStateEqualToAny(temp)) {
		if (!GetCharacterMovement()->IsFalling())
			return true;
	}

	return false;
}

bool AC_MasterAI::CanPreFormAttack()
{
	
	if (IsValid(mainWeapon)) {
		
		if (mainWeapon->bCombat) {
			
			TArray<TEnumAsByte< EChartacterState>> temp = {  EChartacterState::Dead, EChartacterState::Disabled };
			if (!manger->IsCurrentStateEqualToAny(temp)) {
				UE_LOG(LogTemp, Warning, TEXT("Here 4"));
					return true;
			}
			else if (GetMesh()->GetAnimInstance()->Montage_GetIsStopped(HitReaction)) {
				
				if (GetMesh()->GetAnimInstance()->Montage_GetIsStopped(backHitReaction)) {
					return true;
				}

			}
			
		}
	}

	return false;

}

bool AC_MasterAI::CanPreformDodge()
{

	TArray<TEnumAsByte< EChartacterState>> temp = { EChartacterState::Dead, EChartacterState::Disabled, EChartacterState::Dodging, EChartacterState::GeneralActionState };
	if (!manger->IsCurrentStateEqualToAny(temp)) {
		if (!GetCharacterMovement()->IsFalling()) {
			if (StatsComponent->GetCurrentStateValue(Estat::Stamina) > 10) {
				return true;
			}
		}
	}

	return false;
}

bool AC_MasterAI::CanPreformJump()
{
	TArray<TEnumAsByte< EChartacterState>> temp = { EChartacterState::Dead, EChartacterState::Disabled, EChartacterState::Dodging, EChartacterState::GeneralActionState };
	if (!manger->IsCurrentStateEqualToAny(temp)) {

		return true;
	}
	
	return false;
}

bool AC_MasterAI::CanRecieveHitreaction()
{
	TArray<TEnumAsByte< EChartacterState>> temp = { EChartacterState::Dead };
	if (!manger->IsCurrentStateEqualToAny(temp)) {
		
			return true;
	}
	return false;
}

void AC_MasterAI::PreformDodge(int32 MontageIndex, bool bRandomIndex)
{
	if (CanPreformDodge()) {
		if (CombatComponent->GetMainWeapon()) {
			AC_BaseWeapon* x = CombatComponent->GetMainWeapon();
			int32 size = x->DodgeMontage.Num();

			if (size < MontageIndex) {
				return;
			}
			if (size >= 1) {



				if (bRandomIndex) {
					MontageIndex = 0;
				}
				TempMontage = CombatComponent->GetMainWeapon()->DodgeMontage[0];
				if (IsValid(TempMontage)) {

					manger->SetState(EChartacterState::Dodging);
					manger->SetAction(EChartacterAction::Dodge);

					

					PlayAnimMontage(TempMontage, 1.0f, FName("Default"));





				}

			}
		}
	}
}

void AC_MasterAI::AttackEvent()
{
	
	if (mainWeapon) {
		
				PerformAttack(EChartacterAction::LightAttack, CombatComponent->GetAttackCount(), false);
		
	}
}


void AC_MasterAI::PerformAttack(TEnumAsByte< EChartacterAction> AttackType, int32 AttackIndex, bool bRandomIndex)
{

	    

	if (CanPreFormAttack()) {
		
		if (mainWeapon) {


			AC_BaseWeapon* x = CombatComponent->GetMainWeapon();
			int32 size = x->GetActionMontages(AttackType).Num();

			if (size < AttackIndex) {
				return;
			}
			if (size >= 1) {
				if (bRandomIndex) {
					AttackIndex = FMath::RandRange(0, x->AttackMontage.Num());
				}
				LightAttack = CombatComponent->GetMainWeapon()->GetActionMontages(AttackType)[AttackIndex];
				if (IsValid(LightAttack)) {
					manger->SetState(EChartacterState::Attacking);
					manger->SetAction(AttackType);

					
					PlayAnimMontage(LightAttack, 1.0f, FName("Default"));
					AttackIndex++;
					CombatComponent->SetAttackCount(AttackIndex);

					if (AttackIndex > size - 1) {
						CombatComponent->SetAttackCount(0);
					}

				}

			}
		}
	}

}

void AC_MasterAI::PreformAction(UAnimMontage* ActionMontage, EChartacterAction Action, EChartacterState state, int32 MontageIndex, bool bRandomIndex)
{


	if (CombatComponent->GetMainWeapon()) {
		AC_BaseWeapon* x = CombatComponent->GetMainWeapon();
		int32 size = x->DodgeMontage.Num();

		if (size < MontageIndex) {
			return;
		}
		if (size >= 1) {



			if (bRandomIndex) {
				MontageIndex = 0;
			}
			TempMontage = CombatComponent->GetMainWeapon()->GetActionMontages(Action)[0];
			if (IsValid(ActionMontage)) {
				UE_LOG(LogTemp, Warning, TEXT("asdasdasd %d"), size);
				manger->SetState(state);
				manger->SetAction(Action);
				
				PlayAnimMontage(ActionMontage, 1.0f, FName("Default"));



			}

		}

	}

}

void AC_MasterAI::DamegeTaken(float inDamage)
{
	Health = Health - inDamage;
	if (Health < 0) {
		Health = 0;
	}

	if (Health <= 0) {
		manger->SetState(EChartacterState::Dead);

	}
}

void AC_MasterAI::EnableRagdoll()
{
	if (Health >= 0) {
		GetCharacterMovement()->SetMovementMode(MOVE_None, 0);

		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		GetMesh()->SetCollisionProfileName("Ragdoll", true);
		GetMesh()->SetAllBodiesBelowSimulatePhysics(PelvisBoneName, true);
		GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(PelvisBoneName, 1.0f);
	}


}

void AC_MasterAI::ApplyHitReactionPhysicsVelocity(float initialSpeed)
{
	initialSpeed *= -1;
	FVector vector = initialSpeed * GetActorForwardVector();
	GetMesh()->SetPhysicsLinearVelocity(vector, false, PelvisBoneName);



}

void AC_MasterAI::PerformDeath()
{
	FTimerHandle TimerHandle;
	EnableRagdoll();
	ApplyHitReactionPhysicsVelocity(2000);
	if (IsValid(CombatComponent->GetMainWeapon())) {
		CombatComponent->GetMainWeapon()->SimulateWeaponPhysics();

	}

	GetCapsuleComponent()->DestroyComponent();
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AC_MasterAI::PerformDeathAfterDelay, 4.0f, false);


}

void AC_MasterAI::PerformDeathAfterDelay()
{
	if (IsValid(CombatComponent->GetMainWeapon())) {
		CombatComponent->GetMainWeapon()->Destroy();
	}
	GetMesh()->DestroyComponent();



}

void AC_MasterAI::ContinueAttack()
{
	ICombat_CI::ContinueAttack();
	manger->SetState(EChartacterState::Nothing);

	if (CombatComponent->GetAttackSaved()) {
		CombatComponent->SetAttackSaved(false);
	}
}

void AC_MasterAI::RestAttack()
{
	ICombat_CI::RestAttack();
	CombatComponent->RestAttack();

}

FRotator AC_MasterAI::GetDesiredRotation()
{
	ICombat_CI::GetDesiredRotation();
	if (!GetCharacterMovement()->GetLastInputVector().Equals(FVector::ZeroVector, 0.001f)) {

		return	GetLastMovementInputVector().Rotation();
	}
	else {
		return GetActorRotation();
	}
	return GetActorRotation();
}

void AC_MasterAI::RestCombat()
{
	//ICombat_CI::RestCombat();
	UE_LOG(LogTemp, Warning, TEXT("rest "));
	CombatComponent->RestAttack();
	manger->RestState();
	manger->SetAction(EChartacterAction::NoAction);
}

float AC_MasterAI::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	StatsComponent->TakeDamage(DamageAmount);
	FVector DamageLocation = GetActorLocation(); 
	
	APawn* instgater = Cast<APawn>(DamageCauser);
	MakeNoise(1.f,instgater,instgater->GetActorLocation());
	
	 
	if (CanRecieveHitreaction()) {
		if (IsValid(DamageCauser)) {
			FVector thisVector = this->GetActorForwardVector();

			FVector otherVector = (DamageCauser->GetActorForwardVector());
			float n = FVector::DotProduct(thisVector, otherVector);
			if (IsValueInRange(n, -1.0f, 0.1f, true, true)) {
				UE_LOG(LogTemp, Warning, TEXT("name , %f "), n);
				bHitFront = true;
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("noname "));
				bHitFront = false;
			}
		}
		
		UE_LOG(LogTemp, Warning, TEXT("damege cuser %s"),* DamageCauser->GetName());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
			hitParticals,
			GetActorLocation()
		);
	}
	if (StatsComponent->GetCurrentStateValue(Estat::Health) <= 0) {
		manger->SetState(EChartacterState::Dead);
	}
	bisMagic = CombatComponent->bmaigcenabled;
	UE_LOG(LogTemp, Warning, TEXT("name , %d "), bisMagic);
	ApplyHitReaction(damageType);
	
	
	

	return 0.0f;
}

bool AC_MasterAI::CanRecieveDamage()
{

	ICombat_CI::CanRecieveDamage();
	bool check = (manger->GetCurrentState() != EChartacterState::Dead);
	return check;
}

void AC_MasterAI::Jump()
{
	if (CanPreformJump()) {
		StopAnimMontage();
		manger->RestState();
		RestCombat();
		ACharacter::Jump();
	}
}

void AC_MasterAI::SetCanMove(bool isCanMove)
{
	bCanMove = isCanMove;
}

bool AC_MasterAI::CanBeTarget()
{
	
	if (StatsComponent->GetCurrentStateValue(Estat::Health)<=0) {
		
		return false;
	}
	return true;
}

bool AC_MasterAI::IsValueInRange(float Value, float Min, float Max, bool InclusiveMin, bool InclusiveMax)
{
	return (Value >= Min && Value <= Max) || (InclusiveMin && Value == Min) || (InclusiveMax && Value == Max);
}



void AC_MasterAI::OnStateBegin(EChartacterState CharacterState)
{
	if (CharacterState == EChartacterState::Dead) {
		bisGettingTargeted = false;
		PerformDeath();

	}
}

void AC_MasterAI::OnStateEnd(EChartacterState CharacterState)
{
}

void AC_MasterAI::OnActionBegin(EChartacterAction CharacterAction)
{
	
}

void AC_MasterAI::OnActionEnd(EChartacterAction CharacterAction)
{
	
}

void AC_MasterAI::SetMovmentSpeedMode(EMovementSpeedMode NewMovmetSpeed)
{
	if (NewMovmetSpeed != MovmentSpeedMode) {
		MovmentSpeedMode = NewMovmetSpeed;
	}
	switch (MovmentSpeedMode)
	{
	case EMovementSpeedMode::Walking:
		GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
		break;
	case EMovementSpeedMode::Jogging:
		GetCharacterMovement()->MaxWalkSpeed = JoggingSpeed;
		break;
	case EMovementSpeedMode::Sprinting:
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
		break;
	default:
		GetCharacterMovement()->MaxWalkSpeed = JoggingSpeed;
		break;
	}
}

bool AC_MasterAI::IsTrack()
{
	
	if (CanBeTarget()) {
		return bisGettingTargeted;
	}
	return false;
}

TEnumAsByte < EMovementSpeedMode> AC_MasterAI::GetMovmentSpeedMode()
{
	return MovmentSpeedMode;
}



void AC_MasterAI::DisableSprint()
{
}

void AC_MasterAI::Fire()
{

	UE_LOG(LogTemp, Warning, TEXT("yyyyyy"));




}

void AC_MasterAI::ApplyHitReaction(EDamageType DamageType)
{


	if (bisMagic) {
		PerfformKnockdown();
	}
	else {
		PerfformHitStun();
	}





}

void AC_MasterAI::PerfformHitStun()
{
	if (bHitFront) {
		PlayAnimMontage(HitReaction, 1.0f, FName("Default"));
	}
	else {
		PlayAnimMontage(backHitReaction, 1.0f, FName("Default"));
		

	}
	manger->SetState(EChartacterState::Disabled);

}

void AC_MasterAI::PerfformKnockdown()
{

	PlayAnimMontage(KnockdownReaction, 1.0f, FName("Default"));
	PlayAnimMontage(GeettingUpFromKnockdownReaction, 1.0f, FName("Default"));
	manger->SetState(EChartacterState::Disabled);
}

void AC_MasterAI::SetIsMagic(bool isMagic)
{
	bisMagic = isMagic;
}

UBehaviorTree* AC_MasterAI::GetBehaviorTree()
{   
	return BehaviorTree;
}

APatrolPath* AC_MasterAI::GetPatrolPath()
{
	return  PatrolPath;
}

UAnimMontage* AC_MasterAI::GetAIAttackMontage()
{
	return LightAttack;
}

int AC_MasterAI::AIAttack_Implementation()
{
	if (LightAttack) {
		AttackEvent();
	}
	return 0;
}



void AC_MasterAI::Tick(float DeltaTime)
{
	Track();

}

void AC_MasterAI::BeginPlay()
{
	Super::BeginPlay();

	StatsComponent->InitialState();
	if (MainWidget) {

		MainWidget->AddToPlayerScreen();
	}
	float num = StatsComponent->GetCurrentStateValue(Estat::Health);
	UE_LOG(LogTemp, Warning, TEXT("stamina %f"), num);



	FActorSpawnParameters SpawnPerams;
	SpawnPerams.Owner = this;
	FRotator rotat;
	FVector vec = this->GetActorLocation() - FVector(100.0f, 100.0f, 0.0f);
	mainWeapon = GetWorld()->SpawnActor<AC_BaseWeapon>(ToSpawn, vec, rotat, SpawnPerams);
	CombatComponent->SetMainWeapon(mainWeapon);
	mainWeapon->OnAIEquipped();

}
