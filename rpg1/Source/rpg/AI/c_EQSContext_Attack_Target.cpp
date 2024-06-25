// Fill out your copyright notice in the Description page of Project Settings.


#include "c_EQSContext_Attack_Target.h"

Uc_EQSContext_Attack_Target::Uc_EQSContext_Attack_Target()
{
}

UWorld* Uc_EQSContext_Attack_Target::GetWorld() const
{
	
	return GetOuter()->GetWorld();
}

void Uc_EQSContext_Attack_Target::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	
	
}
