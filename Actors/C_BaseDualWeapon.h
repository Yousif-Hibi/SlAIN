// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_BaseWeapon.h"
#include "C_BaseDualWeapon.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AC_BaseDualWeapon : public AC_BaseWeapon
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AC_BaseDualWeapon();

	void ActivateCollision(ECollisionPart collisionPart);
	void DeactivateCollision(ECollisionPart collisionPart);
	void OnEquipped() override;
	UFUNCTION()
	void AttachoffHAndSword(FName SocketName);

	void ToggleCombat(bool bEnableCombat);

public:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent * ItemStaticMesh2;
	UPROPERTY(EditAnywhere)
	FName SecoundWeaponAttachSocket;
	UPROPERTY(EditAnywhere)
	bool  bCombatEnabled;
	UPROPERTY(EditAnywhere)
	FName SecoundWeaponHandSocket;
	UPROPERTY(EditAnywhere)
	class UC_CollisionCombonent* OffHandCollisionCombonent;
	UPROPERTY(EditAnywhere)
	 UC_CollisionCombonent* footCollisionCombonent;
	 
	 void OFFHandOnHit(FHitResult hit);
};
