// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/C_BaseEquippable.h"
#include "DummyCharacter.h"
#include "Actors/C_LeaderPose.h"
#include "Component/C_StatsComponent.h"
void ADummyCharacter::BeginPlay()
{
    Super::BeginPlay();
    
	
	for (TSubclassOf <AC_BaseEquippable> Equiped : StartingEquipmeent) {
		FActorSpawnParameters SpawnPerams;
		SpawnPerams.Owner = this;
		FRotator rotat;
		FVector vec = this->GetActorLocation()- FVector(100.0f, 100.0f, 0.0f);
		AC_LeaderPose* SpawnedActor = GetWorld()->SpawnActor<AC_LeaderPose>(Equiped, vec, rotat, SpawnPerams);
		SpawnedActor->AttachActor(SpawnedActor->AttachSocketName);
		SpawnedActor->OnEquip();
	}
	
}
