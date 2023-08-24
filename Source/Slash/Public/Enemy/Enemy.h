
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Enemy.generated.h"

class AAIController;
class UHealthBarComponent;
class UAttributeComponent;
class UAnimMontage;
class USoundBase;
class UParticleSystem;

UCLASS()
class SLASH_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;
	void DirectionalHitReact(const FVector& ImpactPoint);
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
protected:
	virtual void BeginPlay() override;
	
	void PlayHitReactMontage(const FName& SectionName);

	void Die();

	bool InTargetRange(AActor* Target, double Radius);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	TObjectPtr<UAnimMontage> DeathMontage;
	
	UPROPERTY(EditAnywhere, Category = "Sounds")
	TObjectPtr<USoundBase> HitSound;
	
	UPROPERTY(EditAnywhere, Category = "VisualEffects")
	TObjectPtr<UParticleSystem> HitParticles;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeComponent> Attributes; 

	UPROPERTY(EditAnywhere)
	TObjectPtr<UHealthBarComponent> HealthBarWidget;

	UPROPERTY()
	TObjectPtr<AActor> CombatTarget;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;
	UPROPERTY(EditAnywhere)
	double PatrolRadius = 20.f;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TObjectPtr<AActor> PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<TObjectPtr<AActor>> PatrolTargets; 

	UPROPERTY()
	TObjectPtr<AAIController> AIController;
public:
};
