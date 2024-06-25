// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_BlueprintBase.h"
#include "c_EQSContext_Attack_Target.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API Uc_EQSContext_Attack_Target : public UEnvQueryContext_BlueprintBase
{
	GENERATED_BODY()
public: 
	Uc_EQSContext_Attack_Target();
	virtual UWorld* GetWorld() const override;
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};