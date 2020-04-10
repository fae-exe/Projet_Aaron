// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Projet_Aaron/Character/GrappleHead.h"
#include "GameFramework/PlayerController.h"
#include "Projet_Aaron/Equipment/GrapplePointer.h"
#include "Components/TimelineComponent.h"
#include "Materials/MaterialInstance.h"
#include "HookInterface.h"
#include "CableComponent.h"
#include "CoreMinimal.h"
#include "EquipmentBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GrapnelEquipmentSuperAaron.generated.h"

UCLASS()
class PROJET_AARON_API AGrapnelEquipmentSuperAaron : public AEquipmentBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrapnelEquipmentSuperAaron();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class AGrappleHead* myBullet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UCableComponent* laser;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* hookMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UChildActorComponent* pointer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UCableComponent* cable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UTimelineComponent* timeline;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Distance = 5000.0f;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool isActivated = false;
	bool canHook = true;
	bool foundHookSpot = false;
	bool playTLForLaser = true;

	float TimeInterpolation;

	FVector locationToGrip;
	FOnTimelineEvent updateFunction{};

public:
	virtual void Activate_Implementation(bool isPressed) override;

	UFUNCTION()
		void TimelineCallback();

	UFUNCTION()
		void updatePointerLocation();

	UFUNCTION()
		void AfterHook();

	UFUNCTION()
		void Hook();

	void PlayTimeline();

};
