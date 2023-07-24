// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"


UAttributeComponent::UAttributeComponent()
{

	PrimaryComponentTick.bCanEverTick = false;


}
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();



}
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}
void UAttributeComponent::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}
void UAttributeComponent::UseStamina(float Cost)
{
	Stamina = FMath::Clamp(Stamina - Cost, 0.f, MaxStamina);
}
float UAttributeComponent::GetPercent()
{
	return Health / MaxHealth;
}
float UAttributeComponent::GetStaminaPercent()
{
	return Stamina / MaxStamina;
}
bool UAttributeComponent::IsAlive()
{
	return Health > 0;
}
void UAttributeComponent::AddGold(int32 NumberOfGold)
{
	Gold += NumberOfGold;
}
void UAttributeComponent::AddSoul(int32 NumberOfSoul)
{
	Soul += NumberOfSoul;
}
void UAttributeComponent::RegentStamina(float DeltaTime)
{
	Stamina = FMath::Clamp(Stamina + StaminaRegenRate * DeltaTime, 0.f, MaxStamina);
}