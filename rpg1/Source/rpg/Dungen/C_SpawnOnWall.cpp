// Fill out your copyright notice in the Description page of Project Settings.


#include "C_SpawnOnWall.h"

// Sets default values
AC_SpawnOnWall::AC_SpawnOnWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshToSpawn = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshToSpawn"));
	RootComponent = MeshToSpawn;
}

// Called when the game starts or when spawned
void AC_SpawnOnWall::BeginPlay()
{
	Super::BeginPlay();
	Tospawn.Add("lamp1", lamp1);
	Tospawn.Add("lamp2", lamp2);
	Tospawn.Add("brazier", brazier);
	Tospawn.Add("cage", cage);
	Tospawn.Add("statue", statue);
}

// Called every frame
void AC_SpawnOnWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AC_SpawnOnWall::getItemToSpawn(FString str)
{
    if (Tospawn.Num() > 0) {
        // Remove spaces from the input string
        str = str.Replace(TEXT(" "), TEXT(""));
        str = str.Replace(TEXT("."), TEXT(""));
        if (Tospawn.Contains(str))
        {
            MeshToSpawn->SetStaticMesh(Tospawn[str]);
            UE_LOG(LogTemp, Warning, TEXT("Mesh found and set: %s"), *Tospawn[str]->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No matching mesh found for: %s"), *str);
        }
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Tospawn array is empty"));
    }

}

