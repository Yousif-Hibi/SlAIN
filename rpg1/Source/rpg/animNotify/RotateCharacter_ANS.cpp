// Fill out your copyright notice in the Description page of Project Settings.
#include "RotateCharacter_ANS.h"
#include "Kismet/KismetMathLibrary.h"
#include "rpg/SoulsLikeCharacter.h"


void URotateCharacter_ANS::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	Received_NotifyTick( MeshComp,  Animation, FrameDeltaTime,  EventReference);
}

bool URotateCharacter_ANS::Received_NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) const
{
	AActor* actor = MeshComp->GetOwner();


	if (actor) {
		ASoulsLikeCharacter* MainCharacter = Cast<ASoulsLikeCharacter>(actor);
		if (!MainCharacter) {
			return false;
		}
		if (MainCharacter) {
			FRotator rotate = MainCharacter->GetDesiredRotation();
			MainCharacter->SetActorRotation(rotate);
		}
	}
	return true;

}
