// Fill out your copyright notice in the Description page of Project Settings.


#include "ToggleCombat_AN.h"
#include "rpg/SoulsLikeCharacter.h"
#include "rpg/Actors/C_BaseWeapon.h"
#include "rpg/Component/C_CombatComponent.h"
void UToggleCombat_AN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	Received_Notify(MeshComp, Animation, EventReference);

}

bool UToggleCombat_AN::Received_Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AActor* actor = MeshComp->GetOwner();


	if (actor) {
		ASoulsLikeCharacter* MainCharacter = Cast<ASoulsLikeCharacter>(actor);
		if (MainCharacter) {
			AC_BaseWeapon* mainWeapon = MainCharacter->CombatComponent->GetMainWeapon();

			if (mainWeapon)
			{
				if (bEnableCombat)
				{
				
					mainWeapon->AttachActor(mainWeapon->HandSocketName);
					
				}
				else
				{
					
					mainWeapon->AttachActor(mainWeapon->AttachSocketName);
				}

				

			}
			MainCharacter->CombatComponent->SetCombatEnabled(bEnableCombat);
		
		}


	}



	return true;
}
