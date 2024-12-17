// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FPSCharacter.h"

#include "DoorActor.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Similar Unity Instantiate but only for constructors
	if (!FPSCameraComponent) {
		FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
		FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
		FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
		FPSCameraComponent->bUsePawnControlRotation = true;
	}

	if (!FPSMesh) {
		FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
		FPSMesh->SetupAttachment(FPSCameraComponent);
		FPSMesh->bCastDynamicShadow = false;
		FPSMesh->CastShadow = false;
	}

	GetMesh()->SetOwnerNoSee(true);

	UE_LOG(LogTemp, Warning, TEXT("Character Constructor is called"));
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRightCusIWantTo);

	// Look
	PlayerInputComponent->BindAxis("LookHorizontal", this, &AFPSCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookVertical", this, &AFPSCharacter::AddControllerPitchInput);

	// Jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::EndJump);

	// Fire
	//PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFPSCharacter::Interact);
}

void AFPSCharacter::MoveForward(float value)
{
	// 1. Unreal tutorial way
	// FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	// 2. Looking through documentation for another solution
	FVector Direction = GetActorForwardVector();
	AddMovementInput(Direction, value);
}

void AFPSCharacter::MoveRightCusIWantTo(float value)
{
	//FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	FVector Direction = GetActorRightVector();
	AddMovementInput(Direction, value);
}

void AFPSCharacter::StartJump()
{
	bPressedJump = true;
}

void AFPSCharacter::EndJump()
{
	bPressedJump = false;
}


void AFPSCharacter::Damage(float damageAmt)
{
	// Minus Health from UI
	AFPSHUD* HUD = UGameplayStatics::GetPlayerController(this, 0)->GetHUD<AFPSHUD>();
	if (!HUD) return;

	Health -= damageAmt;
	float HealthPercent = Health / MaxHealth;

	HUD->gameWidgetContainer->SetHealthBar(HealthPercent);
}

void AFPSCharacter::Interact()
{
	if(FPSCameraComponent == nullptr) return;

	[[maybe_unused]] FHitResult HitResult;
	FVector Start = FPSCameraComponent->GetComponentLocation();
	FVector End = Start + FPSCameraComponent->GetForwardVector() * InteractiveLineTraceLength;
	GetWorld()-> LineTraceSingleByChannel(HitResult, Start, End,	ECC_Visibility);

	ADoorActor* Door = Cast<ADoorActor>(HitResult.GetActor());
	if(Door)
	{
		Door->OnInteract();
	}
}


float AFPSCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Damage(FinalDamage);

	return FinalDamage;
}

