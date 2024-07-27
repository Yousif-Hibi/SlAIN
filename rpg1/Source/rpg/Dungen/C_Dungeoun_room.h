// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_room.h"
#include "C_Dungeoun_room.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AC_Dungeoun_room : public AC_room
{
	GENERATED_BODY()
	
public:
	AC_Dungeoun_room();
 




public:
	UPROPERTY(EditAnywhere)
	 UArrowComponent* ArrowGenrate1;

	UPROPERTY(EditAnywhere)
	UArrowComponent* ArrowGenrate2;

	UPROPERTY(EditAnywhere)
	UArrowComponent* ArrowGenrate3;
};
