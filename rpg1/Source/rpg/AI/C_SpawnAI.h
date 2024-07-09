// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_SpawnAI.generated.h"

class AC_mageAI;
UCLASS()
class RPG_API AC_SpawnAI : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_SpawnAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TSubclassOf<AC_mageAI> MageClass;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class APatrolPath* path;
	UPROPERTY(EditAnywhere)
	int32 NumMagesToSpawn = 2;

	APatrolPath* FindClosestPatrolPath() const;
};
