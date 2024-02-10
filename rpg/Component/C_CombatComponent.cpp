// Fill out your copyright notice in the Description page of Project Settings.


#include "C_CombatComponent.h"
#include "rpg/Actors/C_BaseWeapon.h"
#include "rpg/SoulsLikeCharacter.h"

// Sets default values for this component's properties
UC_CombatComponent::UC_CombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UC_CombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UC_CombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



void UC_CombatComponent::RestAttack()
{
	attackCount = 0;
	bAttackSaved = false;
	
}

void UC_CombatComponent::SetCombatEnabled(bool isEnabled)
{
	bCombatEnabled = isEnabled;
	ASoulsLikeCharacter* Character = Cast<ASoulsLikeCharacter>(GetOwner());
	UE_LOG(LogTemp, Warning, TEXT("bbbbbb %d"),isEnabled);
	if (IAnimInstance_CI* anim = Cast<IAnimInstance_CI>(Character->GetMesh()->GetAnimInstance())) {

		anim->UpdateCombatEnabled(bCombatEnabled);

	}
}

bool UC_CombatComponent::IsCombatEnabled()
{
	return bCombatEnabled;
}




void UC_CombatComponent::SetAttackCount(int Count)
{
	attackCount = Count;
}

void UC_CombatComponent::SetAttackSaved(bool attackSaved)
{
	bAttackSaved = attackSaved;
}

bool UC_CombatComponent::GetAttackSaved()
{
	return bAttackSaved;
}



int32 UC_CombatComponent::GetAttackCount()
{
	return attackCount;
}

void UC_CombatComponent::SetMainWeapon(AC_BaseWeapon* Weapon)
{
	if (IsValid(MainWeapon)) {
		MainWeapon->OnUnequipped();
		MainWeapon->Destroy();
	}



	MainWeapon = Weapon;

}


AC_BaseWeapon* UC_CombatComponent::GetMainWeapon()
{
	return MainWeapon;
}