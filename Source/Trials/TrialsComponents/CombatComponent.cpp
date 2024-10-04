

#include "CombatComponent.h"
#include "Trials/Weapon/Weapon.h"
#include "Trials/TrialsCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"



UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;


	BaseWalkSpeed = 600.f;
	BlockWalkSpeed = 300.f;
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent, EquippedWeapon);
	DOREPLIFETIME(UCombatComponent, bBlocking);
	DOREPLIFETIME(UCombatComponent, SheathedWeapon);
}


void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if (Character == nullptr || WeaponToEquip == nullptr) return;
	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	if (!SheathedWeapon)
	{
		if (EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Melee)
		{
			AttachWeapon(EquippedWeapon, FName("RightHandSocket"));
		}
		else if (EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Magic)
		{
			AttachWeapon(EquippedWeapon, FName("MagicSocket"));
		}
	}
	EquippedWeapon->SetOwner(Character);
	EquippedWeapon->ShowPickupWidget(false);
	EquippedWeapon->GetAreaSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->bUseControllerRotationYaw = true;

}

void UCombatComponent::AttachWeapon(AWeapon* WeaponToAttach, FName SocketName)
{
	const USkeletalMeshSocket* AttachSocket = Character->GetMesh()->GetSocketByName(SocketName);
	if (AttachSocket && WeaponToAttach)
	{
		if (SheathedWeapon)
		{
			SheathedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		}
		AttachSocket->AttachActor(WeaponToAttach, Character->GetMesh());
	}
}

void UCombatComponent::DetachWeapon(AWeapon* WeaponToAttach, FName SocketName)
{
	const USkeletalMeshSocket* AttachSocket = Character->GetMesh()->GetSocketByName(SocketName);
	if (AttachSocket && WeaponToAttach)
	{
		if (EquippedWeapon)
		{
			WeaponToAttach->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		}
		AttachSocket->AttachActor(WeaponToAttach, Character->GetMesh());
	}
}

void UCombatComponent::ToggleSheathState(AWeapon* WeaponToSheath)
{
	if (Character == nullptr || WeaponToSheath == nullptr) return;
	if (EquippedWeapon)
	{
		SheathedWeapon = WeaponToSheath;
		if (SheathedWeapon && SheathedWeapon->GetWeaponState() == EWeaponState::EWS_Equipped)
		{
			SheathedWeapon->SetWeaponState(EWeaponState::EWS_Unequipped);
			EquippedWeapon = nullptr;
			Character->GetCharacterMovement()->bOrientRotationToMovement = true;
			Character->bUseControllerRotationYaw = false;
		}
	}
	else if (!EquippedWeapon && SheathedWeapon)
	{
		EquipWeapon(SheathedWeapon);
	}
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	}
}

void UCombatComponent::SetBlocking(bool bIsBlocking)
{
	bBlocking = bIsBlocking;
	ServerSetBlocking(bIsBlocking);
	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = bBlocking ? BlockWalkSpeed : BaseWalkSpeed;
	}
}

void UCombatComponent::OnRep_EquippedWeapon()
{
	if (EquippedWeapon && Character)
	{
		Character->GetCharacterMovement()->bOrientRotationToMovement = false;
		Character->bUseControllerRotationYaw = true;
	}
	else if (EquippedWeapon == nullptr && Character)
	{
		Character->GetCharacterMovement()->bOrientRotationToMovement = true;
		Character->bUseControllerRotationYaw = false;
	}
}

void UCombatComponent::ServerSetBlocking_Implementation(bool bIsBlocking)
{
	bBlocking = bIsBlocking;
	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = bBlocking ? BlockWalkSpeed : BaseWalkSpeed;
	}
}


void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

