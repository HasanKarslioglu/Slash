#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "SlashCharacter.generated.h"

UCLASS()
class SLASH_API ASlashCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ASlashCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	/*
	 * Input Actions
	 */
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

	/*
	 * Move and Key Presses
	 */
	void Move(const struct FInputActionValue& Value);
	void Look(const struct FInputActionValue& Value);
	void EKeyPressed(const struct FInputActionValue& Value);

	/*
	 * Attack Functions
	 */
	virtual void Attack(const FInputActionValue& Value) override;
	virtual void PlayAttackMontage() override;
	virtual bool CanAttack() override;
	virtual void AttackEnd() override;

	/*
	 * Equip weapon
	 */
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

	/*
	 * States
	 */
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	/*
	 * Character Components
	 */
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere)
	class UGroomComponent* Hair;
	UPROPERTY(VisibleAnywhere)
	UGroomComponent* Eyebrows;
	
	/*
	 * Montages
	 */
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<class AItem> OverlappingItem;

public:
	FORCEINLINE	void SetOverlappingItem(AItem* Item) {OverlappingItem = Item;}
	FORCEINLINE	ECharacterState GetCharacterState() const {return CharacterState;}
};



