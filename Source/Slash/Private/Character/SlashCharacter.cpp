
#include "Character/SlashCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GroomComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Animation/AnimMontage.h"
#include "Items/Weapon/Weapon.h"


//-------------------------------------------------------------------------//

ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->bUsePawnControlRotation = true;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");
}

void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Jump);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &ASlashCharacter::EKeyPressed);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ASlashCharacter::Attack);
	}	
}

void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("SlashCharacter"));
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(SlashContext, 0);
		}
	}
}


//-------------------------------------------------------------------------//

void ASlashCharacter::Move(const FInputActionValue& Value)
{
	if(ActionState != EActionState::EAS_Unoccupied) return;
	
	if (GetController() == nullptr) return;
		
	const FVector2d MovementVector = Value.Get<FVector2d>();
	 
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation = FRotator(0.f, Rotation.Yaw, 0.f);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void ASlashCharacter::Look(const FInputActionValue& Value)
{
	if (GetController() == nullptr) return;

	const FVector2d LookVector = Value.Get<FVector2d>();

	AddControllerPitchInput(LookVector.Y);
	AddControllerYawInput(LookVector.X);
}

void ASlashCharacter::EKeyPressed(const FInputActionValue& Value)
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if(OverlappingWeapon)
	{
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"),this ,this);
		OverlappingWeapon->SetOwner(this);
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		EquippedWeapon = OverlappingWeapon;
		OverlappingItem = nullptr;
	}else
	{
		if (CanDisarm())
		{
			PlayEquipMontage(FName("UnEquip"));
			CharacterState = ECharacterState::ECS_Unequipped;
			ActionState = EActionState::EAS_Equipping;
		}
		else if (CanArm())
		{
			PlayEquipMontage(FName("Equip"));
			CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
			ActionState = EActionState::EAS_Equipping;
		}
	}
}

bool ASlashCharacter::CanDisarm()
{
	return
	ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool ASlashCharacter::CanArm()
{
	return
	ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped &&
			EquippedWeapon;
}

void ASlashCharacter::DisArm()
{
	if (EquippedWeapon)
	{
		
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void ASlashCharacter::Arm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void ASlashCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::PlayEquipMontage(FName SectionName)
{

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}


//-------------------------------------------------------------------------//

void ASlashCharacter::Attack(const FInputActionValue& Value)
{
	Super::Attack(Value);
	if(CanAttack())
	{
		PlayAttackMontage();
		ActionState	= EActionState::EAS_Attacking;
	}
}

bool ASlashCharacter::CanAttack()
{
	return
		ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}


void ASlashCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}





