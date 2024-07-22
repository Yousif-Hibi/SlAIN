// Copyright Epic Games, Inc. All Rights Reserved.

#include "SoulsLikeCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Actors/C_BaseWeapon.h"
#include <Kismet/KismetSystemLibrary.h>
#include "Enums/E_CombatType.h"
#include "Component/C_CombatComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include <Sound/SoundCue.h>
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayTagsManager.h"
#include "Component/Manger.h"
#include "GameFramework/PlayerController.h"
#include "Component/C_StatsComponent.h"
#include "Blueprint/UserWidget.h"
#include "C_PlayerController.h"
#include "Component/C_TargetingComponent.h"
#include "Actors/C_BaseMagicWeapon.h"
#include "Actors/C_magicSpell.h"
#include "Particles/ParticleSystemComponent.h"
#include "Enums/E_DamageType.h"
#include "rpg/Actors/C_BaseDualWeapon.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include <Perception/AISense_Sight.h>
#include "rpg/AI/C_QuestKnight.h"
#include "Kismet/GameplayStatics.h"
#include "C_SaveGame.h"
#include "C_SpawnPoints.h"
#include "rpg/AI/C_MasterAI.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "rpg/AI/C_SpawnAI.h"


//////////////////////////////////////////////////////////////////////////
#define Intractable ECC_GameTraceChannel1
// ASoulsLikeCharacter

ASoulsLikeCharacter::ASoulsLikeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)



	CombatComponent= CreateDefaultSubobject<UC_CombatComponent>(TEXT("combat"));
	manger= CreateDefaultSubobject<UManger>(TEXT("Manger"));
	StatsComponents = CreateDefaultSubobject<UC_StatsComponent>(TEXT("StatsComponentss"));
	TargetingComponent = CreateDefaultSubobject<UC_TargetingComponent>(TEXT("TargetingComponent"));


	StatsComponents->tokkenAmount = 5;

	manger->stateBegin.AddDynamic(this, &ASoulsLikeCharacter::OnStateBegin);
	manger->stateEnd.AddDynamic(this, &ASoulsLikeCharacter::OnStateEnd);
	manger->CharacterActionBegin.AddDynamic(this, &ASoulsLikeCharacter::OnActionBegin);
	manger->CharacterActionEnd.AddDynamic(this, &ASoulsLikeCharacter::OnActionEnd);
	SetUpSyimulusSource();
	

}

//////////////////////////////////////////////////////////////////////////
// Input

void ASoulsLikeCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASoulsLikeCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	//ToggleCombat
	PlayerInputComponent->BindAction("ToggleCombat", IE_Pressed, this, &ASoulsLikeCharacter::ToggleCombat );
	//Intract
	PlayerInputComponent->BindAction("Intract", IE_Pressed, this, &ASoulsLikeCharacter::Intract);
	//AttackTypes
	PlayerInputComponent->BindAction("LightAttack", IE_Pressed, this, &ASoulsLikeCharacter::ChargedAttack);
	PlayerInputComponent->BindAction("LightAttack", IE_Released, this, &ASoulsLikeCharacter::LightAttack);
	PlayerInputComponent->BindAction("HeavyAttack", IE_Pressed, this, &ASoulsLikeCharacter::HeavyAttack);
	//pause
	PlayerInputComponent->BindAction("pause", IE_Pressed, this, &ASoulsLikeCharacter::Pause);
	//Dodge
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &ASoulsLikeCharacter::Dodge);
	//stats
	PlayerInputComponent->BindAction("stats", IE_Pressed, this, &ASoulsLikeCharacter::stats);
	//walking
	PlayerInputComponent->BindAction("ToggleWalk", IE_Pressed, this, &ASoulsLikeCharacter::ToggleWalk);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASoulsLikeCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASoulsLikeCharacter::StopSprint);

	//Lock on character
	PlayerInputComponent->BindAction("ToggleLockOn", IE_Pressed, this, &ASoulsLikeCharacter::ToggleLockOn);
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ASoulsLikeCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ASoulsLikeCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ASoulsLikeCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ASoulsLikeCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ASoulsLikeCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ASoulsLikeCharacter::TouchStopped);
}



