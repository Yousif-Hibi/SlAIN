// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_SpawnOnWall.generated.h"

UCLASS()
class RPG_API AC_SpawnOnWall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_SpawnOnWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* lamp1;
	UPROPERTY(EditAnywhere)
	UStaticMesh* lamp2;
	UPROPERTY(EditAnywhere)
	UStaticMesh* brazier;
	UPROPERTY(EditAnywhere)
	UStaticMesh* cage;
	UPROPERTY(EditAnywhere)
	UStaticMesh* statue;

	UPROPERTY(EditAnywhere)
	TMap<FString, UStaticMesh*> Tospawn;
	UFUNCTION()
	void getItemToSpawn(FString str);
};
