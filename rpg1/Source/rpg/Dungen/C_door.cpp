// Fill out your copyright notice in the Description page of Project Settings.


#include "C_door.h"

// Sets default values
AC_door::AC_door()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	RootComponent = DoorMesh;

	

	
}

// Called when the game starts or when spawned
void AC_door::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AC_door::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