void ASoulsLikeCharacter::ToggleCombat()
{
	
	
		UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
		if (IsValid(CombatComponent->GetMainWeapon())) {
			float playrate = 1.0f;
			if (CanPreformToggleCombat()) {
				
			if (!CombatComponent->IsCombatEnabled()) {
				UE_LOG(LogTemp, Warning, TEXT("xxxx %d"), CombatComponent->IsCombatEnabled());
				if (animInstance) {
					TempMontage = CombatComponent->GetMainWeapon()->GetActionMontages(EChartacterAction::EnterCombat)[0];
					manger->SetState(EChartacterState::GeneralActionState);
					manger->SetAction(EChartacterAction::EnterCombat);
					PlayAnimMontage(TempMontage, 1.0f, FName("Default"));
					
				}
				}
			
			else {
				UE_LOG(LogTemp, Warning, TEXT("yyyyyy %d"), CombatComponent->IsCombatEnabled());
				if (animInstance) {
					TempMontage = CombatComponent->GetMainWeapon()->GetActionMontages(EChartacterAction::ExitCombat)[0];
					manger->SetState(EChartacterState::Disabled);
					manger->SetAction(EChartacterAction::ExitCombat);
					PlayAnimMontage(TempMontage, 1.0f, FName("Default"));
					

				}
			}
		}
		}
	
}

void ASoulsLikeCharacter::ToggleLockOn()
{
	TargetingComponent->ToggleLockOn();
}

void ASoulsLikeCharacter::Intract()
{
	FVector Location = GetActorLocation();
	TArray<TEnumAsByte<EObjectTypeQuery>> CollisionObjectTypes;
	CollisionObjectTypes.Add(UEngineTypes::ConvertToObjectType(Intractable));
	TArray<AActor*> ActorsToIgnore ;
	
	ActorsToIgnore.Add(this);
	FHitResult OutHit;
	bool hit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		GetWorld(),
		Location,
		Location,
		100.f,
		CollisionObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		OutHit,
		true
	);
	if (hit) {
	 	AActor* HitRes = OutHit.GetActor();
		if (HitRes)
		{

			AC_BaseWeapon* mainWeapon = CombatComponent->GetMainWeapon();
			AC_BaseDualWeapon* dualweapon = Cast<AC_BaseDualWeapon>(mainWeapon);
			AC_BaseMagicWeapon* magicGlove = Cast<AC_BaseMagicWeapon>(mainWeapon);
			if (mainWeapon)
			{
				switch (mainWeapon->CombatType)
				{
				case None:
					break;
				case Lightsword:
					mainWeapon->ToggleCombat(false);
					damageType = EDamageType::MeleDamage;
					break;
				case GreatSword:

					mainWeapon->ToggleCombat(false);
					damageType = EDamageType::MeleDamage;
					break;
				case Axe:
					mainWeapon->ToggleCombat(false);
					damageType = EDamageType::MeleDamage;
					break;
				case DualSword:
					dualweapon->ToggleCombat(false);
					damageType = EDamageType::MeleDamage;
					break;
				case MagicHand:
					magicGlove->ToggleCombat(false);
					SetIsMagic(true);
					break;
				default:
					break;
				}



			}
			if (IInteractive_CI* interacted = Cast<IInteractive_CI>(HitRes)) {
				interacted->Intracts(this);
			}
		}
	}
	UPrimitiveComponent* HitPrimitiveComponent = OutHit.GetComponent();
	if (HitPrimitiveComponent)
	{
		
		auto* HitCharacter = Cast<AC_QuestKnight>(HitPrimitiveComponent->GetOwner());
		if (HitCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("talking to npc1"));
			if (IInteractive_CI* interacted = Cast<IInteractive_CI>(HitCharacter)) {
				interacted->TalkToNPC();
			}
		}
	}
}

void ASoulsLikeCharacter::LightAttack()
{
	


		bHeavyAttack = false;
		if (manger->GetCurrentState() == EChartacterState::Attacking) {

			CombatComponent->SetAttackSaved(true);

		}
		else {
			ContinueAttack();
			if (CanPreFormAttack()) {
				AttackEvent();


			}
		}
	
}

