// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_MeshToSpawn.generated.h"

UCLASS()
class RPG_API AC_MeshToSpawn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_MeshToSpawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshToSpawn;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMesh* tree;
	UPROPERTY(EditAnywhere)
	UStaticMesh* grass;
	UPROPERTY(EditAnywhere)
	UStaticMesh* brokenfloor1;
	UPROPERTY(EditAnywhere)
	UStaticMesh* well;
	UPROPERTY(EditAnywhere)
	UStaticMesh* barels;
	UPROPERTY(EditAnywhere)
	UStaticMesh* boxes;
	UPROPERTY(EditAnywhere)
	UStaticMesh* lavaBarel;
	UPROPERTY(EditAnywhere)
	UStaticMesh* brokenfloor2;

	UPROPERTY(EditAnywhere)
	TMap<FString, UStaticMesh*> Tospawn;
	UFUNCTION()
	void getItemToSpawn(FString str);
};
