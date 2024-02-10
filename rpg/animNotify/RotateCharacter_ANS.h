// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "RotateCharacter_ANS.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URotateCharacter_ANS : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	bool Received_NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) const;

	UPROPERTY()
	float InterpSpeed=10;

};