void ASoulsLikeCharacter::Pause()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Paused"));
	if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		UE_LOG(LogTemp, Warning, TEXT("Game Resumed"));
		WidgetInstance->RemoveFromViewport();
	}
	else
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		UE_LOG(LogTemp, Warning, TEXT("Game Paused"));
		if (pauseWidgetClass)
		{

			WidgetInstance = CreateWidget<UUserWidget>(GetWorld(), pauseWidgetClass);

			if (WidgetInstance)
			{
				UE_LOG(LogTemp, Warning, TEXT("Settings widget created: %s"), *WidgetInstance->GetClass()->GetName());
				// Add widget to viewport
				WidgetInstance->AddToViewport();
				APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
				if (PlayerController)
				{
					PlayerController->bShowMouseCursor = true;
					PlayerController->SetInputMode(FInputModeUIOnly());
				}
			}
		}
	}
}

void ASoulsLikeCharacter::stats()
{
	UE_LOG(LogTemp, Warning, TEXT("stats"));
	if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		UE_LOG(LogTemp, Warning, TEXT("Game Resumed"));
		WidgetInstance->RemoveFromViewport();
	}
	else
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		UE_LOG(LogTemp, Warning, TEXT("Game Paused"));
		if (statsWidgetClass)
		{

			WidgetInstance = CreateWidget<UUserWidget>(GetWorld(), statsWidgetClass);

			if (WidgetInstance)
			{
				UE_LOG(LogTemp, Warning, TEXT("Settings widget created: %s"), *WidgetInstance->GetClass()->GetName());
				// Add widget to viewport
				WidgetInstance->AddToViewport();

				APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
				if (PlayerController)
				{
					PlayerController->bShowMouseCursor = true;
					PlayerController->SetInputMode(FInputModeUIOnly());
				}


			}
		}
	}
}

void ASoulsLikeCharacter::HeavyAttack()
{
	
	bHeavyAttack = true;
	AttackEvent();
}

void ASoulsLikeCharacter::ChargedAttack(FKey Key)
{
	
	TrackChargedAttack(Key);
}

void ASoulsLikeCharacter::ChargedAttackEvent()
{
	if (CanPreFormAttack()) {
		
		PerformAttack(EChartacterAction::ChargedAttack, CombatComponent->GetAttackCount(), false);
	}
}

void ASoulsLikeCharacter::ChargedAttackTimer()
{
	APlayerController * APlayerController= UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AttackHeldTime= APlayerController->GetInputKeyTimeDown(TempKey)  ;
	if (AttackHeldTime >= 0.18) {
		bChargedAttack = true;
		ChargedAttackEvent();

	}
}

void ASoulsLikeCharacter::TrackChargedAttack(FKey Key)
{
	TempKey = Key;
	FTimerHandle TimeHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimeHandle,
		this,
		&ASoulsLikeCharacter::ChargedAttackTimer, // Corrected function pointer
		0.016f,
		true
	);
}

void ASoulsLikeCharacter::ToggleWalk()
{
	if (GetMovmentSpeedMode() != EMovementSpeedMode::Walking) {
		SetMovmentSpeedMode(EMovementSpeedMode::Walking);
	}
	else {
		SetMovmentSpeedMode(EMovementSpeedMode::Jogging);
	}
}

void ASoulsLikeCharacter::Sprint()
{
	
	
	SetMovmentSpeedMode(EMovementSpeedMode::Sprinting);
	GetWorld()->GetTimerManager().SetTimer(
		SprintTimerHandle,
		this,
		&ASoulsLikeCharacter::SprintStaminaCost,
		0.2f,
		true
	);
	
}

void ASoulsLikeCharacter::StopSprint()
{
	GetWorldTimerManager().ClearTimer(SprintTimerHandle);
	if(GetMovmentSpeedMode()== EMovementSpeedMode::Sprinting)
	SetMovmentSpeedMode(EMovementSpeedMode::Jogging);
}

