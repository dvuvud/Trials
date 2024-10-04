#pragma once

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_Melee UMETA(DisplayName = "MeleeWeapon"),
	EWT_Magic UMETA(DisplayName = "MagicWeapon")
};
