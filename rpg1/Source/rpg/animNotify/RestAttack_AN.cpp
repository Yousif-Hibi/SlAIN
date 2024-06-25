// Fill out your copyright notice in the Description page of Project Settings.


#include "RestAttack_AN.h"
#include "rpg/SoulsLikeCharacter.h"

void  URestAttack_AN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	Received_Notify(MeshComp, Animation, EventReference);

}

bool  URestAttack_AN::Received_Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AActor* actor = MeshComp->GetOwner();


	if (actor) {
		ASoulsLikeCharacter* MainCharacter = Cast<ASoulsLikeCharacter>(actor);
		if (MainCharacter) {
			MainCharacter->RestAttack();
		}
	}
	return true;

}