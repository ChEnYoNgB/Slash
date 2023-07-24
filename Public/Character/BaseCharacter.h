// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAnimMontage;
class UAttributeComponent;
class AItem;

UCLASS()
class SLASH_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:

	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void SetWeaponCollisionEnable(ECollisionEnabled::Type CollisionEnable);


protected:

	virtual void BeginPlay() override;
	virtual void Die();
	virtual void Attack();
	virtual bool CanAttack();
	virtual void PlayAttackMontage();
	virtual void PlayDeadMontage();
	virtual void GetHit_Implementation(const FVector& ImpatPoint, AActor* Hitter)override;
	void PlayHitReactMontage(const FName& SectionName);
	void DirectionHitReact(const FVector& ImpatPoint);
	void PlayHitSound(const FVector& ImpatPoint);
	void SpawnHitParticles(const FVector& ImpatPoint);
	void ReceiveDamage(const float& DamageAmount);
	void PlayMontageSection(UAnimMontage* AnimMontage, const FName& SectionName);
	void StopAttackMontage();


	UFUNCTION(BlueprintCallable)
		virtual void AttackEnd();
	UFUNCTION(BlueprintCallable)
		FVector GetTranslationWarpTarget();
	UFUNCTION(BlueprintCallable)
		FVector GetRotationWarpTarget();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
		AActor* CombatTarget;
	UPROPERTY(EditAnywhere, Category = "Combat")
		double Length = 75.f;
	UPROPERTY(EditAnywhere, Category = "Combat")
		TArray<FName> AttackMontageSections;
	UPROPERTY(EditAnywhere, Category = "Combat")
		TArray<FName> DeadMontageSections;
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		AWeapon* EquippedWeapon;
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
		UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
		UAnimMontage* HitReactMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
		UAnimMontage* DeadMontage;
	UPROPERTY(VisibleAnywhere, Category = "Attribute")
		UAttributeComponent* Attribute;
	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* HitSound;
	UPROPERTY(EditAnywhere, Category = "VisualEffects")
		UParticleSystem* HitParticles;

};
