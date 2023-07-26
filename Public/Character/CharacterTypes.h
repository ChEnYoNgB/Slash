#pragma once
UENUM(BlueprintType)
enum class ECharacterState :uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHandWeapon UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EquippedTwoHandWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon")
};
UENUM(BlueprintType)
enum class EActionState :uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_EquippingWeapon UMETA(DisplayName = "Equipping Weapon"),
	EAS_Landing UMETA(DisplayName = "Landing"),
	EAS_HitReaction UMETA(DisplayName = "HitReaction"),
	EAS_Dodge UMETA(DisplayName = "Dodge"),
	EAS_Dead UMETA(DisplayName = "Dead")

};
UENUM(BlueprintType)
enum class EDeadPose :uint8
{
	EDP_Death1 UMETA(DisplayName = "Death1"),
	EDP_Death2 UMETA(DisplayName = "Death2"),

	EDP_Max UMETA(DisplayName = "Max")
};
UENUM(BlueprintType)
enum class EEnemyState :uint8
{
	EES_Death1 UMETA(DisplayName = "Death1"),
	EES_Death2 UMETA(DisplayName = "Death2"),
	EES_Death3 UMETA(DisplayName = "Death3"),
	EES_Death4 UMETA(DisplayName = "Death4"),

	EES_Max UMETA(DisplayName = "Max")
};
UENUM(BlueprintType)
enum class EEnemyAction :uint8
{
	EEA_NoState UMETA(DisplayName = "NoState"),
	EEA_Dead UMETA(DisplayName = "Dead"),
	EEA_Patroling UMETA(DisplayName = "Patroling"),
	EEA_Chasing UMETA(DisplayName = "Chasing"),
	EEA_Attacking UMETA(DisplayName = "Attacking"),
	EEA_Engaged UMETA(DisplayName = "Engaged")

};