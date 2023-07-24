// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"Character/CharacterTypes.h"
#include"Character/BaseCharacter.h"
#include "Engine/EngineTypes.h"
#include "Enemy.generated.h"


class UHealthBarComponent;
class AAIController;
class UPawnSensingComponent;
class ASoul;

UCLASS()
class SLASH_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()
public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;
	virtual void GetHit_Implementation(const FVector& ImpatPoint, AActor* Hitter)override;

protected:
	virtual void BeginPlay() override;
	virtual void Die()override;
	virtual void Attack()override;
	virtual void AttackEnd()override;
	virtual void PlayAttackMontage()override;
	virtual void PlayDeadMontage()override;



	UFUNCTION()
		void PawnSeen(APawn* SeenPawn);
	UPROPERTY(BlueprintReadOnly, Category = "State")
		EEnemyState EnemyState;
	UPROPERTY(BlueprintReadOnly, Category = "State")
		EEnemyAction EnemyAction = EEnemyAction::EEA_Patroling;
private:
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void PatrolTimerFinished();
	void MoveToTarget(AActor* Target);
	bool InTargetRange(AActor* Target, double Radius);
	AActor* ChoosePatrolTarget();

	UPROPERTY(EditAnyWhere, Category = "Combat")
		float AttackMin = 0.25f;
	UPROPERTY(EditAnyWhere, Category = "Combat")
		float AttackMax = 0.5f;
	UPROPERTY(EditAnyWhere, Category = "Combat")
		TSubclassOf<ASoul> SoulClass;
	UPROPERTY(VisibleAnywhere, Category = "HealthBar")
		UHealthBarComponent* HealthBarWidget;
	UPROPERTY(EditAnyWhere, Category = "Weapon")
		TSubclassOf<AWeapon> EnemyWeapon;



	FTimerHandle PatrolTimer;
	FTimerHandle AttackTimer;


	UPROPERTY()
		AAIController* EnemyController;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
		float TargetRadius = 500.f;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
		float PatrolRadius = 200.f;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
		float AttackRadius = 150.f;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		AActor* PatrolTarget;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		TArray<AActor*> PatrolTargets;
	UPROPERTY(VisibleAnywhere, Category = "AI Navigation")
		UPawnSensingComponent* PawnSensing;



};
