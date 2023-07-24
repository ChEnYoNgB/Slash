// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "AIController.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Items/Weapons/Weapon.h"
#include "Items/Soul.h"
#include "Character/CharacterTypes.h"
#include "Animation/AnimMontage.h"
#include "HUD/HealthBarComponent.h"

AEnemy::AEnemy()
{

	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GetMesh()->SetGenerateOverlapEvents(true);

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 600.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);
}
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	HealthBarWidget->SetVisibility(false);

	EnemyController = Cast<AAIController>(GetController());
	MoveToTarget(PatrolTarget);

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	}

	UWorld* World = GetWorld();
	if (World && EnemyWeapon)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(EnemyWeapon);
		DefaultWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		EquippedWeapon = DefaultWeapon;
	}
	Tags.Add(FName("Enemy"));
}
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyAction == EEnemyAction::EEA_Dead)return;
	if (EnemyAction > EEnemyAction::EEA_Patroling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
}
void AEnemy::Attack()
{
	Super::Attack();
	if (CombatTarget == nullptr) return;
	EnemyAction = EEnemyAction::EEA_Engaged;
	PlayAttackMontage();
}
void AEnemy::AttackEnd()
{
	EnemyAction = EEnemyAction::EEA_NoState;
	CheckCombatTarget();
}
void AEnemy::PlayAttackMontage()
{
	if (AttackMontageSections.Num() <= 0)return;
	const int32 MaxSectionNumber = AttackMontageSections.Num() - 1;
	const int32 Section = FMath::RandRange(0, MaxSectionNumber);
	PlayMontageSection(AttackMontage, AttackMontageSections[Section]);
}
void AEnemy::PlayDeadMontage()
{
	if (DeadMontageSections.Num() <= 0)return;
	const int32 MaxDeadSection = DeadMontageSections.Num() - 1;
	const int32 Section = FMath::RandRange(0, MaxDeadSection);
	TEnumAsByte<EEnemyState> Pose(Section);
	if (Pose < EEnemyState::EES_Max)
	{
		EnemyState = Pose;
		PlayMontageSection(DeadMontage, DeadMontageSections[Section]);
	}
}
void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, 3.f);
	}
}
void AEnemy::CheckCombatTarget()
{
	if (!InTargetRange(CombatTarget, TargetRadius) && HealthBarWidget)
	{
		GetWorldTimerManager().ClearTimer(AttackTimer);
		HealthBarWidget->SetVisibility(false);
		CombatTarget = nullptr;
		if (EnemyAction != EEnemyAction::EEA_Engaged)
		{
			EnemyAction = EEnemyAction::EEA_Patroling;
			GetCharacterMovement()->MaxWalkSpeed = 125.f;
			MoveToTarget(PatrolTarget);
		}

	}
	else if (!InTargetRange(CombatTarget, AttackRadius) && EnemyAction != EEnemyAction::EEA_Chasing)
	{
		GetWorldTimerManager().ClearTimer(AttackTimer);
		if (EnemyAction != EEnemyAction::EEA_Engaged)
		{
			EnemyAction = EEnemyAction::EEA_Chasing;
			GetCharacterMovement()->MaxWalkSpeed = 300.f;
			MoveToTarget(CombatTarget);
		}

	}
	else if (InTargetRange(CombatTarget, AttackRadius) && EnemyAction != EEnemyAction::EEA_Attacking && EnemyAction != EEnemyAction::EEA_Engaged && EnemyAction != EEnemyAction::EEA_Dead)
	{
		EnemyAction = EEnemyAction::EEA_Attacking;
		const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
	}
}
void AEnemy::GetHit_Implementation(const FVector& ImpatPoint, AActor* Hitter)
{
	if (EnemyAction == EEnemyAction::EEA_Dead)return;
	if (GetWorld())
	{
		DrawDebugSphere(GetWorld(), ImpatPoint, 8.f, 12, FColor::Red, false, 5.f);
	}
	Super::GetHit_Implementation(ImpatPoint, Hitter);
	GetWorldTimerManager().ClearTimer(PatrolTimer);
	GetWorldTimerManager().ClearTimer(AttackTimer);
	StopAttackMontage();
	SetWeaponCollisionEnable(ECollisionEnabled::NoCollision);
}
float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	CombatTarget = EventInstigator->GetPawn();
	if (CombatTarget && HealthBarWidget && Attribute)
	{
		HealthBarWidget->SetVisibility(true);
		ReceiveDamage(DamageAmount);
		HealthBarWidget->SetPercent(Attribute->GetPercent());
	}
	if (InTargetRange(CombatTarget, AttackRadius))
	{
		EnemyAction = EEnemyAction::EEA_Attacking;
	}
	else if (!InTargetRange(CombatTarget, AttackRadius))
	{
		EnemyAction = EEnemyAction::EEA_Chasing;
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		MoveToTarget(CombatTarget);
	}

	return DamageAmount;
}
void AEnemy::Die()
{
	Super::Die();
	EnemyAction = EEnemyAction::EEA_Dead;
	PlayDeadMontage();
	GetWorldTimerManager().ClearTimer(AttackTimer);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetWeaponCollisionEnable(ECollisionEnabled::NoCollision);
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
	UWorld* World = GetWorld();
	if (World && SoulClass)
	{

		ASoul* SpawnSoul = World->SpawnActor<ASoul>(SoulClass, GetActorLocation(), GetActorRotation());
		if (SpawnSoul)
		{
			SpawnSoul->SetSoul(Attribute->GetSoul());
		}
	}
}
bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr)return false;
	const double Distance = (Target->GetActorLocation() - GetActorLocation()).Size();
	return Distance <= Radius;

}
void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr)return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(30.f);
	EnemyController->MoveTo(MoveRequest);
}
AActor* AEnemy::ChoosePatrolTarget()
{
	TArray<AActor*> VaildTarget;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			VaildTarget.AddUnique(Target);
		}
	}
	if (VaildTarget.Num() > 0)
	{
		const int32 Select = FMath::RandRange(0, VaildTarget.Num() - 1);
		return VaildTarget[Select];
	}
	return nullptr;
}
void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}
void AEnemy::PawnSeen(APawn* SeenPawn)
{
	bool bShouldChaseTarget = EnemyAction != EEnemyAction::EEA_Dead &&
		EnemyAction != EEnemyAction::EEA_Chasing &&
		EnemyAction != EEnemyAction::EEA_Attacking &&
		EnemyAction != EEnemyAction::EEA_Engaged &&
		SeenPawn->ActorHasTag(FName("EngageableTarget"));
	if (bShouldChaseTarget)
	{
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		EnemyAction = EEnemyAction::EEA_Chasing;
		CombatTarget = SeenPawn;
		MoveToTarget(CombatTarget);
	}
}
