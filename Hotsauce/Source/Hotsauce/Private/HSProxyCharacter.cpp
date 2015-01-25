// Fill out your copyright notice in the Description page of Project Settings.

#include "Hotsauce.h"
#include "HSProxyCharacter.h"
#include "AIController.h"
#include "UnrealNetwork.h"


AHSProxyCharacter::AHSProxyCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    CameraMaxHeight = 3000;
    CameraMinHeight = 1000;
    CameraTargetHeight = 1000;
    CameraTargetOffset = FVector(0,0,0);

    GetCapsuleComponent()->InitCapsuleSize(1.f, 1.0f);
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

    PrimaryActorTick.bCanEverTick = true;

    // Don't rotate character to camera direction
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
    GetCharacterMovement()->RotationRate = FRotator(0.f, 600.f, 0.f);
    GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->bSnapToPlaneAtStart = true;
    GetCharacterMovement()->GravityScale = 0.0f;

    // Create a camera boom...
    CameraRootBoom = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraRootBoom"));
    CameraRootBoom->AttachTo(RootComponent);
    CameraRootBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
    CameraRootBoom->TargetArmLength = CameraMinHeight;
    CameraRootBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f); // 0,-60,0
    CameraRootBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

    // Create a camera boom...
    CameraOffsetBoom = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraOffsetBoom"));
    CameraOffsetBoom->AttachTo(CameraRootBoom);
    CameraOffsetBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
    CameraOffsetBoom->TargetArmLength = 0.f;
    CameraOffsetBoom->RelativeRotation = FRotator(0.f, 0.f, 0.f);
    CameraOffsetBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

    // Create a camera...
    TopDownCameraComponent = ObjectInitializer.CreateDefaultSubobject<UHSCameraComponent>(this, TEXT("TopDownCamera"));
    TopDownCameraComponent->AttachTo(CameraOffsetBoom, USpringArmComponent::SocketName);
    TopDownCameraComponent->bAbsoluteRotation = true;
    TopDownCameraComponent->RelativeRotation = FRotator(-60.0f, 0.0f, 0.0f);
    TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

    if (Role == ROLE_Authority)
    {
        static ConstructorHelpers::FClassFinder<AActor> PlayerPawnBPClass(TEXT("/Game/Blueprints/MyCharacter"));
        CharacterClass = PlayerPawnBPClass.Class;
    }
}

void AHSProxyCharacter::BeginPlay()
{
    Super::BeginPlay();
    UWorld* const World = GetWorld();
    if (Role == ROLE_Authority && World)
    {
        // Get current location of the Player Proxy
        FVector Location = GetActorLocation();
        FRotator Rotation = GetActorRotation();

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = Instigator;
        SpawnParams.bNoCollisionFail = true;

        // Spawn the actual player character at the same location as the Proxy
        Character = World->SpawnActor<AHSPlayerCharacter>(CharacterClass, Location, Rotation, SpawnParams);

        // We use the PlayerAI to control the Player Character for Navigation
        PlayerAI = World->SpawnActor<AAIController>(Location, Rotation);
        PlayerAI->Possess(Character);
    }
}

void AHSProxyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (Character)
    {
        float Mult = 0.05f;
        FVector DifferenceVector = Character->GetActorLocation() - GetActorLocation();
        
        if (DifferenceVector.Size() > 10.f)
        {
            DifferenceVector = DifferenceVector*Mult;
        }
        else
        {
            DifferenceVector = FVector(0, 0, 0);
        }
        SetActorLocation(GetActorLocation() + DifferenceVector);
    }
    MoveCameraHeightRate(DeltaTime);
    MoveCameraOffsetRate(DeltaTime);
}

void AHSProxyCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AHSProxyCharacter, Character);
}

void AHSProxyCharacter::MoveToLocation(const AHSPlayerController* controller, const FVector DestLocation)
{
    PlayerAI->MoveToLocation(DestLocation);
}

void AHSProxyCharacter::MoveCameraHeightRate(float Rate)
{
    float Mult = 0.02f;
    float DistanceToTarget = CameraTargetHeight - CameraRootBoom->TargetArmLength;
    if (abs(DistanceToTarget) > 10.f)
    {
        CameraRootBoom->TargetArmLength += DistanceToTarget*Mult;
    }
}

void AHSProxyCharacter::MoveCameraOffsetRate(float Rate)
{
    float Mult = 0.02f;
    FVector DifferenceVector = FRotator(60.0f, 0.0f, 0.0f).RotateVector(CameraTargetOffset) - CameraOffsetBoom->GetRelativeTransform().GetLocation();
    if (abs(DifferenceVector.Size()) > 10.f)
    {
        DifferenceVector = DifferenceVector*Mult;
        CameraOffsetBoom->AddLocalOffset(DifferenceVector);
    }
}

void AHSProxyCharacter::SetCamreaHeight(float Height)
{
    if (CameraMaxHeight < Height)
    {
        CameraTargetHeight = CameraMaxHeight;
    }
    else if (CameraMinHeight > Height)
    {
        CameraTargetHeight = CameraMinHeight;
    }
    else
    {
        CameraTargetHeight = Height;
    }
}

void AHSProxyCharacter::SetCamreaOffset(FVector Offset)
{
    CameraTargetOffset = Offset;
}
