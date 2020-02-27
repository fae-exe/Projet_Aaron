// Fill out your copyright notice in the Description page of Project Settings.


#include "PoisonAlteration.h"

// Sets default values for this component's properties
UPoisonAlteration::UPoisonAlteration()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	TimeAlteration = TimeMutation;
	PoisonDamageBio = 5.0f;
	PoisonDamageTech = 2.0f;

	// ...
}


// Called when the game starts
void UPoisonAlteration::BeginPlay()
{
	Super::BeginPlay();
	_statManager = GetOwner()->FindComponentByClass<UStatManager>();
	DamageOverTime();
}

// Called every frame
void UPoisonAlteration::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
}

void UPoisonAlteration::DamageOverTime()
{
	UWorld* World = GetWorld();
	World->GetTimerManager().SetTimer(InputTimeHandle, this, &UPoisonAlteration::TakeDamage, 1.0f, true, 0.5f);
}


void UPoisonAlteration::TakeDamage()
{
	if (TimeAlteration <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(InputTimeHandle);
	}
	
	TimeAlteration -= 1.0f;
	_statManager->TakeDamage(PoisonDamageBio, PoisonDamageTech);
}
