// Fill out your copyright notice in the Description page of Project Settings.


#include "C_room.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "NavigationSystem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "NavigationSystem.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BrushComponent.h"
// Sets default values
AC_room::AC_room()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	attach();
	MakeRoomOne();
	Makefloorcollison();
    ForceNavMeshUpdate();
}

// Called when the game starts or when spawned
void AC_room::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AC_room::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AC_room::attach()
{
    // Create root component
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    // Create and attach secondary components
    SecnceOne = CreateDefaultSubobject<USceneComponent>(TEXT("SecnceOne"));
    SecnceOne->SetupAttachment(RootComponent);

    Exits = CreateDefaultSubobject<USceneComponent>(TEXT("Exits"));
    Exits->SetupAttachment(RootComponent);

    floorSpawnPoints = CreateDefaultSubobject<USceneComponent>(TEXT("floorSpawnPoints"));
    floorSpawnPoints->SetupAttachment(RootComponent);

    spawnPoint1 = CreateDefaultSubobject<UArrowComponent>(TEXT("spawnPoint1"));
    spawnPoint1->SetupAttachment(floorSpawnPoints);
    

    wallSpawnPoints = CreateDefaultSubobject<USceneComponent>(TEXT("wallSpawnPoints"));
    wallSpawnPoints->SetupAttachment(RootComponent);

    wallspawnPoint1 = CreateDefaultSubobject<UArrowComponent>(TEXT("wallspawnPoint1"));
    wallspawnPoint1->SetupAttachment(wallSpawnPoints);

    collisons = CreateDefaultSubobject<USceneComponent>(TEXT("collisons"));
    collisons->SetupAttachment(RootComponent);

    directionArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
    directionArrow->SetupAttachment(RootComponent);
}

void AC_room::MakeRoomOne()
{
    // Create and set up floor mesh
    floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("floor"));
    floor->SetupAttachment(SecnceOne);
    FVector floorScale(20.0f, 20.0f, 1.0f);
    floor->SetRelativeScale3D(floorScale);

    // Create and set up walls
    WallBottom = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallBottom"));
    WallBottom->SetupAttachment(SecnceOne);
    WallBottom->SetRelativeLocation(FVector(-950.0f, -600.0f, 150.0f));
    FVector WallBottomScale(1.0f, 8.0f, 4.0f);
    WallBottom->SetRelativeScale3D(WallBottomScale);

    WallTop = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallTop"));
    WallTop->SetupAttachment(SecnceOne);
    WallTop->SetRelativeLocation(FVector(950.0f, -600.0f, 150.0f));
    FVector WallTopScale(1.0f, 8.0f, 4.0f);
    WallTop->SetRelativeScale3D(WallTopScale);

    WallLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallLeft"));
    WallLeft->SetupAttachment(SecnceOne);
    WallLeft->SetRelativeLocation(FVector(-600.0f, -950.0f, 150.0f));
    FVector WallLeftScale(8.0f, 1.0f, 4.0f);
    WallLeft->SetRelativeScale3D(WallLeftScale);

    WallRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallRight"));
    WallRight->SetupAttachment(SecnceOne);
    WallRight->SetRelativeLocation(FVector(-600.0f, 950.0f, 150.0f));
    FVector WallRightScale(8.0f, 1.0f, 4.0f);
    WallRight->SetRelativeScale3D(WallRightScale);

    WallBottom2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallBottom2"));
    WallBottom2->SetupAttachment(SecnceOne);
    WallBottom2->SetRelativeLocation(FVector(-950.0f, 600.0f, 150.0f));
    FVector WallBottom2Scale(1.0f, 8.0f, 4.0f);
    WallBottom2->SetRelativeScale3D(WallBottom2Scale);

    WallTop2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallTop2"));
    WallTop2->SetupAttachment(SecnceOne);
    WallTop2->SetRelativeLocation(FVector(950.0f, 600.0f, 150.0f));
    FVector WallTop2Scale(1.0f, 8.0f, 4.0f);
    WallTop2->SetRelativeScale3D(WallTop2Scale);

    WallLeft2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallLeft2"));
    WallLeft2->SetupAttachment(SecnceOne);
    WallLeft2->SetRelativeLocation(FVector(600.0f, -950.0f, 150.0f));
    FVector WallLeft2Scale(8.0f, 1.0f, 4.0f);
    WallLeft2->SetRelativeScale3D(WallLeft2Scale);

    WallRight2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallRight2"));
    WallRight2->SetupAttachment(SecnceOne);
    WallRight2->SetRelativeLocation(FVector(600.0f, 950.0f, 150.0f));
    FVector WallRight2Scale(8.0f, 1.0f, 4.0f);
    WallRight2->SetRelativeScale3D(WallRight2Scale);
}

void AC_room::Makefloorcollison()
{
    // Create and set up floor collision box
    floorCollision1 = CreateDefaultSubobject<UBoxComponent>(TEXT("floorCollision1"));
    floorCollision1->SetupAttachment(collisons);
}


void AC_room::ForceNavMeshUpdate()
{
   
    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (NavSys)
    {
        NavSys->Build();
    }
}                              