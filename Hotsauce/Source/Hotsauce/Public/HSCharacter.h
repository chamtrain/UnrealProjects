#pragma once
#include "GameFramework/Character.h"
#include "HSCharacter.generated.h"

UCLASS(Blueprintable)
class AHSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
    void Tick(float DeltaTime);

    virtual void BeginPlay() override;
	
    AHSCharacter(const FObjectInitializer& ObjectInitializer);

    int32 MaxLevel;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Stats)
    float RegenerationMultiplayer;

    UPROPERTY(BlueprintReadOnly, Category = Stats)
    TArray<int32> ExperiencePerLevelArray;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Stats)
    float Health;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
    float HealthMax;
    UFUNCTION(BlueprintCallable, Category = Stats)
    void AddHealth(float DeltaHealth, AActor* AddInstigator);
    UFUNCTION(BlueprintImplementableEvent, Category = "Stats Update Event")
    void EventHealthUpdate();

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Stats)
    float Mana;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
    float ManaMax;
    UFUNCTION(BlueprintCallable, Category = Stats)
    void AddMana(float DeltaMana, AActor* AddInstigator);
    UFUNCTION(BlueprintCallable, Category = Fire)
    bool CanCast(float Cost);
    UFUNCTION(BlueprintImplementableEvent, Category = "Stats Update Event")
    void EventManaUpdate();

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Stats)
    float Stamina;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
    float StaminaMax;
    UFUNCTION(BlueprintCallable, Category = Stats)
    void AddStamina(float DeltaStamina, AActor* AddInstigator);
    UFUNCTION(BlueprintImplementableEvent, Category = "Stats Update Event")
    void EventStaminaUpdate();

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Stats)
    float Strength;
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Stats)
    float Intelligence;
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Stats)
    float Agility;


    UFUNCTION(BlueprintCallable, Category = Stats)
    void GainExperience(int32 GainedExperience);
    void GainExperience();
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
    float SecondaryExperienceModifyer;
    void SecondaryExperience(int32 GainedExperience);
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Stats)
    int32 Experience;
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Stats)
    int32 Level;
    UFUNCTION(BlueprintImplementableEvent, Category = "Stats Update Event")
    void EventExperienceUpdate();
    UFUNCTION(BlueprintImplementableEvent, Category = "Stats Update Event")
    void EventGainLevelUpdate();

    bool IsAlive();
    UFUNCTION(BlueprintImplementableEvent, Category = "Death Event")
    void CharacterDeath(AActor* Killer);

    virtual float TakeDamage(float DamageAmount, const FDamageEvent &DamageEvent, AController* EventInstigator, AActor* DamageCauser);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    FVector MuzzleOffset;
    UPROPERTY(EditDefaultsOnly, Category = Projectile)
    TSubclassOf<class AHSProjectile> ProjectileClass;
    UFUNCTION(BlueprintCallable, Category = Fire)
    void OnFire();
};

