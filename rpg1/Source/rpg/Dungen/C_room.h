// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_room.generated.h"

UCLASS()
class RPG_API AC_room : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_room();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


    // Room components
    UPROPERTY(EditAnywhere)
    USceneComponent* SecnceOne;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* floor;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* WallBottom;
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* WallTop;
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* WallLeft;
   
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* WallRight;
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* WallBottom2;
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* WallTop2;
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* WallLeft2;
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* WallRight2;

    UPROPERTY(EditAnywhere)
    class UArrowComponent* directionArrow;
    UPROPERTY(EditAnywhere)
    UArrowComponent* spawnPoint1;
    UPROPERTY(EditAnywhere)
    UArrowComponent* wallspawnPoint1;
    // Exits
    UPROPERTY(EditAnywhere)
    USceneComponent* Exits;

    // Floor collisions
    UPROPERTY(EditAnywhere)
    USceneComponent* collisons;
    UPROPERTY(EditAnywhere)
    USceneComponent* floorSpawnPoints;
    UPROPERTY(EditAnywhere)
    USceneComponent* wallSpawnPoints;
    UPROPERTY(EditAnywhere)
   class  UBoxComponent* floorCollision1;
   UPROPERTY(EditAnywhere)
   AStaticMeshActor* FloorActor;
   UPROPERTY(EditAnywhere)
  class ANavMeshBoundsVolume* NavVolume;
    // Functions to set up room components
    UFUNCTION()
    void attach();
    UFUNCTION()
    void MakeRoomOne();
    UFUNCTION()
    void Makefloorcollison();

   void ForceNavMeshUpdate();


};
