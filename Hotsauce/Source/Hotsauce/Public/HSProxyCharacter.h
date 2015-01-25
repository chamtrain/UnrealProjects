#pragma once
#include "GameFramework/Character.h"
#include "HSPlayerCharacter.h"
#include "HSPlayerController.h"
#include "AIController.h"
#include "HSProxyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class HOTSAUCE_API AHSProxyCharacter : public ACharacter
{
	GENERATED_BODY()
public:	
    void Tick(float DeltaTime);
    
    // We spawn the real player character and other such elements here 
    virtual void BeginPlay() override;

    AHSProxyCharacter(const FObjectInitializer& ObjectInitializer);

    /** Top down camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UHSCameraComponent* TopDownCameraComponent;

    /** Camera boom positioning the camera above the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraRootBoom;

    /** Camera boom positioning the camera above the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraOffsetBoom;

    // Needed so we can pick up the class in the constructor and spawn it elsewhere 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
    TSubclassOf<class AActor> CharacterClass;

    // Pointer to the actual character. We replicate it so we know its location for the camera on the client 
    UPROPERTY(Replicated)
    AHSPlayerCharacter* Character;

    // The AI Controller we will use to auto-navigate the player 
    AAIController* PlayerAI;
    
    UPROPERTY(VisibleAnywhere, Category = Camera)
    float CameraMaxHeight;
    UPROPERTY(VisibleAnywhere, Category = Camera)
    float CameraMinHeight;
    UPROPERTY(VisibleAnywhere, Category = Camera)
    float CameraTargetHeight;
    UPROPERTY(VisibleAnywhere, Category = Camera)
    FVector CameraTargetOffset;

    // Used by the controller to get moving to work 
    void MoveToLocation(const AHSPlayerController* controller, const FVector vector);

    void MoveCameraHeightRate(float Rate);
    void MoveCameraOffsetRate(float Rate);

    void SetCamreaHeight(float Height);
    void SetCamreaOffset(FVector Offset);
};
