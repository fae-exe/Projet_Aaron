// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS_Character.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/Engine.h"
#include "MyHUD.h"
//UMG
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Projet_Aaron/Item/Item.h"

// Sets default values
AFPS_Character::AFPS_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	fpsCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPS_Camera"));
	fpsCamera->SetupAttachment(RootComponent);
	fpsCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
	fpsCamera->bUsePawnControlRotation = true;
	
	stateManager = CreateDefaultSubobject<UStateManager>(TEXT("StateManager"));
	
	GetCharacterMovement()->MaxWalkSpeed = stateManager->speed;
	GetCharacterMovement()->JumpZVelocity = stateManager->jumpForce;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	RightArmEquipment = CreateDefaultSubobject<UChildActorComponent>(TEXT("Right Arm Equipment"));
	RightArmEquipment->SetupAttachment(fpsCamera);

	LeftArmEquipment = CreateDefaultSubobject<UChildActorComponent>(TEXT("Left Arm Equipment"));
	LeftArmEquipment->SetupAttachment(fpsCamera);
}

// Called when the game starts or when spawned
void AFPS_Character::BeginPlay()
{
	Super::BeginPlay();

	//InventoryCastObject = CreateWidget<UUInventoryCastObject>(this, InventoryCastObjectClass);
	//InventoryCastObject->AddToViewport();
}

// Called every frame
void AFPS_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (stateManager->stamina < stateManager->maxStamina)
		RecoveryStamina(DeltaTime);

	FHitResult outHit;
	FVector vStart = fpsCamera->GetComponentLocation();
	FVector vEnd = vStart + fpsCamera->GetForwardVector() * RaycastDistanceInventory;
	FCollisionQueryParams collisionParams;

	//DrawDebugLine(GetWorld(), vStart, vEnd, FColor::Red, false, 1, 0, 1);
	
	if(GetWorld()->LineTraceSingleByChannel(outHit, vStart, vEnd, ECC_Visibility, collisionParams))
	{
		if(outHit.bBlockingHit)
		{
			UStaticMeshComponent* actorMeshComponent = outHit.Actor->FindComponentByClass<UStaticMeshComponent>();
			if (outHit.Actor->ActorHasTag(FName(TEXT("Analysable"))))
			{
				actorMeshComponent->SetCustomDepthStencilValue(2);
				if (hitActor && outHit.Actor != hitActor->Actor)
					hitActor = new FHitResult(outHit);
			}
			else if(outHit.Actor->ActorHasTag(FName(TEXT("Destructable"))))
			{
				actorMeshComponent->SetCustomDepthStencilValue(3);
				if (hitActor && outHit.Actor != hitActor->Actor)
					hitActor = new FHitResult(outHit);
			}
			else if (outHit.Actor->ActorHasTag(FName(TEXT("Item"))))
			{
				UE_LOG(LogActor, Error, TEXT("Item"));
				AItem* item = Cast<AItem>(outHit.Actor);
				InventoryCastObject->nameTextItem = item->ItemStructure->Name;
				if (hitActor && outHit.Actor != hitActor->Actor)
					hitActor = new FHitResult(outHit);
			}
			else
			{
				if (hitActor)
				{
					actorMeshComponent = hitActor->Actor->FindComponentByClass<UStaticMeshComponent>();
					actorMeshComponent->SetCustomDepthStencilValue(1);
					hitActor = nullptr;
				}
				InventoryCastObject->nameTextItem = "";
			}
		}
	} else if(hitActor)
	{
		UStaticMeshComponent* actorMeshComponent = hitActor->Actor->FindComponentByClass<UStaticMeshComponent>();
		actorMeshComponent->SetCustomDepthStencilValue(1);
		hitActor = nullptr;
		InventoryCastObject->nameTextItem = "";
	}
}

// Called to bind functionality to input
void AFPS_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPS_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPS_Character::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AFPS_Character::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPS_Character::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPS_Character::StartJump);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFPS_Character::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFPS_Character::StopSprint);

	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &AFPS_Character::StartAlt);
	PlayerInputComponent->BindAction("Dodge", IE_Released, this, &AFPS_Character::StopAlt);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AFPS_Character::Crouching);
	
	PlayerInputComponent->BindAction("FireLeft", IE_Pressed,this, &AFPS_Character::ActivatePressedLeft);
	PlayerInputComponent->BindAction("FireLeft", IE_Released,this, &AFPS_Character::ActivateReleasedLeft);

	PlayerInputComponent->BindAction("FireRight", IE_Pressed, this, &AFPS_Character::ActivatePressedRight);
	PlayerInputComponent->BindAction("FireRight", IE_Released, this, &AFPS_Character::ActivateReleasedRight);
	
	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &AFPS_Character::Action);
	PlayerInputComponent->BindAction("Action", IE_Repeat, this, &AFPS_Character::Analyse);
	PlayerInputComponent->BindAction("Action", IE_Released,this, &AFPS_Character::StopAction);
}