void ASoulsLikeCharacter::Dodge()
{
	
	if (CombatComponent->GetMainWeapon()) {
		
		if (CanPreformDodge()) {
		
			PreformDodge(1, true);

		}
	}
}

bool ASoulsLikeCharacter::CanPreformToggleCombat()
{
	
		TArray<TEnumAsByte< EChartacterState>> temp = { EChartacterState::Attacking, EChartacterState::Dead, EChartacterState::Disabled, EChartacterState::Dodging, EChartacterState::GeneralActionState };
		if (!manger->IsCurrentStateEqualToAny(temp)) {
			if (!GetCharacterMovement()->IsFalling())
				return true;
		}
	
	return false;
}

bool ASoulsLikeCharacter::CanPreFormAttack()
{
	if (IsValid(CombatComponent->GetMainWeapon())) {
		if (CombatComponent->GetMainWeapon()->bCombat) {
			TArray<TEnumAsByte< EChartacterState>> temp = { EChartacterState::Attacking, EChartacterState::Dead, EChartacterState::Disabled, EChartacterState::Dodging, EChartacterState::GeneralActionState };
			if (!manger->IsCurrentStateEqualToAny(temp)) {
				if (StatsComponents->GetCurrentStateValue(Estat::Stamina) > 10)
					return true;
			}
		}
	}

	return false;

}

bool ASoulsLikeCharacter::CanPreformDodge()
{
	
		TArray<TEnumAsByte< EChartacterState>> temp = { EChartacterState::Dead, EChartacterState::Disabled, EChartacterState::Dodging, EChartacterState::GeneralActionState };
		if (!manger->IsCurrentStateEqualToAny(temp)) {
			if (!GetCharacterMovement()->IsFalling()) {
				if (StatsComponents->GetCurrentStateValue(Estat::Stamina) > 10) {
					return true;
				}
			}
		}
	
	return false;
}

bool ASoulsLikeCharacter::CanPreformJump()
{
	TArray<TEnumAsByte< EChartacterState>> temp = {  EChartacterState::Dead, EChartacterState::Disabled, EChartacterState::Dodging, EChartacterState::GeneralActionState };
	if (!manger->IsCurrentStateEqualToAny(temp)) {
	
			return true;
	}
	return false;
}

bool ASoulsLikeCharacter::CanRecieveHitreaction()
{
	TArray<TEnumAsByte< EChartacterState>> temp = {  EChartacterState::Dead };
	if (!manger->IsCurrentStateEqualToAny(temp)) {
		if (!GetCharacterMovement()->IsFalling())
			return true;
	}
	return false;
}

void ASoulsLikeCharacter::PreformDodge(int32 MontageIndex, bool bRandomIndex)
{
	if (CanPreformDodge()) {
		if (CombatComponent->GetMainWeapon()) {
			AC_BaseWeapon* x = CombatComponent->GetMainWeapon();
			int32 size = x->DodgeMontage.Num();

			if (size < MontageIndex) {
				return;
			}
			if (size >= 1) {



				if (bRandomIndex) {
					MontageIndex = 0;
				}
				TempMontage = CombatComponent->GetMainWeapon()->DodgeMontage[0];
				if (IsValid(TempMontage)) {
					
					manger->SetState(EChartacterState::Dodging);
					manger->SetAction(EChartacterAction::Dodge);

					int32 Value = -1.0f * CombatComponent->GetMainWeapon()->GetStatCostForAction();
					StatsComponents->ModifyCurrentStatValue(Estat::Stamina,Value, true);
					
					PlayAnimMontage(TempMontage, 1.0f, FName("Default"));

					



				}

			}
		}
	}
}

void ASoulsLikeCharacter::AttackEvent()
{
	
	if (CombatComponent->GetMainWeapon()) {
		
		if (bHeavyAttack) {
			PerformAttack(EChartacterAction::HeavyAttack, CombatComponent->GetAttackCount(), false);
			

		}
		else if (GetCharacterMovement()->IsFalling()) {
			PerformAttack(EChartacterAction::FallingAttack, CombatComponent->GetAttackCount(), false);
		}
		else if (GetMovmentSpeedMode() == EMovementSpeedMode::Sprinting) {
			PerformAttack(EChartacterAction::SprintAttack, CombatComponent->GetAttackCount(), false);
		}
		else {
			PerformAttack(EChartacterAction::LightAttack, CombatComponent->GetAttackCount(), false);
		
		}
	}
}


