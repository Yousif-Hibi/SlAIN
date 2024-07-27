// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_room.h"
#include "C_courner_room.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AC_courner_room : public AC_room
{
	GENERATED_BODY()

public:
	AC_courner_room();










	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* WallLeft3;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* WallTop3;
	UPROPERTY(EditAnywhere)
	UArrowComponent* ArrowGenrate1;
};
