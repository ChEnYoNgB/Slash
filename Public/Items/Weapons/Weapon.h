// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class UBoxComponent;
/**
 *
 */
UCLASS()
class SLASH_API AWeapon : public AItem
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;
	UFUNCTION()
		void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintImplementableEvent)
		void CreateFields(const FVector& FieldLocation);
public:
	AWeapon();
	void Equip(USceneComponent* InParent, FName SocketName, AActor* NewOwner, APawn* NewInstigator);
	void AttachToSocket(USceneComponent* InParent, const FName& SocketName);
	FORCEINLINE UBoxComponent* GetWeaponBox()const { return WeaponBox; }
	FORCEINLINE AActor* GetHitCharacter()const { return HitCharacter; }
	TArray<AActor*> IgnoreActors;
private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
		UBoxComponent* WeaponBox;
	UPROPERTY(VisibleAnywhere)
		USceneComponent* BoxTraceStart;
	UPROPERTY(VisibleAnywhere)
		USceneComponent* BoxTraceEnd;
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
		float Damage = 20.f;
	UPROPERTY()
		AActor* HitCharacter = nullptr;

};
