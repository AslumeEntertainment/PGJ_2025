// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/FlatBubbleCharacter.h"

#include "Components/CapsuleComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

AFlatBubbleCharacter::AFlatBubbleCharacter()
{
	InteractionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InteractionCapsule"));
	InteractionCapsule->SetupAttachment(GetRootComponent());
}

void AFlatBubbleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AFlatBubbleCharacter::StartCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AFlatBubbleCharacter::StopCrouch);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AFlatBubbleCharacter::Move(const FInputActionValue& Value)
{
	float MovementValue = Value.Get<float>();

	FVector Direction = (MovementValue > 0) ? FVector(0, 1, 0) : FVector(0, -1, 0);
	
	AddMovementInput(Direction, FMath::Abs(MovementValue));
}

void AFlatBubbleCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//SetActorRotation(FRotator(0, 0, 0));
	InteractionCapsule->OnComponentBeginOverlap.AddDynamic(this, &AFlatBubbleCharacter::OnInteractionCapsuleBeginOverlap);
}

void AFlatBubbleCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InteractionCapsule->OnComponentBeginOverlap.AddDynamic(this, &AFlatBubbleCharacter::OnInteractionCapsuleBeginOverlap);
}

void AFlatBubbleCharacter::StartCrouch()
{
	Crouch();
}

void AFlatBubbleCharacter::StopCrouch()
{
	UnCrouch();
}

void AFlatBubbleCharacter::OnInteractionCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Flat bubble has overlaped"));
	if (CheckForInteractables(SweepResult))
	{
		TriggerInteraction();
	}
}
