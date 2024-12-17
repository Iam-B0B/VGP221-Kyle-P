// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Projectile/FPSProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/FPSHUD.h"
#include "FPSCharacter.generated.h"

UCLASS()
class VGP221FALLTERM2024_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* FPSMesh;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FPSCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FVector MuzzleOffset;

	//UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	//TSubclassOf<class AFPSProjectile> ProjectileClass;

	UFUNCTION()
	void MoveForward(float value);

	UFUNCTION()
	void MoveRightCusIWantTo(float value);

	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void EndJump();

	UFUNCTION()
	void Interact();

	//UFUNCTION()
	//void Fire();

	UFUNCTION()
	void Damage(float damageAmt);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
	// Temp Health Code: Try making the health component from last term
	float Health = -1;
	const float MaxHealth = 100;

	UPROPERTY(EditAnywhere)
	float InteractiveLineTraceLength = 350.f;

};
