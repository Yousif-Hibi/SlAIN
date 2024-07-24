// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_Dungeon.generated.h"

UCLASS()
class RPG_API AC_Dungeon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_Dungeon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