void ASoulsLikeCharacter::PerformAttack(TEnumAsByte< EChartacterAction> AttackType,int32 AttackIndex, bool bRandomIndex)
{
	
	

	if (CanPreFormAttack()) {
		if (CombatComponent->GetMainWeapon()) {
			
		
				AC_BaseWeapon* x = CombatComponent->GetMainWeapon();
				int32 size = x->GetActionMontages(AttackType).Num();

				if (size < AttackIndex) {
					return;
				}
				if (size >= 1) {
					if (bRandomIndex) {
						AttackIndex = FMath::RandRange(0, x->AttackMontage.Num());
					}
					TempMontage = CombatComponent->GetMainWeapon()->GetActionMontages(AttackType)[AttackIndex];
					if (IsValid(TempMontage)) {
						manger->SetState(EChartacterState::Attacking);
						manger->SetAction(AttackType);
						
						int32 Value = -1.0f * CombatComponent->GetMainWeapon()->GetStatCostForAction();
						UE_LOG(LogTemp, Warning, TEXT("hi there  in preform attack %d"), Value);
						StatsComponents->ModifyCurrentStatValue(Estat::Stamina, Value, true);
						PlayAnimMontage(TempMontage, 1.0f, FName("Default"));
						AttackIndex++;
						CombatComponent->SetAttackCount(AttackIndex);

						if (AttackIndex > size - 1) {
							CombatComponent->SetAttackCount(0);
						}
						
					}

				}
			}
		}
	
}

void ASoulsLikeCharacter::PreformAction(UAnimMontage* ActionMontage, EChartacterAction Action, EChartacterState state, int32 MontageIndex, bool bRandomIndex)
{
	
	
		if (CombatComponent->GetMainWeapon()) {
			AC_BaseWeapon* x = CombatComponent->GetMainWeapon();
			int32 size = x->DodgeMontage.Num();

			if (size < MontageIndex) {
				return;
			}
			if (size >= 1) {



				if (bRandomIndex) {
					MontageIndex = 0;
				}
				TempMontage = CombatComponent->GetMainWeapon()->GetActionMontages(Action)[0];
				if (IsValid(ActionMontage)) {
					
					manger->SetState(state);
					manger->SetAction(Action);
					int32 Value = CombatComponent->GetMainWeapon()->GetStatCostForAction();
					StatsComponents->ModifyCurrentStatValue(Estat::Stamina,Value, true);
					PlayAnimMontage(ActionMontage, 1.0f, FName("Default"));



				}

				}

			}
		
}

void ASoulsLikeCharacter::DamegeTaken(float inDamage)
{
	Health =Health- inDamage;
	if (Health < 0) {
		Health = 0;
	}
	
	if (Health <= 0) {
		manger->SetState(EChartacterState::Dead);

	}
}

void ASoulsLikeCharacter::EnableRagdoll()
{
	if (Health >=0) {
		GetCharacterMovement()->SetMovementMode(MOVE_None, 0);

		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		CameraBoom->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, PelvisBoneName);
		CameraBoom->bDoCollisionTest = false;
		GetMesh()->SetCollisionProfileName("Ragdoll", true);
		GetMesh()->SetAllBodiesBelowSimulatePhysics(PelvisBoneName, true);
		GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(PelvisBoneName, 1.0f);
	}


}

void ASoulsLikeCharacter::ApplyHitReactionPhysicsVelocity(float initialSpeed)
{
	initialSpeed *= -1;
	FVector vector= initialSpeed * GetActorForwardVector();
	GetMesh()->SetPhysicsLinearVelocity(vector, false,PelvisBoneName);



}

