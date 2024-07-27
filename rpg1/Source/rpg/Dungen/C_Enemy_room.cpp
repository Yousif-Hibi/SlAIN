// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Enemy_room.h"
#include "Components/ArrowComponent.h"
#include "rpg/AI/C_SpawnAI.h"
#include "rpg/AI/PatrolPath.h"
#include "NavigationSystem.h"
AC_Enemy_room::AC_Enemy_room()
{
    ArrowGenrate1 = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowGenrate1"));
    ArrowGenrate1->SetupAttachment(Exits);

    WallLeft3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallLeft3"));
    WallLeft3->SetupAttachment(SecnceOne);
    WallLeft3->SetRelativeLocation(FVector(0.0f, -950.0f, 150.0f));
    FVector WallLeft3Scale(4.0f, 1.0f, 4.0f);
    WallLeft3->SetRelativeScale3D(WallLeft3Scale);

    WallTop3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallTop3"));
    WallTop3->SetupAttachment(SecnceOne);
    WallTop3->SetRelativeLocation(FVector(950.0f, 0.0f, 150.0f));
    FVector WallRight3Scale(1.0f, 4.0f, 4.0f);
    WallTop3->SetRelativeScale3D(WallRight3Scale);

    pathplacer = CreateDefaultSubobject<APatrolPath>(TEXT("pathPlacer"));
    pathplacer->AttachToComponent(SecnceOne, FAttachmentTransformRules::KeepRelativeTransform);

    SpawnEnemyplacer= CreateDefaultSubobject<AC_SpawnAI>(TEXT("SpawnPlacer"));
    SpawnEnemyplacer->AttachToComponent(SecnceOne, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

void AC_Enemy_room::BeginPlay()
{
    Super::BeginPlay();
    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (NavSys)
    {
        NavSys->Build();
    }
    SpawnActors();

}

void AC_Enemy_room::SpawnActors()
{
    if (path)
    {
        pathplacer = GetWorld()->SpawnActor<APatrolPath>(path, GetActorLocation() + (FVector(10.f, 10.f, 250.f)), GetActorRotation());
        pathplacer->PatrolPath.Add(FVector::ZeroVector);

    }

    if (SpawnEnemy)
    {
        SpawnEnemyplacer = GetWorld()->SpawnActor<AC_SpawnAI>(SpawnEnemy, GetActorLocation()+(FVector(500.f,50.f,250.f)), GetActorRotation());
    }
}
