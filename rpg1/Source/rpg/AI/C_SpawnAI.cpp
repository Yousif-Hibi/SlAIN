#include "C_SpawnAI.h"
#include "Engine/World.h"
#include "C_mageAI.h"
#include "PatrolPath.h"
#include "Kismet/GameplayStatics.h"
#include "rpg/Component/C_StatsComponent.h"
#include "rpg/Component/C_CombatComponent.h"
#include "rpg/Actors/C_magicSpell.h"
#include "rpg/Actors/C_BaseMagicWeapon.h"
#include "rpg/Actors/C_BaseWeapon.h"
#include "rpg/AI/C_MasterAI.h"
// Sets default values
AC_SpawnAI::AC_SpawnAI()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Ensure MageClass is initialized (optional, typically done in Editor)
	// MageClass = AC_mageAI::StaticClass();
}

// Called when the game starts or when spawned
void AC_SpawnAI::BeginPlay()
{
	Super::BeginPlay();
	for (int32 i = 0; i < NumMagesToSpawn; ++i)
	{
		AC_mageAI* SpawnedMage = GetWorld()->SpawnActor<AC_mageAI>(MageClass, GetActorLocation(), FRotator::ZeroRotator);
		if (SpawnedMage)
		{
			APatrolPath* ClosestPath = FindClosestPatrolPath();
			if (ClosestPath)
			{
				// Assign the patrol path
				SpawnedMage->SetPatrolPath(ClosestPath);
				SpawnedMage->StatsComponent->SetMaxStateValue(Estat::Health,200);
				SpawnedMage->StatsComponent->SetBaseStateValue(Estat::Health, 200);
				SpawnedMage->StatsComponent->ModifyCurrentStatValue(Estat::Health, 200, false);
				auto* weapon = Cast<AC_BaseMagicWeapon>( SpawnedMage->CombatComponent->GetMainWeapon());
				if (weapon)
				{
					weapon->MagicDamegeSet( 90.f);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Weapon  is null"));
				}
				UE_LOG(LogTemp, Warning, TEXT("Mage %d spawned and closest patrol path assigned."), i);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No patrol path found for Mage %d."), i);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn Mage %d."), i);
		}
	}


}

// Called every frame
void AC_SpawnAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

APatrolPath* AC_SpawnAI::FindClosestPatrolPath() const
{
	TArray<AActor*> PatrolPaths;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APatrolPath::StaticClass(), PatrolPaths);

	APatrolPath* ClosestPath = nullptr;
	float MinDistance = TNumericLimits<float>::Max();
	FVector SpawnLocation = GetActorLocation();

	for (AActor* Actor : PatrolPaths)
	{
		APatrolPath* PatrolPath = Cast<APatrolPath>(Actor);
		if (PatrolPath)
		{
			float Distance = FVector::Dist(SpawnLocation, PatrolPath->GetActorLocation());
			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				ClosestPath = PatrolPath;
			}
		}
	}

	return ClosestPath;
}
