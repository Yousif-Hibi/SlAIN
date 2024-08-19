// Fill out your copyright notice in the Description page of Project Settings.


#include "C_MeshToSpawn.h"

// Sets default values
AC_MeshToSpawn::AC_MeshToSpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshToSpawn = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshToSpawn"));
	RootComponent = MeshToSpawn;


	

}

// Called when the game starts or when spawned
void AC_MeshToSpawn::BeginPlay()
{
	Super::BeginPlay();
	
	Tospawn.Add("tree", tree);
	Tospawn.Add("grass", grass);
	Tospawn.Add("brokenfloor1", brokenfloor1);
	Tospawn.Add("well", well);
	Tospawn.Add("barrels", barels);
	Tospawn.Add("boxes", boxes);
	Tospawn.Add("lavaBarrel", lavaBarel);
	Tospawn.Add("brokenfloor2", brokenfloor2);
}

// Called every frame
void AC_MeshToSpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AC_MeshToSpawn::getItemToSpawn(FString str)
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

