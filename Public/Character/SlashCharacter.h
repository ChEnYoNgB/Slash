// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"BaseCharacter.h"
#include"CharacterTypes.h"
#include "Engine/EngineTypes.h"
#include "Interfaces/PickupInterface.h"
#include "SlashCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AItem;
class UAnimMontage;
class AWeapon;
class USlashOverlay;
class ASoul;
class UKnapsackComponent;
class ASlashHUD;

UCLASS()
class SLASH_API ASlashCharacter : public ABaseCharacter, public IPickupInterface
{
	GENERATED_BODY()
public:
	ASlashCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump()override;
	virtual void GetHit_Implementation(const FVector& ImpatPoint, AActor* Hitter)override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;
	virtual void SetOverlappingItem(AItem* Item)override;
	virtual void AddSouls(ASoul* Soul) override;


	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE EDeadPose GetDeadPose() const { return DeadPose; }
	FORCEINLINE EActionState GetActionState()const { return ActionState; }
	FORCEINLINE UKnapsackComponent* GetKnapsackComponent()const { return KnapsackComponent; }

private:

	FTimerHandle AttackTimer;
	int32 AttackIndex = 0;
	int32 AttackSectionMax = 0;
	ECharacterState CharacterState;

	UPROPERTY(VisibleAnywhere)
		ASlashHUD* SlashHUD;
	UPROPERTY(VisibleAnywhere)
		AItem* NewItem = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EDeadPose DeadPose;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EActionState ActionState;
	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* ViewCamera;
	UPROPERTY(EditDefaultsOnly, Category = Montage)
		UAnimMontage* EquipMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montage)
		UAnimMontage* DodgeMontage;
	UPROPERTY()
		USlashOverlay* SlashOverlay;
	UPROPERTY(VisibleInstanceOnly, Category = "Overlap")
		AItem* OverlappingItem;


	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void Dodge();
	void EKeyPressed();
	void Text();
	void ResetAttack();
	void OpenKnapsack();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Knapsack")
		UKnapsackComponent* KnapsackComponent;

	virtual void BeginPlay() override;
	virtual void Die()override;
	virtual void Attack()override;
	virtual void PlayAttackMontage()override;
	virtual void PlayDeadMontage()override;
	virtual bool CanAttack()override;
	virtual void AttackEnd()override;
	void PlayEquipMontage(FName SectionName);
	void PlayDodgeMontage();


	UFUNCTION(BlueprintCallable)
		void DodgeEnd();
	UFUNCTION(BlueprintCallable)
		void Disarm();
	UFUNCTION(BlueprintCallable)
		void Arm();
	UFUNCTION(BlueprintCallable)
		void FinishEquipping();
	UFUNCTION(BlueprintCallable)
		void BeginLand();
	UFUNCTION(BlueprintCallable)
		void EndLand();
	UFUNCTION(BlueprintCallable)
		void HitReactionEnd();

};
