// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SLASH_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		float Health;
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		float MaxHealth;
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		float Stamina;
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		float MaxStamina;
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		float DodgeCost = 14.f;
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		int32 Gold;
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		int32 Soul;
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		float StaminaRegenRate = 8.f;
public:
	void ReceiveDamage(float Damage);
	void UseStamina(float Cost);
	float GetPercent();
	float GetStaminaPercent();
	void RegentStamina(float DeltaTime);
	bool IsAlive();
	void AddGold(int32 NumberOfGold);
	void AddSoul(int32 NumberOfSoul);
	FORCEINLINE int32 GetGold()const { return Gold; }
	FORCEINLINE int32 GetSoul()const { return Soul; }
	FORCEINLINE float GetDodgeCost()const { return DodgeCost; }
	FORCEINLINE float GetStamina()const { return Stamina; }
};
