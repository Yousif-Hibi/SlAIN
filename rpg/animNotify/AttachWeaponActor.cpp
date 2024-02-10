// AttachWeaponActor.cpp
#include "AttachWeaponActor.h"
#include "rpg/SoulsLikeCharacter.h"
#include "rpg/Actors/C_BaseWeapon.h"
#include "rpg/Component/C_CombatComponent.h"
void UAttachWeaponActor::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation,EventReference);
	Received_Notify(MeshComp, Animation, EventReference);
	
}

bool UAttachWeaponActor::Received_Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AActor* actor = MeshComp->GetOwner();
	

	if (actor) {
		ASoulsLikeCharacter* MainCharacter = Cast<ASoulsLikeCharacter>(actor);
		if (MainCharacter) {
			AC_BaseWeapon* mainWeapon = MainCharacter->CombatComponent->GetMainWeapon();

			if (mainWeapon )
			{

					mainWeapon->AttachActor(AttachSocketName);
		
			}
		}


	}








	return true;
}