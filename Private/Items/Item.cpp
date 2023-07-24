

#include "Items/Item.h"
#include"Components/SphereComponent.h"
#include "Interfaces/PickupInterface.h"
#include"NiagaraComponent.h"
#include"NiagaraFunctionLibrary.h"
#include"Kismet/GameplayStatics.h"

AItem::AItem()
{

	PrimaryActorTick.bCanEverTick = true;

	Amplitudef = 0.25f;
	TimeConstant = 5.f;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = ItemMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

	ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ItemEffect"));
	ItemEffect->SetupAttachment(GetRootComponent());
}


void AItem::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
}


void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunningTime += DeltaTime;
	if (ItemState == EItemState::EIS_Hovering)
	{
		float DeltaZ = Amplitudef * FMath::Sin(RunningTime * TimeConstant);
		AddActorWorldOffset(FVector(0.f, 0.f, DeltaZ));
	}

}
void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*
	const FString OtherActorName = OtherActor->GetName();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Red, OtherActorName);
	}*/

	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		PickupInterface->SetOverlappingItem(this);
	}
}
void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const FString OtherActorName = OtherActor->GetName();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Blue, OtherActorName);
	}

	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		PickupInterface->SetOverlappingItem(nullptr);
	}
}
void AItem::SpawnPickUpEffect()
{
	if (PickUpEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			PickUpEffect,
			GetActorLocation()
		);
	}
}
void AItem::SpawnPickUpSound()
{
	if (PickUpSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickUpSound, GetActorLocation());
	}
}