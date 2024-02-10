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
	StatsComponent = CreateDefaultSubobject<UC_StatsComponent>(TEXT("StatsComponent"));

	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	ParentTag = Manager.RequestGameplayTag("character.state");
	ChildTags = Manager.RequestGameplayTagChildren(ParentTag);
	
	manger->stateBegin.AddDynamic(this, &ASoulsLikeCharacter::OnStateBegin);
	manger->stateEnd.AddDynamic(this, &ASoulsLikeCharacter::OnStateEnd);
	manger->CharacterActionBegin.AddDynamic(this, &ASoulsLikeCharacter::OnActionBegin);
	manger->CharacterActionEnd.AddDynamic(this, &ASoulsLikeCharacter::OnActionEnd);


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


	//Dodge
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &ASoulsLikeCharacter::Dodge);

	//walking
	PlayerInputComponent->BindAction("ToggleWalk", IE_Pressed, this, &ASoulsLikeCharacter::ToggleWalk);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASoulsLikeCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASoulsLikeCharacter::StopSprint);


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
			
					if (animInstance) {
						TempMontage = CombatComponent->GetMainWeapon()->GetActionMontages(EChartacterAction::EnterCombat)[0];
						manger->SetState(EChartacterState::GeneralActionState);
						manger->SetAction(EChartacterAction::EnterCombat);
						PlayAnimMontage(TempMontage, 1.0f, FName("Default"));
					

						
					}
				}
			
			else {
				if (animInstance) {
					TempMontage = CombatComponent->GetMainWeapon()->GetActionMontages(EChartacterAction::EnterCombat)[0];
					manger->SetState(EChartacterState::GeneralActionState);
					manger->SetAction(EChartacterAction::ExitCombat);
					PlayAnimMontage(TempMontage, 1.0f, FName("Default"));


				}
			}
		}
		}
	
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
		EDrawDebugTrace::ForDuration,
		OutHit,
		true
	);
	if (hit) {
		AActor* HitRes = OutHit.GetActor();
		
		if (IInteractive_CI* interacted = Cast<IInteractive_CI>(HitRes)) {
		  interacted->Intracts(this);
		}
		
	}
}

void ASoulsLikeCharacter::LightAttack()
{
	bHeavyAttack = false;
	if (manger->GetCurrentState()==EChartacterState::Attacking) {

		CombatComponent->SetAttackSaved(true);

	}
	else {
		ContinueAttack();
		if (CanPreFormAttack()) {
			AttackEvent();
			
			
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

	TArray<TEnumAsByte< EChartacterState>> temp = { EChartacterState::Attacking, EChartacterState::Dead, EChartacterState::Disabled, EChartacterState::Dodging, EChartacterState::GeneralActionState };
	if (!manger->IsCurrentStateEqualToAny(temp)) {
		if (StatsComponent->GetCurrentStateValue(Estat::Stamina)>10)
			return true;
	}
	return false;

}

bool ASoulsLikeCharacter::CanPreformDodge()
{
	TArray<TEnumAsByte< EChartacterState>> temp = {  EChartacterState::Dead, EChartacterState::Disabled, EChartacterState::Dodging, EChartacterState::GeneralActionState };
	if (!manger->IsCurrentStateEqualToAny(temp)) {
		if (!GetCharacterMovement()->IsFalling()) {
			if (StatsComponent->GetCurrentStateValue(Estat::Stamina) > 10) {
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
					StatsComponent->ModifyCurrentStatValue(Estat::Stamina,Value, true);
					
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
					UE_LOG(LogTemp, Warning, TEXT("hi there %d"), Value);
					StatsComponent->ModifyCurrentStatValue(Estat::Stamina, Value, true);
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
					UE_LOG(LogTemp, Warning, TEXT("asdasdasd %d"), size);
					manger->SetState(state);
					manger->SetAction(Action);
					int32 Value = CombatComponent->GetMainWeapon()->GetStatCostForAction();
					StatsComponent->ModifyCurrentStatValue(Estat::Stamina,Value, true);
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
	EnableRagdoll();
	ApplyHitReactionPhysicsVelocity(2000);
	if (IsValid(CombatComponent->GetMainWeapon())) {
		CombatComponent->GetMainWeapon()->SimulateWeaponPhysics();
		
	}
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASoulsLikeCharacter::PerformDeathAfterDelay, 4.0f, false);
		
	
}

void ASoulsLikeCharacter::PerformDeathAfterDelay()
{
	if (IsValid(CombatComponent->GetMainWeapon())) {
		 CombatComponent->GetMainWeapon()->Destroy();
	}
	GetMesh()->DestroyComponent();

		


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
	
	
		
	

	StatsComponent->TakeDamage(DamageAmount);

	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
		hitParticals,
		GetActorLocation()
		);
	if (CanRecieveHitreaction()) {
		PlayAnimMontage(HitReaction, 1.0f, FName("Default"));
		manger->SetState(EChartacterState::Disabled);
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

void ASoulsLikeCharacter::OnStateBegin(EChartacterState CharacterState)
{
	if (CharacterState == EChartacterState::Dead) {
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
	
		StatsComponent->ModifyCurrentStatValue(Estat::Stamina, -2, true);
		if (StatsComponent->GetCurrentStateValue(Estat::Stamina) <= 10) {
			
			StopSprint();
		}
	


}

void ASoulsLikeCharacter::DisableSprint()
{
}










void ASoulsLikeCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	StatsComponent->InitialState();
	if (MainWidget) {
		
		MainWidget->AddToPlayerScreen();
	}
	float num = StatsComponent->GetCurrentStateValue(Estat::Health);
	UE_LOG(LogTemp, Warning, TEXT("stamina %f"), num);
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
