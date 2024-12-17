// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/FPSProjectile.h"

// Sets default values
AFPSProjectile::AFPSProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!CollisionComponent) {
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		CollisionComponent->InitSphereRadius(15.0f);
        CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
        CollisionComponent->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnWhateverYouWantToNameIt);
		RootComponent = CollisionComponent;
	}

    if (!ProjectileMovementComponent)
    {
        // Use this component to drive this projectile's movement.
        ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
        ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
        ProjectileMovementComponent->InitialSpeed = BulletSpeed;
        ProjectileMovementComponent->MaxSpeed = BulletSpeed;
        ProjectileMovementComponent->bRotationFollowsVelocity = true;
        ProjectileMovementComponent->bShouldBounce = true;
        ProjectileMovementComponent->Bounciness = 0.3f;
        ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
    }

    if (!ProjectileMeshComponent) {
        ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
        static ConstructorHelpers::FObjectFinder<UStaticMesh>SphereMeshAsset(TEXT("/Game/Mesh/Projectile/Sphere.Sphere"));
        if (SphereMeshAsset.Succeeded()) {
            ProjectileMeshComponent->SetStaticMesh(SphereMeshAsset.Object);
        }

        static ConstructorHelpers::FObjectFinder<UMaterial>SphereMaterial(TEXT("/Game/Material/M_Projectile.M_Projectile"));
        if (SphereMaterial.Succeeded()) {
            ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(SphereMaterial.Object, ProjectileMeshComponent);
        }

        ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
        ProjectileMeshComponent->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
        ProjectileMeshComponent->SetupAttachment(RootComponent);
    }

    InitialLifeSpan = 3.0f; // Unity Destroy(3.0f)
}

// Called when the game starts or when spawned
void AFPSProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPSProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSProjectile::FireInDirection(const FVector& ShootDirection)
{
    // 1. FVector ShootDirection: Pass by value, least efficient
    // 2. FVector* ShootDirection: Pass by memory, saves these siutation where you will need to keep using a pointer
    // 3. FVector& ShootDirection: Pass by ref, memory: Someone can change it content. Basically out
    // 4. const FVector& ShootDirection: Pass by ref, memory: We can't directly change the parameter

    ProjectileMovementComponent->Velocity = ProjectileMovementComponent->InitialSpeed * ShootDirection;
}

void AFPSProjectile::OnWhateverYouWantToNameIt(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor != this && OtherComponent->IsSimulatingPhysics()) {
        OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
        Destroy();
    }
}