void ASoulsLikeCharacter::PerformDeath()
{
	FTimerHandle TimerHandle;
	manger->SetState(EChartacterState::Dead);
	
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	
				if (animInstance) {
					
					PlayAnimMontage(DeathMontage, 1.0f, FName("Default"));

				}

	ApplyHitReactionPhysicsVelocity(2000);
	
	GetCharacterMovement()->DisableMovement();

	DespawnAllMasterAI();

	// Start a timer to respawn
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ASoulsLikeCharacter::Respawn, 4.0f, false);

	
}

void ASoulsLikeCharacter::PerformDeathAfterDelay()
{
	if (IsValid(CombatComponent->GetMainWeapon())) {
		 CombatComponent->GetMainWeapon()->Destroy();
	}
	GetMesh()->DestroyComponent();

	
	
}

void ASoulsLikeCharacter::DespawnAllMasterAI()
{

	UWorld* World = GetWorld();
	// Ensure the World is valid
	if (!World) return;

	// Iterate over all instances of AC_MasterAI in the world
	for (TActorIterator<AC_MasterAI> It(World); It; ++It)
	{
		// Get the current actor
		AC_MasterAI* MasterAI = *It;

		// Ensure the actor is valid
		if (MasterAI)
		{
			// Destroy the actor
			MasterAI->GetRootComponent()->SetVisibility(false, true);
			MasterAI->PerformDeath();
		}
	}



}

void ASoulsLikeCharacter::ContinueAttack()
{
	ICombat_CI::ContinueAttack();
	manger->SetState(EChartacterState::Nothing);
	
	if (CombatComponent->GetAttackSaved()) {
		CombatComponent->SetAttackSaved(false);
	}
}

void ASoulsLikeCharacter::RestAttack()
{
	ICombat_CI::RestAttack();
	CombatComponent->RestAttack();
	
}

FRotator ASoulsLikeCharacter::GetDesiredRotation()
{
	ICombat_CI::GetDesiredRotation();
	if (!GetCharacterMovement()->GetLastInputVector().Equals( FVector::ZeroVector, 0.001f)) {
		
		return	GetLastMovementInputVector().Rotation();
	}
	else {
		return GetActorRotation();
	}
	return GetActorRotation();
}

void ASoulsLikeCharacter::RestCombat()
{
	ICombat_CI::RestCombat();

	CombatComponent->RestAttack();
	manger->RestState();
	manger->SetAction(EChartacterAction::NoAction);
}

float ASoulsLikeCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	StatsComponents->TakeDamage(DamageAmount);
	
	 
	
	if (CanRecieveHitreaction()) {
		if (IsValid(DamageCauser)) {
			FVector thisVector = this->GetActorForwardVector();

			FVector otherVector = (DamageCauser->GetActorForwardVector());
			float n = FVector::DotProduct(thisVector, otherVector);
			if (IsValueInRange(n, -1.0f, 0.1f, true, true)) {
				UE_LOG(LogTemp, Warning, TEXT("name , %f "), n);
				bHitFront = true;
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("noname ")); 
				bHitFront = false;
			}
		}

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
			hitParticals,
			 GetActorLocation()
		);
		}
		bisMagic = CombatComponent->bmaigcenabled;
		UE_LOG(LogTemp, Warning, TEXT("name , %d "), bisMagic);
		ApplyHitReaction(damageType);
		if (StatsComponents->GetCurrentStateValue(Estat::Health) == 0) {
			manger->SetState(EChartacterState::Dead);
		}
	
	return 0.0f;
}

bool ASoulsLikeCharacter::CanRecieveDamage()
{

	ICombat_CI::CanRecieveDamage();
	bool check = (manger->GetCurrentState()!=EChartacterState::Dead);
	return check;
}

void ASoulsLikeCharacter::Jump()
{
	if (CanPreformJump()) {
		StopAnimMontage();
		manger->RestState();
		RestCombat();
		ACharacter::Jump();
	}
}

void ASoulsLikeCharacter::SetCanMove(bool isCanMove)
{
	bCanMove = isCanMove;
}

bool ASoulsLikeCharacter::ReserveAttakTokken(int32 Amount)
{
	return StatsComponents->ReserveToken(Amount);
}

