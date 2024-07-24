// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Room.h"

// Sets default values
AC_Room::AC_Room()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AC_Room::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AC_Room::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

