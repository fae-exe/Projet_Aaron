// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Projet_Aaron/Character/CharacterUtils.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "CoreMinimal.h"
#include "CreatureStatManager.h"
#include "CharacterStatManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJET_AARON_API UCharacterStatManager : public UCreatureStatManager
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
		float GravityScale = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
		float AirControl = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Walking)
		float SprintSpeed = 800.0f;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Crouching)
		float CrouchSpeed = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Crouching)
		float CrouchWalkSpeed = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Swimming)
		float SwimmingSpeed = 350.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Swimming)
		float SwimmingSprintSpeed = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sliding)
		float SlideForce = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sliding)
		float SlideStopVelocity = 25.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sliding)
		float SlopeSlidingAngle = 0.3f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sliding)
		float SlopeStoppingAngle = 0.3f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dodging)
		float DodgeStaminaCost = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dodging)
		float DodgeForce = 2500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Climbing)
		float ClimbSpeed = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Climbing)
		float ClimbRange = 400.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Oxygene)
		float Oxygene;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Oxygene)
		float OxygeneMax = 60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Jumping)
		float MaxJumpMulti = 3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Glider)
		float GlidingGravityScale = 0.15f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Glider)
		float GlidingAirControl = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Glider)
		float GlidingFallingLateralFriction = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UMaterialParameterCollectionInstance* ParameterCollectionInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UMaterialParameterCollection* ParameterCollection;

	virtual void BeginPlay() override;

public:

	/*Contructors*/
	UCharacterStatManager();

	/*Getters*/
	UFUNCTION(BlueprintCallable)
		UMaterialParameterCollectionInstance* GetParameterCollectionInstance() const { return ParameterCollectionInstance; }
	UFUNCTION(BlueprintCallable)
		float GetGravityScale() const { return GravityScale; }
	UFUNCTION(BlueprintCallable)
		float GetAirControl() const { return AirControl; }
	UFUNCTION(BlueprintCallable)
		float GetDodgeStaminaCost() const { return DodgeStaminaCost; }
	UFUNCTION(BlueprintCallable)
		float GetDodgeForce() const { return DodgeForce; }
	UFUNCTION(BlueprintCallable)
		float GetSprintSpeed() const { return SprintSpeed; }
	UFUNCTION(BlueprintCallable)
		float GetCrouchRunSpeed() const { return CrouchSpeed; }
	UFUNCTION(BlueprintCallable)
		float GetCrouchWalkSpeed() const { return CrouchWalkSpeed; }
	UFUNCTION(BlueprintCallable)
		float GetSwimmingSpeed() const { return SwimmingSpeed; }
	UFUNCTION(BlueprintCallable)
		float GetSwimmingSprintSpeed() const { return SwimmingSprintSpeed; }
	UFUNCTION(BlueprintCallable)
		float GetClimbRange() const { return ClimbRange; }
	UFUNCTION(BlueprintCallable)
		float GetClimbSpeed() const { return ClimbSpeed; }
	UFUNCTION(BlueprintCallable)
		float GetSlideForce() const { return SlideForce; }
	UFUNCTION(BlueprintCallable)
		float GetSlideStopVelocity() const { return SlideStopVelocity; }
	UFUNCTION(BlueprintCallable)
		float GetOxygeneRate() const { return Oxygene / OxygeneMax; }
	UFUNCTION(BlueprintCallable)
		float GetSlopeSlideAngle() const { return SlopeSlidingAngle; }
	UFUNCTION(BlueprintCallable)
		float GetSlopeStoppingAngle() const { return SlopeStoppingAngle; }
	UFUNCTION(BlueprintCallable)
		float GetGlidingGravityScale() const { return GlidingGravityScale; }
	UFUNCTION(BlueprintCallable)
		float GetGlidingAirControl() const { return GlidingAirControl; }
	UFUNCTION(BlueprintCallable)
		float GetGlidingFallingLateralFriction() const { return GlidingFallingLateralFriction; }
	UFUNCTION(BlueprintCallable)
		float GetNightVisionEffect();

	UPROPERTY(BlueprintReadOnly)
	FCharacterSkills Skills;

	/*Setters*/
	UFUNCTION(BlueprintCallable)
		void SetDodgeStaminaCost(float NewDodgeStaminaCost) { DodgeStaminaCost = NewDodgeStaminaCost; }
	UFUNCTION(BlueprintCallable)
		void SetDodgeForce(float NewDodgeForce) { DodgeForce = NewDodgeForce; }
	UFUNCTION(BlueprintCallable)
		void SetPoisonEffect(float Value);
	UFUNCTION(BlueprintCallable)
		void SetNightVisionEffect(float Value);

	/*Others*/
	void TakeDamage(float BioDamage, float TechDamage);
	void ConsumeOxygene(float OxygeneToConsume);
	void RecoveryOxygene(float DeltaTime);
};
