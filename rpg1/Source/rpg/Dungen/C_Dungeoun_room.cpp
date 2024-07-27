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
}


