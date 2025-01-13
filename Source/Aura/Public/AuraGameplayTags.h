// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * AuraGameplayTags
 *
 * Singleton Containing native Gameplay Tags
 */

struct FAuraGameplayTags
{
	static const FAuraGameplayTags& Get() {return GameplayTags;}
	static void InitializeNativeGameplayTags();

	/* Primary Attributes Start */
	FGameplayTag Attribute_Primary_Intelligence;
	FGameplayTag Attribute_Primary_Strength;
	FGameplayTag Attribute_Primary_Resilience;
	FGameplayTag Attribute_Primary_Vigor;
	/* Primary Attributes End */ 	

	/* Secondary Attributes Start */ 
	FGameplayTag Attribute_Secondary_Armor;
	FGameplayTag Attribute_Secondary_ArmorPenetration;
	FGameplayTag Attribute_Secondary_BlockChance;
	FGameplayTag Attribute_Secondary_CriticalHitChance;
	FGameplayTag Attribute_Secondary_CriticalHitDamage;
	FGameplayTag Attribute_Secondary_CriticalHitResistance;
	FGameplayTag Attribute_Secondary_HealthRegeneration;
	FGameplayTag Attribute_Secondary_ManaRegeneration;
	FGameplayTag Attribute_Secondary_MaxHealth;
	FGameplayTag Attribute_Secondary_MaxMana;
	/* Secondary Attributes End */

	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_5;
	FGameplayTag InputTag_6;
	FGameplayTag InputTag_7;
	FGameplayTag InputTag_8;
	FGameplayTag InputTag_9;
	FGameplayTag InputTag_0;

	FGameplayTag Damage;
	FGameplayTag Effects_HitReact;

private:
	static FAuraGameplayTags GameplayTags;
};
