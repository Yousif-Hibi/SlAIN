// Fill out your copyright notice in the Description page of Project Settings.


#include "CollisionTrace_ANS.h"
#include "rpg/Component/C_CombatComponent.h"
#include "rpg/Actors/C_BaseWeapon.h"
#include "rpg/Component/C_CollisionCombonent.h"
#include "rpg/SoulsLikeCharacter.h"
#include <rpg/Actors/C_BaseDualWeapon.h>
#include "rpg/AI/C_MasterAI.h"
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
		auto* AICharacter = Cast<AC_MasterAI>(actor);
		AC_BaseWeapon* Weapon=nullptr;
		if (MainCharacter) {
			UE_LOG(LogTemp, Warning, TEXT("sadsdadasdas"));

			Weapon = MainCharacter->CombatComponent->GetMainWeapon();
		}
		else if (AICharacter) {
			UE_LOG(LogTemp, Warning, TEXT("ccccccccccccc"));
			Weapon = AICharacter->CombatComponent->GetMainWeapon();
		}
		if (IsValid(Weapon)) {
			AC_BaseDualWeapon* dualweapon = Cast<AC_BaseDualWeapon>(Weapon);
			switch (Weapon->CombatType)
			{
			case None:
				break;
			case Lightsword:
				Weapon->ActivateCollision(collisionPart);
				break;
			case GreatSword:
				Weapon->ActivateCollision(collisionPart);
				break;
			case Axe:
				Weapon->ActivateCollision(collisionPart);
				break;
			case DualSword:
				dualweapon->ActivateCollision(collisionPart);
				break;
			case MagicHand:
				Weapon->ActivateCollision(collisionPart);
				break;
			default:
				break;
			}
		}

		
	}
	return true;
}
bool UCollisionTrace_ANS::Received_NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) 
{
	AActor* actor = MeshComp->GetOwner();
	AC_BaseWeapon* Weapon = nullptr;

	if (actor) {
		ASoulsLikeCharacter* MainCharacter = Cast<ASoulsLikeCharacter>(actor);
		auto* AICharacter = Cast<AC_MasterAI>(actor);
		if (MainCharacter) {
			Weapon = MainCharacter->CombatComponent->GetMainWeapon();
		}
		else if (AICharacter) {
			UE_LOG(LogTemp, Warning, TEXT("ccccccccccccc"));
			Weapon = AICharacter->CombatComponent->GetMainWeapon();
		}
		if (IsValid(Weapon)) {
			AC_BaseDualWeapon* dualweapon = Cast<AC_BaseDualWeapon>(Weapon);
			
				
				switch (Weapon->CombatType)
				{
				case None:
					break;
				case Lightsword:
					Weapon->DeactivateCollision(collisionPart);
					break;
				case GreatSword:
					Weapon->DeactivateCollision(collisionPart);
					break;
				case Axe:
					Weapon->DeactivateCollision(collisionPart);
					break;
				case DualSword:
					dualweapon->DeactivateCollision(collisionPart);
					break;
				case MagicHand:
					Weapon->DeactivateCollision(collisionPart);
					break;
				default:
					break;
				}
				
				

			}
		
	}
	return true;
}
