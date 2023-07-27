// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SlashCharacter.h"
#include"GameFramework/SpringArmComponent.h"
#include"GameFramework/CharacterMovementComponent.h"
#include"Components/BoxComponent.h"
#include"Components/SkeletalMeshComponent.h"
#include "Components/AttributeComponent.h"
#include "Components/KnapsackComponent.h"
#include"Camera/CameraComponent.h"
#include"Animation/AnimMontage.h"
#include "Items/Weapons/Weapon.h"
#include "Items/Item.h"
#include "Items/Soul.h"
#include "TimerManager.h"
#include "HUD/SlashHUD.h"
#include "HUD/SlashOverlay.h"
#include "HUD/KnapsackWidget.h"
#include "Interfaces/PickupItemInterface.h"
#include "UObject/UObjectGlobals.h"


ASlashCharacter::ASlashCharacter()
{

	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);

	KnapsackComponent = CreateDefaultSubobject<UKnapsackComponent>(TEXT("Knapsack"));

}
void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("ASlashCharacter"));
	AttackSectionMax = AttackMontageSections.Num();
	Tags.Add(FName("EngageableTarget"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_Unoccupied;
	if (KnapsackComponent)
	{
		KnapsackComponent->InitKnapsack();
	}
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		SlashHUD = Cast<ASlashHUD>(PlayerController->GetHUD());
		if (SlashHUD)
		{
			SlashOverlay = SlashHUD->GetSlashOverlay();
			SlashOverlay->SetHealthPercent(Attribute->GetPercent());
			SlashHUD->CreateKnapsackWidget();
		}
	}

}
void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Attribute && SlashOverlay)
	{
		Attribute->RegentStamina(DeltaTime);
		SlashOverlay->SetStaminaPercent(Attribute->GetStaminaPercent());
	}
}
void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ASlashCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Turn", this, &ASlashCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ASlashCharacter::LookUp);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASlashCharacter::MoveRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASlashCharacter::Jump);
	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ASlashCharacter::EKeyPressed);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ASlashCharacter::Attack);
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &ASlashCharacter::Dodge);
	PlayerInputComponent->BindAction("OpenKnapsack", IE_Pressed, this, &ASlashCharacter::OpenKnapsack);
	PlayerInputComponent->BindAction("Throw", IE_Pressed, this, &ASlashCharacter::ThrowAll);
	PlayerInputComponent->BindAction("Throw", IE_Released, this, &ASlashCharacter::ThrowOne);
}
void ASlashCharacter::Jump()
{
	if (ActionState == EActionState::EAS_Unoccupied)
	{
		Super::Jump();
	}
}
void ASlashCharacter::Dodge()
{
	if (ActionState != EActionState::EAS_Unoccupied)return;
	if (Attribute && Attribute->GetStamina() > Attribute->GetDodgeCost())
	{
		PlayDodgeMontage();
		ActionState = EActionState::EAS_Dodge;
		Attribute->UseStamina(Attribute->GetDodgeCost());
		SlashOverlay->SetStaminaPercent(Attribute->GetStaminaPercent());
	}
}
void ASlashCharacter::DodgeEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}
void ASlashCharacter::Text()
{
	UE_LOG(LogTemp, Warning, TEXT("1"));
}
void ASlashCharacter::MoveForward(float Value)
{
	if (ActionState != EActionState::EAS_Unoccupied)return;
	if (Controller && Value != 0)
	{
		const FRotator ControllerRotation = GetControlRotation();
		const FRotator Rotation(0.f, ControllerRotation.Yaw, 0.f);

		const FVector Forward = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Forward, Value);
	}
}
void ASlashCharacter::MoveRight(float Value)
{
	if (ActionState != EActionState::EAS_Unoccupied)return;
	if (Controller && Value != 0)
	{
		const FRotator ControllerRotation = GetControlRotation();
		const FRotator Rotation(0.f, ControllerRotation.Yaw, 0.f);

		const FVector Right = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Right, Value);
	}
}
void ASlashCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}
void ASlashCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}
void ASlashCharacter::EKeyPressed()
{

	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		CharacterState = ECharacterState::ECS_EquippedOneHandWeapon;
		EquippedWeapon = OverlappingWeapon;
		if (KnapsackComponent && SlashHUD && SlashHUD->GetSlashKnapsack())
		{
			KnapsackComponent->AddItem(OverlappingWeapon);
			SlashHUD->GetSlashKnapsack()->UpdateList();
		}

	}
	else
	{

		if (CharacterState != ECharacterState::ECS_Unequipped && ActionState == EActionState::EAS_Unoccupied && EquipMontage && EquippedWeapon)
		{
			PlayEquipMontage(FName("Unequip"));
			CharacterState = ECharacterState::ECS_Unequipped;
			ActionState = EActionState::EAS_EquippingWeapon;
			EquippedWeapon->SetItemInpackage();
			CombatTarget = nullptr;
		}
		else if (CharacterState == ECharacterState::ECS_Unequipped && ActionState == EActionState::EAS_Unoccupied && EquipMontage && EquippedWeapon)
		{
			PlayEquipMontage(FName("Equip"));
			CharacterState = ECharacterState::ECS_EquippedOneHandWeapon;
			ActionState = EActionState::EAS_EquippingWeapon;
			EquippedWeapon->SetItemEquip();
		}
	}
	if (OverlappingItem)
	{
		IPickupItemInterface* PickupItem = Cast<IPickupItemInterface>(OverlappingItem);
		if (PickupItem && KnapsackComponent && SlashHUD && SlashHUD->GetSlashKnapsack())
		{
			NewItem = DuplicateObject<AItem>(OverlappingItem, NewItem);
			KnapsackComponent->AddItem(NewItem);
			SlashHUD->GetSlashKnapsack()->UpdateList();
			PickupItem->PickUp();
			NewItem = nullptr;
		}
	}
}
void ASlashCharacter::Attack()
{

	if (CanAttack())
	{
		if (AttackTimer.IsValid())
		{
			GetWorldTimerManager().ClearTimer(AttackTimer);
			GetWorldTimerManager().SetTimer(AttackTimer, this, &ASlashCharacter::ResetAttack, 2.0f, false);
			AttackIndex = (AttackIndex + 1) % AttackSectionMax;
		}
		else
		{
			GetWorldTimerManager().SetTimer(AttackTimer, this, &ASlashCharacter::ResetAttack, 2.0f, false);
			AttackIndex = 0;
		}

		if (EquippedWeapon && EquippedWeapon->GetHitCharacter())
		{
			CombatTarget = EquippedWeapon->GetHitCharacter();
			if (CombatTarget->ActorHasTag(TEXT("Dead")))
			{
				CombatTarget = nullptr;
			}
		}
		PlayAttackMontage();

		ActionState = EActionState::EAS_Attacking;
	}
}
bool ASlashCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}
void ASlashCharacter::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}
void ASlashCharacter::PlayAttackMontage()
{
	PlayMontageSection(AttackMontage, AttackMontageSections[AttackIndex]);
}
void ASlashCharacter::PlayDodgeMontage()
{
	if (DodgeMontage)
	{
		PlayMontageSection(DodgeMontage, FName("Default"));
	}
}
void ASlashCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;

}
void ASlashCharacter::Disarm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachToSocket(GetMesh(), FName("SpineSocket"));
	}
}
void ASlashCharacter::Arm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachToSocket(GetMesh(), FName("RightHandSocket"));
	}
}
void ASlashCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}
void ASlashCharacter::ResetAttack()
{
	AttackIndex = -1;
}
void ASlashCharacter::BeginLand()
{
	ActionState = EActionState::EAS_Landing;
}
void ASlashCharacter::EndLand()
{
	ActionState = EActionState::EAS_Unoccupied;
}
void ASlashCharacter::GetHit_Implementation(const FVector& ImpatPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpatPoint, Hitter);
	CombatTarget = Hitter;
	SetWeaponCollisionEnable(ECollisionEnabled::NoCollision);
	if (Attribute && Attribute->IsAlive())
	{
		ActionState = EActionState::EAS_HitReaction;
	}

}
void ASlashCharacter::HitReactionEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}
float ASlashCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	ReceiveDamage(DamageAmount);
	if (SlashOverlay && Attribute)
	{
		SlashOverlay->SetHealthPercent(Attribute->GetPercent());
	}
	return DamageAmount;
}
void ASlashCharacter::Die()
{
	Super::Die();
	ActionState = EActionState::EAS_Dead;
	PlayDeadMontage();
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetWeaponCollisionEnable(ECollisionEnabled::NoCollision);
	CombatTarget = nullptr;
}
void ASlashCharacter::PlayDeadMontage()
{
	if (DeadMontageSections.Num() <= 0)return;
	const int32 MaxDeadSection = DeadMontageSections.Num() - 1;
	const int32 Section = FMath::RandRange(0, MaxDeadSection);
	TEnumAsByte<EDeadPose> Pose(Section);
	if (Pose < EDeadPose::EDP_Max)
	{
		DeadPose = Pose;
		PlayMontageSection(DeadMontage, DeadMontageSections[Section]);
	}
}
void ASlashCharacter::AddSouls(ASoul* Soul)
{
	UE_LOG(LogTemp, Warning, TEXT("ASlashCharacter::AddSouls"));
	if (Attribute)
	{
		Attribute->AddSoul(Soul->GetSoul());
	}
}
void ASlashCharacter::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}
void ASlashCharacter::OpenKnapsack()
{
	if (SlashHUD)
	{
		bool Select = SlashHUD->GetKnapsackVisibility();
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		UWorld* World = GetWorld();
		SlashHUD->SetKnapsackVisibility(!Select);
		if (!Select)
		{
			if (PlayerController && World)
			{
				World->GetWorldSettings()->SetTimeDilation(0.f);
				PlayerController->bShowMouseCursor = true;
				PlayerController->bEnableMouseOverEvents = true;
				PlayerController->SetInputMode(FInputModeGameAndUI());
			}
		}
		else
		{
			if (PlayerController && World)
			{
				World->GetWorldSettings()->SetTimeDilation(1.f);
				PlayerController->bShowMouseCursor = false;
				PlayerController->bEnableMouseOverEvents = false;
				PlayerController->SetInputMode(FInputModeGameOnly());
			}
		}
	}
}
void ASlashCharacter::ThrowAll()
{
	if (KnapsackComponent)
	{
		KnapsackComponent->SetIsThrowAll(true);
	}
}
void ASlashCharacter::ThrowOne()
{
	if (KnapsackComponent)
	{
		KnapsackComponent->SetIsThrowAll(false);
	}
}