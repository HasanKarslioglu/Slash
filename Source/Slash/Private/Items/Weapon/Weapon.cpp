// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapon/Weapon.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Interfaces/HitInterface.h"

AWeapon::AWeapon()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	WeaponBox->SetupAttachment(GetRootComponent());
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());
	
	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName&  InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget,false);
	ItemMesh->AttachToComponent(InParent, TransformRules,InSocketName);
}

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName)
{
	AttachMeshToSocket(InParent, InSocketName);		
	ItemState = EItemState::ETS_Equipped;

	if(EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			EquipSound,
			GetActorLocation()
			);
	}

	if(Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);		
	}
	
}


void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FHitResult BoxHitResult;

	for (AActor* Actor: IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}
	
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		 FVector(5.f, 5.f, 5.f),
		 BoxTraceStart->GetComponentRotation(),
		 ETraceTypeQuery::TraceTypeQuery1,
		 false,
		 ActorsToIgnore,
		 EDrawDebugTrace::ForDuration,
		 BoxHitResult,
		 true
	);

	if (BoxHitResult.GetActor())
	{
		IHitInterface* HitInterface = Cast<IHitInterface>(BoxHitResult.GetActor());
		if (HitInterface)
		{
			HitInterface->GetHit(BoxHitResult.ImpactPoint);
		}
		IgnoreActors.AddUnique(BoxHitResult.GetActor());
	}
		 
}

