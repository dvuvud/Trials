

#include "MeleeWeapon.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Trials/Interfaces/GetHitInterface.h"
#include "Components/BoxComponent.h"
#include "Trials/TrialsTypes/WeaponType.h"




AMeleeWeapon::AMeleeWeapon()
{
	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceStart"));
	BoxTraceStart->SetupAttachment(RootComponent);
	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceEnd"));
	BoxTraceEnd->SetupAttachment(RootComponent);

	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
	WeaponBox->SetupAttachment(RootComponent);
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnBoxOverlap);
}

void AMeleeWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(this->GetOwner());

	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	FHitResult BoxHit;
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		FVector(5.f, 5.f, 5.f),
		BoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		BoxHit,
		true
	);
	if (BoxHit.GetActor())
	{
		IGetHitInterface* HitInterface = Cast<IGetHitInterface>(BoxHit.GetActor());
		if (HitInterface)
		{
			HitInterface->GetHit(BoxHit.ImpactPoint);
		}
		IgnoreActors.AddUnique(BoxHit.GetActor());
	}
}

void AMeleeWeapon::OnRep_WeaponState()
{
	Super::OnRep_WeaponState();

	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		WeaponType = EWeaponType::EWT_Melee;
		break;
	case EWeaponState::EWS_Unequipped:

		break;
	}
}

void AMeleeWeapon::SetWeaponState(EWeaponState State)
{
	Super::SetWeaponState(State);
	WeaponState = State;
	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		WeaponType = EWeaponType::EWT_Melee;
		break;
	case EWeaponState::EWS_Unequipped:
		break;
	}
}
