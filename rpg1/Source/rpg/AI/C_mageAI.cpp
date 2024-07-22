// Fill out your copyright notice in the Description page of Project Settings.


#include "C_mageAI.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PatrolPath.h" 
#include "EngineUtils.h" 


AC_mageAI::AC_mageAI()
{
	// Set this character to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize PatrolPath pointer
	PatrolPath = nullptr;
	
}
void AC_mageAI::Tick(float DeltaTime)
{  
	ChangeHealth();
	Track();
		
}

void AC_mageAI::Teleport(FVector Location)
{

	FVector ActorLocation = GetActorLocation();
	ActorLocation.Z += 10.f;
	SetActorLocation(Location);
	UParticleSystemComponent* SpawnedEmitter = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
		TeleportParticalImpact,
		ActorLocation
	);

	if (SpawnedEmitter)
	{
		// Set the particle system component to automatically destroy itself when finished
		SpawnedEmitter->bAutoDestroy = true;

		// Optionally, you can set the duration if the particle system template supports it
		SpawnedEmitter->SetFloatParameter(TEXT("EmitterDuration"), 2.0f); // Set duration to 2 seconds

		// Schedule a timer to destroy the emitter after 2 seconds
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			[SpawnedEmitter]() {
				SpawnedEmitter->DestroyComponent();
			},
			2.0f, // 2 seconds delay
			false // One-shot timer
		);
	}

	
	
	
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
	
}
void AC_mageAI::BeginPlay()
{
	AC_MasterAI::BeginPlay();

	// Find the patrol path actor in the world
	for (TActorIterator<APatrolPath> It(GetWorld()); It; ++It)
	{
		PatrolPath = *It;
		break; // Assuming there's only one patrol path in the world, break after finding it
	}
}

// Get the patrol path
APatrolPath* AC_mageAI::GetPatrolPath() const
{
	return PatrolPath;
}

// Set the patrol path
void AC_mageAI::SetPatrolPath(APatrolPath* NewPatrolPath)
{
	PatrolPath = NewPatrolPath;
}