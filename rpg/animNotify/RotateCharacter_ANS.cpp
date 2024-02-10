// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/KismetMathLibrary.h"
#include "RotateCharacter_ANS.h"
#include "rpg/SoulsLikeCharacter.h"

bool URotateCharacter_ANS::Received_NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) const
{

	AActor* actor = MeshComp->GetOwner();


	if (actor) {
		ASoulsLikeCharacter* MainCharacter = Cast<ASoulsLikeCharacter>(actor);
		if (MainCharacter) {
		 FRotator rotate =	UKismetMathLibrary::RInterpTo_Constant(MainCharacter->GetActorRotation(), MainCharacter->GetDesiredRotation(), GetWorld()->GetDeltaSeconds(), InterpSpeed);
				MainCharacter->SetActorRotation(rotate);
		}
	}
	return true;

}
