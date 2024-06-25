// Fill out your copyright notice in the Description page of Project Settings.


#include "CanMove_ANS.h"
#include <rpg/SoulsLikeCharacter.h>

void UCanMove_ANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	Received_NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}
void  UCanMove_ANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	Received_NotifyEnd(MeshComp, Animation, EventReference);
}

bool UCanMove_ANS::Received_NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	AActor* actor = MeshComp->GetOwner();


	if (actor) {
		ASoulsLikeCharacter* MainCharacter = Cast<ASoulsLikeCharacter>(actor);
		if (MainCharacter) {
			MainCharacter->SetCanMove(false);
		}
	}
	return false;
}
bool  UCanMove_ANS::Received_NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AActor* actor = MeshComp->GetOwner();


	if (actor) {
		ASoulsLikeCharacter* MainCharacter = Cast<ASoulsLikeCharacter>(actor);
		if (MainCharacter) {
			MainCharacter->SetCanMove(true);
		} 
	}
	return true;
}