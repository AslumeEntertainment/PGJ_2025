// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/FlatBubbleCharacter.h"

#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

AFlatBubbleCharacter::AFlatBubbleCharacter()
{
	InteractionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InteractionCapsule"));
	InteractionCapsule->SetupAttachment(GetRootComponent());
	InteractionCapsule->OnComponentBeginOverlap.AddDynamic(this, &AFlatBubbleCharacter::OnInteractionCapsuleBeginOverlap);
}

void AFlatBubbleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AFlatBubbleCharacter::StartCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AFlatBubbleCharacter::StopCrouch);
	}
}

void AFlatBubbleCharacter::Move(const FInputActionValue& Value)
{
	float MovementValue = Value.Get<float>();

	FVector Direction = (MovementValue > 0) ? FVector(0, 1, 0) : FVector(0, -1, 0);
	
	AddMovementInput(Direction, FMath::Abs(MovementValue));
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
}
