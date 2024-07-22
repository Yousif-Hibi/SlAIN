// Fill out your copyright notice in the Description page of Project Settings.


#include "C_SpawnPoints.h"
#include "Components/BoxComponent.h"
#include "SoulsLikeCharacter.h"

// Sets default values
AC_SpawnPoints::AC_SpawnPoints()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	Cube->SetupAttachment(Root);
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(Root);

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this,&AC_SpawnPoints::OnOverlapBegin);

}

// Called when the game starts or when spawned
void AC_SpawnPoints::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AC_SpawnPoints::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AC_SpawnPoints::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp,Warning,TEXT("SaveGame();saved"));
	ASoulsLikeCharacter* Player = Cast<ASoulsLikeCharacter>(OtherActor);
	if (Player)
	{
		Player->SetRespawnPoint(Player->GetActorTransform());
	}

}

