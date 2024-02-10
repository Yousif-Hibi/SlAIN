// Fill out your copyright notice in the Description page of Project Settings.




#include "rpg/Enums/E_CharacterAction.h"
#include "rpg/Component/Manger.h"
#include "rpg/Component/C_CombatComponent.h"
#include "rpg/SoulsLikeCharacter.h"
#include "C_BaseWeapon.h"
#include "rpg/Component/C_CollisionCombonent.h"
#include "Kismet/GameplayStatics.h"



AC_BaseWeapon::AC_BaseWeapon() {

	CollisionCombonent= CreateDefaultSubobject<UC_CollisionCombonent>(TEXT("Collision"));
	CollisionCombonent->StartSocketName = TEXT("WeaponStart");
	CollisionCombonent->EndSocketName = TEXT("WeaponEnd");

	CollisionCombonent->FonHit.AddDynamic(this, &AC_BaseWeapon::OnHit);

	ActionStatCost.Add(EChartacterAction::LightAttack, 10.0f);
	ActionStatCost.Add(EChartacterAction::ChargedAttack, 20.0f);
	ActionStatCost.Add(EChartacterAction::SprintAttack, 10.0f);
	ActionStatCost.Add(EChartacterAction::HeavyAttack, 15.0f);
	ActionStatCost.Add(EChartacterAction::Dodge, 10.0f);
	ActionStatCost.Add(EChartacterAction::FallingAttack, 10.0f);
	ActionDamageMultiplyer.Add(EChartacterAction::LightAttack, 1.0f);
	ActionDamageMultiplyer.Add(EChartacterAction::HeavyAttack, 1.4f);
	ActionDamageMultiplyer.Add(EChartacterAction::SprintAttack, 1.2f);
	ActionDamageMultiplyer.Add(EChartacterAction::ChargedAttack, 1.5f);
	ActionDamageMultiplyer.Add(EChartacterAction::FallingAttack, 1.2f);

	
}

void AC_BaseWeapon::BeginPlay()
{
	Super::BeginPlay();

}

void AC_BaseWeapon::OnEquipped()
{
	
	AC_BaseEquippable::OnEquipped();
	ASoulsLikeCharacter* Character = Cast<ASoulsLikeCharacter>(GetOwner()); 
		
	CombatComponent = Character->CombatComponent;
	manger = Character->manger;

	if (CombatComponent) {

		if (CombatComponent->IsCombatEnabled()) {
			this->AttachActor(HandSocketName);
		}
		else {
			this->AttachActor(AttachSocketName);
		}

		CombatComponent->SetMainWeapon(this);
		AActor* actor = Cast<AActor>(Character);


		if (IAnimInstance_CI* anim = Cast<IAnimInstance_CI>(Character->GetMesh()->GetAnimInstance())) {

			//resone for crash 
			anim->UpdateCombatType(CombatType);

			CollisionCombonent->SetCollisonMesh(GetItemMesh());
			CollisionCombonent->AddActorsToIgnore(GetOwner());
				

			

		}
	}

}

void AC_BaseWeapon::OnHit(FHitResult hit)
{
	ASoulsLikeCharacter* Character = Cast<ASoulsLikeCharacter>(hit.GetActor());
	if (Character->CanRecieveDamage()) {
		UE_LOG(LogTemp, Warning, TEXT("hit %s"), *hit.GetActor()->GetName());
		UGameplayStatics::ApplyPointDamage(hit.GetActor(),
			GetDamage(),
			GetOwner()->GetActorForwardVector(),
			hit,
			GetInstigatorController(),
			GetOwner(),
			nullptr
		);
	}
}

void AC_BaseWeapon::SimulateWeaponPhysics()
{
	GetItemMesh()->SetCollisionProfileName(TEXT("PhysicsActor"), true);
	GetItemMesh()->SetSimulatePhysics(true);
}

TArray<UAnimMontage*> AC_BaseWeapon::GetActionMontages(EChartacterAction ChartacterAction)
{
	switch (ChartacterAction)
	{
	case SprintAttack:
		return SprintAttackMontage;
	case LightAttack:
		return  AttackMontage;
	case HeavyAttack:
		return HeavyAttackMontage;
	case ChargedAttack:
		return ChargedAttackMontage;
	case FallingAttack:
		return FallingAttackMontage;
	case Dodge:
		return DodgeMontage;
	case EnterCombat:
		return EnterCombatMontage;
	case ExitCombat:
		return ExitCombatMontage;
	case NoAction:
		break;
	default:
		break;
	}
	return TArray<UAnimMontage*>();
}

float AC_BaseWeapon::GetStatCostForAction()
{
	float* value = 0;
	if (IsValid(manger)) {
		value = ActionStatCost.Find(manger->GetCurrentAction());
		
		return *value;
	}
	return 1;
}

int32 AC_BaseWeapon::GetDamage()
{
	float value = 0;
	if (IsValid(manger)) {
		value = *ActionDamageMultiplyer.Find(manger->GetCurrentAction());
		FMath::Clamp(value, 1, value);
		value = Damege * value;
		return value;

	}

	return 0;
}

