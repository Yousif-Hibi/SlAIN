// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
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
	
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION()
	void SendPostRequest(FString ApiEndpoint, FString JsonContent);
	UFUNCTION()
	void SendPostQestRequest(FString FileContent);

	UFUNCTION()
	void spawnMage(int32 numToSpawn);
	UFUNCTION()
	void spawnKnight(int32 numToSpawn);
	UFUNCTION(BlueprintCallable)
	void ProcessReceivedText(const FString& ReceivedText);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TSubclassOf<AC_mageAI> MageClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TSubclassOf<AC_HumanoidEnemy> KnightClass;
	
	 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class APatrolPath* path;
	UPROPERTY(EditAnywhere)
	int32 NumMagesToSpawn = 2;
	UPROPERTY(EditAnywhere)
	int32 NumknightsToSpawn = 2;

	UPROPERTY()
	int32 knightHealth;
	UPROPERTY()
	int32 magiHealth;
	UPROPERTY()
	int32 knightDamage;
	UPROPERTY()
	int32 magiDamage;


	APatrolPath* FindClosestPatrolPath() const;






};
