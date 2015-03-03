#include "Hotsauce.h"
#include "HSPlayerCharacter.h"
#include "UnrealNetwork.h"
#include "Engine.h"

AHSPlayerCharacter::AHSPlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    CameraMaxHeight = 3000;
    CameraMinHeight = 1000;
    CameraTargetHeight = 1000;
    CameraTargetOffset = FVector(0, 0, 0);
    MuzzleOffset = FVector(120.0f, 0.0f, 0.0f);

    CameraRotationSpeed = 0.5f;
    CameraRotationAngle = FRotator();

    CharacterDirection = FRotator(0.f, 0.f, 0.f);

    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
    //GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    //GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

    PrimaryActorTick.bCanEverTick = true;

    // Don't rotate character to camera direction
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = false;
    //GetCharacterMovement()->RotationRate = FRotator(0.f, 600.f, 0.f);
    GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->bSnapToPlaneAtStart = true;
    GetCharacterMovement()->GravityScale = 1.0f;

    // Create a camera boom...
    CameraBoom = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraRootBoom"));
    CameraBoom->AttachTo(RootComponent);
    CameraBoom->bAbsoluteRotation = true;
    CameraBoom->TargetArmLength = CameraMinHeight;
    CameraBoom->RelativeRotation = FRotator(-60.f,0.f,0.f);
    CameraBoom->bDoCollisionTest = false;

    // Create a camera...
    TopDownCameraComponent = ObjectInitializer.CreateDefaultSubobject<UHSCameraComponent>(this, TEXT("TopDownCamera"));
    TopDownCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
    TopDownCameraComponent->bAbsoluteRotation = true;
    TopDownCameraComponent->RelativeRotation = FRotator();
    TopDownCameraComponent->bUsePawnControlRotation = false;

    if (Role == ROLE_Authority)
    {
        static ConstructorHelpers::FClassFinder<AActor> PlayerPawnBPClass(TEXT("/Game/Blueprints/MyCharacter"));
        CharacterClass = PlayerPawnBPClass.Class;
    }
}

void AHSPlayerCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AHSPlayerCharacter, CharacterDirection);
}

void AHSPlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    //if (CameraBoom->RelativeRotation.Yaw != CameraRotation.Yaw) {
    FRotator current = FMath::RInterpTo(CameraBoom->RelativeRotation, CameraRotationAngle, DeltaTime, CameraRotationSpeed);
    CameraBoom->RelativeRotation.Yaw = current.Yaw;
    //CharacterDirection.Yaw += 0.2f;

    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, CharacterDirection.ToString());
    SetActorRotation(CharacterDirection);
    //    float diff = FMath::Abs(FMath::Abs(CameraBoom->RelativeRotation.Yaw) - FMath::Abs(CameraRotation.Yaw));
    //    if (diff <= 0.2f)
    //    {
    //        CameraBoom->RelativeRotation.Yaw = CameraRotation.Yaw;
    //    }
    //}
}

void AHSPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void AHSPlayerCharacter::OnFire()
{
    if (ProjectileClass != NULL)
    {
        FVector const MuzzleLocation = GetActorLocation() + FTransform(GetActorRotation()).TransformVector(MuzzleOffset);
        FRotator MuzzleRotation = GetActorRotation();
        MuzzleRotation.Pitch += 10.0f;
        
        //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, MuzzleRotation.ToString());
        UWorld* const World = GetWorld();
        if (World)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = Instigator;

            // spawn the projectile at the muzzle
            AHSProjectile* Projectile = World->SpawnActor<AHSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
            if (Projectile && CanCast(Projectile))
            {
                // find launch direction
                FVector const LaunchDir = MuzzleRotation.Vector();
                Projectile->Start(LaunchDir);
            }
        }
    }
}

bool AHSPlayerCharacter::CanCast(AHSProjectile* Projectile)
{
    float Temp = Mana - Projectile->ManaCost;
    if (Temp > 0.f) {
        Mana = Temp;
        return true;
    }
    else {
        Mana = 0;
    }
    return false;
}

bool AHSPlayerCharacter::HSRotateCharacter_Validate(float Angle)
{
    return true;
}

void AHSPlayerCharacter::HSRotateCharacter_Implementation(float Angle)
{
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(Angle));
    CharacterDirection.Yaw = Angle;
}