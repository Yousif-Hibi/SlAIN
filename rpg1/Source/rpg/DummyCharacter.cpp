// Fill out your copyright notice in the Description page of Project Settings.


#include "DummyCharacter.h"
#include "Actors/C_BaseEquippable.h"
#include "Actors/C_LeaderPose.h"
#include "Component/C_StatsComponent.h"
#include "Component/C_CombatComponent.h"
#include "Actors/C_BaseWeapon.h"
#include "Component/Manger.h"
#include "Blueprint/UserWidget.h"
#include "rpg/Component/C_TargetingComponent.h"

#include "rpg/Component/C_StatsComponent.h"

void ADummyCharacter::BeginPlay()
{
    Super::BeginPlay();
    

	
	//for (TSubclassOf <AC_BaseEquippable> Equiped : StartingEquipmeent) {
	//	FActorSpawnParameters SpawnPerams;
	//	SpawnPerams.Owner = this;
	//	FRotator rotat;
	//	FVector vec = this->GetActorLocation()- FVector(100.0f, 100.0f, 0.0f);
	//	AC_LeaderPose* SpawnedActor = GetWorld()->SpawnActor<AC_LeaderPose>(Equiped, vec, rotat, SpawnPerams);
	//	SpawnedActor->AttachActor(SpawnedActor->AttachSocketName);
	//	SpawnedActor->OnEquip();
	//}
	//
}

ADummyCharacter::ADummyCharacter()
{
	
	
	manger->stateBegin.AddDynamic(this, &ADummyCharacter::OnStateBegin);
	manger->stateEnd.AddDynamic(this, &ADummyCharacter::OnStateEnd);
	manger->CharacterActionBegin.AddDynamic(this, &ADummyCharacter::OnActionBegin);
	manger->CharacterActionEnd.AddDynamic(this, &ADummyCharacter::OnActionEnd);
}

void ADummyCharacter::PerformDeath()
{
	bisGettingTargeted = false;
	FTimerHandle TimerHandle;
	EnableRagdoll();
	ApplyHitReactionPhysicsVelocity(2000);
	if (IsValid(CombatComponent->GetMainWeapon())) {
		CombatComponent->GetMainWeapon()->SimulateWeaponPhysics();

	}
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ADummyCharacter::PerformDeathAfterDelay, 4.0f, false);

}

void ADummyCharacter::PerformDeathAfterDelay()
{
	

	bisGettingTargeted = false;
}

void ADummyCharacter::OnStateBegin(EChartacterState CharacterState)
{
	if (CharacterState == EChartacterState::Dead) {
		UE_LOG(LogTemp, Warning, TEXT("DEAD"));
		PerformDeath();
	}
}

void ADummyCharacter::OnStateEnd(EChartacterState CharacterState)
{
}

bool ADummyCharacter::CanRecieveDamage()
{
	ICombat_CI::CanRecieveDamage();
	bool check = (manger->GetCurrentState() != EChartacterState::Dead);
	return check;
}



void ADummyCharacter::OnActionBegin(EChartacterAction CharacterAction)
{
}

void ADummyCharacter::OnActionEnd(EChartacterAction CharacterAction)
{
}

bool ADummyCharacter::CanBeTarget()
{
	TArray<TEnumAsByte< EChartacterState>> temp = {  EChartacterState::Dead };
	if (StatsComponents->GetCurrentStateValue(Estat::Health)==0) {
		UE_LOG(LogTemp, Warning, TEXT("DEAD"));
		return false;
	}
	if (!manger->IsCurrentStateEqualToAny(temp)) {
		
		return true;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("DEAD"));
		return false;
	}
	
	return false;
}



bool ADummyCharacter::OnTargete()
{
	
	if (CanBeTarget()) {
		return bisGettingTargeted;
	}
	return false;
}
void ADummyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	OnTargete();
	target();
	
}