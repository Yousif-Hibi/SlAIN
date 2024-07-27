// Fill out your copyright notice in the Description page of Project Settings.


#include "C_Dungen_Path1.h"
#include "Components/ArrowComponent.h"


AC_Dungen_Path1::AC_Dungen_Path1()
{
	ArrowGenrate1 = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowGenrate1"));
	ArrowGenrate1->SetupAttachment(Exits);

    WallLeft3= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallLeft3"));
    WallLeft3->SetupAttachment(SecnceOne);
    WallLeft3->SetRelativeLocation(FVector(0.0f, -950.0f, 150.0f));
    FVector WallLeft3Scale(4.0f, 1.0f, 4.0f);
    WallLeft3->SetRelativeScale3D(WallLeft3Scale);

    WallRight3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallRight3"));
    WallRight3->SetupAttachment(SecnceOne);
    WallRight3->SetRelativeLocation(FVector(0.0f, 950.0f, 150.0f));
    FVector WallRight3Scale(4.0f, 1.0f, 4.0f);
    WallRight3->SetRelativeScale3D(WallRight3Scale);
}
