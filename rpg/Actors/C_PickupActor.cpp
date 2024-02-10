 // Fill out your copyright notice in the Description page of Project Settings.


#include "C_PickupActor.h"
#include "C_BaseWeapon.h"
#include "rpg/SoulsLikeCharacter.h"


// Sets default values
AC_PickupActor::AC_PickupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	Cube->SetupAttachment(Root);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(Cube);


}

// Called when the game starts or when spawned
void AC_PickupActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AC_PickupActor::Intracts(AActor* Caller) {
	IInteractive_CI::Intracts(Caller);

	ASoulsLikeCharacter* Character = Cast<ASoulsLikeCharacter>(Caller);

	FActorSpawnParameters SpawnPerams;
	SpawnPerams.Owner = Caller;
	FRotator rotat;
	FVector vec = Caller->GetActorLocation() - FVector(100.0f, 100.0f, 0.0f);
	AC_BaseWeapon* SpawnedActor = GetWorld()->SpawnActor<AC_BaseWeapon>(ToSpawn, vec, rotat, SpawnPerams);

	// Check if the character already has a weapon of the same type
	

		if (IsValid(SpawnedActor)) {

		
			SpawnedActor->SetOwner(Caller);
			SpawnedActor->OnEquipped();
		}
	
}



