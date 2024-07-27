// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_room.h"
#include "C_Dungen_Path1.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AC_Dungen_Path1 : public AC_room
{
	GENERATED_BODY()
public:
	AC_Dungen_Path1();


	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* WallLeft3;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* WallRight3;
	UPROPERTY(EditAnywhere)
	UArrowComponent* ArrowGenrate1;
};
