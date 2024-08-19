// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "C_Dungeons.generated.h"

class AC_room;
class AC_fillerWall;
class AC_MeshToSpawn;
class AC_SpawnOnWall;
class AC_door;

UCLASS()
class RPG_API AC_Dungeons : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AC_Dungeons();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AC_room> roomClass1;



	UPROPERTY(EditAnywhere)
	AC_room* room;
	UPROPERTY(EditAnywhere)
	TArray<USceneComponent*> SpawnDiractions;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AC_room>> RoomList;
	UPROPERTY(EditAnywhere)
	AC_room* Randomroom;
	UPROPERTY(EditAnywhere)
	TSubclassOf <AC_MeshToSpawn> meshClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf < AC_SpawnOnWall> wallMeshClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf <AC_door> doorClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AC_fillerWall> FillerWallClass;
	UPROPERTY(EditAnywhere)
	TArray<USceneComponent*> SpawnPointsList;
	UPROPERTY(EditAnywhere)
	TArray<USceneComponent*> SpawnOnWallList;

	//Functions
	UFUNCTION()
	void SpawnStartRoom();
	UFUNCTION()
	void SpawnNextRoom();
	UFUNCTION()
	bool checkOverLap(AC_room* NewRoom);
	UFUNCTION()
	void FillWalls();
	
	

	UFUNCTION()
	void spawnMeshatLocation();
	UFUNCTION()
	void spawnWallMeshatLocation();



	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION()
	void SendPostRequest(FString ApiEndpoint, FString JsonContent);
	UFUNCTION()
	void SendPostQestRequest(FString FileContent);
	UFUNCTION(BlueprintCallable)
	void ProcessReceivedText(const FString& ReceivedText);
	TArray<FString> ParsedWallItems;
	TArray<FString> ParsedFloorItems;
};
