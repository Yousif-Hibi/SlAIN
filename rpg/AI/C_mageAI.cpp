// Fill out your copyright notice in the Description page of Project Settings.


#include "C_mageAI.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Components/CapsuleComponent.h"
void AC_mageAI::Tick(float DeltaTime)
{
	ChangeHealth();
	Track();
}

void AC_mageAI::Teleport(FVector Location)
{
	/*GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	GetCharacterMovement()->MaxFlySpeed=5000.0f;
	GetCharacterMovement()->MaxAcceleration=99999.0f;
	GetMesh()->SetVisibility(false,true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
		TeleportParticalTrial,
		GetActorLocation()
	);*/
	FVector ActorLocation = GetActorLocation();
	ActorLocation.Z += 10.f;
	SetActorLocation(Location);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
		TeleportParticalImpact,
		ActorLocation
	);

	UAIBlueprintHelperLibrary::CreateMoveToProxyObject(this,
		 this,
		 Location,
		nullptr,
		 5.0f,
		true);
	EndTeleport();
}

void AC_mageAI::EndTeleport()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetCharacterMovement()->MaxAcceleration = 1500.0f;
	GetMesh()->SetVisibility(true, true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	 
}
