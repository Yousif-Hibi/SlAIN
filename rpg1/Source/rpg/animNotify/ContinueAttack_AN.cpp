// Fill out your copyright notice in the Description page of Project Settings.


#include "ContinueAttack_AN.h"
#include "rpg/SoulsLikeCharacter.h"

void UContinueAttack_AN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	Received_Notify(MeshComp, Animation, EventReference);
}

bool UContinueAttack_AN::Received_Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AActor* actor = MeshComp->GetOwner();


	if (actor) {
		ASoulsLikeCharacter* MainCharacter = Cast<ASoulsLikeCharacter>(actor);
		if (MainCharacter) {
			MainCharacter->ContinueAttack();
		}
	}
	return true;
}
