// Fill out your copyright notice in the Description page of Project Settings.
#include "C_BaseDualWeapon.h"
#include "GameFramework/Character.h"
#include "rpg/Component/C_CollisionCombonent.h"
#include "rpg/Component/C_CombatComponent.h"

AC_BaseDualWeapon::AC_BaseDualWeapon()
{
	ItemStaticMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemStaticMesh2"));
	ItemStaticMesh2->SetupAttachment(Root);

	
	OffHandCollisionCombonent = CreateDefaultSubobject<UC_CollisionCombonent>(TEXT("LeftHandCollision"));
	OffHandCollisionCombonent->StartSocketName = TEXT("WeaponStart");
	OffHandCollisionCombonent->EndSocketName = TEXT("WeaponEnd");
	OffHandCollisionCombonent->FonHit.AddDynamic(this, &AC_BaseWeapon::OnHit);

	footCollisionCombonent = CreateDefaultSubobject<UC_CollisionCombonent>(TEXT("footCollision"));
	footCollisionCombonent->StartSocketName = TEXT("foot_r_Socket");
	footCollisionCombonent->EndSocketName = TEXT("foot_r_Socket");
	footCollisionCombonent->FonHit.AddDynamic(this, &AC_BaseWeapon::OnHit);
}

void AC_BaseDualWeapon::ActivateCollision(ECollisionPart collisionPart)
{
	switch (collisionPart)
	{
	case MainWeapon:
		CollisionCombonent->ActivateColision();
		break;
	case OffhandWeeapon:
		OffHandCollisionCombonent->ActivateColision();
		break;
	case RightFoot:
		footCollisionCombonent->ActivateColision();

	default:
		break;
	}
}

void AC_BaseDualWeapon::DeactivateCollision(ECollisionPart collisionPart)
{

	switch (collisionPart)
	{
	case MainWeapon:
		CollisionCombonent->DeactivatColision();
		break;
	case OffhandWeeapon:
		OffHandCollisionCombonent->DeactivatColision();
		break;
	case RightFoot:
		footCollisionCombonent->DeactivatColision();

	default:
		break;
	}
}

void AC_BaseDualWeapon::OnEquipped()
{
	
	AC_BaseWeapon::OnEquipped();
	
	OffHandCollisionCombonent->SetCollisonMesh(ItemStaticMesh2);
	if (CombatComponent->IsCombatEnabled()) {
		
		AttachoffHAndSword(SecoundWeaponHandSocket);
	}
	else {
		
		AttachoffHAndSword(SecoundWeaponAttachSocket);
	}
	ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner());
	if (CharacterOwner)
	{
		UMeshComponent* Mesh = CharacterOwner->GetMesh();
		if (Mesh)
		{
			footCollisionCombonent->SetCollisonMesh(Mesh);
		}
		
	}
	OffHandCollisionCombonent->AddActorsToIgnore(GetOwner());
	footCollisionCombonent->AddActorsToIgnore(GetOwner());
}

void AC_BaseDualWeapon::AttachoffHAndSword(FName SocketName)
{
	
	ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner());
	if (CharacterOwner)
	{
		UMeshComponent* Mesh = CharacterOwner->GetMesh();
		if (Mesh)
		{
			ItemStaticMesh2->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
		}
	}

}

void AC_BaseDualWeapon::ToggleCombat(bool bEnableCombat)
{
	bCombatEnabled = bEnableCombat;
	AC_BaseWeapon::ToggleCombat(bEnableCombat);
	if (bEnableCombat)
	{

		AttachoffHAndSword(SecoundWeaponHandSocket);

	}
	else
	{

		AttachoffHAndSword(SecoundWeaponAttachSocket);
	}
}

void AC_BaseDualWeapon::OFFHandOnHit(FHitResult hit)
{
	UE_LOG(LogTemp, Warning, TEXT("sdsadsd hit "));
}

