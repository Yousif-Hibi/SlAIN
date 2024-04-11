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
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	bool Received_NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) const;
	UPROPERTY()
	float InterpSpeed=10;

};
