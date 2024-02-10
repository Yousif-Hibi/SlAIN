// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "rpg/Interfaces/Interactive_CI.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/SphereComponent.h>
#include "C_PickupActor.generated.h"


UCLASS(Blueprintable)
class  RPG_API AC_PickupActor : public AActor , public IInteractive_CI
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_PickupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void Intracts(AActor* Caller) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class AC_BaseWeapon> ToSpawn;

	

	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Cube;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USphereComponent* Sphere;



};