void ASoulsLikeCharacter::ReturnAttackTokken(int32 Amount)
{
	StatsComponents->ReturnToken(Amount);
}

bool ASoulsLikeCharacter::IsValueInRange(float Value, float Min, float Max, bool InclusiveMin, bool InclusiveMax)
{
	return (Value >= Min && Value <= Max) || (InclusiveMin && Value == Min) || (InclusiveMax && Value == Max);
}



void ASoulsLikeCharacter::OnStateBegin(EChartacterState CharacterState)
{
	if (CharacterState == EChartacterState::Dead) {
		bisGettingTargeted = false;
		PerformDeath();
		
	}
}

void ASoulsLikeCharacter::OnStateEnd(EChartacterState CharacterState)
{
}

void ASoulsLikeCharacter::OnActionBegin(EChartacterAction CharacterAction)
{
}

void ASoulsLikeCharacter::OnActionEnd(EChartacterAction CharacterAction)
{
}

void ASoulsLikeCharacter::SetMovmentSpeedMode(EMovementSpeedMode NewMovmetSpeed)
{
	if (NewMovmetSpeed != MovmentSpeedMode) {
		MovmentSpeedMode = NewMovmetSpeed;
	}
	switch (MovmentSpeedMode)
	{
	case EMovementSpeedMode::Walking:
		GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
		break;
	case EMovementSpeedMode::Jogging:
		GetCharacterMovement()->MaxWalkSpeed = JoggingSpeed;
		break;
	case EMovementSpeedMode::Sprinting:
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
		break;
	default:
		GetCharacterMovement()->MaxWalkSpeed = JoggingSpeed;
		break;
	}
}

TEnumAsByte < EMovementSpeedMode> ASoulsLikeCharacter::GetMovmentSpeedMode()
{
	return MovmentSpeedMode;
}

void ASoulsLikeCharacter::SprintStaminaCost()
{
	
		StatsComponents->ModifyCurrentStatValue(Estat::Stamina, -2, true);
		if (StatsComponents->GetCurrentStateValue(Estat::Stamina) <= 10) {
			
			StopSprint();
		}
	


}

void ASoulsLikeCharacter::DisableSprint()
{
}

void ASoulsLikeCharacter::Fire()
{

		UE_LOG(LogTemp, Warning, TEXT("yyyyyy"));
		
	


}

void ASoulsLikeCharacter::ApplyHitReaction(EDamageType DamageType)
{


	if (bisMagic) {
		PerfformKnockdown();
	}
	else {
		PerfformHitStun();
	}
		


	

}

void ASoulsLikeCharacter::PerfformHitStun()
{
	if (bHitFront) {
		PlayAnimMontage(HitReaction, 1.0f, FName("Default"));
	}
	else {
		PlayAnimMontage(backHitReaction, 1.0f, FName("Default"));
	}
	manger->SetState(EChartacterState::Disabled);

}

void ASoulsLikeCharacter::PerfformKnockdown()
{

	PlayAnimMontage(KnockdownReaction, 1.0f, FName("Default"));
	PlayAnimMontage(GeettingUpFromKnockdownReaction, 1.0f, FName("Default"));
	manger->SetState(EChartacterState::Disabled);
}

void ASoulsLikeCharacter::SetIsMagic(bool isMagic)
{
	bisMagic = isMagic;
}

void ASoulsLikeCharacter::SetUpSyimulusSource()
{

	stimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	if (stimuliSource) {
		stimuliSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		stimuliSource->RegisterWithPerceptionSystem();
	}




}

void ASoulsLikeCharacter::SaveGame()
{
	FString SaveSlotName = TEXT("PlayerSaveSlot");
	const int32 UserIndex = 0;

	UC_SaveGame* SaveGameInstance;

	// Check if the save game already exists
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, UserIndex))
	{
		// If it exists, load the existing save game
		SaveGameInstance = Cast<UC_SaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserIndex));
	}
	else
	{
		// If it does not exist, create a new save game instance
		SaveGameInstance = Cast<UC_SaveGame>(UGameplayStatics::CreateSaveGameObject(UC_SaveGame::StaticClass()));
	}

	if (SaveGameInstance)
	{
		// Update the save game data with the current game state
		SaveGameInstance->PlayerLocation = RespawnPoint.GetLocation();
		SaveGameInstance->PlayerRotation = this->GetActorRotation();
		SaveGameInstance->points = this->CharacterPoints;
		SaveGameInstance->Healthpoints = this->Healthpoints;
		SaveGameInstance->Staminapoints = this->Staminapoints;
		SaveGameInstance->Damagepoints = this->Damagepoints;

		// Save the updated game instance
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, UserIndex);
	}
}

