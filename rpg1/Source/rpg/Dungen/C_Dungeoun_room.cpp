// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Dungeoun_room.h"
#include "Components/ArrowComponent.h"
AC_Dungeoun_room::AC_Dungeoun_room()
{
	ArrowGenrate1 = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowGenrate1"));
	ArrowGenrate1->SetupAttachment(Exits);

	ArrowGenrate2 = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowGenrate2"));
	ArrowGenrate2->SetupAttachment(Exits);

	ArrowGenrate3 = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowGenrate3"));
	ArrowGenrate3->SetupAttachment(Exits);





	spawnPoint2 = CreateDefaultSubobject<UArrowComponent>(TEXT("spawnPoint2"));
	spawnPoint2->SetupAttachment(floorSpawnPoints);

	spawnPoint3 = CreateDefaultSubobject<UArrowComponent>(TEXT("spawnPoint3"));
	spawnPoint3->SetupAttachment(floorSpawnPoints);
	
	spawnPoint4 = CreateDefaultSubobject<UArrowComponent>(TEXT("spawnPoint4"));
	spawnPoint4->SetupAttachment(floorSpawnPoints);

	spawnPoint5 = CreateDefaultSubobject<UArrowComponent>(TEXT("spawnPoint5"));
	spawnPoint5->SetupAttachment(floorSpawnPoints);

	spawnPoint6 = CreateDefaultSubobject<UArrowComponent>(TEXT("spawnPoint6"));
	spawnPoint6->SetupAttachment(floorSpawnPoints);

	spawnPoint7 = CreateDefaultSubobject<UArrowComponent>(TEXT("spawnPoint7"));
	spawnPoint7->SetupAttachment(floorSpawnPoints);

	spawnPoint8 = CreateDefaultSubobject<UArrowComponent>(TEXT("spawnPoint8"));
	spawnPoint8->SetupAttachment(floorSpawnPoints);

	spawnPoint9 = CreateDefaultSubobject<UArrowComponent>(TEXT("spawnPoint9"));
	spawnPoint9->SetupAttachment(floorSpawnPoints);


	wallspawnPoint2 = CreateDefaultSubobject<UArrowComponent>(TEXT("wallspawnPoint2"));
	wallspawnPoint2->SetupAttachment(wallSpawnPoints);
	
	wallspawnPoint3 = CreateDefaultSubobject<UArrowComponent>(TEXT("wallspawnPoint3"));
	wallspawnPoint3->SetupAttachment(wallSpawnPoints);
	
	wallspawnPoint4 = CreateDefaultSubobject<UArrowComponent>(TEXT("wallspawnPoint4"));
	wallspawnPoint4->SetupAttachment(wallSpawnPoints);
	
	wallspawnPoint5 = CreateDefaultSubobject<UArrowComponent>(TEXT("wallspawnPoint5"));
	wallspawnPoint5->SetupAttachment(wallSpawnPoints);
	
	wallspawnPoint6 = CreateDefaultSubobject<UArrowComponent>(TEXT("wallspawnPoint6"));
	wallspawnPoint6->SetupAttachment(wallSpawnPoints);
	
	wallspawnPoint7 = CreateDefaultSubobject<UArrowComponent>(TEXT("wallspawnPoint7"));
	wallspawnPoint7->SetupAttachment(wallSpawnPoints);

	wallspawnPoint8 = CreateDefaultSubobject<UArrowComponent>(TEXT("wallspawnPoint8"));
	wallspawnPoint8->SetupAttachment(wallSpawnPoints);

	

}


