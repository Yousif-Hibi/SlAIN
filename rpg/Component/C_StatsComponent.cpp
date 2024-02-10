// Fill out your copyright notice in the Description page of Project Settings.


#include "rpg/Component/C_StatsComponent.h"
#include <Kismet/BlueprintMapLibrary.h>
#include <Kismet/KismetSystemLibrary.h>
#include <rpg/Enums/E_ChartacterState.h>
#include "rpg/SoulsLikeCharacter.h"
#include "rpg/Component/Manger.h"

// Sets default values for this component's properties
UC_StatsComponent::UC_StatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UC_StatsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UC_StatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	

}

float UC_StatsComponent::GetBaseStateValue(Estat stat)
{


	if (BaseStat.Find(stat)) {
		return BaseStat.Find(stat)->BaseValue;


	}
	

	
	return 0.0f;
}

float UC_StatsComponent::GetMaxStateValue(Estat stat)
{
	if (BaseStat.Find(stat)) {
		return BaseStat.Find(stat)->MaxValue;


	}
	return 0.0f;
}

void UC_StatsComponent::SetBaseStateValue(Estat stat, float Value)
{
	FBaseStat& BaseState = BaseStat.FindOrAdd(stat);
	BaseState.BaseValue = Value;
}

void UC_StatsComponent::SetMaxStateValue(Estat stat, float Value)
{
	FBaseStat& BaseState = BaseStat.FindOrAdd(stat);
	BaseState.MaxValue = Value;

}

void UC_StatsComponent::SetCurrentStateValue(Estat stat, float Value)
{
	// Add the key-value pair to the StatsMap
	CurrentState.Add(stat, Value);
}

float UC_StatsComponent::GetCurrentStateValue(Estat stat)
{
	
	float Value = 0.0f; // Default value
	
	float* FoundValue = CurrentState.Find(stat);
	
	if (FoundValue)
	{
		return *FoundValue;
	}
	
	return 0;
}

void UC_StatsComponent::InitialState()
{
	
	TArray<TEnumAsByte< Estat>> Keys;

	BaseStat.GetKeys(Keys);

	for (const Estat & Key : Keys)
	{
		FBaseStat Value;
		if (BaseStat.Find(Key))
		{
		
			SetCurrentStateValue(Key, BaseStat.Find(Key)->BaseValue);
		}
	}

}

void UC_StatsComponent::ModifyCurrentStatValue(Estat stat, float Value, bool bShouldRegenrate)
{
	if (bShouldRegenrate) {
		checkRegen = false;
		if (UKismetSystemLibrary::K2_IsTimerActive(this, "dealyRegen")) {
			UKismetSystemLibrary::K2_ClearTimer(this, "dealyRegen");
		}
	}
	if (CurrentState.Contains(stat)) {
		
	   float Temp=GetCurrentStateValue(stat) + Value;
	  
	  Temp= FMath::Clamp(Temp, 0, GetMaxStateValue(stat));
	  

	  SetCurrentStateValue(stat, Temp);
	  
	  if (bShouldRegenrate) {
		  temp = stat;

		  UKismetSystemLibrary::K2_SetTimer(this, "dealyRegen", 5.0f, true);
	  }
	  
	}

}

void UC_StatsComponent::TakeDamage(float InDamage)
{
	float Damage =  InDamage+GetCurrentStateValue(Estat::Armor);
	 Damage=InDamage*(InDamage/ Damage);
	 
	LocalDamage= FMath::Clamp(Damage, 0, Damage);
	LocalDamage *= -1;
	UE_LOG(LogTemp, Warning, TEXT("Damege dsds %f"), LocalDamage);
	
	ModifyCurrentStatValue(Health, LocalDamage, true);
	if (GetCurrentStateValue(Health) <= 0) {

		ASoulsLikeCharacter* Character = Cast<ASoulsLikeCharacter>(GetOwner());
	 Character->manger->SetState(EChartacterState::Dead);

	}
}

void UC_StatsComponent::RegenStamina()
{
	if (checkRegen) {
		float MaxStamina = GetMaxStateValue(Stamina);
		float StaminaRegen = StaminaRegenRate + GetCurrentStateValue(Stamina);
		float sTmp = FMath::Clamp(StaminaRegen, 0, MaxStamina);
		SetCurrentStateValue(Stamina, sTmp);
		UE_LOG(LogTemp, Warning, TEXT("regen %f"), GetCurrentStateValue(Stamina));
		if (GetCurrentStateValue(Stamina) >= MaxStamina) {

			UKismetSystemLibrary::K2_ClearTimer(this, "RegenStamina");

		}
	}

}

void UC_StatsComponent::StartRegen(Estat stat)
{

	FLatentActionInfo LatentInfo;
	switch (stat)
	{
	case Estat::NoState:
		//logic for 'None' enum entry
		
		break;

	case Estat::Health:
		// logic for 'Health' enum entry
	
		break;

	case Estat::Stamina:
		//logic for 'Stamina' enum entry
		
		LatentInfo.ExecutionFunction = FName("RegenStamina");  
		
		UKismetSystemLibrary::K2_ClearTimer(this, "RegenStamina");
		UKismetSystemLibrary::RetriggerableDelay(this, 5.0f, LatentInfo);
		UKismetSystemLibrary::K2_SetTimer(this, "RegenStamina",0.3f, true);
		
		

		break;

	case Estat::Armor:
		//  logic for 'Armor' enum entry
	
		break;

	default:
	//Implement default case or handle any other enum values

		break;
	}
}

void UC_StatsComponent::dealyRegen()
{
	checkRegen = true;
	StartRegen(temp);
}




