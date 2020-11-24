// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "ModularPower/RPMPBuilding.h"
#include "FGPipeConnectionComponent.h"
#include "RPMPTurbineBuilding.generated.h"

UCLASS()
class REFINEDPOWER_API URPMPTurbineBuildingRCO : public UFGRemoteCallObject
{
    GENERATED_BODY()

public:
    UFUNCTION(Server, WithValidation, Unreliable)
    void SetSteamDiscard(ARPMPTurbineBuilding* panel, float value);

    UPROPERTY(Replicated)
    bool bTest = true;
};

/**
 * 
 */
UCLASS()
class REFINEDPOWER_API ARPMPTurbineBuilding : public ARPMPBuilding
{
    GENERATED_BODY()

public:
    ARPMPTurbineBuilding();

    virtual void BeginPlay() override;
    virtual void Factory_Tick(float dt) override;
    virtual void Tick(float dt) override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // Functions
    void CacheConnections();
    void CollectSteam(float dt);

    void OutputSteam(float dt);

    void CalcTurbineState();

    bool CanStartSteamConsumption();
    void StartSteamConsumption();

    void ConvertSteamToRPM();

    void ReduceRPM();

    //TL044CN: FIN Integration Patch 0.0.3
    // Added Running State Checking as override for the Base Class's function
    bool CehckMPBuildingRunningState() override;


    //RCO Functions
    UFUNCTION(BlueprintCallable, Category = "RefinedPower|ModularPower|Turbine")
    void SetSteamDiscard(float value);


    // Variables

    // RPM Stuff
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Replicated, Category = "RefinedPower")
    int mCurrentTurbineRPM = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    int mYellowMinTurbineRPM = 15000;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    int mYellowMaxTurbineRPM = 19999;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    int mRedMinTurbineRPM = 20000;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    int mRedMaxTurbineRPM = 25000;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    int mRPMDrag = 20;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
    int mRPMRate = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    float mRPMMultiplier = 2;

    //TL044CN: FIN Integration Patch 0.0.3
    // Im not sure how this works but this probably adds m_Standby as a variable to mess with maybe even ingame?
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
    bool m_Standby = false;

    // Steam Stuff

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    int mSteamPullAmount = 70;

    UPROPERTY(BlueprintReadOnly, SaveGame, Replicated, meta = (UIMin = "0", UIMax = "1.0"), Category =
        "RefinedPower")
    float mSteamDiscardPercent = 0.0f;

    // For BP Animations

    bool TriggerTurbineStateUpdated = false;

    UFUNCTION(NetMulticast, Unreliable)
    void Multicast_TurbineStateUpdated();

    UFUNCTION(BlueprintImplementableEvent, Category = "RefinedPower|ModularPower")
    void OnRep_TurbineStateUpdated();

    // FIN Intergration
    UFUNCTION()
    int netFunc_getCurrentRPM();

    UFUNCTION()
    float netFunc_getSteamDiscardPercent();

    //TL044CN: FIN Integration Patch 0.0.2
    UFUNCTION()
    void netFunc_setSteamDiscardPercent(float);

    // Fluid Buffer

    virtual bool CanTransferToFluidBuffer() override;
    virtual void TransferToFluidBuffer() override;

    // Components

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    TSubclassOf<UFGItemDescriptor> mHighSteamItemClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    TSubclassOf<UFGItemDescriptor> mLowSteamItemClass;

    UFGPipeConnectionComponent* InputSteamPipe;
    UFGPipeConnectionComponent* OutputSteamPipe;

    int mInputInvIndex = 0;
    int mOutputInvIndex = 1;

    //TL044CN: FIN Integration Patch 0.0.3
    //Wether the Turbine is running or not. Depends on m_Standby (which probably does nothing as of now)
    bool m_running = false;
};
