// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "Hotsauce.h"
#include "HSProjectile.h"
#include "HSCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"

AHSProjectile::AHSProjectile(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    ManaCost = 1.f;
    HitDammage = 5.f;
	// Use a sphere as a simple collision representation
	CollisionComp = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
    CollisionComp->OnComponentHit.AddDynamic(this, &AHSProjectile::OnHit);		// set up a notification for when this component hits something blocking
    RootComponent = CollisionComp;
	
    // Players can't walk on it
	//CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	//CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 1500.f;
	ProjectileMovement->MaxSpeed = 1500.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AHSProjectile::OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Hit Something!");
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
        if (OtherComp->IsSimulatingPhysics())
        {
            OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
        }
        AHSCharacter* Character = (AHSCharacter*)OtherActor;
        if (Character)
        {
            /*
            if (OtherActor == GetOwner())
            {
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Hit Self!");
            }
            else
            {
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Hit a Character!");
            }
            */
            
            FDamageEvent damageEvent;
            OtherActor->TakeDamage(HitDammage, damageEvent, GetInstigatorController(), GetOwner());
            EventHit();
            Destroy();
        }
	}
}

void AHSProjectile::Start(FVector Direction) {
    if (ProjectileMovement)
    {
        ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;
    }
    EventStart();
}