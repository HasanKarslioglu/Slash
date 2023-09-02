
#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Character/CharacterTypes.h"
#include "Enemy.generated.h"

class UPawnSensingComponent;
class AAIController;
class UHealthBarComponent;

UCLASS()
class SLASH_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;

	/*
	 * Hit and Damage
	 */
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual void Destroyed() override;
	
protected:
	virtual void BeginPlay() override;

	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);

	virtual void Die() override;

	/*
	 * PATROL
	 */
	TObjectPtr<AActor> ChoosePatrolTarget();
	void CheckPatrolTarget();

	UPROPERTY(EditAnywhere, Category = "Patrol Target")
	float PatrolPointMinTime = 3.f;

	UPROPERTY(EditAnywhere, Category = "Patrol Target")
	float PatrolPointMaxTime = 6.f;

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 20.f;
	
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TObjectPtr<AActor> PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<TObjectPtr<AActor>> PatrolTargets; 

	FTimerHandle PatrolTimer;
	void PatrolTimerFinished();
	
	/*
	 * COMBAT
	 */
	void CheckCombatTarget();
	UPROPERTY()
	TObjectPtr<AActor> CombatTarget;
	UPROPERTY(EditAnywhere)
	double CombatRadius = 600.f;
	UPROPERTY(EditAnywhere)
	double AttackRadius = 150.f;

	virtual void Attack() override;
	virtual void PlayAttackMontage() override;

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	/*
	 * Enemy Components
	 */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPawnSensingComponent> PawnSensing;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UHealthBarComponent> HealthBarWidget;
	UPROPERTY()
	TObjectPtr<AAIController> AIController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, Category = "LifeSpan Time", meta = (AllowPrivateAccess = "true"))
	float LifeSpanTime = 5.f;
};
