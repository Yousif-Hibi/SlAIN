// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C_room.h"
#include "C_Enemy_room.generated.h"


class UArrowComponent;
class APatrolPath;
class AC_SpawnAI;
/**
 * 
 */
UCLASS()
class RPG_API AC_Enemy_room : public AC_room
{
	GENERATED_BODY()
public:
	AC_Enemy_room();



	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* WallLeft3;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* WallTop3;
	UPROPERTY(EditAnywhere)
	UArrowComponent* ArrowGenrate1;
	UPROPERTY(EditAnywhere)
	TSubclassOf<APatrolPath> path;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AC_SpawnAI> SpawnEnemy;
	UPROPERTY(EditAnywhere)
	APatrolPath * pathplacer;
	UPROPERTY(EditAnywhere)
	AC_SpawnAI * SpawnEnemyplacer;
	UPROPERTY(EditAnywhere)
	UArrowComponent* spawnPoint2;
	UPROPERTY(EditAnywhere)
	UArrowComponent* spawnPoint3;
	UPROPERTY(EditAnywhere)
	UArrowComponent* spawnPoint4;
	UPROPERTY(EditAnywhere)
	UArrowComponent* spawnPoint5;
	UPROPERTY(EditAnywhere)
	UArrowComponent* spawnPoint6;
	UPROPERTY(EditAnywhere)
	UArrowComponent* spawnPoint7;
	UPROPERTY(EditAnywhere)
	UArrowComponent* spawnPoint8;
	UPROPERTY(EditAnywhere)
	UArrowComponent* spawnPoint9;


	UPROPERTY(EditAnywhere)
	UArrowComponent* wallspawnPoint2;
	UPROPERTY(EditAnywhere)
	UArrowComponent* wallspawnPoint3;
	UPROPERTY(EditAnywhere)
	UArrowComponent* wallspawnPoint4;
	UPROPERTY(EditAnywhere)
	UArrowComponent* wallspawnPoint5;
	UPROPERTY(EditAnywhere)
	UArrowComponent* wallspawnPoint6;
	UPROPERTY(EditAnywhere)
	UArrowComponent* wallspawnPoint7;
	UPROPERTY(EditAnywhere)
	UArrowComponent* wallspawnPoint8;

protected:
	virtual void BeginPlay() override;

private:
	void SpawnActors();

};
