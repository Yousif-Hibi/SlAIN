// Fill out your copyright notice in the Description page of Project Settings.


#include "C_CollisionCombonent.h"
#include "Engine/EngineTypes.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>


UC_CollisionCombonent* UC_CollisionCombonent::eventDispatherSingletons = nullptr;

// Sets default values for this component's properties
UC_CollisionCombonent::UC_CollisionCombonent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	eventDispatherSingletons = this;
	CollisionObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
	// ...
}


// Called when the game starts
void UC_CollisionCombonent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

void UC_CollisionCombonent::OnHitEvent(FHitResult hit)
{
}


// Called every frame
void UC_CollisionCombonent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsCollisionEnabled)
	{

		CollisionTrace();

	}

}

TArray<AActor*> UC_CollisionCombonent::GetAreadyHitActiors()
{
	return AreadyHitActors;
}

void UC_CollisionCombonent::SetAreadyHitActiors(TArray<AActor*> AreadyHit)
{
	AreadyHitActors = AreadyHit;
}

bool UC_CollisionCombonent::IsCollisionEnabled()
{

	return bIsCollisionEnabled;
}

FHitResult UC_CollisionCombonent::GetLastHit()
{

	return LastHit;
}

UPrimitiveComponent* UC_CollisionCombonent::GetCollisionMesh()
{
	return CollisionMeshComponent;
}

TArray<AActor*> UC_CollisionCombonent::GetActorsToIgnor()
{
	return ActorsToIgnore;
}

void UC_CollisionCombonent::AddActorsToIgnore(AActor* Actor)
{

	ActorsToIgnore.AddUnique(Actor);

}

void UC_CollisionCombonent::RemoveActorToIgnore(AActor* Actor)
{

	ActorsToIgnore.Remove(Actor);

}

void UC_CollisionCombonent::SetCollisonMesh(UPrimitiveComponent* CollisionMesh)
{
	CollisionMeshComponent = CollisionMesh;
}

void UC_CollisionCombonent::ActivateColision()
{

	ClearHitActors();
	bIsCollisionEnabled = true;

}

void UC_CollisionCombonent::DeactivatColision()
{

	bIsCollisionEnabled = false;

}

void UC_CollisionCombonent::ClearHitActors()
{

	AreadyHitActors.Empty();

}

bool UC_CollisionCombonent::CollisionTrace()
{
	
	FVector StartLocation;
	FVector EndLocation;
	FHitResult OutHit;  // Use FHitResult, not FHitResult&
	OutHit.Init();
	if (CollisionMeshComponent) {
		StartLocation = CollisionMeshComponent->GetSocketLocation(StartSocketName);
		EndLocation = CollisionMeshComponent->GetSocketLocation(EndSocketName);
	}

	bool OnHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		GetWorld(),
		StartLocation,
		EndLocation,
		TraceRadius,
		CollisionObjectTypes,  
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		OutHit,
		true
	);

	if (OnHit) {
		
		LastHit = OutHit;
		UC_CollisionCombonent* Singleton = GetEventSingleton();
		Singleton->LastHit = OutHit;
		if (!AreadyHitActors.Contains(LastHit.GetActor()))
		{
			// Ensure the actor in LastHit is valid before using it
			 HitActor = LastHit.GetActor();
			if (HitActor)
			{
				AreadyHitActors.Add(HitActor);
				CallEventDispatherCpp();
				return true;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("HitActor is null"));
				return false; // Return early if HitActor is null
			}
		}
		else
		{
			return false;
		}

	}

	return false;
}

UC_CollisionCombonent* UC_CollisionCombonent::GetEventSingleton()
{
	return UC_CollisionCombonent::eventDispatherSingletons;
}


void UC_CollisionCombonent::BindEventDispatherCpp()
{
	UC_CollisionCombonent* Singleton = GetEventSingleton();

	Singleton->FonHit.AddUniqueDynamic(Singleton, &UC_CollisionCombonent::OnHitEvent);
}

void UC_CollisionCombonent::UnbindEventDispatherCpp()
{
	UC_CollisionCombonent* Singleton = GetEventSingleton();

	Singleton->FonHit.RemoveDynamic(Singleton, &UC_CollisionCombonent::OnHitEvent);

}




void UC_CollisionCombonent::CallEventDispatherCpp()
{
	
	
	UC_CollisionCombonent* Singleton = GetEventSingleton();
	FHitResult hit = Singleton->LastHit;
	Singleton->FonHit.Broadcast(hit);

}


