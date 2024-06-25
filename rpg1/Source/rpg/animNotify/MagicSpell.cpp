// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicSpell.h"
#include <rpg/SoulsLikeCharacter.h>
#include "rpg/Actors/C_BaseMagicWeapon.h"
#include "rpg/Component/C_CombatComponent.h"
#include "rpg/AI/C_MasterAI.h"


void  UMagicSpell::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	Received_Notify(MeshComp, Animation, EventReference);

}

bool  UMagicSpell::Received_Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AActor* actor = MeshComp->GetOwner();


	if (actor) {
		auto* MainCharacter = Cast<AC_MasterAI>(actor);
		if (MainCharacter) {
			AC_BaseMagicWeapon* magicGlove = Cast<AC_BaseMagicWeapon>(MainCharacter->CombatComponent->GetMainWeapon());
			if (magicGlove)
			{
				magicGlove->fire();
			}
		}
	}
	return true;
}
