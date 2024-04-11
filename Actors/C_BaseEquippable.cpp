// Fill out your copyright notice in the Description page of Project Settings.


#include "C_BaseEquippable.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"

// Sets default values
AC_BaseEquippable::AC_BaseEquippable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	RootComponent = Root;

	ItemStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemStaticMesh"));
	ItemStaticMesh->SetupAttachment(Root);

	ItemSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemSkeletalMesh"));
	ItemSkeletalMesh->SetupAttachment(Root);



}

// Called when the game starts or when spawned
void AC_BaseEquippable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AC_BaseEquippable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UPrimitiveComponent* AC_BaseEquippable::GetItemMesh()
{

	if (IsValid( ItemStaticMesh->GetStaticMesh())) {
		return ItemStaticMesh;
	}
	else {
		return ItemSkeletalMesh;
	}
}

bool AC_BaseEquippable::IsEquipped()
{
	return bIsEquipped;
}

void AC_BaseEquippable::SetIsEquipped(bool IsEquipped)
{
	bIsEquipped = IsEquipped;
}

void AC_BaseEquippable::OnUnequipped()
{
	if (!bIsEquipped) {
		bIsEquipped = false;
	}
}

void AC_BaseEquippable::OnEquipped()
{
	bIsEquipped = true;
	AttachActor(AttachSocketName);
}

void AC_BaseEquippable::OnAIEquipped()
{
	bIsEquipped = true;
	AttachActor(AttachSocketName);
}

void AC_BaseEquippable::AttachActor(FName SocketName)
{
	
	

		ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner());
		if (CharacterOwner)
		{
			
			UMeshComponent* Mesh = CharacterOwner->GetMesh();
			if (Mesh)
			{
				this->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
			}
		}
	
	
}

