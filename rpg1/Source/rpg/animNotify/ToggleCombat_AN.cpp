// Fill out your copyright notice in the Description page of Project Settings.


#include "ToggleCombat_AN.h"
#include "rpg/SoulsLikeCharacter.h"
#include "rpg/Actors/C_BaseWeapon.h"
#include "rpg/Actors/C_BaseDualWeapon.h"
#include "rpg/Component/C_CombatComponent.h"
#include <rpg/Actors/C_BaseMagicWeapon.h>
#include "rpg/Enums/E_DamageType.h"
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
			AC_BaseDualWeapon* dualweapon = Cast<AC_BaseDualWeapon>(mainWeapon);
			AC_BaseMagicWeapon* magicGlove = Cast<AC_BaseMagicWeapon>(mainWeapon);
			if (mainWeapon)
			{
				switch (mainWeapon->CombatType)
				{
				case None:
					break;
				case Lightsword:
					mainWeapon->ToggleCombat(bEnableCombat);
					MainCharacter->damageType = EDamageType::MeleDamage;
					break;
				case GreatSword:
				
					mainWeapon->ToggleCombat(bEnableCombat);
					MainCharacter->damageType = EDamageType::MeleDamage;
					break;
				case Axe:
					mainWeapon->ToggleCombat(bEnableCombat);
					MainCharacter->damageType = EDamageType::MeleDamage;
					break;
				case DualSword:
					dualweapon->ToggleCombat(bEnableCombat);
					MainCharacter->damageType = EDamageType::MeleDamage;
					break;
				case MagicHand:
					magicGlove->ToggleCombat(bEnableCombat);
					MainCharacter->SetIsMagic(true);
					break;
				default:
					break;
				}
				
				

			}
			
		
		}


	}



	return true;
}
