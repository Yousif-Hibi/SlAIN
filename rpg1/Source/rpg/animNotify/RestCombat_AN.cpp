// Fill out your copyright notice in the Description page of Project Settings.


#include "RestCombat_AN.h"
#include "rpg/SoulsLikeCharacter.h"

void URestCombat_AN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	Received_Notify(MeshComp, Animation, EventReference);

}

bool URestCombat_AN::Received_Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AActor* actor = MeshComp->GetOwner();


	if (actor) {
		ASoulsLikeCharacter* MainCharacter = Cast<ASoulsLikeCharacter>(actor);
		if (MainCharacter) {
			MainCharacter->RestCombat();
		}
	}
	return true;

}