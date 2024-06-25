// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Delegates/Delegate.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "C_CollisionCombonent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHits,FHitResult,onHit);




UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UC_CollisionCombonent : public UActorComponent
{
	GENERATED_BODY()
private:
	static  UC_CollisionCombonent* eventDispatherSingletons;
	
	FHitResult HitRes;

public:
	// Sets default values for this component's properties
	UC_CollisionCombonent();
	~UC_CollisionCombonent() noexcept;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void OnHitEvent(FHitResult hit);
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY(EditAnywhere)
	bool bIsCollisionEnabled = false;


	UPROPERTY(EditAnywhere)
	TArray<AActor*> AreadyHitActors;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(EditAnywhere)
	 FHitResult LastHit;

	UPROPERTY(EditAnywhere)
	AActor* HitActor;

	UPROPERTY(EditAnywhere)
	UPrimitiveComponent* CollisionMeshComponent;


	UPROPERTY(EditAnywhere, Category = "Intlization")
	FName StartSocketName;

	UPROPERTY(EditAnywhere, Category = "Intlization")
	FName EndSocketName;

	UPROPERTY(EditAnywhere, Category = "Intlization")
	float TraceRadius = 20.0f;


	UPROPERTY(EditAnywhere, Category = "Intlization")
	TArray<TEnumAsByte<EObjectTypeQuery>> CollisionObjectTypes;




	UPROPERTY(EditAnywhere)
	FHitResult HitResult;


	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<AActor*> GetAreadyHitActiors();

	UFUNCTION(BlueprintCallable)
	void SetAreadyHitActiors(TArray<AActor*> AreadyHit);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool  IsCollisionEnabled();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FHitResult GetLastHit();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UPrimitiveComponent* GetCollisionMesh();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<AActor*> GetActorsToIgnor();

	UFUNCTION(BlueprintCallable)
	void AddActorsToIgnore(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	void RemoveActorToIgnore(AActor* actor);

	UFUNCTION(BlueprintCallable)
	void SetCollisonMesh(UPrimitiveComponent* CollisionMesh);

	UFUNCTION(BlueprintCallable)
	void ActivateColision();

	UFUNCTION(BlueprintCallable)
	void DeactivatColision();

	UFUNCTION(BlueprintCallable)
	void ClearHitActors();


	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CollisionTrace();





	UFUNCTION(BlueprintPure, Category = "eventSetUp")
	static   UC_CollisionCombonent* GetEventSingleton();

	UFUNCTION(BlueprintCallable, Category = "eventSetUp")
	static  void BindEventDispatherCpp();

	UFUNCTION(BlueprintCallable, Category = "eventSetUp")
	static  void UnbindEventDispatherCpp();

	UFUNCTION(BlueprintCallable, Category = "eventSetUp")
	static  void CallEventDispatherCpp();

	UPROPERTY(BlueprintAssignable)
	FOnHits FonHit;











};
