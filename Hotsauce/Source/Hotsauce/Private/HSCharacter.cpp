#include "Hotsauce.h"
#include "HSCharacter.h"
#include "HSProjectile.h"
#include "UnrealNetwork.h"
#include "Engine.h"


AHSCharacter::AHSCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    MaxLevel = 20;
    RegenerationMultiplayer = 0.0005f;

    Health = 100.0f;
    HealthMax = 100.0f;
    Mana = 100.f;
    ManaMax = 100.f;
    Stamina = 100.f;
    StaminaMax = 100.f;
    Strength = 10.f;
    Intelligence = 10.f;
    Agility = 10.f;
    SecondaryExperienceModifyer = .8f;
    Experience = 0;
    Level = 1;

    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
    GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Block);
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

    PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // this must be false for Players, watch out in blueprints
	GetCharacterMovement()->RotationRate = FRotator(0.f, 600.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
    GetCharacterMovement()->GravityScale = 1.0f;   

    for (int32 level = 0; level <= MaxLevel; level++)
    {
        int32 PreviousLevel = 100 * level;
        int32 ThisLevel = 100 * (level + 1);
        ExperiencePerLevelArray.Add(PreviousLevel + ThisLevel);
    }
}

void AHSCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void AHSCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    AddHealth(RegenerationMultiplayer*HealthMax, this);
    AddMana(RegenerationMultiplayer*ManaMax, this);
    AddStamina(RegenerationMultiplayer*StaminaMax, this);
}

void AHSCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    
    DOREPLIFETIME(AHSCharacter, RegenerationMultiplayer);
    DOREPLIFETIME(AHSCharacter, Health);
    DOREPLIFETIME(AHSCharacter, Mana);
    DOREPLIFETIME(AHSCharacter, Stamina);
    DOREPLIFETIME(AHSCharacter, Strength);
    DOREPLIFETIME(AHSCharacter, Intelligence);
    DOREPLIFETIME(AHSCharacter, Agility);
    DOREPLIFETIME(AHSCharacter, Experience);
    DOREPLIFETIME(AHSCharacter, Level);
}

void AHSCharacter::GainExperience(int32 GainedExperience)
{
    Experience += GainedExperience;
    GainExperience();
    for (TActorIterator<AHSCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
    {
        AHSCharacter* Character = (*ActorItr);
        Character->SecondaryExperience(GainedExperience);
    }
    EventExperienceUpdate();
}

void AHSCharacter::GainExperience()
{
    if (Level >= MaxLevel)
    {
        return;
    }
    if (Experience > ExperiencePerLevelArray[Level - 1])
    {
        Level++;
        EventGainLevelUpdate();
        GetWorldTimerManager().SetTimer(this, &AHSCharacter::GainExperience, 1.0f, false);
    }
}

void AHSCharacter::SecondaryExperience(int32 GainedExperience)
{
    Experience += GainedExperience*SecondaryExperienceModifyer;
    GainExperience();
    EventExperienceUpdate();
}

bool AHSCharacter::IsAlive()
{
    if (Health > 0)
    {
        return true;
    }
    return false;
}

float AHSCharacter::TakeDamage(float DamageAmount, const FDamageEvent &DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float Result = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    AddHealth(-DamageAmount, DamageCauser);
    return Result;
}

void AHSCharacter::OnFire()
{
    if (Mana == 0.f) {
        return;
    }
    if (ProjectileClass != NULL)
    {
        FRotator MuzzleRotation = GetActorRotation();
        FVector const MuzzleLocation = GetActorLocation() + FTransform(MuzzleRotation).TransformVector(MuzzleOffset);
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
            if (Projectile && CanCast(Projectile->ManaCost))
            {
                // find launch direction
                FVector const LaunchDir = MuzzleRotation.Vector();
                Projectile->Start(LaunchDir);
            }
        }
    }
}

bool AHSCharacter::CanCast(float Cost)
{
    float Temp = Mana - Cost;
    if (Temp > 0.f) {
        return true;
    }
    return false;
}


void AHSCharacter::AddHealth(float DeltaHealth, AActor* AddInstigator)
{
    if (DeltaHealth == 0.f) {
        return;
    }
    Health = Health + DeltaHealth;

    if (Health < 0)
    {
        Health = 0.f;
    }
    else if (Health > HealthMax)
    {
        Health = HealthMax;
    }
    if (!IsAlive())
    {
        CharacterDeath(AddInstigator);
    }
    EventHealthUpdate();
}

void AHSCharacter::AddMana(float DeltaMana, AActor* AddInstigator)
{
    if (DeltaMana == 0.f) {
        return;
    }
    Mana = Mana + DeltaMana;

    if (Mana < 0)
    {
        Mana = 0.f;
    }
    else if (Mana > ManaMax)
    {
        Mana = ManaMax;
    }
    EventManaUpdate();
}

void AHSCharacter::AddStamina(float DeltaStamina, AActor* AddInstigator)
{
    if (DeltaStamina == 0.f) {
        return;
    }
    Stamina = Stamina + DeltaStamina;

    if (Stamina < 0)
    {
        Stamina = 0.f;
    }
    else if (Stamina > HealthMax)
    {
        Stamina = HealthMax;
    }
    EventStaminaUpdate();
}
