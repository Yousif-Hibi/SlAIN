// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_magicSpell.generated.h"


UCLASS()
class RPG_API AC_magicSpell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_magicSpell();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent * Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystemComponent * Particales;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	 UParticleSystem* StartParticales;
	 UPROPERTY(EditAnywhere, BlueprintReadWrite)
	 UParticleSystem* hitParticales;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 intDamege = 20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float  Dameges = 20.f;
	UPROPERTY()
	AActor* TargetActor;

	UFUNCTION()
	void SetSpellDameges(float spellDmg);


	UFUNCTION()
	void OnHit(UPrimitiveComponent * hitComp,AActor* OtherActor, UPrimitiveComponent* OtherComp,FVector NormalImpulse,const FHitResult& Hit);
	UFUNCTION()
	void AttackDealy();
	UFUNCTION()
	void SetTargetActor(AActor* NewTarget);
};
