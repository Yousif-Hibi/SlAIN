// Fill out your copyright notice in the Description page of Project Settings.

#include "C_TargetingComponent.h"
#include "rpg/Interfaces/Targeting_CI.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include "GameFramework/Controller.h" 
#include "GameFramework/CharacterMovementComponent.h"
#include <rpg/AI/C_MasterAI.h>
#include <rpg/Component/C_CombatComponent.h>

// Sets default values for this component's properties
UC_TargetingComponent::UC_TargetingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

 
// Called when the game starts
void UC_TargetingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	
	if (OwnerCharacter) {
		OwnerController = OwnerCharacter->GetController();
		if (OwnerController) {
			FollowCamera = OwnerCharacter->FindComponentByClass<UCameraComponent>();
		}
	}


}


// Called every frame
void UC_TargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsTargeting) {
		UpdateTargetingControlRotation();
	}
}

void UC_TargetingComponent::SetIsTargeting(bool bEnableTargeting)
{
	bIsTargeting = bEnableTargeting;
}

bool UC_TargetingComponent::GetIsTargeting()
{
	return bIsTargeting;
}

void UC_TargetingComponent::SetTargetActor(AActor* NewTargetActor)
{
	TargetActor = NewTargetActor;
}

AActor* UC_TargetingComponent::GetTargetActor()
{
	return TargetActor;
}

AActor* UC_TargetingComponent::FindTarget()
{
	TArray<AActor*> TargetArray;
	AActor* OwnerActor = GetOwner();
	FVector StartLocation;
	FHitResult OutHit;
	AActor* HitActor = nullptr;

	if (OwnerActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerActor is null!"));
		return nullptr;
	}

	// Get the location of the owner actor
	StartLocation = OwnerActor->GetActorLocation();

	TargetArray.Add(OwnerActor);

	if (FollowCamera == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("FollowCamera is null!"));
		return nullptr;
	}
	TargetObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	FVector X = FollowCamera->GetForwardVector() * TargetingDistance;
	FVector EndLocation = X + OwnerActor->GetActorLocation();

	bool hit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		GetWorld(),
		StartLocation,
		EndLocation,
		TargetRadius,
		TargetObjectTypes,
		false,
		TargetArray,
		EDrawDebugTrace::None,
		OutHit,
		true,
		FColor::Red,
		FColor::Green,
		5
	);

	if (hit && OutHit.GetActor())
	{
		HitActor = OutHit.GetActor();
		if (HitActor->GetClass()->ImplementsInterface(UTargeting_CI::StaticClass())) {
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName());
			return HitActor;
		}
		else {
			return nullptr;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid hit actor found!"));
		return nullptr;
	}
}

void UC_TargetingComponent::UpdateTargetingControlRotation()
{
	auto* TargetCharecter = Cast<AC_MasterAI>(TargetActor);
	if (!TargetActor  ) {
		disableLockOn();
		return;
	}
	FRotator NewRrotation;
	NewRrotation.Roll = OwnerController->GetControlRotation().Roll;

	FVector TargetLocation = TargetActor->GetActorLocation() - FVector(0.0f, 0.0f, 150.0f);  // Adjusted the calculation here
	FVector OwnerLocation = GetOwner()->GetActorLocation();

	// Calculate the look rotation based on the relative location of TargetActor
	FRotator LookAt = UKismetMathLibrary::FindLookAtRotation( OwnerLocation, TargetLocation);

	// Interpolate the control rotation towards the look rotation
	FRotator NewTemp = UKismetMathLibrary::RInterpTo(OwnerController->GetControlRotation(),
		LookAt, GetWorld()->GetDeltaSeconds(),
		TargetInterpSpeed);

	NewRrotation.Yaw = NewTemp.Yaw;
	NewRrotation.Pitch = NewTemp.Pitch;

	if (OwnerCharacter && TargetActor)
	{
		
		
		if (TargetCharecter->CanBeTarget()) {
			
			TargetCharecter->bisGettingTargeted = true;
			OwnerController->SetControlRotation(NewRrotation);
		}
		else {
			TargetCharecter->bisGettingTargeted = false;
			disableLockOn();
		}
	}
	else {
		TargetCharecter->bisGettingTargeted = false;
		disableLockOn();
	}
}


void UC_TargetingComponent::SetRotationMode(TEnumAsByte<ERotationMode> NewRotationMode)
{
	UCharacterMovementComponent* CharacterMovement = OwnerCharacter->GetCharacterMovement();
	CurrentRotationMode = NewRotationMode;
	switch (CurrentRotationMode)
	{
	case ERotationMode::OrientToCamara:
		OwnerCharacter->bUseControllerRotationYaw = false;
	

		if (CharacterMovement)
		{
			// Set bUseControllerDesiredRotation
			CharacterMovement->bUseControllerDesiredRotation = true;

			// Set bOrientRotationToMovement
			CharacterMovement->bOrientRotationToMovement = false;
		}

		break;

	case ERotationMode::OrintToMovment:
		OwnerCharacter->bUseControllerRotationYaw = false;


		if (CharacterMovement)
		{
			// Set bUseControllerDesiredRotation
			CharacterMovement->bUseControllerDesiredRotation = false;

			// Set bOrientRotationToMovement
			CharacterMovement->bOrientRotationToMovement = true;
		}

		break;

		

	default:
		break;
	}
}

ERotationMode UC_TargetingComponent::GetRotationMode() const
{
	return CurrentRotationMode;
}

void UC_TargetingComponent::EnableLockon()
{
	TargetActor = FindTarget();
	
	
		SetTargetActor(TargetActor);
		SetIsTargeting(true);
		SetRotationMode(OrientToCamara);
	
}

void UC_TargetingComponent::disableLockOn()
{
	if(TargetActor){
	auto* TargetCharecter = Cast<AC_MasterAI>(TargetActor);
	
	TargetCharecter->bisGettingTargeted = false;
	}
	SetIsTargeting(false);
	SetTargetActor(nullptr);
	SetRotationMode(OrintToMovment);

}

void UC_TargetingComponent::ToggleLockOn()
{
	if (!GetIsTargeting()) {
		EnableLockon();
	}
	else {
		
		disableLockOn();
	}
}


		
	






