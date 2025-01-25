// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BubbleCharacter.h"


#include "Interactables/Interactable.h"

#include "GAS/BubbleAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABubbleCharacter::ABubbleCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UBubbleAttributeSet>(TEXT("AttributeSet"));
}

void ABubbleCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ABubbleCharacter, FocusedInteractableObject, COND_None, REPNOTIFY_Always);
}

void ABubbleCharacter::Server_SetFocusedInteractable_Implementation(UObject* InFocusedInteractable)
{
	SetFocusedInteractable(InFocusedInteractable);
}

void ABubbleCharacter::SetFocusedInteractable(UObject* InFocusedInteractable)
{
	if (HasAuthority() == false)
	{
		Server_SetFocusedInteractable(InFocusedInteractable);
		return;
	}
	FocusedInteractableObject = InFocusedInteractable;
}

void ABubbleCharacter::EmitInteractionChecker()
{
	UE_LOG(LogTemp, Warning, TEXT("Object has not implemented EmitInteractionChecker"));
}

bool ABubbleCharacter::CheckForInteractables(FHitResult HitResult)
{
	//APlayerController* PC = Cast<APlayerController>(GetController());
	//if(PC!= nullptr && PC->GetInput)

	AActor* HitActor = HitResult.GetActor();
	UPrimitiveComponent* HitComponent = HitResult.GetComponent();
	IInteractable* InteractableActor = Cast<IInteractable>(HitActor);

	if (FocusedInteractableObject == HitActor)
	{
		return true;
	}
	if (InteractableActor == nullptr)
	{
		if (FocusedInteractableObject == nullptr)
		{
			return false;
		}
		UActorComponent* InteractableMeshComponent = Cast<AActor>(FocusedInteractableObject)->GetComponentByClass(UMeshComponent::StaticClass());
		if (Cast<UMeshComponent>(InteractableMeshComponent))
		{
			Cast<UMeshComponent>(InteractableMeshComponent)->SetOverlayMaterial(nullptr);
		}

		InteractIndicationTextDelegate.Broadcast(FText::FromString(""));
		Server_SetFocusedInteractable(nullptr);
		return false;
	}
	if (!InteractableActor->bCanInteract(GetController()))
	{
		return false;
	}
	if (FocusedInteractableObject != nullptr)
	{
		UActorComponent* InteractableMeshComponent = Cast<AActor>(FocusedInteractableObject)->GetComponentByClass(UMeshComponent::StaticClass());
		if (Cast<UMeshComponent>(InteractableMeshComponent))
		{
			Cast<UMeshComponent>(InteractableMeshComponent)->SetOverlayMaterial(nullptr);
		}
	}

	UMeshComponent* HitMeshComponent = Cast<UMeshComponent>(HitActor->GetComponentByClass(UMeshComponent::StaticClass()));
	if (HitMeshComponent != nullptr)
	{
		HitMeshComponent->SetOverlayMaterial(InteractableOverlayMaterial);
	}

	InteractIndicationTextDelegate.Broadcast(FText::FromString("E " + InteractableActor->GetInteractableName().ToString()));
	Server_SetFocusedInteractable(HitActor);
	return true;
}

void ABubbleCharacter::TriggerInteraction()
{
	FGameplayEventData Data = FGameplayEventData();
	AbilitySystemComponent->HandleGameplayEvent(InteractionAbilityTag, &Data);
}

void ABubbleCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitCharacterDefaults();
}

void ABubbleCharacter::OnRep_PlayerState()
{
}

void ABubbleCharacter::InitCharacterDefaults()
{
	if (IsValid(AbilitySystemComponent) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ABubbleCharacter::InitCharacterDefaults IsValid(AbilitySystemComponent) == false"));
		return;
	}

	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultAbilities)
	{
		FGameplayAbilitySpec Spec = FGameplayAbilitySpec(AbilityClass, 0, INDEX_NONE, this);

		if (const UGameplayAbility* Ability = Cast<UGameplayAbility>(Spec.Ability))
		{
			//Spec.DynamicAbilityTags.AddTag(Ability->InputTag);
			AbilitySystemComponent->GiveAbility(Spec);
		}
	}

	FActiveGameplayEffectHandle GEHandle;

	if (IsValid(DefaultAttributeEffect) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("USG_AbilitySystemComponent::AddEffectFromClass IsValid(GameplayEffectClass) == false"));
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 0, EffectContext);

	if (EffectSpecHandle.IsValid())
	{
		GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}
}

void ABubbleCharacter::Move(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Object has not implemented Move"));
}

// Called to bind functionality to input
void ABubbleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABubbleCharacter::Move);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

}

