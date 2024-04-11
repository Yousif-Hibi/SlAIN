// Fill out your copyright notice in the Description page of Project Settings.
#include "C_LeaderPose.h"
#include "rpg/Component/C_StatsComponent.h"
#include "GameFramework/Character.h"
#include "rpg/DummyCharacter.h"

void AC_LeaderPose::AttachActor(FName SocketName)
{

	
	ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner());
	
	if (IsValid(CharacterOwner))
	{
		USkinnedMeshComponent* Mesh = CharacterOwner->GetMesh();
		if (Mesh)
		{
			this->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
			ItemSkeletalMesh->SetLeaderPoseComponent(Mesh);
		}
	}
	
}

void AC_LeaderPose::OnEquip()
{
	
	ADummyCharacter* CharacterOwner = Cast<ADummyCharacter>(GetOwner());
	CharacterOwner->StatsComponent-> ModifyCurrentStatValue(Estat::Armor, 5.0f, false);
	
}

void AC_LeaderPose::OnUnequipped()
{
	
	ADummyCharacter* CharacterOwner = Cast<ADummyCharacter>(GetOwner());
	CharacterOwner->StatsComponent->ModifyCurrentStatValue(Estat::Armor, -5.0f, false);
}