void ASoulsLikeCharacter::LoadSaveGame()
{
	
		// Load the save game instance from the specified slot
		UC_SaveGame* LoadGameInstance = Cast<UC_SaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerSaveSlot"), 0));

		if (LoadGameInstance)
		{
			// Update the character's properties with the loaded values
			if (LoadGameInstance->PlayerLocation!=FVector::ZeroVector) {
				this->SetActorLocation(LoadGameInstance->PlayerLocation);
			}
			this->SetActorRotation(LoadGameInstance->PlayerRotation);
			this->CharacterPoints = LoadGameInstance->points;
			this->Healthpoints = LoadGameInstance->Healthpoints;
			this->Staminapoints = LoadGameInstance->Staminapoints;
			this->Damagepoints = LoadGameInstance->Damagepoints;
			StatsComponents->SetMaxStateValue(Estat::Health,100.f+(Healthpoints)*10);
			StatsComponents->SetMaxStateValue(Estat::Stamina, 100.f + (Staminapoints) * 10);
			if (LoadGameInstance->mainWeapon) {
				CombatComponent->SetMainWeapon(LoadGameInstance->mainWeapon);
				CombatComponent->GetMainWeapon()->Damege = (Damagepoints * 3) + CombatComponent->GetMainWeapon()->Damege;
			}
		}
		else {
			SaveGame();
		}
}





void ASoulsLikeCharacter::Respawn()
{
	UE_LOG(LogTemp, Warning, TEXT("asdasdasdasdasdasdasdasdas spawn"));
	SetActorTransform(RespawnPoint);
	SetActorLocation(RespawnPoint.GetLocation());
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	StatsComponents->ModifyCurrentStatValue(Estat::Health,100.f,false);
	StatsComponents->ModifyCurrentStatValue(Estat::Stamina, 100.f, false);
	GetMesh()->SetCollisionProfileName("Ragdoll", false);
	UWorld* World = GetWorld();
	// Ensure the World is valid
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("World is null"));
		return;
	}

	// Iterate over all instances of AC_SpawnAI in the world
	for (TActorIterator<AC_SpawnAI> It(World); It; ++It)
	{
		// Get the current actor
		AC_SpawnAI* SpawnAI = *It;

		// Ensure the actor is valid
		if (SpawnAI)
		{
			// Call the SendPostQuestRequest method
			SpawnAI->SendPostQestRequest ("x");
		}
	}
	

    SaveGame();
}

void ASoulsLikeCharacter::SetRespawnPoint(FTransform NewRespawnPoint)
{
	UE_LOG(LogTemp, Warning,TEXT("asdasdasdasdasdasdasdasdas set point"));
	RespawnPoint = NewRespawnPoint;
	SaveGame();
}



void ASoulsLikeCharacter::Tick(float DeltaTime)
{
	
}
	


void ASoulsLikeCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	StatsComponents->InitialState();
	if (MainWidget) {
		MainWidget->AddToPlayerScreen();
	}
	float num = StatsComponents->GetCurrentStateValue(Estat::Health);
	UE_LOG(LogTemp, Warning, TEXT("stamina %f"), num);
	damageType = EDamageType::noDamage;

	LoadSaveGame();










}

void ASoulsLikeCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	
		
		Jump();
	
}

void ASoulsLikeCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{

	
		StopJumping();

	
}



void ASoulsLikeCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ASoulsLikeCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ASoulsLikeCharacter::MoveForward(float Value)
{
	if(bCanMove)
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASoulsLikeCharacter::MoveRight(float Value)
{
	if (bCanMove)
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
