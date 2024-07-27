// Fill out your copyright notice in the Description page of Project Settings.


#include "C_fillerWall.h"

// Sets default values
AC_fillerWall::AC_fillerWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallMesh"));
	RootComponent = WallMesh;

	// Set the location and scale
	
	WallMesh->SetWorldScale3D(FVector(1.0f, 4.0f, 4.0f));

	
}

// Called when the game starts or when spawned
void AC_fillerWall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AC_fillerWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

