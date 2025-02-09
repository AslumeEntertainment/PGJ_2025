// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BubbleCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Net/UnrealNetwork.h"

#include "Interactables/Interactable.h"
#include "GAS/BubbleAttributeSet.h"
#include "BubbleController.h"

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
	ABubbleController* PC = Cast<ABubbleController>(GetController());
	if (IsValid(PC) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ABubbleCharacter::CheckForInteractables IsValid(PC) == false"));
		return false;
	}
	if (PC->IsInputLocked)
	{
		if (FocusedInteractableObject != nullptr)
		{
			UActorComponent* InteractableMeshComponent = Cast<AActor>(FocusedInteractableObject)->GetComponentByClass(UMeshComponent::StaticClass());
			if (Cast<UMeshComponent>(InteractableMeshComponent))
			{
				Cast<UMeshComponent>(InteractableMeshComponent)->SetOverlayMaterial(nullptr);
			}
		}
		Server_SetFocusedInteractable(nullptr);
		return false;
	}

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
			UpdateInteractionText(FText::FromString(""), false);
			return false;
		}
		UActorComponent* InteractableMeshComponent = Cast<AActor>(FocusedInteractableObject)->GetComponentByClass(UMeshComponent::StaticClass());
		if (Cast<UMeshComponent>(InteractableMeshComponent))
		{
			UpdateInteractionText(FText::FromString(""), false);
			Cast<UMeshComponent>(InteractableMeshComponent)->SetOverlayMaterial(nullptr);
		}

		UpdateInteractionText(FText::FromString(""), false);
		Server_SetFocusedInteractable(nullptr);
		return false;
	}
	if (!InteractableActor->bCanInteract(GetController()))
	{
		UpdateInteractionText(InteractableActor->GetInteractableName(), false);
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

	Server_SetFocusedInteractable(HitActor);
	UpdateInteractionText(InteractableActor->GetInteractableName(), true);
	return true;
}

void ABubbleCharacter::TriggerInteraction()
{
	UE_LOG(LogTemp, Warning, TEXT("%s: Interact"), *GetName());

	FGameplayEventData Data = FGameplayEventData();
	AbilitySystemComponent->HandleGameplayEvent(InteractionAbilityTag, &Data);
}

void ABubbleCharacter::UpdateInteractionText(FText InteractableName, bool bCanInteract)
{
	if (bCanInteract)
	{
		InteractIndicationTextDelegate.Broadcast(FText::FromString("E " + InteractableName.ToString()));
	}
	else
	{
		InteractIndicationTextDelegate.Broadcast(InteractableName);
	}
	
}

void ABubbleCharacter::InitCharacterDefaults()
{
	if (bHasBeenInited)
	{
		return;
	}
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

	bHasBeenInited = true;
}

void ABubbleCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	Client_BindMappingContext();

	InitCharacterDefaults();
}

void ABubbleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABubbleCharacter::Move);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ABubbleCharacter::Client_BindMappingContext_Implementation()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (IsValid(PlayerController) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ABubbleCharacter::Client_BindMappingContext IsValid(PlayerController) == false"));
		return;
	}
	UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (IsValid(InputSystem) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ABubbleCharacter::Client_BindMappingContext IsValid(InputSystem) == false"));
		return;
	}

	InputSystem->AddMappingContext(DefaultMappingContext, 0);
}

void ABubbleCharacter::Client_UnbindMappingContext_Implementation()
{
	APlayerController* PlayerCont = Cast<APlayerController>(GetController());
	if (IsValid(PlayerCont) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ABubbleCharacter::Client_UnbindMappingContext IsValid(PlayerCont) == false"));
		return;
	}
	UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerCont->GetLocalPlayer());
	if (IsValid(InputSystem) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ABubbleCharacter::Client_UnbindMappingContext IsValid(InputSystem) == false"));
		return;
	}

	InputSystem->RemoveMappingContext(DefaultMappingContext);
}

void ABubbleCharacter::NetMulticast_ShowEffectAtCharacterLocation_Implementation(UNiagaraSystem* NiagaraEffect)
{
	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ABubbleCharacter::NetMulticast_ShowEffectAtCharacterLocation_Implementation IsValid(World) == false"));
		return;
	}
	if (IsValid(NiagaraEffect) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ABubbleCharacter::NetMulticast_ShowEffectAtCharacterLocation_Implementation IsValid(AbilityNiagaraEffect) == false"));
		return;
	}
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, NiagaraEffect, GetActorLocation(), GetActorRotation());
}

void ABubbleCharacter::RotateTowardsActor(UWorld* World, AActor* TargetActor)
{
	if (IsValid(World) == false || IsValid(TargetActor) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ABubbleCharacter::RotateTowardsActor IsValid(World) == false || IsValid(TargetActor) == false"));
		return;
	}

	FVector NormalizedLocation = TargetActor->GetActorLocation() - GetActorLocation();
	UKismetMathLibrary::Vector_Normalize(NormalizedLocation);

	float DotProduct = FVector::DotProduct(NormalizedLocation, GetMesh()->GetForwardVector());
	
	if (FMath::Abs(DotProduct) <= 0.01f)
	{
		return;
	}

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetActor->GetActorLocation());
	FRotator InterpRotation = UKismetMathLibrary::RInterpTo(GetActorRotation(), LookAtRotation, World->DeltaTimeSeconds, 20);
	
	SetActorRotation(FRotator(GetActorRotation().Pitch, InterpRotation.Yaw, GetActorRotation().Roll));

	FTimerHandle RotateTimer;
	FTimerDelegate RotateTimerDelegate;

	RotateTimerDelegate.BindUFunction(this, FName("RotateTowardsActor"), World, TargetActor);
	GetWorldTimerManager().SetTimer(RotateTimer, RotateTimerDelegate, 0.01f, false);
}

void ABubbleCharacter::NetMulticast_PlayAnimationMontage_Implementation(UAnimMontage* Animation)
{
	if (IsValid(Animation) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ABubbleCharacter::NetMulticast_PlayAnimationMontage_Implementation IsValid(Animation) == false"));
		return;
	}
	PlayAnimMontage(Animation);
}

void ABubbleCharacter::NetMulticast_StopAnimationMontage_Implementation(UAnimMontage* Animation)
{
	if (IsValid(Animation) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("ABubbleCharacter::NetMulticast_StopAnimationMontage_Implementation IsValid(Animation) == false"));
		return;
	}
	StopAnimMontage(Animation);
}

void ABubbleCharacter::Move(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Object has not implemented Move"));
}

