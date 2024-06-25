// Fill out your copyright notice in the Description page of Project Settings.


#include "C_HumanoidEnemy.h"

void AC_HumanoidEnemy::Tick(float DeltaTime)
{
	ChangeHealth();
	Track();
}
