// Fill out your copyright notice in the Description page of Project Settings.


#include "CollisionTrace_ANS.h"
#include "rpg/Component/C_CombatComponent.h"
#include "rpg/Actors/C_BaseWeapon.h"
#include "rpg/Component/C_CollisionCombonent.h"
#include "rpg/SoulsLikeCharacter.h"
void UCollisionTrace_ANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation,TotalDuration ,EventReference);
	Received_NotifyBegin(MeshComp, Animation,TotalDuration, EventReference);
}
void UCollisionTrace_ANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation,  EventReference);
	Received_NotifyEnd(MeshComp, Animation,  EventReference);
}
bool UCollisionTrace_ANS::Received_NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	AActor* actor = MeshComp->GetOwner();


	if (actor) {
		ASoulsLikeCharacter* MainCharacter = Cast<ASoulsLikeCharacter>(actor);
		if (MainCharacter) {
			AC_BaseWeapon* Weapon = MainCharacter->CombatComponent->GetMainWeapon();

			if (IsValid(Weapon)) {
				
				Weapon->CollisionCombonent->ActivateColision();

			}
		}
	}
	return true;
}

bool UCollisionTrace_ANS::Received_NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) 
{
	AActor* actor = MeshComp->GetOwner();


	if (actor) {
		ASoulsLikeCharacter* MainCharacter = Cast<ASoulsLikeCharacter>(actor);
		if (MainCharacter) {
			AC_BaseWeapon* Weapon = MainCharacter->CombatComponent->GetMainWeapon();

			if (IsValid(Weapon)) {
				
				Weapon->CollisionCombonent->DeactivatColision();

			}
		}
	}
	return true;
}
