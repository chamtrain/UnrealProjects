#pragma once

#include "HSCharacter.h"
#include "HSProjectile.h"
#include "HSPlayerCharacter.generated.h"


UCLASS()
class HOTSAUCE_API AHSPlayerCharacter : public AHSCharacter
{
    GENERATED_BODY()

public:

    void Tick(float DeltaTime);

    virtual void BeginPlay() override;

    AHSPlayerCharacter(const FObjectInitializer& ObjectInitializer);

    /** Top down camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UHSCameraComponent* TopDownCameraComponent;

    /** Camera boom positioning the camera above the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

    // Needed so we can pick up the class in the constructor and spawn it elsewhere 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
    TSubclassOf<class AActor> CharacterClass;

    UPROPERTY(VisibleAnywhere, Category = Camera)
    float CameraMaxHeight;
    UPROPERTY(VisibleAnywhere, Category = Camera)
    float CameraMinHeight;
    UPROPERTY(VisibleAnywhere, Category = Camera)
    float CameraTargetHeight;
    UPROPERTY(VisibleAnywhere, Category = Camera)
    FVector CameraTargetOffset;
	
    /** Gun muzzle's offset from the camera location */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    FVector MuzzleOffset;

    /** Projectile class to spawn */
    UPROPERTY(EditDefaultsOnly, Category = Projectile)
    TSubclassOf<class AHSProjectile> ProjectileClass;

    UFUNCTION(BlueprintCallable, Category = Fire)
    void OnFire();

    UFUNCTION(BlueprintCallable, Category = Fire)
    bool CanCast(AHSProjectile* Projectile);

    UFUNCTION(BlueprintImplementableEvent, Category = Events)
    void EventSelectTarget();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
    float CameraRotationSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
    FRotator CameraRotationAngle;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Character)
    FRotator CharacterDirection;

    UFUNCTION(BlueprintCallable, Reliable, Server, WithValidation, Category=Rotations)
    void HSRotateCharacter(float Angle);
};
