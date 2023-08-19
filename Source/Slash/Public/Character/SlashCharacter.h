#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "SlashCharacter.generated.h"

UCLASS()
class SLASH_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASlashCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnable(ECollisionEnabled::Type CollisionEnabled);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* SlashContext;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, Category = Input)
    UInputAction* EquipAction;
    
    UPROPERTY(EditAnywhere, Category = Input)
    UInputAction* AttackAction;
	
	void Move(const struct FInputActionValue& Value);
	void Look(const struct FInputActionValue& Value);
	void EKeyPressed(const struct FInputActionValue& Value);
	void Attack(const struct FInputActionValue& Value);

	void PlayAttackMontage();
	
	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	bool CanAttack();
	void PlayEquipMontage(FName SectionName);
	bool CanDisarm();
	bool CanArm();

	UFUNCTION(BlueprintCallable)
	void DisArm();

	UFUNCTION(BlueprintCallable)
	void Arm();
	
	UFUNCTION(BlueprintCallable)
	void FinishEquipping();
	
private:
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;
	
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;
	
	UPROPERTY(VisibleAnywhere)
	class UGroomComponent* Hair;
	
	UPROPERTY(VisibleAnywhere)
	UGroomComponent* Eyebrows;

	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<class AItem> OverlappingItem;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	TObjectPtr<class AWeapon> EquippedWeapon;
	
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

public:
	FORCEINLINE	void SetOverlappingItem(AItem* Item) {OverlappingItem = Item;}
	FORCEINLINE	ECharacterState GetCharacterState() const {return CharacterState;}
};



