// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"
#include"Components/BoxComponent.h"
#include "Components/AttributeComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Items/Weapons/Weapon.h"
#include"Animation/AnimMontage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

ABaseCharacter::ABaseCharacter()
{

	PrimaryActorTick.bCanEverTick = true;

	Attribute = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
}
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

}
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ABaseCharacter::SetWeaponCollisionEnable(ECollisionEnabled::Type CollisionEnable)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnable);
		EquippedWeapon->IgnoreActors.Empty();
	}
}
void ABaseCharacter::Attack()
{
	if (CombatTarget && CombatTarget->ActorHasTag(TEXT("Dead")))
	{
		CombatTarget = nullptr;
	}
}
void ABaseCharacter::PlayAttackMontage()
{

}
void ABaseCharacter::PlayDeadMontage()
{

}
bool ABaseCharacter::CanAttack()
{
	return false;
}
void ABaseCharacter::AttackEnd()
{

}
void ABaseCharacter::Die()
{
	Tags.Add(TEXT("Dead"));
}
void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}
void ABaseCharacter::DirectionHitReact(const FVector& ImpatPoint)
{
	const FVector Forward = GetActorForwardVector();
	const FVector OnHitLower = FVector(ImpatPoint.X, ImpatPoint.Y, GetActorLocation().Z);
	const FVector OnHit = (OnHitLower - GetActorLocation()).GetSafeNormal();
	const double CosTheta = FVector::DotProduct(Forward, OnHit);

	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	const FVector CrossPuoduct = FVector::CrossProduct(Forward, OnHit);
	if (CrossPuoduct.Z < 0)
	{
		Theta *= -1.f;
	}
	FName Section("FromBack");
	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = "FromFront";
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = "FromRight";
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = "FromLeft";
	}
	PlayHitReactMontage(Section);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));
	}
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossPuoduct * 60.f, 5.f, FColor::Green, 5.f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + OnHit * 60.f, 5.f, FColor::Blue, 5.f);
}
void ABaseCharacter::PlayHitSound(const FVector& ImpatPoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpatPoint);
	}
}
void ABaseCharacter::SpawnHitParticles(const FVector& ImpatPoint)
{
	if (HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, ImpatPoint);
	}
}
void ABaseCharacter::ReceiveDamage(const float& DamageAmount)
{
	if (Attribute)
	{
		Attribute->ReceiveDamage(DamageAmount);
	}
}
void ABaseCharacter::PlayMontageSection(UAnimMontage* AnimMontage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AnimMontage)
	{
		AnimInstance->Montage_Play(AnimMontage);
		AnimInstance->Montage_JumpToSection(SectionName, AnimMontage);
	}
}
void ABaseCharacter::GetHit_Implementation(const FVector& ImpatPoint, AActor* Hitter)
{
	if (Attribute && Attribute->IsAlive() && Hitter)
	{
		DirectionHitReact(Hitter->GetActorLocation());
	}
	else if (Attribute && !Attribute->IsAlive())
	{
		Die();
	}
	PlayHitSound(ImpatPoint);
	SpawnHitParticles(ImpatPoint);
}
void ABaseCharacter::StopAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Stop(1.f, AttackMontage);
	}
}
FVector ABaseCharacter::GetTranslationWarpTarget()
{
	if (CombatTarget)
	{
		const FVector TargetLocation = CombatTarget->GetActorLocation();
		const FVector Location = GetActorLocation();

		const FVector TargetToThis = (Location - TargetLocation).GetSafeNormal();
		return TargetToThis * Length + TargetLocation;
	}
	return FVector();
}
FVector ABaseCharacter::GetRotationWarpTarget()
{
	if (CombatTarget)
	{
		return CombatTarget->GetActorLocation();
	}
	return FVector();
}
