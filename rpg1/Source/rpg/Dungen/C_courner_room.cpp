// Fill out your copyright notice in the Description page of Project Settings.


#include "C_courner_room.h"
#include "Components/ArrowComponent.h"
AC_courner_room::AC_courner_room()
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
    WallTop3->SetRelativeLocation(FVector(950.0f,  0.0f, 150.0f));
    FVector WallRight3Scale(1.0f, 4.0f, 4.0f);
    WallTop3->SetRelativeScale3D(WallRight3Scale);
}