void AFPS_Character::MoveForward(float value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	if (bPressedAlt)
	{
		Dodge(Direction * value);
	}
	else if (isNearClimbing)
	{
		Climb(value);
	}
	else
	{
		AddMovementInput(Direction, value);
	}

	if (isSprinting)
	{
		if (stateManager->stamina - stateManager->sprintCostStamina >= 0)
			stateManager->stamina -= stateManager->sprintCostStamina;
		else
			StopSprint();
	}
}

void AFPS_Character::MoveRight(float value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	if (bPressedAlt)
	{
		Dodge(Direction * value);
	}
	else
	{
		AddMovementInput(Direction, value);
	}
}

void AFPS_Character::StartJump()
{
	if(!GetCharacterMovement()->IsFalling() && stateManager->stamina >= 5.0f)
	{
		stateManager->stamina -= 5.0f;
		Jump();
	}
}

void AFPS_Character::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed *= stateManager->sprintSpeedScalar;
	isSprinting = true;
}

void AFPS_Character::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = stateManager->speed;
	isSprinting = false;
}

void AFPS_Character::Dodge(FVector direction)
{
	if(!GetCharacterMovement()->IsFalling())
		GetCharacterMovement()->AddImpulse(direction * 1000.0f, true);
}

void AFPS_Character::StartAlt()
{
	bPressedAlt = true;
}

void AFPS_Character::StopAlt()
{
	bPressedAlt = false;
}

void AFPS_Character::Crouching()
{  
	if (CanCrouch())
		Crouch();
	else
		UnCrouch();
}

void AFPS_Character::RecoveryStamina(float deltaTime)
{
	stateManager->stamina += stateManager->recorveryStamina * deltaTime;

	if (stateManager->stamina > stateManager->maxStamina)
		stateManager->stamina = stateManager->maxStamina;
}

void AFPS_Character::Action()
{
	UE_LOG(LogActor, Error, TEXT("Salut"));
	if(hitActor)
	{
		UE_LOG(LogActor, Error, TEXT("Salut actor"));
		 if (hitActor->Actor->ActorHasTag(FName(TEXT("Destructable"))))
		 {
			hitActor->Actor->Destroy();
			hitActor = nullptr;
		 }else if (hitActor->Actor->ActorHasTag(FName(TEXT("Item"))))
		 {
			 UE_LOG(LogActor, Warning, TEXT("ImplementInteractInterface"));

			 //if(hitActor->GetActor()->Implements<UInteract_Interface>())
			 //{
				// //IInteract_Interface::Execute_Interact(hitActor->GetActor());
			 //}
		 }
	}
}

void AFPS_Character::StopAction()
{
	Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->ResetCircleRadius();
}


void AFPS_Character::Analyse()
{
	AMyHUD* myHUD = Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	float val = 0;
	myHUD->BarMatInstance->GetScalarParameterValue(FName(TEXT("Decimal")), val);
	myHUD->UpdateCircleRadius(val + 0.01f);
}

void AFPS_Character::Climb(float value)
{
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	AddMovementInput(GetActorUpVector(), value);
}

void AFPS_Character::StopClimbing()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void AFPS_Character::ActivatePressedLeft()
{
	AActor* ChildActor = LeftArmEquipment->GetChildActor();
	if (IsValid(ChildActor) && ChildActor->Implements<UEquipmentInterface>())
		IEquipmentInterface::Execute_Activate(ChildActor, true);
}

void AFPS_Character::ActivateReleasedLeft()
{
	AActor* ChildActor = LeftArmEquipment->GetChildActor();
	if (IsValid(ChildActor) && ChildActor->Implements<UEquipmentInterface>())
		IEquipmentInterface::Execute_Activate(ChildActor, false);
}

void AFPS_Character::ActivatePressedRight()
{
	AActor* ChildActor = RightArmEquipment->GetChildActor();
	if (IsValid(ChildActor) && ChildActor->Implements<UEquipmentInterface>())
		IEquipmentInterface::Execute_Activate(ChildActor, true);
}

void AFPS_Character::ActivateReleasedRight()
{
	AActor* ChildActor = RightArmEquipment->GetChildActor();
	if (IsValid(ChildActor) && ChildActor->Implements<UEquipmentInterface>())
		IEquipmentInterface::Execute_Activate(ChildActor, false);
}

void AFPS_Character::PressedItemWheel()
{
}

void AFPS_Character::RealeaseItemWheel()
{
}