#pragma once
#include <globals.h>
#include "utils\mem.h"

// Messages are intiliazed with the default values from behaviours.xml
// In this case, Euphoria Ragdoll Overhaul v1.9.4 was used
#pragma region Messages
struct NMMessageActivePose
{
    std::string mask = "fb";
    bool useGravityCompensation = false;
    int animSource = 0; // ART::kITSourceCurrent
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "mask", mask.c_str());
        nGame::SetNMMessageParam(msgPtr, "useGravityCompensation", useGravityCompensation);
        nGame::SetNMMessageParam(msgPtr, "animSource", animSource);
        return;
    }
};

struct NMMessageApplyImpulse
{
    float equalizeAmount = 0.0f;
    int partIndex = 0;
    Vector3 impulse{ 0.0f, 0.0f, 0.0f };
    Vector3 hitPoint{ 0.0f, 0.0f, 0.0f };
    bool localHitPointInfo = false;
    bool localImpulseInfo = false;
    bool angularImpulse = false;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "equalizeAmount", equalizeAmount);
        nGame::SetNMMessageParam(msgPtr, "partIndex", partIndex);
        nGame::SetNMMessageParam(msgPtr, "impulse", impulse.x, impulse.y, impulse.z);
        nGame::SetNMMessageParam(msgPtr, "hitPoint", hitPoint.x, hitPoint.y, hitPoint.z);
        nGame::SetNMMessageParam(msgPtr, "localHitPointInfo", localHitPointInfo);
        nGame::SetNMMessageParam(msgPtr, "localImpulseInfo", localImpulseInfo);
        nGame::SetNMMessageParam(msgPtr, "angularImpulse", angularImpulse);
        return;
    }
};

struct NMMessageApplyBulletImpulse
{
    float equalizeAmount = 0.0f;
    int partIndex = 0;
    Vector3 impulse{ 0.0f, 0.0f, 0.0f };
    Vector3 hitPoint{ 0.0f, 0.0f, 0.0f };
    bool localHitPointInfo = false;
    float extraShare = 0.0f;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "equalizeAmount", equalizeAmount);
        nGame::SetNMMessageParam(msgPtr, "partIndex", partIndex);
        nGame::SetNMMessageParam(msgPtr, "impulse", impulse.x, impulse.y, impulse.z);
        nGame::SetNMMessageParam(msgPtr, "hitPoint", hitPoint.x, hitPoint.y, hitPoint.z);
        nGame::SetNMMessageParam(msgPtr, "localHitPointInfo", localHitPointInfo);
        nGame::SetNMMessageParam(msgPtr, "extraShare", extraShare);
        return;
    }
};

struct NMMessageBodyRelax
{
    float relaxation = 50.0f;
    float damping = 1.0f;
    std::string mask = "fb";
    bool holdPose = false;
    bool disableJointDriving = false;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "relaxation", relaxation);
        nGame::SetNMMessageParam(msgPtr, "damping", damping);
        nGame::SetNMMessageParam(msgPtr, "mask", mask.c_str());
        nGame::SetNMMessageParam(msgPtr, "holdPose", holdPose);
        nGame::SetNMMessageParam(msgPtr, "disableJointDriving", disableJointDriving);
        return;
    }
};

struct NMMessageConfigureBalance
{
    float stepHeight = 0.1f;
    float stepHeightInc4Step = 0.1f;
    float legsApartRestep = 0.2f;
    float legsTogetherRestep = 0.1f;
    float legsApartMax = 2.0f;
    bool taperKneeStrength = true;
    float legStiffness = 6.0f;
    float leftLegSwingDamping = 1.0f;
    float rightLegSwingDamping = 1.0f;
    float opposeGravityLegs = 1.0f;
    float opposeGravityAnkles = 1.0f;
    float leanAcc = 0.0f;
    float hipLeanAcc = 0.5f;
    float leanAccMax = 5.0f;
    float resistAcc = 0.5f;
    float resistAccMax = 3.0f;
    bool footSlipCompOnMovingFloor = true;
    float ankleEquilibrium = 0.0f;
    float extraFeetApart = 0.0f;
    float dontStepTime = 0.0f;
    float balanceAbortThreshold = 1.0f;
    float giveUpHeight = 1.0f;
    float stepClampScale = 1.0f;
    float stepClampScaleVariance = 0.0f;
    float predictionTimeHip = 0.3f;
    float predictionTime = 0.2f;
    float predictionTimeVariance = 0.0f;
    int maxSteps = 100;
    float maxBalanceTime = 50.0f;
    int extraSteps = -1;
    float extraTime = -1.0f;
    int fallType = 2;
    float fallMult = 40.0f;
    bool fallReduceGravityComp = false;
    bool rampHipPitchOnFail = false;
    float stableLinSpeedThresh = 3.750f;
    float stableRotSpeedThresh = 3.250f;
    bool failMustCollide = true;
    bool ignoreFailure = true;
    float changeStepTime = -1.0f;
    bool balanceIndefinitely = true;
    bool movingFloor = false;
    bool airborneStep = true;
    float useComDirTurnVelThresh = 0.0f;
    float minKneeAngle = -0.5f;
    bool flatterSwingFeet = false;
    bool flatterStaticFeet = false;
    bool avoidLeg = false;
    float avoidFootWidth = 0.1f;
    float avoidFeedback = 0.6f;
    float leanAgainstVelocity = 0.0f;
    float stepDecisionThreshold = 0.0f;
    bool stepIfInSupport = true;
    bool alwaysStepWithFarthest = false;
    bool standUp = false;
    float depthFudge = 0.010f;
    float depthFudgeStagger = 0.010f;
    float footFriction = 1.0f;
    float footFrictionStagger = 1.0f;
    float backwardsLeanCutoff = 0.9f;
    float giveUpHeightEnd = 0.5f;
    float balanceAbortThresholdEnd = 0.6f;
    float giveUpRampDuration = -1.0f;
    float leanToAbort = 0.6f;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "stepHeight", stepHeight);
        nGame::SetNMMessageParam(msgPtr, "stepHeightInc4Step", stepHeightInc4Step);
        nGame::SetNMMessageParam(msgPtr, "legsApartRestep", legsApartRestep);
        nGame::SetNMMessageParam(msgPtr, "legsTogetherRestep", legsTogetherRestep);
        nGame::SetNMMessageParam(msgPtr, "legsApartMax", legsApartMax);
        nGame::SetNMMessageParam(msgPtr, "taperKneeStrength", taperKneeStrength);
        nGame::SetNMMessageParam(msgPtr, "legStiffness", legStiffness);
        nGame::SetNMMessageParam(msgPtr, "leftLegSwingDamping", leftLegSwingDamping);
        nGame::SetNMMessageParam(msgPtr, "rightLegSwingDamping", rightLegSwingDamping);
        nGame::SetNMMessageParam(msgPtr, "opposeGravityLegs", opposeGravityLegs);
        nGame::SetNMMessageParam(msgPtr, "opposeGravityAnkles", opposeGravityAnkles);
        nGame::SetNMMessageParam(msgPtr, "leanAcc", leanAcc);
        nGame::SetNMMessageParam(msgPtr, "hipLeanAcc", hipLeanAcc);
        nGame::SetNMMessageParam(msgPtr, "leanAccMax", leanAccMax);
        nGame::SetNMMessageParam(msgPtr, "resistAcc", resistAcc);
        nGame::SetNMMessageParam(msgPtr, "resistAccMax", resistAccMax);
        nGame::SetNMMessageParam(msgPtr, "footSlipCompOnMovingFloor", footSlipCompOnMovingFloor);
        nGame::SetNMMessageParam(msgPtr, "ankleEquilibrium", ankleEquilibrium);
        nGame::SetNMMessageParam(msgPtr, "extraFeetApart", extraFeetApart);
        nGame::SetNMMessageParam(msgPtr, "dontStepTime", dontStepTime);
        nGame::SetNMMessageParam(msgPtr, "balanceAbortThreshold", balanceAbortThreshold);
        nGame::SetNMMessageParam(msgPtr, "giveUpHeight", giveUpHeight);
        nGame::SetNMMessageParam(msgPtr, "stepClampScale", stepClampScale);
        nGame::SetNMMessageParam(msgPtr, "stepClampScaleVariance", stepClampScaleVariance);
        nGame::SetNMMessageParam(msgPtr, "predictionTimeHip", predictionTimeHip);
        nGame::SetNMMessageParam(msgPtr, "predictionTime", predictionTime);
        nGame::SetNMMessageParam(msgPtr, "predictionTimeVariance", predictionTimeVariance);
        nGame::SetNMMessageParam(msgPtr, "maxSteps", maxSteps);
        nGame::SetNMMessageParam(msgPtr, "maxBalanceTime", maxBalanceTime);
        nGame::SetNMMessageParam(msgPtr, "extraSteps", extraSteps);
        nGame::SetNMMessageParam(msgPtr, "extraTime", extraTime);
        nGame::SetNMMessageParam(msgPtr, "fallType", fallType);
        nGame::SetNMMessageParam(msgPtr, "fallMult", fallMult);
        nGame::SetNMMessageParam(msgPtr, "fallReduceGravityComp", fallReduceGravityComp);
        nGame::SetNMMessageParam(msgPtr, "rampHipPitchOnFail", rampHipPitchOnFail);
        nGame::SetNMMessageParam(msgPtr, "stableLinSpeedThresh", stableLinSpeedThresh);
        nGame::SetNMMessageParam(msgPtr, "stableRotSpeedThresh", stableRotSpeedThresh);
        nGame::SetNMMessageParam(msgPtr, "failMustCollide", failMustCollide);
        nGame::SetNMMessageParam(msgPtr, "ignoreFailure", ignoreFailure);
        nGame::SetNMMessageParam(msgPtr, "changeStepTime", changeStepTime);
        nGame::SetNMMessageParam(msgPtr, "balanceIndefinitely", balanceIndefinitely);
        nGame::SetNMMessageParam(msgPtr, "movingFloor", movingFloor);
        nGame::SetNMMessageParam(msgPtr, "airborneStep", airborneStep);
        nGame::SetNMMessageParam(msgPtr, "useComDirTurnVelThresh", useComDirTurnVelThresh);
        nGame::SetNMMessageParam(msgPtr, "minKneeAngle", minKneeAngle);
        nGame::SetNMMessageParam(msgPtr, "flatterSwingFeet", flatterSwingFeet);
        nGame::SetNMMessageParam(msgPtr, "flatterStaticFeet", flatterStaticFeet);
        nGame::SetNMMessageParam(msgPtr, "avoidLeg", avoidLeg);
        nGame::SetNMMessageParam(msgPtr, "avoidFootWidth", avoidFootWidth);
        nGame::SetNMMessageParam(msgPtr, "avoidFeedback", avoidFeedback);
        nGame::SetNMMessageParam(msgPtr, "leanAgainstVelocity", leanAgainstVelocity);
        nGame::SetNMMessageParam(msgPtr, "stepDecisionThreshold", stepDecisionThreshold);
        nGame::SetNMMessageParam(msgPtr, "stepIfInSupport", stepIfInSupport);
        nGame::SetNMMessageParam(msgPtr, "alwaysStepWithFarthest", alwaysStepWithFarthest);
        nGame::SetNMMessageParam(msgPtr, "standUp", standUp);
        nGame::SetNMMessageParam(msgPtr, "depthFudge", depthFudge);
        nGame::SetNMMessageParam(msgPtr, "depthFudgeStagger", depthFudgeStagger);
        nGame::SetNMMessageParam(msgPtr, "footFriction", footFriction);
        nGame::SetNMMessageParam(msgPtr, "footFrictionStagger", footFrictionStagger);
        nGame::SetNMMessageParam(msgPtr, "backwardsLeanCutoff", backwardsLeanCutoff);
        nGame::SetNMMessageParam(msgPtr, "giveUpHeightEnd", giveUpHeightEnd);
        nGame::SetNMMessageParam(msgPtr, "balanceAbortThresholdEnd", balanceAbortThresholdEnd);
        nGame::SetNMMessageParam(msgPtr, "giveUpRampDuration", giveUpRampDuration);
        nGame::SetNMMessageParam(msgPtr, "leanToAbort", leanToAbort);
        return;
    }
};

struct NMMessageConfigureSelfAvoidance
{
    bool useSelfAvoidance = true;
    bool overwriteDragReduction = false;
    float torsoSwingFraction = 0.750f;
    float maxTorsoSwingAngleRad = 0.7580f;
    bool selfAvoidIfInSpineBoundsOnly = false;
    float selfAvoidAmount = 0.5f;
    bool overwriteTwist = false;
    bool usePolarPathAlgorithm = false;
    float radius = 0.3f;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "useSelfAvoidance", useSelfAvoidance);
        nGame::SetNMMessageParam(msgPtr, "overwriteDragReduction", overwriteDragReduction);
        nGame::SetNMMessageParam(msgPtr, "torsoSwingFraction", torsoSwingFraction);
        nGame::SetNMMessageParam(msgPtr, "maxTorsoSwingAngleRad", maxTorsoSwingAngleRad);
        nGame::SetNMMessageParam(msgPtr, "selfAvoidIfInSpineBoundsOnly", selfAvoidIfInSpineBoundsOnly);
        nGame::SetNMMessageParam(msgPtr, "selfAvoidAmount", selfAvoidAmount);
        nGame::SetNMMessageParam(msgPtr, "overwriteTwist", overwriteTwist);
        nGame::SetNMMessageParam(msgPtr, "usePolarPathAlgorithm", usePolarPathAlgorithm);
        nGame::SetNMMessageParam(msgPtr, "radius", radius);
        return;
    }
};

struct NMMessageConfigureBullets
{
    bool impulseSpreadOverParts = false;
    bool impulseLeakageStrengthScaled = false;
    float impulsePeriod = 0.1f;
    float impulseTorqueScale = 1.0f;
    bool loosenessFix = true;
    float impulseDelay = 0.0f;
    float impulseReductionPerShot = 0.0f;
    float impulseRecovery = 0.0f;
    float impulseMinLeakage = 0.2f;
    int torqueMode = 2;
    int torqueSpinMode = 0;
    int torqueFilterMode = 0;
    bool torqueAlwaysSpine3 = true;
    float torqueDelay = 0.0f;
    float torquePeriod = 0.120f;
    float torqueGain = 4.0f;
    float torqueCutoff = 0.0f;
    float torqueReductionPerTick = 0.0f;
    float liftGain = 0.0f;
    float counterImpulseDelay = 0.033330f;
    float counterImpulseMag = 0.5f;
    bool counterAfterMagReached = false;
    bool doCounterImpulse = false;
    float counterImpulse2Hips = 1.0f;
    float impulseNoBalMult = 1.0f;
    float impulseBalStabStart = 3.0f;
    float impulseBalStabEnd = 10.0f;
    float impulseBalStabMult = 1.0f;
    float impulseSpineAngStart = 0.7f;
    float impulseSpineAngEnd = 0.2f;
    float impulseSpineAngMult = 1.0f;
    float impulseVelStart = 1.0f;
    float impulseVelEnd = 4.0f;
    float impulseVelMult = 1.0f;
    float impulseAirMult = 1.0f;
    float impulseAirMultStart = 100.0f;
    float impulseAirMax = 100.0f;
    float impulseAirApplyAbove = 399.0f;
    bool impulseAirOn = false;
    float impulseOneLegMult = 1.0f;
    float impulseOneLegMultStart = 100.0f;
    float impulseOneLegMax = 100.0f;
    float impulseOneLegApplyAbove = 399.0f;
    bool impulseOneLegOn = false;
    float rbRatio = 0.0f;
    float rbLowerShare = 0.5f;
    float rbMoment = 1.0f;
    float rbMaxTwistMomentArm = 0.5f;
    float rbMaxBroomMomentArm = 1.0f;
    float rbRatioAirborne = 0.0f;
    float rbMomentAirborne = 1.0f;
    float rbMaxTwistMomentArmAirborne = 0.5f;
    float rbMaxBroomMomentArmAirborne = 1.0f;
    float rbRatioOneLeg = 0.0f;
    float rbMomentOneLeg = 1.0f;
    float rbMaxTwistMomentArmOneLeg = 0.5f;
    float rbMaxBroomMomentArmOneLeg = 1.0f;
    int rbTwistAxis = 0;
    bool rbPivot = false;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "impulseSpreadOverParts", impulseSpreadOverParts);
        nGame::SetNMMessageParam(msgPtr, "impulseLeakageStrengthScaled", impulseLeakageStrengthScaled);
        nGame::SetNMMessageParam(msgPtr, "impulsePeriod", impulsePeriod);
        nGame::SetNMMessageParam(msgPtr, "impulseTorqueScale", impulseTorqueScale);
        nGame::SetNMMessageParam(msgPtr, "loosenessFix", loosenessFix);
        nGame::SetNMMessageParam(msgPtr, "impulseDelay", impulseDelay);
        nGame::SetNMMessageParam(msgPtr, "impulseReductionPerShot", impulseReductionPerShot);
        nGame::SetNMMessageParam(msgPtr, "impulseRecovery", impulseRecovery);
        nGame::SetNMMessageParam(msgPtr, "impulseMinLeakage", impulseMinLeakage);
        nGame::SetNMMessageParam(msgPtr, "torqueMode", torqueMode);
        nGame::SetNMMessageParam(msgPtr, "torqueSpinMode", torqueSpinMode);
        nGame::SetNMMessageParam(msgPtr, "torqueFilterMode", torqueFilterMode);
        nGame::SetNMMessageParam(msgPtr, "torqueAlwaysSpine3", torqueAlwaysSpine3);
        nGame::SetNMMessageParam(msgPtr, "torqueDelay", torqueDelay);
        nGame::SetNMMessageParam(msgPtr, "torquePeriod", torquePeriod);
        nGame::SetNMMessageParam(msgPtr, "torqueGain", torqueGain);
        nGame::SetNMMessageParam(msgPtr, "torqueCutoff", torqueCutoff);
        nGame::SetNMMessageParam(msgPtr, "torqueReductionPerTick", torqueReductionPerTick);
        nGame::SetNMMessageParam(msgPtr, "liftGain", liftGain);
        nGame::SetNMMessageParam(msgPtr, "counterImpulseDelay", counterImpulseDelay);
        nGame::SetNMMessageParam(msgPtr, "counterImpulseMag", counterImpulseMag);
        nGame::SetNMMessageParam(msgPtr, "counterAfterMagReached", counterAfterMagReached);
        nGame::SetNMMessageParam(msgPtr, "doCounterImpulse", doCounterImpulse);
        nGame::SetNMMessageParam(msgPtr, "counterImpulse2Hips", counterImpulse2Hips);
        nGame::SetNMMessageParam(msgPtr, "impulseNoBalMult", impulseNoBalMult);
        nGame::SetNMMessageParam(msgPtr, "impulseBalStabStart", impulseBalStabStart);
        nGame::SetNMMessageParam(msgPtr, "impulseBalStabEnd", impulseBalStabEnd);
        nGame::SetNMMessageParam(msgPtr, "impulseBalStabMult", impulseBalStabMult);
        nGame::SetNMMessageParam(msgPtr, "impulseSpineAngStart", impulseSpineAngStart);
        nGame::SetNMMessageParam(msgPtr, "impulseSpineAngEnd", impulseSpineAngEnd);
        nGame::SetNMMessageParam(msgPtr, "impulseSpineAngMult", impulseSpineAngMult);
        nGame::SetNMMessageParam(msgPtr, "impulseVelStart", impulseVelStart);
        nGame::SetNMMessageParam(msgPtr, "impulseVelEnd", impulseVelEnd);
        nGame::SetNMMessageParam(msgPtr, "impulseVelMult", impulseVelMult);
        nGame::SetNMMessageParam(msgPtr, "impulseAirMult", impulseAirMult);
        nGame::SetNMMessageParam(msgPtr, "impulseAirMultStart", impulseAirMultStart);
        nGame::SetNMMessageParam(msgPtr, "impulseAirMax", impulseAirMax);
        nGame::SetNMMessageParam(msgPtr, "impulseAirApplyAbove", impulseAirApplyAbove);
        nGame::SetNMMessageParam(msgPtr, "impulseAirOn", impulseAirOn);
        nGame::SetNMMessageParam(msgPtr, "impulseOneLegMult", impulseOneLegMult);
        nGame::SetNMMessageParam(msgPtr, "impulseOneLegMultStart", impulseOneLegMultStart);
        nGame::SetNMMessageParam(msgPtr, "impulseOneLegMax", impulseOneLegMax);
        nGame::SetNMMessageParam(msgPtr, "impulseOneLegApplyAbove", impulseOneLegApplyAbove);
        nGame::SetNMMessageParam(msgPtr, "impulseOneLegOn", impulseOneLegOn);
        nGame::SetNMMessageParam(msgPtr, "rbRatio", rbRatio);
        nGame::SetNMMessageParam(msgPtr, "rbLowerShare", rbLowerShare);
        nGame::SetNMMessageParam(msgPtr, "rbMoment", rbMoment);
        nGame::SetNMMessageParam(msgPtr, "rbMaxTwistMomentArm", rbMaxTwistMomentArm);
        nGame::SetNMMessageParam(msgPtr, "rbMaxBroomMomentArm", rbMaxBroomMomentArm);
        nGame::SetNMMessageParam(msgPtr, "rbRatioAirborne", rbRatioAirborne);
        nGame::SetNMMessageParam(msgPtr, "rbMomentAirborne", rbMomentAirborne);
        nGame::SetNMMessageParam(msgPtr, "rbMaxTwistMomentArmAirborne", rbMaxTwistMomentArmAirborne);
        nGame::SetNMMessageParam(msgPtr, "rbMaxBroomMomentArmAirborne", rbMaxBroomMomentArmAirborne);
        nGame::SetNMMessageParam(msgPtr, "rbRatioOneLeg", rbRatioOneLeg);
        nGame::SetNMMessageParam(msgPtr, "rbMomentOneLeg", rbMomentOneLeg);
        nGame::SetNMMessageParam(msgPtr, "rbMaxTwistMomentArmOneLeg", rbMaxTwistMomentArmOneLeg);
        nGame::SetNMMessageParam(msgPtr, "rbMaxBroomMomentArmOneLeg", rbMaxBroomMomentArmOneLeg);
        nGame::SetNMMessageParam(msgPtr, "rbTwistAxis", rbTwistAxis);
        nGame::SetNMMessageParam(msgPtr, "rbPivot", rbPivot);
        return;
    }
};

struct NMMessageConfigureBulletsExtra
{
    bool impulseSpreadOverParts = false;
    float impulsePeriod = 0.1f;
    float impulseTorqueScale = 1.0f;
    bool loosenessFix = true;
    float impulseDelay = 0.0f;
    int torqueMode = 0;
    int torqueSpinMode = 0;
    int torqueFilterMode = 0;
    bool torqueAlwaysSpine3 = true;
    float torqueDelay = 0.0f;
    float torquePeriod = 0.120f;
    float torqueGain = 4.0f;
    float torqueCutoff = 0.0f;
    float torqueReductionPerTick = 0.0f;
    float liftGain = 0.0f;
    float counterImpulseDelay = 0.033330f;
    float counterImpulseMag = 0.5f;
    bool counterAfterMagReached = false;
    bool doCounterImpulse = false;
    float counterImpulse2Hips = 1.0f;
    float impulseNoBalMult = 1.0f;
    float impulseBalStabStart = 3.0f;
    float impulseBalStabEnd = 10.0f;
    float impulseBalStabMult = 1.0f;
    float impulseSpineAngStart = 0.7f;
    float impulseSpineAngEnd = 0.2f;
    float impulseSpineAngMult = 1.0f;
    float impulseVelStart = 4.0f;
    float impulseVelEnd = 4.0f;
    float impulseVelMult = 1.0f;
    float impulseAirMult = 1.0f;
    float impulseAirMultStart = 100.0f;
    float impulseAirMax = 100.0f;
    float impulseAirApplyAbove = 399.0f;
    bool impulseAirOn = false;
    float impulseOneLegMult = 1.0f;
    float impulseOneLegMultStart = 100.0f;
    float impulseOneLegMax = 100.0f;
    float impulseOneLegApplyAbove = 399.0f;
    bool impulseOneLegOn = false;
    float rbRatio = 0.0f;
    float rbLowerShare = 0.5f;
    float rbMoment = 1.0f;
    float rbMaxTwistMomentArm = 0.5f;
    float rbMaxBroomMomentArm = 1.0f;
    float rbRatioAirborne = 0.0f;
    float rbMomentAirborne = 1.0f;
    float rbMaxTwistMomentArmAirborne = 0.5f;
    float rbMaxBroomMomentArmAirborne = 1.0f;
    float rbRatioOneLeg = 0.0f;
    float rbMomentOneLeg = 1.0f;
    float rbMaxTwistMomentArmOneLeg = 0.5f;
    float rbMaxBroomMomentArmOneLeg = 1.0f;
    int rbTwistAxis = 0;
    bool rbPivot = false;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "impulseSpreadOverParts", impulseSpreadOverParts);
        nGame::SetNMMessageParam(msgPtr, "impulsePeriod", impulsePeriod);
        nGame::SetNMMessageParam(msgPtr, "impulseTorqueScale", impulseTorqueScale);
        nGame::SetNMMessageParam(msgPtr, "loosenessFix", loosenessFix);
        nGame::SetNMMessageParam(msgPtr, "impulseDelay", impulseDelay);
        nGame::SetNMMessageParam(msgPtr, "torqueMode", torqueMode);
        nGame::SetNMMessageParam(msgPtr, "torqueSpinMode", torqueSpinMode);
        nGame::SetNMMessageParam(msgPtr, "torqueFilterMode", torqueFilterMode);
        nGame::SetNMMessageParam(msgPtr, "torqueAlwaysSpine3", torqueAlwaysSpine3);
        nGame::SetNMMessageParam(msgPtr, "torqueDelay", torqueDelay);
        nGame::SetNMMessageParam(msgPtr, "torquePeriod", torquePeriod);
        nGame::SetNMMessageParam(msgPtr, "torqueGain", torqueGain);
        nGame::SetNMMessageParam(msgPtr, "torqueCutoff", torqueCutoff);
        nGame::SetNMMessageParam(msgPtr, "torqueReductionPerTick", torqueReductionPerTick);
        nGame::SetNMMessageParam(msgPtr, "liftGain", liftGain);
        nGame::SetNMMessageParam(msgPtr, "counterImpulseDelay", counterImpulseDelay);
        nGame::SetNMMessageParam(msgPtr, "counterImpulseMag", counterImpulseMag);
        nGame::SetNMMessageParam(msgPtr, "counterAfterMagReached", counterAfterMagReached);
        nGame::SetNMMessageParam(msgPtr, "doCounterImpulse", doCounterImpulse);
        nGame::SetNMMessageParam(msgPtr, "counterImpulse2Hips", counterImpulse2Hips);
        nGame::SetNMMessageParam(msgPtr, "impulseNoBalMult", impulseNoBalMult);
        nGame::SetNMMessageParam(msgPtr, "impulseBalStabStart", impulseBalStabStart);
        nGame::SetNMMessageParam(msgPtr, "impulseBalStabEnd", impulseBalStabEnd);
        nGame::SetNMMessageParam(msgPtr, "impulseBalStabMult", impulseBalStabMult);
        nGame::SetNMMessageParam(msgPtr, "impulseSpineAngStart", impulseSpineAngStart);
        nGame::SetNMMessageParam(msgPtr, "impulseSpineAngEnd", impulseSpineAngEnd);
        nGame::SetNMMessageParam(msgPtr, "impulseSpineAngMult", impulseSpineAngMult);
        nGame::SetNMMessageParam(msgPtr, "impulseVelStart", impulseVelStart);
        nGame::SetNMMessageParam(msgPtr, "impulseVelEnd", impulseVelEnd);
        nGame::SetNMMessageParam(msgPtr, "impulseVelMult", impulseVelMult);
        nGame::SetNMMessageParam(msgPtr, "impulseAirMult", impulseAirMult);
        nGame::SetNMMessageParam(msgPtr, "impulseAirMultStart", impulseAirMultStart);
        nGame::SetNMMessageParam(msgPtr, "impulseAirMax", impulseAirMax);
        nGame::SetNMMessageParam(msgPtr, "impulseAirApplyAbove", impulseAirApplyAbove);
        nGame::SetNMMessageParam(msgPtr, "impulseAirOn", impulseAirOn);
        nGame::SetNMMessageParam(msgPtr, "impulseOneLegMult", impulseOneLegMult);
        nGame::SetNMMessageParam(msgPtr, "impulseOneLegMultStart", impulseOneLegMultStart);
        nGame::SetNMMessageParam(msgPtr, "impulseOneLegMax", impulseOneLegMax);
        nGame::SetNMMessageParam(msgPtr, "impulseOneLegApplyAbove", impulseOneLegApplyAbove);
        nGame::SetNMMessageParam(msgPtr, "impulseOneLegOn", impulseOneLegOn);
        nGame::SetNMMessageParam(msgPtr, "rbRatio", rbRatio);
        nGame::SetNMMessageParam(msgPtr, "rbLowerShare", rbLowerShare);
        nGame::SetNMMessageParam(msgPtr, "rbMoment", rbMoment);
        nGame::SetNMMessageParam(msgPtr, "rbMaxTwistMomentArm", rbMaxTwistMomentArm);
        nGame::SetNMMessageParam(msgPtr, "rbMaxBroomMomentArm", rbMaxBroomMomentArm);
        nGame::SetNMMessageParam(msgPtr, "rbRatioAirborne", rbRatioAirborne);
        nGame::SetNMMessageParam(msgPtr, "rbMomentAirborne", rbMomentAirborne);
        nGame::SetNMMessageParam(msgPtr, "rbMaxTwistMomentArmAirborne", rbMaxTwistMomentArmAirborne);
        nGame::SetNMMessageParam(msgPtr, "rbMaxBroomMomentArmAirborne", rbMaxBroomMomentArmAirborne);
        nGame::SetNMMessageParam(msgPtr, "rbRatioOneLeg", rbRatioOneLeg);
        nGame::SetNMMessageParam(msgPtr, "rbMomentOneLeg", rbMomentOneLeg);
        nGame::SetNMMessageParam(msgPtr, "rbMaxTwistMomentArmOneLeg", rbMaxTwistMomentArmOneLeg);
        nGame::SetNMMessageParam(msgPtr, "rbMaxBroomMomentArmOneLeg", rbMaxBroomMomentArmOneLeg);
        nGame::SetNMMessageParam(msgPtr, "rbTwistAxis", rbTwistAxis);
        nGame::SetNMMessageParam(msgPtr, "rbPivot", rbPivot);
        return;
    }
};

struct NMMessageConfigureLimits
{
    std::string mask = "fb";
    bool enable = true;
    bool toDesired = false;
    bool restore = false;
    bool toCurAnimation = false;
    int index = -1;
    float lean1 = 1.570796f;
    float lean2 = 1.570796f;
    float twist = 1.570796f;
    float margin = 0.196350f;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "mask", mask.c_str());
        nGame::SetNMMessageParam(msgPtr, "enable", enable);
        nGame::SetNMMessageParam(msgPtr, "toDesired", toDesired);
        nGame::SetNMMessageParam(msgPtr, "restore", restore);
        nGame::SetNMMessageParam(msgPtr, "toCurAnimation", toCurAnimation);
        nGame::SetNMMessageParam(msgPtr, "index", index);
        nGame::SetNMMessageParam(msgPtr, "lean1", lean1);
        nGame::SetNMMessageParam(msgPtr, "lean2", lean2);
        nGame::SetNMMessageParam(msgPtr, "twist", twist);
        nGame::SetNMMessageParam(msgPtr, "margin", margin);
        return;
    }
};

struct NMMessageConfigureSoftLimit
{
    int index = 0;
    float stiffness = 1.0f;
    float damping = 1.0f;
    float limitAngle = 0.4f;
    int approachDirection = 1;
    bool velocityScaled = false;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "index", index);
        nGame::SetNMMessageParam(msgPtr, "stiffness", stiffness);
        nGame::SetNMMessageParam(msgPtr, "damping", damping);
        nGame::SetNMMessageParam(msgPtr, "limitAngle", limitAngle);
        nGame::SetNMMessageParam(msgPtr, "approachDirection", approachDirection);
        nGame::SetNMMessageParam(msgPtr, "velocityScaled", velocityScaled);
        return;
    }
};

struct NMMessageConfigureShotInjuredArm
{
    float injuredArmTime = 0.250f;
    float hipYaw = 0.8f;
    float hipRoll = 0.0f;
    float forceStepExtraHeight = 0.070f;
    bool forceStep = true;
    bool stepTurn = true;
    float velMultiplierStart = 1.0f;
    float velMultiplierEnd = 5.0f;
    float velForceStep = 0.8f;
    float velStepTurn = 0.8f;
    bool velScales = true;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "injuredArmTime", injuredArmTime);
        nGame::SetNMMessageParam(msgPtr, "hipYaw", hipYaw);
        nGame::SetNMMessageParam(msgPtr, "hipRoll", hipRoll);
        nGame::SetNMMessageParam(msgPtr, "forceStepExtraHeight", forceStepExtraHeight);
        nGame::SetNMMessageParam(msgPtr, "forceStep", forceStep);
        nGame::SetNMMessageParam(msgPtr, "stepTurn", stepTurn);
        nGame::SetNMMessageParam(msgPtr, "velMultiplierStart", velMultiplierStart);
        nGame::SetNMMessageParam(msgPtr, "velMultiplierEnd", velMultiplierEnd);
        nGame::SetNMMessageParam(msgPtr, "velForceStep", velForceStep);
        nGame::SetNMMessageParam(msgPtr, "velStepTurn", velStepTurn);
        nGame::SetNMMessageParam(msgPtr, "velScales", velScales);
        return;
    }
};

struct NMMessageConfigureShotInjuredLeg
{
    float timeBeforeCollapseWoundLeg = 0.6f;
    float legInjuryTime = 0.6f;
    bool legForceStep = true;
    float legLimpBend = 0.3f;
    float legLiftTime = 1.0f;
    float legInjury = 0.3f;
    float legInjuryHipPitch = 0.0f;
    float legInjuryLiftHipPitch = 0.0f;
    float legInjurySpineBend = 0.3f;
    float legInjuryLiftSpineBend = 0.3f;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "timeBeforeCollapseWoundLeg", timeBeforeCollapseWoundLeg);
        nGame::SetNMMessageParam(msgPtr, "legInjuryTime", legInjuryTime);
        nGame::SetNMMessageParam(msgPtr, "legForceStep", legForceStep);
        nGame::SetNMMessageParam(msgPtr, "legLimpBend", legLimpBend);
        nGame::SetNMMessageParam(msgPtr, "legLiftTime", legLiftTime);
        nGame::SetNMMessageParam(msgPtr, "legInjury", legInjury);
        nGame::SetNMMessageParam(msgPtr, "legInjuryHipPitch", legInjuryHipPitch);
        nGame::SetNMMessageParam(msgPtr, "legInjuryLiftHipPitch", legInjuryLiftHipPitch);
        nGame::SetNMMessageParam(msgPtr, "legInjurySpineBend", legInjurySpineBend);
        nGame::SetNMMessageParam(msgPtr, "legInjuryLiftSpineBend", legInjuryLiftSpineBend);
        return;
    }
};

struct NMMessageDefineAttachedObject
{
    int partIndex = -1;
    float objectMass = 0.0f;
    Vector3 worldPos{ 0.0f, 0.0f, 0.0f };
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "partIndex", partIndex);
        nGame::SetNMMessageParam(msgPtr, "objectMass", objectMass);
        nGame::SetNMMessageParam(msgPtr, "worldPos", worldPos.x, worldPos.y, worldPos.z);
        return;
    }
};

struct NMMessageForceToBodyPart
{
    int partIndex = 0;
    Vector3 force{ 0.0f, -50.0f, 0.0f };
    bool forceDefinedInPartSpace = false;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "partIndex", partIndex);
        nGame::SetNMMessageParam(msgPtr, "force", force.x, force.y, force.z);
        nGame::SetNMMessageParam(msgPtr, "forceDefinedInPartSpace", forceDefinedInPartSpace);
        return;
    }
};

struct NMMessageLeanInDirection
{
    float leanAmount = 0.2f;
    Vector3 dir{ 0.0f, 0.0f, 1.0f };
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "leanAmount", leanAmount);
        nGame::SetNMMessageParam(msgPtr, "dir", dir.x, dir.y, dir.z);
        return;
    }
};

struct NMMessageLeanRandom
{
    float leanAmountMin = 0.2f;
    float leanAmountMax = 0.2f;
    float changeTimeMin = 0.5f;
    float changeTimeMax = 1.0f;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "leanAmountMin", leanAmountMin);
        nGame::SetNMMessageParam(msgPtr, "leanAmountMax", leanAmountMax);
        nGame::SetNMMessageParam(msgPtr, "changeTimeMin", changeTimeMin);
        nGame::SetNMMessageParam(msgPtr, "changeTimeMax", changeTimeMax);
        return;
    }
};

struct NMMessageLeanToPosition
{
    float leanAmount = 0.2f;
    Vector3 pos{ 0.0f, 0.0f, 0.0f };
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "leanAmount", leanAmount);
        nGame::SetNMMessageParam(msgPtr, "pos", pos.x, pos.y, pos.z);
        return;
    }
};

struct NMMessageLeanTowardsObject
{
    float leanAmount = 0.2f;
    Vector3 offset{ 0.0f, 0.0f, 0.0f };
    int instanceIndex = -1;
    int boundIndex = 0;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "leanAmount", leanAmount);
        nGame::SetNMMessageParam(msgPtr, "offset", offset.x, offset.y, offset.z);
        nGame::SetNMMessageParam(msgPtr, "instanceIndex", instanceIndex);
        nGame::SetNMMessageParam(msgPtr, "boundIndex", boundIndex);
        return;
    }
};

struct NMMessageHipsLeanInDirection
{
    float leanAmount = 0.2f;
    Vector3 dir{ 0.0f, 0.0f, 1.0f };
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "leanAmount", leanAmount);
        nGame::SetNMMessageParam(msgPtr, "dir", dir.x, dir.y, dir.z);
        return;
    }
};

struct NMMessageHipsLeanRandom
{
    float leanAmountMin = 0.3f;
    float leanAmountMax = 0.4f;
    float changeTimeMin = 2.0f;
    float changeTimeMax = 4.0f;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "leanAmountMin", leanAmountMin);
        nGame::SetNMMessageParam(msgPtr, "leanAmountMax", leanAmountMax);
        nGame::SetNMMessageParam(msgPtr, "changeTimeMin", changeTimeMin);
        nGame::SetNMMessageParam(msgPtr, "changeTimeMax", changeTimeMax);
        return;
    }
};

struct NMMessageHipsLeanToPosition
{
    float leanAmount = 0.2f;
    Vector3 pos{ 0.0f, 0.0f, 0.0f };
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "leanAmount", leanAmount);
        nGame::SetNMMessageParam(msgPtr, "pos", pos.x, pos.y, pos.z);
        return;
    }
};

struct NMMessageHipsLeanTowardsObject
{
    float leanAmount = 0.2f;
    Vector3 offset{ 0.0f, 0.0f, 0.0f };
    int instanceIndex = -1;
    int boundIndex = 0;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "leanAmount", leanAmount);
        nGame::SetNMMessageParam(msgPtr, "offset", offset.x, offset.y, offset.z);
        nGame::SetNMMessageParam(msgPtr, "instanceIndex", instanceIndex);
        nGame::SetNMMessageParam(msgPtr, "boundIndex", boundIndex);
        return;
    }
};

struct NMMessageForceLeanInDirection
{
    float leanAmount = 0.2f;
    Vector3 dir{ 0.0f, 0.0f, 1.0f };
    int bodyPart = 0;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "leanAmount", leanAmount);
        nGame::SetNMMessageParam(msgPtr, "dir", dir.x, dir.y, dir.z);
        nGame::SetNMMessageParam(msgPtr, "bodyPart", bodyPart);
        return;
    }
};

struct NMMessageForceLeanRandom
{
    float leanAmountMin = 0.3f;
    float leanAmountMax = 0.4f;
    float changeTimeMin = 2.0f;
    float changeTimeMax = 4.0f;
    int bodyPart = 0;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "leanAmountMin", leanAmountMin);
        nGame::SetNMMessageParam(msgPtr, "leanAmountMax", leanAmountMax);
        nGame::SetNMMessageParam(msgPtr, "changeTimeMin", changeTimeMin);
        nGame::SetNMMessageParam(msgPtr, "changeTimeMax", changeTimeMax);
        nGame::SetNMMessageParam(msgPtr, "bodyPart", bodyPart);
        return;
    }
};

struct NMMessageForceLeanToPosition
{
    float leanAmount = 0.2f;
    Vector3 pos{ 0.0f, 0.0f, 0.0f };
    int bodyPart = 0;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "leanAmount", leanAmount);
        nGame::SetNMMessageParam(msgPtr, "pos", pos.x, pos.y, pos.z);
        nGame::SetNMMessageParam(msgPtr, "bodyPart", bodyPart);
        return;
    }
};

struct NMMessageForceLeanTowardsObject
{
    float leanAmount = 0.2f;
    Vector3 offset{ 0.0f, 0.0f, 0.0f };
    int instanceIndex = -1;
    int boundIndex = 0;
    int bodyPart = 0;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "leanAmount", leanAmount);
        nGame::SetNMMessageParam(msgPtr, "offset", offset.x, offset.y, offset.z);
        nGame::SetNMMessageParam(msgPtr, "instanceIndex", instanceIndex);
        nGame::SetNMMessageParam(msgPtr, "boundIndex", boundIndex);
        nGame::SetNMMessageParam(msgPtr, "bodyPart", bodyPart);
        return;
    }
};

struct NMMessageSetStiffness
{
    float bodyStiffness = 2.0f;
    float damping = 1.0f;
    std::string mask = "fb";
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "bodyStiffness", bodyStiffness);
        nGame::SetNMMessageParam(msgPtr, "damping", damping);
        nGame::SetNMMessageParam(msgPtr, "mask", mask.c_str());
        return;
    }
};

struct NMMessageSetMuscleStiffness
{
    float muscleStiffness = 1.0f;
    std::string mask = "fb";
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "muscleStiffness", muscleStiffness);
        nGame::SetNMMessageParam(msgPtr, "mask", mask.c_str());
        return;
    }
};

struct NMMessageSetWeaponMode
{
    int weaponMode = 5;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "weaponMode", weaponMode);
        return;
    }
};

struct NMMessageRegisterWeapon
{
    int hand = 1;
    int levelIndex = -1;
    int constraintHandle = -1;
    Vector3 gunToHandA{ 1.0f, 0.0f, 0.0f };
    Vector3 gunToHandB{ 0.0f, 1.0f, 0.0f };
    Vector3 gunToHandC{ 0.0f, 0.0f, 1.0f };
    Vector3 gunToHandD{ 0.0f, 0.0f, 0.0f };
    Vector3 gunToMuzzleInGun{ 0.0f, 0.0f, 0.0f };
    Vector3 gunToButtInGun{ 0.0f, 0.0f, 0.0f };
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "hand", hand);
        nGame::SetNMMessageParam(msgPtr, "levelIndex", levelIndex);
        nGame::SetNMMessageParam(msgPtr, "constraintHandle", constraintHandle);
        nGame::SetNMMessageParam(msgPtr, "gunToHandA", gunToHandA.x, gunToHandA.y, gunToHandA.z);
        nGame::SetNMMessageParam(msgPtr, "gunToHandB", gunToHandB.x, gunToHandB.y, gunToHandB.z);
        nGame::SetNMMessageParam(msgPtr, "gunToHandC", gunToHandC.x, gunToHandC.y, gunToHandC.z);
        nGame::SetNMMessageParam(msgPtr, "gunToHandD", gunToHandD.x, gunToHandD.y, gunToHandD.z);
        nGame::SetNMMessageParam(msgPtr, "gunToMuzzleInGun", gunToMuzzleInGun.x, gunToMuzzleInGun.y, gunToMuzzleInGun.z);
        nGame::SetNMMessageParam(msgPtr, "gunToButtInGun", gunToButtInGun.x, gunToButtInGun.y, gunToButtInGun.z);
        return;
    }
};

struct NMMessageShotRelax
{
    float relaxPeriodUpper = 0.2f;
    float relaxPeriodLower = 0.2f;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "relaxPeriodUpper", relaxPeriodUpper);
        nGame::SetNMMessageParam(msgPtr, "relaxPeriodLower", relaxPeriodLower);
        return;
    }
};

struct NMMessageFireWeapon
{
    float firedWeaponStrength = 1000.0f;
    int gunHandEnum = 0;
    bool applyFireGunForceAtClavicle = false;
    float inhibitTime = 0.4f;
    Vector3 direction{ 0.0f, 0.0f, 0.0f };
    float split = 0.5f;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "firedWeaponStrength", firedWeaponStrength);
        nGame::SetNMMessageParam(msgPtr, "gunHandEnum", gunHandEnum);
        nGame::SetNMMessageParam(msgPtr, "applyFireGunForceAtClavicle", applyFireGunForceAtClavicle);
        nGame::SetNMMessageParam(msgPtr, "inhibitTime", inhibitTime);
        nGame::SetNMMessageParam(msgPtr, "direction", direction.x, direction.y, direction.z);
        nGame::SetNMMessageParam(msgPtr, "split", split);
        return;
    }
};

struct NMMessageConfigureConstraints
{
    bool handCuffs = false;
    bool handCuffsBehindBack = false;
    bool legCuffs = false;
    bool rightDominant = false;
    int passiveMode = 0;
    bool bespokeBehaviour = false;
    float blend2ZeroPose = 0.0f;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "handCuffs", handCuffs);
        nGame::SetNMMessageParam(msgPtr, "handCuffsBehindBack", handCuffsBehindBack);
        nGame::SetNMMessageParam(msgPtr, "legCuffs", legCuffs);
        nGame::SetNMMessageParam(msgPtr, "rightDominant", rightDominant);
        nGame::SetNMMessageParam(msgPtr, "passiveMode", passiveMode);
        nGame::SetNMMessageParam(msgPtr, "bespokeBehaviour", bespokeBehaviour);
        nGame::SetNMMessageParam(msgPtr, "blend2ZeroPose", blend2ZeroPose);
        return;
    }
};

struct NMMessageStayUpright
{
    bool useForces = false;
    bool useTorques = false;
    bool lastStandMode = false;
    float lastStandSinkRate = 0.3f;
    float lastStandHorizDamping = 0.4f;
    float lastStandMaxTime = 0.4f;
    bool turnTowardsBullets = false;
    bool velocityBased = false;
    bool torqueOnlyInAir = false;
    float forceStrength = 3.0f;
    float forceDamping = -1.0f;
    float forceFeetMult = 1.0f;
    float forceSpine3Share = 0.3f;
    float forceLeanReduction = 1.0f;
    float forceInAirShare = 0.5f;
    float forceMin = -1.0f;
    float forceMax = -1.0f;
    float forceSaturationVel = 4.0f;
    float forceThresholdVel = 0.5f;
    float torqueStrength = 2.0f;
    float torqueDamping = 0.5f;
    float torqueSaturationVel = 4.0f;
    float torqueThresholdVel = 2.5f;
    float supportPosition = 2.0f;
    float noSupportForceMult = 1.0f;
    float stepUpHelp = 0.0f;
    float stayUpAcc = 0.7f;
    float stayUpAccMax = 5.0f;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "useForces", useForces);
        nGame::SetNMMessageParam(msgPtr, "useTorques", useTorques);
        nGame::SetNMMessageParam(msgPtr, "lastStandMode", lastStandMode);
        nGame::SetNMMessageParam(msgPtr, "lastStandSinkRate", lastStandSinkRate);
        nGame::SetNMMessageParam(msgPtr, "lastStandHorizDamping", lastStandHorizDamping);
        nGame::SetNMMessageParam(msgPtr, "lastStandMaxTime", lastStandMaxTime);
        nGame::SetNMMessageParam(msgPtr, "turnTowardsBullets", turnTowardsBullets);
        nGame::SetNMMessageParam(msgPtr, "velocityBased", velocityBased);
        nGame::SetNMMessageParam(msgPtr, "torqueOnlyInAir", torqueOnlyInAir);
        nGame::SetNMMessageParam(msgPtr, "forceStrength", forceStrength);
        nGame::SetNMMessageParam(msgPtr, "forceDamping", forceDamping);
        nGame::SetNMMessageParam(msgPtr, "forceFeetMult", forceFeetMult);
        nGame::SetNMMessageParam(msgPtr, "forceSpine3Share", forceSpine3Share);
        nGame::SetNMMessageParam(msgPtr, "forceLeanReduction", forceLeanReduction);
        nGame::SetNMMessageParam(msgPtr, "forceInAirShare", forceInAirShare);
        nGame::SetNMMessageParam(msgPtr, "forceMin", forceMin);
        nGame::SetNMMessageParam(msgPtr, "forceMax", forceMax);
        nGame::SetNMMessageParam(msgPtr, "forceSaturationVel", forceSaturationVel);
        nGame::SetNMMessageParam(msgPtr, "forceThresholdVel", forceThresholdVel);
        nGame::SetNMMessageParam(msgPtr, "torqueStrength", torqueStrength);
        nGame::SetNMMessageParam(msgPtr, "torqueDamping", torqueDamping);
        nGame::SetNMMessageParam(msgPtr, "torqueSaturationVel", torqueSaturationVel);
        nGame::SetNMMessageParam(msgPtr, "torqueThresholdVel", torqueThresholdVel);
        nGame::SetNMMessageParam(msgPtr, "supportPosition", supportPosition);
        nGame::SetNMMessageParam(msgPtr, "noSupportForceMult", noSupportForceMult);
        nGame::SetNMMessageParam(msgPtr, "stepUpHelp", stepUpHelp);
        nGame::SetNMMessageParam(msgPtr, "stayUpAcc", stayUpAcc);
        nGame::SetNMMessageParam(msgPtr, "stayUpAccMax", stayUpAccMax);
        return;
    }
};

struct NMMessageSetCharacterStrength
{
    float characterStrength = 1.0f;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "characterStrength", characterStrength);
        return;
    }
};

struct NMMessageSetCharacterHealth
{
    float characterHealth = 1.0f;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "characterHealth", characterHealth);
        return;
    }
};

struct NMMessageSetFallingReaction
{
    bool handsAndKnees = true;
    bool callRDS = true;
    float comVelRDSThresh = 0.5f;
    bool resistRolling = false;
    float armReduceSpeed = 2.5f;
    float reachLengthMultiplier = 1.0f;
    float inhibitRollingTime = 0.05f;
    float changeFrictionTime = 0.05f;
    float groundFriction = 2.0f;
    float frictionMin = 1.5f;
    float frictionMax = 9999.0f;
    bool stopOnSlopes = false;
    float stopManual = 0.0f;
    float stoppedStrengthDecay = 5.0f;
    float spineLean1Offset = 0.0f;
    bool riflePose = false;
    bool hkHeadAvoid = true;
    bool antiPropClav = false;
    bool antiPropWeak = false;
    bool headAsWeakAsArms = true;
    float successStrength = 1.0f;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "handsAndKnees", handsAndKnees);
        nGame::SetNMMessageParam(msgPtr, "callRDS", callRDS);
        nGame::SetNMMessageParam(msgPtr, "comVelRDSThresh", comVelRDSThresh);
        nGame::SetNMMessageParam(msgPtr, "resistRolling", resistRolling);
        nGame::SetNMMessageParam(msgPtr, "armReduceSpeed", armReduceSpeed);
        nGame::SetNMMessageParam(msgPtr, "reachLengthMultiplier", reachLengthMultiplier);
        nGame::SetNMMessageParam(msgPtr, "inhibitRollingTime", inhibitRollingTime);
        nGame::SetNMMessageParam(msgPtr, "changeFrictionTime", changeFrictionTime);
        nGame::SetNMMessageParam(msgPtr, "groundFriction", groundFriction);
        nGame::SetNMMessageParam(msgPtr, "frictionMin", frictionMin);
        nGame::SetNMMessageParam(msgPtr, "frictionMax", frictionMax);
        nGame::SetNMMessageParam(msgPtr, "stopOnSlopes", stopOnSlopes);
        nGame::SetNMMessageParam(msgPtr, "stopManual", stopManual);
        nGame::SetNMMessageParam(msgPtr, "stoppedStrengthDecay", stoppedStrengthDecay);
        nGame::SetNMMessageParam(msgPtr, "spineLean1Offset", spineLean1Offset);
        nGame::SetNMMessageParam(msgPtr, "riflePose", riflePose);
        nGame::SetNMMessageParam(msgPtr, "hkHeadAvoid", hkHeadAvoid);
        nGame::SetNMMessageParam(msgPtr, "antiPropClav", antiPropClav);
        nGame::SetNMMessageParam(msgPtr, "antiPropWeak", antiPropWeak);
        nGame::SetNMMessageParam(msgPtr, "headAsWeakAsArms", headAsWeakAsArms);
        nGame::SetNMMessageParam(msgPtr, "successStrength", successStrength);
        return;
    }
};

struct NMMessageSetCharacterUnderwater
{
    bool underwater = false;
    float viscosity = -1.0f;
    float gravityFactor = 1.0f;
    float stroke = 0.0f;
    bool linearStroke = false;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "underwater", underwater);
        nGame::SetNMMessageParam(msgPtr, "viscosity", viscosity);
        nGame::SetNMMessageParam(msgPtr, "gravityFactor", gravityFactor);
        nGame::SetNMMessageParam(msgPtr, "stroke", stroke);
        nGame::SetNMMessageParam(msgPtr, "linearStroke", linearStroke);
        return;
    }
};

struct NMMessageSetCharacterCollisions
{
    float spin = 0.0f;
    float maxVelocity = 8.0f;
    bool applyToAll = false;
    bool applyToSpine = true;
    bool applyToThighs = true;
    bool applyToClavicles = true;
    bool applyToUpperArms = true;
    bool footSlip = true;
    int vehicleClass = 15;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "spin", spin);
        nGame::SetNMMessageParam(msgPtr, "maxVelocity", maxVelocity);
        nGame::SetNMMessageParam(msgPtr, "applyToAll", applyToAll);
        nGame::SetNMMessageParam(msgPtr, "applyToSpine", applyToSpine);
        nGame::SetNMMessageParam(msgPtr, "applyToThighs", applyToThighs);
        nGame::SetNMMessageParam(msgPtr, "applyToClavicles", applyToClavicles);
        nGame::SetNMMessageParam(msgPtr, "applyToUpperArms", applyToUpperArms);
        nGame::SetNMMessageParam(msgPtr, "footSlip", footSlip);
        nGame::SetNMMessageParam(msgPtr, "vehicleClass", vehicleClass);
        return;
    }
};

struct NMMessageSetCharacterDamping
{
    float somersaultThresh = 120.0f;
    float somersaultDamp = 0.0f;
    float cartwheelThresh = 120.0f;
    float cartwheelDamp = 0.0f;
    float vehicleCollisionTime = 0.0f;
    bool v2 = false;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "somersaultThresh", somersaultThresh);
        nGame::SetNMMessageParam(msgPtr, "somersaultDamp", somersaultDamp);
        nGame::SetNMMessageParam(msgPtr, "cartwheelThresh", cartwheelThresh);
        nGame::SetNMMessageParam(msgPtr, "cartwheelDamp", cartwheelDamp);
        nGame::SetNMMessageParam(msgPtr, "vehicleCollisionTime", vehicleCollisionTime);
        nGame::SetNMMessageParam(msgPtr, "v2", v2);
        return;
    }
};

struct NMMessageSetFrictionScale
{
    float scale = 1.0f;
    float globalMin = 0.0f;
    float globalMax = 999999.0f;
    std::string mask = "fb";
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "scale", scale);
        nGame::SetNMMessageParam(msgPtr, "globalMin", globalMin);
        nGame::SetNMMessageParam(msgPtr, "globalMax", globalMax);
        nGame::SetNMMessageParam(msgPtr, "mask", mask.c_str());
        return;
    }
};

struct NMMessageAnimPose
{
    float muscleStiffness = -1.0f;
    float stiffness = -1.0f;
    float damping = 1.0f;
    std::string effectorMask = "ub";
    bool overideHeadlook = false;
    bool overidePointArm = false;
    bool overidePointGun = false;
    bool useZMPGravityCompensation = true;
    float gravityCompensation = -1.0f;
    float muscleLeftArm = -1.0f;
    float muscleStiffnessRightArm = -1.0f;
    float muscleStiffnessSpine = -1.0f;
    float muscleStiffnessLeftLeg = -1.0f;
    float muscleStiffnessRightLeg = -1.0f;
    float stiffnessLeftArm = -1.0f;
    float stiffnessRightArm = -1.0f;
    float stiffnessSpine = -1.0f;
    float stiffnessLeftLeg = -1.0f;
    float stiffnessRightLeg = -1.0f;
    float dampingLeftArm = 1.0f;
    float dampingRightArm = 1.0f;
    float dampingSpine = 1.0f;
    float dampingLeftLeg = 1.0f;
    float dampingRightLeg = 1.0f;
    float gravCompLeftArm = -1.0f;
    float gravCompRightArm = -1.0f;
    float gravCompSpine = -1.0f;
    float gravCompLeftLeg = -1.0f;
    float gravCompRightLeg = -1.0f;
    int connectedLeftHand = 0;
    int connectedRightHand = 0;
    int connectedLeftFoot = -2;
    int connectedRightFoot = -2;
    int animSource = 0; // ART::kITSourceCurrent
    int dampenSideMotionInstanceIndex = -1;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "muscleStiffness", muscleStiffness);
        nGame::SetNMMessageParam(msgPtr, "stiffness", stiffness);
        nGame::SetNMMessageParam(msgPtr, "damping", damping);
        nGame::SetNMMessageParam(msgPtr, "effectorMask", effectorMask.c_str());
        nGame::SetNMMessageParam(msgPtr, "overideHeadlook", overideHeadlook);
        nGame::SetNMMessageParam(msgPtr, "overidePointArm", overidePointArm);
        nGame::SetNMMessageParam(msgPtr, "overidePointGun", overidePointGun);
        nGame::SetNMMessageParam(msgPtr, "useZMPGravityCompensation", useZMPGravityCompensation);
        nGame::SetNMMessageParam(msgPtr, "gravityCompensation", gravityCompensation);
        nGame::SetNMMessageParam(msgPtr, "muscleLeftArm", muscleLeftArm);
        nGame::SetNMMessageParam(msgPtr, "muscleStiffnessRightArm", muscleStiffnessRightArm);
        nGame::SetNMMessageParam(msgPtr, "muscleStiffnessSpine", muscleStiffnessSpine);
        nGame::SetNMMessageParam(msgPtr, "muscleStiffnessLeftLeg", muscleStiffnessLeftLeg);
        nGame::SetNMMessageParam(msgPtr, "muscleStiffnessRightLeg", muscleStiffnessRightLeg);
        nGame::SetNMMessageParam(msgPtr, "stiffnessLeftArm", stiffnessLeftArm);
        nGame::SetNMMessageParam(msgPtr, "stiffnessRightArm", stiffnessRightArm);
        nGame::SetNMMessageParam(msgPtr, "stiffnessSpine", stiffnessSpine);
        nGame::SetNMMessageParam(msgPtr, "stiffnessLeftLeg", stiffnessLeftLeg);
        nGame::SetNMMessageParam(msgPtr, "stiffnessRightLeg", stiffnessRightLeg);
        nGame::SetNMMessageParam(msgPtr, "dampingLeftArm", dampingLeftArm);
        nGame::SetNMMessageParam(msgPtr, "dampingRightArm", dampingRightArm);
        nGame::SetNMMessageParam(msgPtr, "dampingSpine", dampingSpine);
        nGame::SetNMMessageParam(msgPtr, "dampingLeftLeg", dampingLeftLeg);
        nGame::SetNMMessageParam(msgPtr, "dampingRightLeg", dampingRightLeg);
        nGame::SetNMMessageParam(msgPtr, "gravCompLeftArm", gravCompLeftArm);
        nGame::SetNMMessageParam(msgPtr, "gravCompRightArm", gravCompRightArm);
        nGame::SetNMMessageParam(msgPtr, "gravCompSpine", gravCompSpine);
        nGame::SetNMMessageParam(msgPtr, "gravCompLeftLeg", gravCompLeftLeg);
        nGame::SetNMMessageParam(msgPtr, "gravCompRightLeg", gravCompRightLeg);
        nGame::SetNMMessageParam(msgPtr, "connectedLeftHand", connectedLeftHand);
        nGame::SetNMMessageParam(msgPtr, "connectedRightHand", connectedRightHand);
        nGame::SetNMMessageParam(msgPtr, "connectedLeftFoot", connectedLeftFoot);
        nGame::SetNMMessageParam(msgPtr, "connectedRightFoot", connectedRightFoot);
        nGame::SetNMMessageParam(msgPtr, "animSource", animSource);
        nGame::SetNMMessageParam(msgPtr, "dampenSideMotionInstanceIndex", dampenSideMotionInstanceIndex);
        return;
    }
};

struct NMMessageArmsWindmill
{
    int leftPartID = 10;
    float leftRadius1 = 0.750f;
    float leftRadius2 = 0.750f;
    float leftSpeed = 1.0f;
    Vector3 leftNormal{ 0.0f, 0.2f, 0.2f };
    Vector3 leftCentre{ 0.0f, 0.5f, -0.1f };
    int rightPartID = 10;
    float rightRadius1 = 0.750f;
    float rightRadius2 = 0.750f;
    float rightSpeed = 1.0f;
    Vector3 rightNormal{ 0.0f, -0.2f, -0.2f };
    Vector3 rightCentre{ 0.0f, -0.5f, -0.1f };
    float shoulderStiffness = 2.0f;
    float shoulderDamping = 1.0f;
    float elbowStiffness = 2.0f;
    float elbowDamping = 1.0f;
    float leftElbowMin = 0.5f;
    float rightElbowMin = 0.5f;
    float phaseOffset = 0.0f;
    float dragReduction = 0.2f;
    float IKtwist = 0.0f;
    float angVelThreshold = 0.1f;
    float angVelGain = 1.0f;
    int mirrorMode = 1;
    int adaptiveMode = 0;
    bool forceSync = true;
    bool useLeft = true;
    bool useRight = true;
    bool disableOnImpact = true;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "leftPartID", leftPartID);
        nGame::SetNMMessageParam(msgPtr, "leftRadius1", leftRadius1);
        nGame::SetNMMessageParam(msgPtr, "leftRadius2", leftRadius2);
        nGame::SetNMMessageParam(msgPtr, "leftSpeed", leftSpeed);
        nGame::SetNMMessageParam(msgPtr, "leftNormal", leftNormal.x, leftNormal.y, leftNormal.z);
        nGame::SetNMMessageParam(msgPtr, "leftCentre", leftCentre.x, leftCentre.y, leftCentre.z);
        nGame::SetNMMessageParam(msgPtr, "rightPartID", rightPartID);
        nGame::SetNMMessageParam(msgPtr, "rightRadius1", rightRadius1);
        nGame::SetNMMessageParam(msgPtr, "rightRadius2", rightRadius2);
        nGame::SetNMMessageParam(msgPtr, "rightSpeed", rightSpeed);
        nGame::SetNMMessageParam(msgPtr, "rightNormal", rightNormal.x, rightNormal.y, rightNormal.z);
        nGame::SetNMMessageParam(msgPtr, "rightCentre", rightCentre.x, rightCentre.y, rightCentre.z);
        nGame::SetNMMessageParam(msgPtr, "shoulderStiffness", shoulderStiffness);
        nGame::SetNMMessageParam(msgPtr, "shoulderDamping", shoulderDamping);
        nGame::SetNMMessageParam(msgPtr, "elbowStiffness", elbowStiffness);
        nGame::SetNMMessageParam(msgPtr, "elbowDamping", elbowDamping);
        nGame::SetNMMessageParam(msgPtr, "leftElbowMin", leftElbowMin);
        nGame::SetNMMessageParam(msgPtr, "rightElbowMin", rightElbowMin);
        nGame::SetNMMessageParam(msgPtr, "phaseOffset", phaseOffset);
        nGame::SetNMMessageParam(msgPtr, "dragReduction", dragReduction);
        nGame::SetNMMessageParam(msgPtr, "IKtwist", IKtwist);
        nGame::SetNMMessageParam(msgPtr, "angVelThreshold", angVelThreshold);
        nGame::SetNMMessageParam(msgPtr, "angVelGain", angVelGain);
        nGame::SetNMMessageParam(msgPtr, "mirrorMode", mirrorMode);
        nGame::SetNMMessageParam(msgPtr, "adaptiveMode", adaptiveMode);
        nGame::SetNMMessageParam(msgPtr, "forceSync", forceSync);
        nGame::SetNMMessageParam(msgPtr, "useLeft", useLeft);
        nGame::SetNMMessageParam(msgPtr, "useRight", useRight);
        nGame::SetNMMessageParam(msgPtr, "disableOnImpact", disableOnImpact);
        return;
    }
};

struct NMMessageArmsWindmillAdaptive
{
    float angSpeed = 1.0f;
    float bodyStiffness = 6.0f;
    float amplitude = 0.6f;
    float phase = 0.0f;
    float armStiffness = 6.140f;
    float leftElbowAngle = -1.0f;
    float rightElbowAngle = -1.0f;
    float lean1mult = 1.0f;
    float lean1offset = 0.0f;
    float elbowRate = 1.0f;
    int armDirection = 0;
    bool disableOnImpact = true;
    bool setBackAngles = true;
    bool useAngMom = false;
    bool bendLeftElbow = false;
    bool bendRightElbow = false;
    std::string mask = "ub";
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "angSpeed", angSpeed);
        nGame::SetNMMessageParam(msgPtr, "bodyStiffness", bodyStiffness);
        nGame::SetNMMessageParam(msgPtr, "amplitude", amplitude);
        nGame::SetNMMessageParam(msgPtr, "phase", phase);
        nGame::SetNMMessageParam(msgPtr, "armStiffness", armStiffness);
        nGame::SetNMMessageParam(msgPtr, "leftElbowAngle", leftElbowAngle);
        nGame::SetNMMessageParam(msgPtr, "rightElbowAngle", rightElbowAngle);
        nGame::SetNMMessageParam(msgPtr, "lean1mult", lean1mult);
        nGame::SetNMMessageParam(msgPtr, "lean1offset", lean1offset);
        nGame::SetNMMessageParam(msgPtr, "elbowRate", elbowRate);
        nGame::SetNMMessageParam(msgPtr, "armDirection", armDirection);
        nGame::SetNMMessageParam(msgPtr, "disableOnImpact", disableOnImpact);
        nGame::SetNMMessageParam(msgPtr, "setBackAngles", setBackAngles);
        nGame::SetNMMessageParam(msgPtr, "useAngMom", useAngMom);
        nGame::SetNMMessageParam(msgPtr, "bendLeftElbow", bendLeftElbow);
        nGame::SetNMMessageParam(msgPtr, "bendRightElbow", bendRightElbow);
        nGame::SetNMMessageParam(msgPtr, "mask", mask.c_str());
        return;
    }
};

struct NMMessageBalancerCollisionsReaction
{
    int numStepsTillSlump = 100;
    float stable2SlumpTime = 5.0f;
    float exclusionZone = 0.05f;
    float footFrictionMultStart = 2.0f;
    float footFrictionMultRate = 2.0f;
    float backFrictionMultStart = 1.0f;
    float backFrictionMultRate = 2.0f;
    float impactLegStiffReduction = 3.0f;
    float slumpLegStiffReduction = 1.0f;
    float slumpLegStiffRate = 8.0f;
    float reactTime = 0.3f;
    float impactExagTime = 0.3f;
    float glanceSpinTime = 0.5f;
    float glanceSpinMag = 50.0f;
    float glanceSpinDecayMult = 0.3f;
    int ignoreColWithIndex = -2;
    int slumpMode = 0;
    int reboundMode = 1;
    float ignoreColMassBelow = 20.0f;
    int forwardMode = 0;
    float timeToForward = 0.5f;
    float reboundForce = 0.0f;
    bool braceWall = true;
    float ignoreColVolumeBelow = 0.1f;
    bool fallOverWallDrape = true;
    bool fallOverHighWalls = true;
    bool snap = false;
    float snapMag = -0.1f;
    float snapDirectionRandomness = 0.3f;
    bool snapLeftArm = false;
    bool snapRightArm = false;
    bool snapLeftLeg = false;
    bool snapRightLeg = false;
    bool snapSpine = true;
    bool snapNeck = true;
    bool snapPhasedLegs = true;
    int snapHipType = 0;
    float unSnapInterval = 0.010f;
    float unSnapRatio = 0.7f;
    bool snapUseTorques = true;
    float impactWeaknessZeroDuration = 0.2f;
    float impactWeaknessRampDuration = 0.010f;
    float impactLoosenessAmount = 1.0f;
    bool objectBehindVictim = false;
    Vector3 objectBehindVictimPos{ 0.0f, 0.0f, 0.0f };
    Vector3 objectBehindVictimNormal{ 0.0f, 0.0f, 0.0f };
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "numStepsTillSlump", numStepsTillSlump);
        nGame::SetNMMessageParam(msgPtr, "stable2SlumpTime", stable2SlumpTime);
        nGame::SetNMMessageParam(msgPtr, "exclusionZone", exclusionZone);
        nGame::SetNMMessageParam(msgPtr, "footFrictionMultStart", footFrictionMultStart);
        nGame::SetNMMessageParam(msgPtr, "footFrictionMultRate", footFrictionMultRate);
        nGame::SetNMMessageParam(msgPtr, "backFrictionMultStart", backFrictionMultStart);
        nGame::SetNMMessageParam(msgPtr, "backFrictionMultRate", backFrictionMultRate);
        nGame::SetNMMessageParam(msgPtr, "impactLegStiffReduction", impactLegStiffReduction);
        nGame::SetNMMessageParam(msgPtr, "slumpLegStiffReduction", slumpLegStiffReduction);
        nGame::SetNMMessageParam(msgPtr, "slumpLegStiffRate", slumpLegStiffRate);
        nGame::SetNMMessageParam(msgPtr, "reactTime", reactTime);
        nGame::SetNMMessageParam(msgPtr, "impactExagTime", impactExagTime);
        nGame::SetNMMessageParam(msgPtr, "glanceSpinTime", glanceSpinTime);
        nGame::SetNMMessageParam(msgPtr, "glanceSpinMag", glanceSpinMag);
        nGame::SetNMMessageParam(msgPtr, "glanceSpinDecayMult", glanceSpinDecayMult);
        nGame::SetNMMessageParam(msgPtr, "ignoreColWithIndex", ignoreColWithIndex);
        nGame::SetNMMessageParam(msgPtr, "slumpMode", slumpMode);
        nGame::SetNMMessageParam(msgPtr, "reboundMode", reboundMode);
        nGame::SetNMMessageParam(msgPtr, "ignoreColMassBelow", ignoreColMassBelow);
        nGame::SetNMMessageParam(msgPtr, "forwardMode", forwardMode);
        nGame::SetNMMessageParam(msgPtr, "timeToForward", timeToForward);
        nGame::SetNMMessageParam(msgPtr, "reboundForce", reboundForce);
        nGame::SetNMMessageParam(msgPtr, "braceWall", braceWall);
        nGame::SetNMMessageParam(msgPtr, "ignoreColVolumeBelow", ignoreColVolumeBelow);
        nGame::SetNMMessageParam(msgPtr, "fallOverWallDrape", fallOverWallDrape);
        nGame::SetNMMessageParam(msgPtr, "fallOverHighWalls", fallOverHighWalls);
        nGame::SetNMMessageParam(msgPtr, "snap", snap);
        nGame::SetNMMessageParam(msgPtr, "snapMag", snapMag);
        nGame::SetNMMessageParam(msgPtr, "snapDirectionRandomness", snapDirectionRandomness);
        nGame::SetNMMessageParam(msgPtr, "snapLeftArm", snapLeftArm);
        nGame::SetNMMessageParam(msgPtr, "snapRightArm", snapRightArm);
        nGame::SetNMMessageParam(msgPtr, "snapLeftLeg", snapLeftLeg);
        nGame::SetNMMessageParam(msgPtr, "snapRightLeg", snapRightLeg);
        nGame::SetNMMessageParam(msgPtr, "snapSpine", snapSpine);
        nGame::SetNMMessageParam(msgPtr, "snapNeck", snapNeck);
        nGame::SetNMMessageParam(msgPtr, "snapPhasedLegs", snapPhasedLegs);
        nGame::SetNMMessageParam(msgPtr, "snapHipType", snapHipType);
        nGame::SetNMMessageParam(msgPtr, "unSnapInterval", unSnapInterval);
        nGame::SetNMMessageParam(msgPtr, "unSnapRatio", unSnapRatio);
        nGame::SetNMMessageParam(msgPtr, "snapUseTorques", snapUseTorques);
        nGame::SetNMMessageParam(msgPtr, "impactWeaknessZeroDuration", impactWeaknessZeroDuration);
        nGame::SetNMMessageParam(msgPtr, "impactWeaknessRampDuration", impactWeaknessRampDuration);
        nGame::SetNMMessageParam(msgPtr, "impactLoosenessAmount", impactLoosenessAmount);
        nGame::SetNMMessageParam(msgPtr, "objectBehindVictim", objectBehindVictim);
        nGame::SetNMMessageParam(msgPtr, "objectBehindVictimPos", objectBehindVictimPos.x, objectBehindVictimPos.y, objectBehindVictimPos.z);
        nGame::SetNMMessageParam(msgPtr, "objectBehindVictimNormal", objectBehindVictimNormal.x, objectBehindVictimNormal.y, objectBehindVictimNormal.z);
        return;
    }
};

struct NMMessageBodyBalance
{
    float armStiffness = 6.0f;
    float elbow = 1.75f;
    float shoulder = 1.0f;
    float armDamping = 0.7f;
    bool useHeadLook = false;
    Vector3 headLookPos{ 0.0f, 0.0f, 0.0f };
    int headLookInstanceIndex = -1;
    float spineStiffness = 6.0f;
    float somersaultAngle = 1.0f;
    float somersaultAngleThreshold = 0.25f;
    float sideSomersaultAngle = 1.0f;
    float sideSomersaultAngleThreshold = 0.25f;
    bool backwardsAutoTurn = false;
    float turnWithBumpRadius = 0.9f;
    bool backwardsArms = false;
    bool blendToZeroPose = false;
    bool armsOutOnPush = true;
    float armsOutOnPushMultiplier = 1.0f;
    float armsOutOnPushTimeout = 2.0f;
    float returningToBalanceArmsOut = 0.0f;
    float armsOutStraightenElbows = 0.0f;
    float armsOutMinLean2 = -9.9f;
    float spineDamping = 1.0f;
    bool useBodyTurn = true;
    float elbowAngleOnContact = 1.9f;
    float bendElbowsTime = 0.3f;
    float bendElbowsGait = 0.7f;
    float hipL2ArmL2 = 0.3f;
    float shoulderL2 = 0.5f;
    float shoulderL1 = 0.0f;
    float shoulderTwist = -0.35f;
    float headLookAtVelProb = -1.0f;
    float turnOffProb = 0.1f;
    float turn2VelProb = 0.3f;
    float turnAwayProb = 0.15f;
    float turnLeftProb = 0.125f;
    float turnRightProb = 0.125f;
    float turn2TargetProb = 0.2f;
    Vector3 angVelMultiplier{ 4.0f, 1.0f, 4.0f };
    Vector3 angVelThreshold{ 1.2f, 3.0f, 1.2f };
    float braceDistance = 1.0f;
    float targetPredictionTime = 0.1f;
    float reachAbsorbtionTime = 1.0f;
    float braceStiffness = 6.0f;
    float minBraceTime = 2.0f;
    float timeToBackwardsBrace = 0.5f;
    float handsDelayMin = 0.3f;
    float handsDelayMax = 0.7f;
    float braceOffset = 0.0f;
    float moveRadius = 2.0f;
    float moveAmount = 0.8f;
    bool moveWhenBracing = true;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "armStiffness", armStiffness);
        nGame::SetNMMessageParam(msgPtr, "elbow", elbow);
        nGame::SetNMMessageParam(msgPtr, "shoulder", shoulder);
        nGame::SetNMMessageParam(msgPtr, "armDamping", armDamping);
        nGame::SetNMMessageParam(msgPtr, "useHeadLook", useHeadLook);
        nGame::SetNMMessageParam(msgPtr, "headLookPos", headLookPos.x, headLookPos.y, headLookPos.z);
        nGame::SetNMMessageParam(msgPtr, "headLookInstanceIndex", headLookInstanceIndex);
        nGame::SetNMMessageParam(msgPtr, "spineStiffness", spineStiffness);
        nGame::SetNMMessageParam(msgPtr, "somersaultAngle", somersaultAngle);
        nGame::SetNMMessageParam(msgPtr, "somersaultAngleThreshold", somersaultAngleThreshold);
        nGame::SetNMMessageParam(msgPtr, "sideSomersaultAngle", sideSomersaultAngle);
        nGame::SetNMMessageParam(msgPtr, "sideSomersaultAngleThreshold", sideSomersaultAngleThreshold);
        nGame::SetNMMessageParam(msgPtr, "backwardsAutoTurn", backwardsAutoTurn);
        nGame::SetNMMessageParam(msgPtr, "turnWithBumpRadius", turnWithBumpRadius);
        nGame::SetNMMessageParam(msgPtr, "backwardsArms", backwardsArms);
        nGame::SetNMMessageParam(msgPtr, "blendToZeroPose", blendToZeroPose);
        nGame::SetNMMessageParam(msgPtr, "armsOutOnPush", armsOutOnPush);
        nGame::SetNMMessageParam(msgPtr, "armsOutOnPushMultiplier", armsOutOnPushMultiplier);
        nGame::SetNMMessageParam(msgPtr, "armsOutOnPushTimeout", armsOutOnPushTimeout);
        nGame::SetNMMessageParam(msgPtr, "returningToBalanceArmsOut", returningToBalanceArmsOut);
        nGame::SetNMMessageParam(msgPtr, "armsOutStraightenElbows", armsOutStraightenElbows);
        nGame::SetNMMessageParam(msgPtr, "armsOutMinLean2", armsOutMinLean2);
        nGame::SetNMMessageParam(msgPtr, "spineDamping", spineDamping);
        nGame::SetNMMessageParam(msgPtr, "useBodyTurn", useBodyTurn);
        nGame::SetNMMessageParam(msgPtr, "elbowAngleOnContact", elbowAngleOnContact);
        nGame::SetNMMessageParam(msgPtr, "bendElbowsTime", bendElbowsTime);
        nGame::SetNMMessageParam(msgPtr, "bendElbowsGait", bendElbowsGait);
        nGame::SetNMMessageParam(msgPtr, "hipL2ArmL2", hipL2ArmL2);
        nGame::SetNMMessageParam(msgPtr, "shoulderL2", shoulderL2);
        nGame::SetNMMessageParam(msgPtr, "shoulderL1", shoulderL1);
        nGame::SetNMMessageParam(msgPtr, "shoulderTwist", shoulderTwist);
        nGame::SetNMMessageParam(msgPtr, "headLookAtVelProb", headLookAtVelProb);
        nGame::SetNMMessageParam(msgPtr, "turnOffProb", turnOffProb);
        nGame::SetNMMessageParam(msgPtr, "turn2VelProb", turn2VelProb);
        nGame::SetNMMessageParam(msgPtr, "turnAwayProb", turnAwayProb);
        nGame::SetNMMessageParam(msgPtr, "turnLeftProb", turnLeftProb);
        nGame::SetNMMessageParam(msgPtr, "turnRightProb", turnRightProb);
        nGame::SetNMMessageParam(msgPtr, "turn2TargetProb", turn2TargetProb);
        nGame::SetNMMessageParam(msgPtr, "angVelMultiplier", angVelMultiplier.x, angVelMultiplier.y, angVelMultiplier.z);
        nGame::SetNMMessageParam(msgPtr, "angVelThreshold", angVelThreshold.x, angVelThreshold.y, angVelThreshold.z);
        nGame::SetNMMessageParam(msgPtr, "braceDistance", braceDistance);
        nGame::SetNMMessageParam(msgPtr, "targetPredictionTime", targetPredictionTime);
        nGame::SetNMMessageParam(msgPtr, "reachAbsorbtionTime", reachAbsorbtionTime);
        nGame::SetNMMessageParam(msgPtr, "braceStiffness", braceStiffness);
        nGame::SetNMMessageParam(msgPtr, "minBraceTime", minBraceTime);
        nGame::SetNMMessageParam(msgPtr, "timeToBackwardsBrace", timeToBackwardsBrace);
        nGame::SetNMMessageParam(msgPtr, "handsDelayMin", handsDelayMin);
        nGame::SetNMMessageParam(msgPtr, "handsDelayMax", handsDelayMax);
        nGame::SetNMMessageParam(msgPtr, "braceOffset", braceOffset);
        nGame::SetNMMessageParam(msgPtr, "moveRadius", moveRadius);
        nGame::SetNMMessageParam(msgPtr, "moveAmount", moveAmount);
        nGame::SetNMMessageParam(msgPtr, "moveWhenBracing", moveWhenBracing);
        return;
    }
};

struct NMMessageBodyFoetal
{
    float stiffness = 6.0f;
    float dampingFactor = 1.4f;
    float asymmetry = 0.0f;
    int randomSeed = 100;
    float backTwist = 0.0f;
    std::string mask = "fb";
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "stiffness", stiffness);
        nGame::SetNMMessageParam(msgPtr, "dampingFactor", dampingFactor);
        nGame::SetNMMessageParam(msgPtr, "asymmetry", asymmetry);
        nGame::SetNMMessageParam(msgPtr, "randomSeed", randomSeed);
        nGame::SetNMMessageParam(msgPtr, "backTwist", backTwist);
        nGame::SetNMMessageParam(msgPtr, "mask", mask.c_str());
        return;
    }
};

struct NMMessageBodyRollUp
{
    float stiffness = 6.0f;
    float useArmToSlowDown = 1.3f;
    float armReachAmount = 1.4f;
    std::string mask = "fb";
    float legPush = -1.0f;
    float asymmetricalLegs = 0.0f;
    float noRollTimeBeforeSuccess = 0.5f;
    float rollVelForSuccess = 0.2f;
    float rollVelLinearContribution = 1.0f;
    float velocityScale = 1.0f;
    float velocityOffset = 0.0f;
    bool applyMinMaxFriction = true;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "stiffness", stiffness);
        nGame::SetNMMessageParam(msgPtr, "useArmToSlowDown", useArmToSlowDown);
        nGame::SetNMMessageParam(msgPtr, "armReachAmount", armReachAmount);
        nGame::SetNMMessageParam(msgPtr, "mask", mask.c_str());
        nGame::SetNMMessageParam(msgPtr, "legPush", legPush);
        nGame::SetNMMessageParam(msgPtr, "asymmetricalLegs", asymmetricalLegs);
        nGame::SetNMMessageParam(msgPtr, "noRollTimeBeforeSuccess", noRollTimeBeforeSuccess);
        nGame::SetNMMessageParam(msgPtr, "rollVelForSuccess", rollVelForSuccess);
        nGame::SetNMMessageParam(msgPtr, "rollVelLinearContribution", rollVelLinearContribution);
        nGame::SetNMMessageParam(msgPtr, "velocityScale", velocityScale);
        nGame::SetNMMessageParam(msgPtr, "velocityOffset", velocityOffset);
        nGame::SetNMMessageParam(msgPtr, "applyMinMaxFriction", applyMinMaxFriction);
        return;
    }
};

struct NMMessageBodyWrithe
{
    float armStiffness = 6.0f;
    float backStiffness = 6.0f;
    float legStiffness = 6.0f;
    float armDamping = 0.5f;
    float backDamping = 0.5f;
    float legDamping = 0.5f;
    float armPeriod = 3.0f;
    float backPeriod = 3.0f;
    float legPeriod = 1.0f;
    std::string mask = "fb";
    float armAmplitude = 1.0f;
    float backAmplitude = 1.0f;
    float legAmplitude = 1.0f;
    float elbowAmplitude = 1.0f;
    float kneeAmplitude = 1.0f;
    bool rollOverFlag = false;
    float blendArms = 1.0f;
    float blendBack = 1.0f;
    float blendLegs = 1.0f;
    bool applyStiffness = false;
    bool onFire = false;
    float shoulderLean1 = 0.7f;
    float shoulderLean2 = 0.4f;
    float lean1BlendFactor = 0.0f;
    float lean2BlendFactor = 0.0f;
    float rollTorqueScale = 150.0f;
    float maxRollOverTime = 8.0f;
    float rollOverRadius = 2.0f;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "armStiffness", armStiffness);
        nGame::SetNMMessageParam(msgPtr, "backStiffness", backStiffness);
        nGame::SetNMMessageParam(msgPtr, "legStiffness", legStiffness);
        nGame::SetNMMessageParam(msgPtr, "armDamping", armDamping);
        nGame::SetNMMessageParam(msgPtr, "backDamping", backDamping);
        nGame::SetNMMessageParam(msgPtr, "legDamping", legDamping);
        nGame::SetNMMessageParam(msgPtr, "armPeriod", armPeriod);
        nGame::SetNMMessageParam(msgPtr, "backPeriod", backPeriod);
        nGame::SetNMMessageParam(msgPtr, "legPeriod", legPeriod);
        nGame::SetNMMessageParam(msgPtr, "mask", mask.c_str());
        nGame::SetNMMessageParam(msgPtr, "armAmplitude", armAmplitude);
        nGame::SetNMMessageParam(msgPtr, "backAmplitude", backAmplitude);
        nGame::SetNMMessageParam(msgPtr, "legAmplitude", legAmplitude);
        nGame::SetNMMessageParam(msgPtr, "elbowAmplitude", elbowAmplitude);
        nGame::SetNMMessageParam(msgPtr, "kneeAmplitude", kneeAmplitude);
        nGame::SetNMMessageParam(msgPtr, "rollOverFlag", rollOverFlag);
        nGame::SetNMMessageParam(msgPtr, "blendArms", blendArms);
        nGame::SetNMMessageParam(msgPtr, "blendBack", blendBack);
        nGame::SetNMMessageParam(msgPtr, "blendLegs", blendLegs);
        nGame::SetNMMessageParam(msgPtr, "applyStiffness", applyStiffness);
        nGame::SetNMMessageParam(msgPtr, "onFire", onFire);
        nGame::SetNMMessageParam(msgPtr, "shoulderLean1", shoulderLean1);
        nGame::SetNMMessageParam(msgPtr, "shoulderLean2", shoulderLean2);
        nGame::SetNMMessageParam(msgPtr, "lean1BlendFactor", lean1BlendFactor);
        nGame::SetNMMessageParam(msgPtr, "lean2BlendFactor", lean2BlendFactor);
        nGame::SetNMMessageParam(msgPtr, "rollTorqueScale", rollTorqueScale);
        nGame::SetNMMessageParam(msgPtr, "maxRollOverTime", maxRollOverTime);
        nGame::SetNMMessageParam(msgPtr, "rollOverRadius", rollOverRadius);
        return;
    }
};

struct NMMessageBraceForImpact
{
    float braceDistance = 0.7f;
    float targetPredictionTime = 0.1f;
    float reachAbsorbtionTime = 1.0f;
    int instanceIndex = -1;
    float bodyStiffness = 6.0f;
    bool grabDontLetGo = true;
    float grabStrength = -1.0f;
    float grabDistance = 2.3f;
    float grabReachAngle = 1.5f;
    float grabHoldTimer = 6.5f;
    bool justBrace = true;
    float maxGrabCarVelocity = 3.0f;
    float timeToBackwardsBrace = 1.0f;
    Vector3 look{ 0.0f, 0.0f, 0.0f };
    Vector3 pos{ 0.0f, 0.0f, 0.0f };
    float minBraceTime = 3.0f;
    float handsDelayMin = 0.1f;
    float handsDelayMax = 0.3f;
    bool moveAway = true;
    float moveAwayAmount = 0.5f;
    float moveAwayLean = -0.3f;
    float moveSideways = 4.3f;
    bool bbArms = false;
    bool newBrace = true;
    bool braceOnImpact = true;
    bool roll2Velocity = false;
    int rollType = 0;
    bool snapImpacts = true;
    float snapImpact = 7.0f;
    float snapBonnet = -7.0f;
    float snapFloor = 7.0f;
    bool dampVel = false;
    float dampSpin = 0.0f;
    float dampUpVel = 10.0f;
    float dampSpinThresh = 4.0f;
    float dampUpVelThresh = 2.0f;
    bool gsHelp = false;
    float gsEndMin = -0.1f;
    float gsSideMin = -0.2f;
    float gsSideMax = 0.5f;
    float gsUpness = 0.9f;
    float gsCarVelMin = 3.0f;
    bool gsScale1Foot = true;
    float gsFricScale1 = 8.0f;
    std::string gsFricMask1 = "fb";
    float gsFricScale2 = 0.2f;
    std::string gsFricMask2 = "la";
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "braceDistance", braceDistance);
        nGame::SetNMMessageParam(msgPtr, "targetPredictionTime", targetPredictionTime);
        nGame::SetNMMessageParam(msgPtr, "reachAbsorbtionTime", reachAbsorbtionTime);
        nGame::SetNMMessageParam(msgPtr, "instanceIndex", instanceIndex);
        nGame::SetNMMessageParam(msgPtr, "bodyStiffness", bodyStiffness);
        nGame::SetNMMessageParam(msgPtr, "grabDontLetGo", grabDontLetGo);
        nGame::SetNMMessageParam(msgPtr, "grabStrength", grabStrength);
        nGame::SetNMMessageParam(msgPtr, "grabDistance", grabDistance);
        nGame::SetNMMessageParam(msgPtr, "grabReachAngle", grabReachAngle);
        nGame::SetNMMessageParam(msgPtr, "grabHoldTimer", grabHoldTimer);
        nGame::SetNMMessageParam(msgPtr, "justBrace", justBrace);
        nGame::SetNMMessageParam(msgPtr, "maxGrabCarVelocity", maxGrabCarVelocity);
        nGame::SetNMMessageParam(msgPtr, "timeToBackwardsBrace", timeToBackwardsBrace);
        nGame::SetNMMessageParam(msgPtr, "look", look.x, look.y, look.z);
        nGame::SetNMMessageParam(msgPtr, "pos", pos.x, pos.y, pos.z);
        nGame::SetNMMessageParam(msgPtr, "minBraceTime", minBraceTime);
        nGame::SetNMMessageParam(msgPtr, "handsDelayMin", handsDelayMin);
        nGame::SetNMMessageParam(msgPtr, "handsDelayMax", handsDelayMax);
        nGame::SetNMMessageParam(msgPtr, "moveAway", moveAway);
        nGame::SetNMMessageParam(msgPtr, "moveAwayAmount", moveAwayAmount);
        nGame::SetNMMessageParam(msgPtr, "moveAwayLean", moveAwayLean);
        nGame::SetNMMessageParam(msgPtr, "moveSideways", moveSideways);
        nGame::SetNMMessageParam(msgPtr, "bbArms", bbArms);
        nGame::SetNMMessageParam(msgPtr, "newBrace", newBrace);
        nGame::SetNMMessageParam(msgPtr, "braceOnImpact", braceOnImpact);
        nGame::SetNMMessageParam(msgPtr, "roll2Velocity", roll2Velocity);
        nGame::SetNMMessageParam(msgPtr, "rollType", rollType);
        nGame::SetNMMessageParam(msgPtr, "snapImpacts", snapImpacts);
        nGame::SetNMMessageParam(msgPtr, "snapImpact", snapImpact);
        nGame::SetNMMessageParam(msgPtr, "snapBonnet", snapBonnet);
        nGame::SetNMMessageParam(msgPtr, "snapFloor", snapFloor);
        nGame::SetNMMessageParam(msgPtr, "dampVel", dampVel);
        nGame::SetNMMessageParam(msgPtr, "dampSpin", dampSpin);
        nGame::SetNMMessageParam(msgPtr, "dampUpVel", dampUpVel);
        nGame::SetNMMessageParam(msgPtr, "dampSpinThresh", dampSpinThresh);
        nGame::SetNMMessageParam(msgPtr, "dampUpVelThresh", dampUpVelThresh);
        nGame::SetNMMessageParam(msgPtr, "gsHelp", gsHelp);
        nGame::SetNMMessageParam(msgPtr, "gsEndMin", gsEndMin);
        nGame::SetNMMessageParam(msgPtr, "gsSideMin", gsSideMin);
        nGame::SetNMMessageParam(msgPtr, "gsSideMax", gsSideMax);
        nGame::SetNMMessageParam(msgPtr, "gsUpness", gsUpness);
        nGame::SetNMMessageParam(msgPtr, "gsCarVelMin", gsCarVelMin);
        nGame::SetNMMessageParam(msgPtr, "gsScale1Foot", gsScale1Foot);
        nGame::SetNMMessageParam(msgPtr, "gsFricScale1", gsFricScale1);
        nGame::SetNMMessageParam(msgPtr, "gsFricMask1", gsFricMask1.c_str());
        nGame::SetNMMessageParam(msgPtr, "gsFricScale2", gsFricScale2);
        nGame::SetNMMessageParam(msgPtr, "gsFricMask2", gsFricMask2.c_str());
        return;
    }
};

struct NMMessageBuoyancy
{
    Vector3 surfacePoint{ 0.0f, 0.0f, 0.0f };
    Vector3 surfaceNormal{ 0.0f, 0.0f, 1.0f };
    float buoyancy = 1.0f;
    float chestBuoyancy = 8.0f;
    float damping = 40.0f;
    bool righting = true;
    float rightingStrength = 25.0f;
    float rightingTime = 1.0f;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "surfacePoint", surfacePoint.x, surfacePoint.y, surfacePoint.z);
        nGame::SetNMMessageParam(msgPtr, "surfaceNormal", surfaceNormal.x, surfaceNormal.y, surfaceNormal.z);
        nGame::SetNMMessageParam(msgPtr, "buoyancy", buoyancy);
        nGame::SetNMMessageParam(msgPtr, "chestBuoyancy", chestBuoyancy);
        nGame::SetNMMessageParam(msgPtr, "damping", damping);
        nGame::SetNMMessageParam(msgPtr, "righting", righting);
        nGame::SetNMMessageParam(msgPtr, "rightingStrength", rightingStrength);
        nGame::SetNMMessageParam(msgPtr, "rightingTime", rightingTime);
        return;
    }
};

struct NMMessageCatchFall
{
    float torsoStiffness = 6.0f;
    float legsStiffness = 4.0f;
    float armsStiffness = 6.0f;
    float backwardsMinArmOffset = -0.25f;
    float forwardMaxArmOffset = 0.45f;
    float zAxisSpinReduction = 0.0f;
    float extraSit = 1.0f;
    bool useHeadLook = true;
    std::string mask = "fb";
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "torsoStiffness", torsoStiffness);
        nGame::SetNMMessageParam(msgPtr, "legsStiffness", legsStiffness);
        nGame::SetNMMessageParam(msgPtr, "armsStiffness", armsStiffness);
        nGame::SetNMMessageParam(msgPtr, "backwardsMinArmOffset", backwardsMinArmOffset);
        nGame::SetNMMessageParam(msgPtr, "forwardMaxArmOffset", forwardMaxArmOffset);
        nGame::SetNMMessageParam(msgPtr, "zAxisSpinReduction", zAxisSpinReduction);
        nGame::SetNMMessageParam(msgPtr, "extraSit", extraSit);
        nGame::SetNMMessageParam(msgPtr, "useHeadLook", useHeadLook);
        nGame::SetNMMessageParam(msgPtr, "mask", mask.c_str());
        return;
    }
};

struct NMMessageElectrocute
{
    float stunMag = 0.250f;
    float initialMult = 1.0f;
    float largeMult = 1.0f;
    float largeMinTime = 1.0f;
    float largeMaxTime = 2.0f;
    float movingMult = 1.0f;
    float balancingMult = 1.0f;
    float airborneMult = 1.0f;
    float movingThresh = 1.0f;
    float stunInterval = 0.070f;
    float directionRandomness = 0.3f;
    bool leftArm = true;
    bool rightArm = true;
    bool leftLeg = true;
    bool rightLeg = true;
    bool spine = true;
    bool neck = true;
    bool phasedLegs = true;
    bool applyStiffness = true;
    bool useTorques = true;
    int hipType = 2;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "stunMag", stunMag);
        nGame::SetNMMessageParam(msgPtr, "initialMult", initialMult);
        nGame::SetNMMessageParam(msgPtr, "largeMult", largeMult);
        nGame::SetNMMessageParam(msgPtr, "largeMinTime", largeMinTime);
        nGame::SetNMMessageParam(msgPtr, "largeMaxTime", largeMaxTime);
        nGame::SetNMMessageParam(msgPtr, "movingMult", movingMult);
        nGame::SetNMMessageParam(msgPtr, "balancingMult", balancingMult);
        nGame::SetNMMessageParam(msgPtr, "airborneMult", airborneMult);
        nGame::SetNMMessageParam(msgPtr, "movingThresh", movingThresh);
        nGame::SetNMMessageParam(msgPtr, "stunInterval", stunInterval);
        nGame::SetNMMessageParam(msgPtr, "directionRandomness", directionRandomness);
        nGame::SetNMMessageParam(msgPtr, "leftArm", leftArm);
        nGame::SetNMMessageParam(msgPtr, "rightArm", rightArm);
        nGame::SetNMMessageParam(msgPtr, "leftLeg", leftLeg);
        nGame::SetNMMessageParam(msgPtr, "rightLeg", rightLeg);
        nGame::SetNMMessageParam(msgPtr, "spine", spine);
        nGame::SetNMMessageParam(msgPtr, "neck", neck);
        nGame::SetNMMessageParam(msgPtr, "phasedLegs", phasedLegs);
        nGame::SetNMMessageParam(msgPtr, "applyStiffness", applyStiffness);
        nGame::SetNMMessageParam(msgPtr, "useTorques", useTorques);
        nGame::SetNMMessageParam(msgPtr, "hipType", hipType);
        return;
    }
};

struct NMMessageFallOverWall
{
    float bodyStiffness = 6.0f;
    float damping = 0.5f;
    float magOfForce = 0.6f;
    float maxDistanceFromPelToHitPoint = 0.250f;
    float maxForceDist = 1.0f;
    float stepExclusionZone = 0.2f;
    float minLegHeight = 0.4f;
    float bodyTwist = 0.540f;
    float maxTwist = 3.141593f;
    Vector3 fallOverWallEndA{ 0.0f, 0.0f, 0.0f };
    Vector3 fallOverWallEndB{ 0.0f, 0.0f, 0.0f };
    float forceAngleAbort = -0.2f;
    float forceTimeOut = 2.0f;
    bool moveArms = true;
    bool moveLegs = true;
    bool bendSpine = true;
    float angleDirWithWallNormal = 180.0f;
    float leaningAngleThreshold = 180.0f;
    float maxAngVel = 2.0f;
    bool adaptForcesToLowWall = false;
    float maxWallHeight = 1.5f;
    float distanceToSendSuccessMessage = 0.25f;
    float rollingBackThr = 0.5f;
    float rollingPotential = 0.1f;
    bool useArmIK = false;
    float reachDistanceFromHitPoint = 0.3f;
    float minReachDistanceFromHitPoint = 0.1f;
    float angleTotallyBack = 15.0f;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "bodyStiffness", bodyStiffness);
        nGame::SetNMMessageParam(msgPtr, "damping", damping);
        nGame::SetNMMessageParam(msgPtr, "magOfForce", magOfForce);
        nGame::SetNMMessageParam(msgPtr, "maxDistanceFromPelToHitPoint", maxDistanceFromPelToHitPoint);
        nGame::SetNMMessageParam(msgPtr, "maxForceDist", maxForceDist);
        nGame::SetNMMessageParam(msgPtr, "stepExclusionZone", stepExclusionZone);
        nGame::SetNMMessageParam(msgPtr, "minLegHeight", minLegHeight);
        nGame::SetNMMessageParam(msgPtr, "bodyTwist", bodyTwist);
        nGame::SetNMMessageParam(msgPtr, "maxTwist", maxTwist);
        nGame::SetNMMessageParam(msgPtr, "fallOverWallEndA", fallOverWallEndA.x, fallOverWallEndA.y, fallOverWallEndA.z);
        nGame::SetNMMessageParam(msgPtr, "fallOverWallEndB", fallOverWallEndB.x, fallOverWallEndB.y, fallOverWallEndB.z);
        nGame::SetNMMessageParam(msgPtr, "forceAngleAbort", forceAngleAbort);
        nGame::SetNMMessageParam(msgPtr, "forceTimeOut", forceTimeOut);
        nGame::SetNMMessageParam(msgPtr, "moveArms", moveArms);
        nGame::SetNMMessageParam(msgPtr, "moveLegs", moveLegs);
        nGame::SetNMMessageParam(msgPtr, "bendSpine", bendSpine);
        nGame::SetNMMessageParam(msgPtr, "angleDirWithWallNormal", angleDirWithWallNormal);
        nGame::SetNMMessageParam(msgPtr, "leaningAngleThreshold", leaningAngleThreshold);
        nGame::SetNMMessageParam(msgPtr, "maxAngVel", maxAngVel);
        nGame::SetNMMessageParam(msgPtr, "adaptForcesToLowWall", adaptForcesToLowWall);
        nGame::SetNMMessageParam(msgPtr, "maxWallHeight", maxWallHeight);
        nGame::SetNMMessageParam(msgPtr, "distanceToSendSuccessMessage", distanceToSendSuccessMessage);
        nGame::SetNMMessageParam(msgPtr, "rollingBackThr", rollingBackThr);
        nGame::SetNMMessageParam(msgPtr, "rollingPotential", rollingPotential);
        nGame::SetNMMessageParam(msgPtr, "useArmIK", useArmIK);
        nGame::SetNMMessageParam(msgPtr, "reachDistanceFromHitPoint", reachDistanceFromHitPoint);
        nGame::SetNMMessageParam(msgPtr, "minReachDistanceFromHitPoint", minReachDistanceFromHitPoint);
        nGame::SetNMMessageParam(msgPtr, "angleTotallyBack", angleTotallyBack);
        return;
    }
};

struct NMMessageGrab
{
    bool useLeft = false;
    bool useRight = false;
    bool dropWeaponIfNecessary = true;
    float dropWeaponDistance = 0.3f;
    float grabStrength = 600.0f;
    float stickyHands = 4.0f;
    int turnToTarget = 1;
    float grabHoldMaxTimer = 100.0f;
    float pullUpTime = 1.0f;
    float pullUpStrengthRight = 0.0f;
    float pullUpStrengthLeft = 0.0f;
    Vector3 pos1{ 0.0f, 0.0f, 0.0f };
    Vector3 pos2{ 0.0f, 0.0f, 0.0f };
    Vector3 pos3{ 0.0f, 0.0f, 0.0f };
    Vector3 pos4{ 0.0f, 0.0f, 0.0f };
    Vector3 normalR{ 0.0f, 0.0f, 0.0f };
    Vector3 normalL{ 0.0f, 0.0f, 0.0f };
    Vector3 normalR2{ 0.0f, 0.0f, 0.0f };
    Vector3 normalL2{ 0.0f, 0.0f, 0.0f };
    bool handsCollide = false;
    bool justBrace = false;
    bool useLineGrab = false;
    bool pointsX4grab = false;
    bool fromEA = false;
    bool surfaceGrab = false;
    int instanceIndex = -1;
    int instancePartIndex = 0;
    bool dontLetGo = false;
    float bodyStiffness = 6.0f;
    float reachAngle = 2.8f;
    float oneSideReachAngle = 1.4f;
    float grabDistance = 0.45f;
    float move2Radius = 0.0f;
    float armStiffness = 6.0f;
    float maxReachDistance = 0.7f;
    float orientationConstraintScale = 1.0f;
    float maxWristAngle = 3.141593f;
    bool useHeadLookToTarget = false;
    bool lookAtGrab = true;
    Vector3 targetForHeadLook{ 0.0f, 0.0f, 0.0f };
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "useLeft", useLeft);
        nGame::SetNMMessageParam(msgPtr, "useRight", useRight);
        nGame::SetNMMessageParam(msgPtr, "dropWeaponIfNecessary", dropWeaponIfNecessary);
        nGame::SetNMMessageParam(msgPtr, "dropWeaponDistance", dropWeaponDistance);
        nGame::SetNMMessageParam(msgPtr, "grabStrength", grabStrength);
        nGame::SetNMMessageParam(msgPtr, "stickyHands", stickyHands);
        nGame::SetNMMessageParam(msgPtr, "turnToTarget", turnToTarget);
        nGame::SetNMMessageParam(msgPtr, "grabHoldMaxTimer", grabHoldMaxTimer);
        nGame::SetNMMessageParam(msgPtr, "pullUpTime", pullUpTime);
        nGame::SetNMMessageParam(msgPtr, "pullUpStrengthRight", pullUpStrengthRight);
        nGame::SetNMMessageParam(msgPtr, "pullUpStrengthLeft", pullUpStrengthLeft);
        nGame::SetNMMessageParam(msgPtr, "pos1", pos1.x, pos1.y, pos1.z);
        nGame::SetNMMessageParam(msgPtr, "pos2", pos2.x, pos2.y, pos2.z);
        nGame::SetNMMessageParam(msgPtr, "pos3", pos3.x, pos3.y, pos3.z);
        nGame::SetNMMessageParam(msgPtr, "pos4", pos4.x, pos4.y, pos4.z);
        nGame::SetNMMessageParam(msgPtr, "normalR", normalR.x, normalR.y, normalR.z);
        nGame::SetNMMessageParam(msgPtr, "normalL", normalL.x, normalL.y, normalL.z);
        nGame::SetNMMessageParam(msgPtr, "normalR2", normalR2.x, normalR2.y, normalR2.z);
        nGame::SetNMMessageParam(msgPtr, "normalL2", normalL2.x, normalL2.y, normalL2.z);
        nGame::SetNMMessageParam(msgPtr, "handsCollide", handsCollide);
        nGame::SetNMMessageParam(msgPtr, "justBrace", justBrace);
        nGame::SetNMMessageParam(msgPtr, "useLineGrab", useLineGrab);
        nGame::SetNMMessageParam(msgPtr, "pointsX4grab", pointsX4grab);
        nGame::SetNMMessageParam(msgPtr, "fromEA", fromEA);
        nGame::SetNMMessageParam(msgPtr, "surfaceGrab", surfaceGrab);
        nGame::SetNMMessageParam(msgPtr, "instanceIndex", instanceIndex);
        nGame::SetNMMessageParam(msgPtr, "instancePartIndex", instancePartIndex);
        nGame::SetNMMessageParam(msgPtr, "dontLetGo", dontLetGo);
        nGame::SetNMMessageParam(msgPtr, "bodyStiffness", bodyStiffness);
        nGame::SetNMMessageParam(msgPtr, "reachAngle", reachAngle);
        nGame::SetNMMessageParam(msgPtr, "oneSideReachAngle", oneSideReachAngle);
        nGame::SetNMMessageParam(msgPtr, "grabDistance", grabDistance);
        nGame::SetNMMessageParam(msgPtr, "move2Radius", move2Radius);
        nGame::SetNMMessageParam(msgPtr, "armStiffness", armStiffness);
        nGame::SetNMMessageParam(msgPtr, "maxReachDistance", maxReachDistance);
        nGame::SetNMMessageParam(msgPtr, "orientationConstraintScale", orientationConstraintScale);
        nGame::SetNMMessageParam(msgPtr, "maxWristAngle", maxWristAngle);
        nGame::SetNMMessageParam(msgPtr, "useHeadLookToTarget", useHeadLookToTarget);
        nGame::SetNMMessageParam(msgPtr, "lookAtGrab", lookAtGrab);
        nGame::SetNMMessageParam(msgPtr, "targetForHeadLook", targetForHeadLook.x, targetForHeadLook.y, targetForHeadLook.z);
        return;
    }
};

struct NMMessageHeadLook
{
    float damping = 1.0f;
    float stiffness = 6.0f;
    int instanceIndex = -1;
    Vector3 vel{ 0.0f, 0.0f, 0.0f };
    Vector3 pos{ 0.0f, 0.0f, 0.0f };
    bool alwaysLook = false;
    bool eyesHorizontal = true;
    bool alwaysEyesHorizontal = true;
    bool keepHeadAwayFromGround = false;
    bool twistSpine = true;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "damping", damping);
        nGame::SetNMMessageParam(msgPtr, "stiffness", stiffness);
        nGame::SetNMMessageParam(msgPtr, "instanceIndex", instanceIndex);
        nGame::SetNMMessageParam(msgPtr, "vel", vel.x, vel.y, vel.z);
        nGame::SetNMMessageParam(msgPtr, "pos", pos.x, pos.y, pos.z);
        nGame::SetNMMessageParam(msgPtr, "alwaysLook", alwaysLook);
        nGame::SetNMMessageParam(msgPtr, "eyesHorizontal", eyesHorizontal);
        nGame::SetNMMessageParam(msgPtr, "alwaysEyesHorizontal", alwaysEyesHorizontal);
        nGame::SetNMMessageParam(msgPtr, "keepHeadAwayFromGround", keepHeadAwayFromGround);
        nGame::SetNMMessageParam(msgPtr, "twistSpine", twistSpine);
        return;
    }
};

struct NMMessageHighFall
{
    float bodyStiffness = 6.0f;
    float bodydamping = 1.0f;
    float catchfalltime = 0.2f;
    float crashOrLandCutOff = 0.52f;
    float pdStrength = 0.3f;
    float pdDamping = 1.0f;
    float armAngSpeed = 7.850f;
    float armAmplitude = 2.0f;
    float armPhase = 3.1f;
    bool armBendElbows = true;
    float legRadius = 0.25f;
    float legAngSpeed = 5.0f;
    float legAsymmetry = 4.0f;
    float arms2LegsPhase = 0.0f;
    int arms2LegsSync = 1;
    float armsUp = 1.0f;
    bool orientateBodyToFallDirection = true;
    bool orientateTwist = true;
    float orientateMax = 300.0f;
    bool alanRickman = false;
    bool fowardRoll = false;
    bool useZeroPose_withFowardRoll = false;
    float aimAngleBase = 0.180f;
    float fowardVelRotation = -0.050f;
    float footVelCompScale = 0.050f;
    float sideD = 0.15f;
    float fowardOffsetOfLegIK = 0.2f;
    float legL = 0.8f;
    float catchFallCutOff = 0.4f;
    float legStrength = 12.0f;
    bool balance = true;
    bool ignorWorldCollisions = false;
    bool adaptiveCircling = true;
    bool hula = true;
    float maxSpeedForRecoverableFall = 15.0f;
    float minSpeedForBrace = 10.0f;
    float landingNormal = 0.6f;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "bodyStiffness", bodyStiffness);
        nGame::SetNMMessageParam(msgPtr, "bodydamping", bodydamping);
        nGame::SetNMMessageParam(msgPtr, "catchfalltime", catchfalltime);
        nGame::SetNMMessageParam(msgPtr, "crashOrLandCutOff", crashOrLandCutOff);
        nGame::SetNMMessageParam(msgPtr, "pdStrength", pdStrength);
        nGame::SetNMMessageParam(msgPtr, "pdDamping", pdDamping);
        nGame::SetNMMessageParam(msgPtr, "armAngSpeed", armAngSpeed);
        nGame::SetNMMessageParam(msgPtr, "armAmplitude", armAmplitude);
        nGame::SetNMMessageParam(msgPtr, "armPhase", armPhase);
        nGame::SetNMMessageParam(msgPtr, "armBendElbows", armBendElbows);
        nGame::SetNMMessageParam(msgPtr, "legRadius", legRadius);
        nGame::SetNMMessageParam(msgPtr, "legAngSpeed", legAngSpeed);
        nGame::SetNMMessageParam(msgPtr, "legAsymmetry", legAsymmetry);
        nGame::SetNMMessageParam(msgPtr, "arms2LegsPhase", arms2LegsPhase);
        nGame::SetNMMessageParam(msgPtr, "arms2LegsSync", arms2LegsSync);
        nGame::SetNMMessageParam(msgPtr, "armsUp", armsUp);
        nGame::SetNMMessageParam(msgPtr, "orientateBodyToFallDirection", orientateBodyToFallDirection);
        nGame::SetNMMessageParam(msgPtr, "orientateTwist", orientateTwist);
        nGame::SetNMMessageParam(msgPtr, "orientateMax", orientateMax);
        nGame::SetNMMessageParam(msgPtr, "alanRickman", alanRickman);
        nGame::SetNMMessageParam(msgPtr, "fowardRoll", fowardRoll);
        nGame::SetNMMessageParam(msgPtr, "useZeroPose_withFowardRoll", useZeroPose_withFowardRoll);
        nGame::SetNMMessageParam(msgPtr, "aimAngleBase", aimAngleBase);
        nGame::SetNMMessageParam(msgPtr, "fowardVelRotation", fowardVelRotation);
        nGame::SetNMMessageParam(msgPtr, "footVelCompScale", footVelCompScale);
        nGame::SetNMMessageParam(msgPtr, "sideD", sideD);
        nGame::SetNMMessageParam(msgPtr, "fowardOffsetOfLegIK", fowardOffsetOfLegIK);
        nGame::SetNMMessageParam(msgPtr, "legL", legL);
        nGame::SetNMMessageParam(msgPtr, "catchFallCutOff", catchFallCutOff);
        nGame::SetNMMessageParam(msgPtr, "legStrength", legStrength);
        nGame::SetNMMessageParam(msgPtr, "balance", balance);
        nGame::SetNMMessageParam(msgPtr, "ignorWorldCollisions", ignorWorldCollisions);
        nGame::SetNMMessageParam(msgPtr, "adaptiveCircling", adaptiveCircling);
        nGame::SetNMMessageParam(msgPtr, "hula", hula);
        nGame::SetNMMessageParam(msgPtr, "maxSpeedForRecoverableFall", maxSpeedForRecoverableFall);
        nGame::SetNMMessageParam(msgPtr, "minSpeedForBrace", minSpeedForBrace);
        nGame::SetNMMessageParam(msgPtr, "landingNormal", landingNormal);
        return;
    }
};

struct NMMessageInjuredOnGround
{
    int numInjuries = 1;
    int injury1Component = 0;
    int injury2Component = 0;
    Vector3 injury1LocalPosition{ 0.0f, 0.0f, 0.0f };
    Vector3 injury2LocalPosition{ 0.0f, 0.0f, 0.0f };
    Vector3 injury1LocalNormal{ 1.0f, 0.0f, 0.0f };
    Vector3 injury2LocalNormal{ 1.0f, 0.0f, 0.0f };
    Vector3 attackerPos{ 1.0f, 0.0f, 0.0f };
    bool dontReachWithLeft = false;
    bool dontReachWithRight = false;
    bool strongRollForce = false;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "numInjuries", numInjuries);
        nGame::SetNMMessageParam(msgPtr, "injury1Component", injury1Component);
        nGame::SetNMMessageParam(msgPtr, "injury2Component", injury2Component);
        nGame::SetNMMessageParam(msgPtr, "injury1LocalPosition", injury1LocalPosition.x, injury1LocalPosition.y, injury1LocalPosition.z);
        nGame::SetNMMessageParam(msgPtr, "injury2LocalPosition", injury2LocalPosition.x, injury2LocalPosition.y, injury2LocalPosition.z);
        nGame::SetNMMessageParam(msgPtr, "injury1LocalNormal", injury1LocalNormal.x, injury1LocalNormal.y, injury1LocalNormal.z);
        nGame::SetNMMessageParam(msgPtr, "injury2LocalNormal", injury2LocalNormal.x, injury2LocalNormal.y, injury2LocalNormal.z);
        nGame::SetNMMessageParam(msgPtr, "attackerPos", attackerPos.x, attackerPos.y, attackerPos.z);
        nGame::SetNMMessageParam(msgPtr, "dontReachWithLeft", dontReachWithLeft);
        nGame::SetNMMessageParam(msgPtr, "dontReachWithRight", dontReachWithRight);
        nGame::SetNMMessageParam(msgPtr, "strongRollForce", strongRollForce);
        return;
    }
};

struct NMMessageDangle
{
    bool doGrab = true;
    float grabFrequency = 1.0f;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "doGrab", doGrab);
        nGame::SetNMMessageParam(msgPtr, "grabFrequency", grabFrequency);
        return;
    }
};

struct NMMessageOnFire
{
    float staggerTime = 3.0f;
    float staggerLeanRate = 0.3f;
    float stumbleMaxLeanBack = 0.1f;
    float stumbleMaxLeanForward = 0.2f;
    float armsWindmillWritheBlend = 0.4f;
    float spineStumbleWritheBlend = 0.7f;
    float legsStumbleWritheBlend = 0.2f;
    float armsPoseWritheBlend = 0.7f;
    float spinePoseWritheBlend = 0.550f;
    float legsPoseWritheBlend = 0.5f;
    bool rollOverFlag = true;
    float rollTorqueScale = 50.0f;
    float predictTime = 0.1f;
    float maxRollOverTime = 60.0f;
    float rollOverRadius = 5.0f;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "staggerTime", staggerTime);
        nGame::SetNMMessageParam(msgPtr, "staggerLeanRate", staggerLeanRate);
        nGame::SetNMMessageParam(msgPtr, "stumbleMaxLeanBack", stumbleMaxLeanBack);
        nGame::SetNMMessageParam(msgPtr, "stumbleMaxLeanForward", stumbleMaxLeanForward);
        nGame::SetNMMessageParam(msgPtr, "armsWindmillWritheBlend", armsWindmillWritheBlend);
        nGame::SetNMMessageParam(msgPtr, "spineStumbleWritheBlend", spineStumbleWritheBlend);
        nGame::SetNMMessageParam(msgPtr, "legsStumbleWritheBlend", legsStumbleWritheBlend);
        nGame::SetNMMessageParam(msgPtr, "armsPoseWritheBlend", armsPoseWritheBlend);
        nGame::SetNMMessageParam(msgPtr, "spinePoseWritheBlend", spinePoseWritheBlend);
        nGame::SetNMMessageParam(msgPtr, "legsPoseWritheBlend", legsPoseWritheBlend);
        nGame::SetNMMessageParam(msgPtr, "rollOverFlag", rollOverFlag);
        nGame::SetNMMessageParam(msgPtr, "rollTorqueScale", rollTorqueScale);
        nGame::SetNMMessageParam(msgPtr, "predictTime", predictTime);
        nGame::SetNMMessageParam(msgPtr, "maxRollOverTime", maxRollOverTime);
        nGame::SetNMMessageParam(msgPtr, "rollOverRadius", rollOverRadius);
        return;
    }
};

struct NMMessagePedalLegs
{
    bool pedalLeftLeg = true;
    bool pedalRightLeg = true;
    bool backPedal = true;
    float radius = 0.450f;
    float angularSpeed = 10.0f;
    float legStiffness = 6.0f;
    float pedalOffset = 0.15f;
    int randomSeed = 100;
    float speedAsymmetry = 0.0f;
    bool adaptivePedal4Dragging = false;
    float angSpeedMultiplier4Dragging = 0.3f;
    float radiusVariance = 0.4f;
    float legAngleVariance = 0.5f;
    float centreSideways = 0.0f;
    float centreForwards = 0.0f;
    float centreUp = 0.0f;
    float ellipse = 1.0f;
    float dragReduction = 0.250f;
    float spread = 0.0f;
    bool hula = false;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "pedalLeftLeg", pedalLeftLeg);
        nGame::SetNMMessageParam(msgPtr, "pedalRightLeg", pedalRightLeg);
        nGame::SetNMMessageParam(msgPtr, "backPedal", backPedal);
        nGame::SetNMMessageParam(msgPtr, "radius", radius);
        nGame::SetNMMessageParam(msgPtr, "angularSpeed", angularSpeed);
        nGame::SetNMMessageParam(msgPtr, "legStiffness", legStiffness);
        nGame::SetNMMessageParam(msgPtr, "pedalOffset", pedalOffset);
        nGame::SetNMMessageParam(msgPtr, "randomSeed", randomSeed);
        nGame::SetNMMessageParam(msgPtr, "speedAsymmetry", speedAsymmetry);
        nGame::SetNMMessageParam(msgPtr, "adaptivePedal4Dragging", adaptivePedal4Dragging);
        nGame::SetNMMessageParam(msgPtr, "angSpeedMultiplier4Dragging", angSpeedMultiplier4Dragging);
        nGame::SetNMMessageParam(msgPtr, "radiusVariance", radiusVariance);
        nGame::SetNMMessageParam(msgPtr, "legAngleVariance", legAngleVariance);
        nGame::SetNMMessageParam(msgPtr, "centreSideways", centreSideways);
        nGame::SetNMMessageParam(msgPtr, "centreForwards", centreForwards);
        nGame::SetNMMessageParam(msgPtr, "centreUp", centreUp);
        nGame::SetNMMessageParam(msgPtr, "ellipse", ellipse);
        nGame::SetNMMessageParam(msgPtr, "dragReduction", dragReduction);
        nGame::SetNMMessageParam(msgPtr, "spread", spread);
        nGame::SetNMMessageParam(msgPtr, "hula", hula);
        return;
    }
};

struct NMMessagePointArm
{
    Vector3 targetLeft{ 0.0f, 0.0f, 0.0f };
    float twistLeft = 0.3f;
    float armStraightnessLeft = 0.8f;
    bool useLeftArm = false;
    float armStiffnessLeft = 6.0f;
    float armDampingLeft = 1.0f;
    int instanceIndexLeft = -1;
    float pointSwingLimitLeft = 1.5f;
    bool useZeroPoseWhenNotPointingLeft = false;
    Vector3 targetRight{ 0.0f, 0.0f, 0.0f };
    float twistRight = 0.3f;
    float armStraightnessRight = 0.8f;
    bool useRightArm = false;
    float armStiffnessRight = 6.0f;
    float armDampingRight = 1.0f;
    int instanceIndexRight = -1;
    float pointSwingLimitRight = 1.5f;
    bool useZeroPoseWhenNotPointingRight = false;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "targetLeft", targetLeft.x, targetLeft.y, targetLeft.z);
        nGame::SetNMMessageParam(msgPtr, "twistLeft", twistLeft);
        nGame::SetNMMessageParam(msgPtr, "armStraightnessLeft", armStraightnessLeft);
        nGame::SetNMMessageParam(msgPtr, "useLeftArm", useLeftArm);
        nGame::SetNMMessageParam(msgPtr, "armStiffnessLeft", armStiffnessLeft);
        nGame::SetNMMessageParam(msgPtr, "armDampingLeft", armDampingLeft);
        nGame::SetNMMessageParam(msgPtr, "instanceIndexLeft", instanceIndexLeft);
        nGame::SetNMMessageParam(msgPtr, "pointSwingLimitLeft", pointSwingLimitLeft);
        nGame::SetNMMessageParam(msgPtr, "useZeroPoseWhenNotPointingLeft", useZeroPoseWhenNotPointingLeft);
        nGame::SetNMMessageParam(msgPtr, "targetRight", targetRight.x, targetRight.y, targetRight.z);
        nGame::SetNMMessageParam(msgPtr, "twistRight", twistRight);
        nGame::SetNMMessageParam(msgPtr, "armStraightnessRight", armStraightnessRight);
        nGame::SetNMMessageParam(msgPtr, "useRightArm", useRightArm);
        nGame::SetNMMessageParam(msgPtr, "armStiffnessRight", armStiffnessRight);
        nGame::SetNMMessageParam(msgPtr, "armDampingRight", armDampingRight);
        nGame::SetNMMessageParam(msgPtr, "instanceIndexRight", instanceIndexRight);
        nGame::SetNMMessageParam(msgPtr, "pointSwingLimitRight", pointSwingLimitRight);
        nGame::SetNMMessageParam(msgPtr, "useZeroPoseWhenNotPointingRight", useZeroPoseWhenNotPointingRight);
        return;
    }
};

struct NMMessagePointGun
{
    bool enableRight = true;
    bool enableLeft = true;
    Vector3 leftHandTarget{ 0.0f, 0.0f, 0.0f };
    int leftHandTargetIndex = -1;
    Vector3 rightHandTarget{ 0.0f, 0.0f, 0.0f };
    int rightHandTargetIndex = -1;
    float leadTarget = 0.0f;
    float armStiffness = 2.0f;
    float armStiffnessDetSupport = 8.0f;
    float armDamping = 1.0f;
    float gravityOpposition = 1.0f;
    float gravOppDetachedSupport = 0.5f;
    float massMultDetachedSupport = 0.1f;
    bool allowShotLooseness = false;
    float clavicleBlend = 0.0f;
    float elbowAttitude = 0.3f;
    int supportConstraint = 1;
    float constraintMinDistance = 0.0150f;
    float makeConstraintDistance = 0.1f;
    float reduceConstraintLengthVel = 1.5f;
    float breakingStrength = -1.0f;
    float brokenSupportTime = 1.0f;
    float brokenToSideProb = 0.5f;
    float connectAfter = 0.7f;
    float connectFor = 0.550f;
    int oneHandedPointing = 1;
    bool alwaysSupport = false;
    bool poseUnusedGunArm = false;
    bool poseUnusedSupportArm = false;
    bool poseUnusedOtherArm = false;
    float maxAngleAcross = 90.0f;
    float maxAngleAway = 90.0f;
    int fallingLimits = 0;
    float acrossLimit = 90.0f;
    float awayLimit = 90.0f;
    float upLimit = 90.0f;
    float downLimit = 45.0f;
    int rifleFall = 0;
    int fallingSupport = 1;
    int fallingTypeSupport = 0;
    int pistolNeutralType = 0;
    bool neutralPoint4Pistols = false;
    bool neutralPoint4Rifle = true;
    bool checkNeutralPoint = false;
    Vector3 point2Side{ 5.0f, -5.0f, -2.0f };
    float add2WeaponDistSide = 0.3f;
    Vector3 point2Connect{ -1.0f, -0.9f, -0.2f };
    float add2WeaponDistConnect = 0.0f;
    bool usePistolIK = true;
    bool useSpineTwist = true;
    bool useTurnToTarget = false;
    bool useHeadLook = true;
    float errorThreshold = 0.39260f;
    float fireWeaponRelaxTime = 0.4f;
    float fireWeaponRelaxAmount = 0.5f;
    float fireWeaponRelaxDistance = 0.050f;
    bool useIncomingTransforms = true;
    bool measureParentOffset = true;
    Vector3 leftHandParentOffset{ 0.0f, 0.0f, 0.0f };
    int leftHandParentEffector = -1;
    Vector3 rightHandParentOffset{ 0.0f, 0.0f, 0.0f };
    int rightHandParentEffector = -1;
    float primaryHandWeaponDistance = -1.0f;
    bool constrainRifle = true;
    float rifleConstraintMinDistance = 0.2f;
    bool disableArmCollisions = false;
    bool disableRifleCollisions = false;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "enableRight", enableRight);
        nGame::SetNMMessageParam(msgPtr, "enableLeft", enableLeft);
        nGame::SetNMMessageParam(msgPtr, "leftHandTarget", leftHandTarget.x, leftHandTarget.y, leftHandTarget.z);
        nGame::SetNMMessageParam(msgPtr, "leftHandTargetIndex", leftHandTargetIndex);
        nGame::SetNMMessageParam(msgPtr, "rightHandTarget", rightHandTarget.x, rightHandTarget.y, rightHandTarget.z);
        nGame::SetNMMessageParam(msgPtr, "rightHandTargetIndex", rightHandTargetIndex);
        nGame::SetNMMessageParam(msgPtr, "leadTarget", leadTarget);
        nGame::SetNMMessageParam(msgPtr, "armStiffness", armStiffness);
        nGame::SetNMMessageParam(msgPtr, "armStiffnessDetSupport", armStiffnessDetSupport);
        nGame::SetNMMessageParam(msgPtr, "armDamping", armDamping);
        nGame::SetNMMessageParam(msgPtr, "gravityOpposition", gravityOpposition);
        nGame::SetNMMessageParam(msgPtr, "gravOppDetachedSupport", gravOppDetachedSupport);
        nGame::SetNMMessageParam(msgPtr, "massMultDetachedSupport", massMultDetachedSupport);
        nGame::SetNMMessageParam(msgPtr, "allowShotLooseness", allowShotLooseness);
        nGame::SetNMMessageParam(msgPtr, "clavicleBlend", clavicleBlend);
        nGame::SetNMMessageParam(msgPtr, "elbowAttitude", elbowAttitude);
        nGame::SetNMMessageParam(msgPtr, "supportConstraint", supportConstraint);
        nGame::SetNMMessageParam(msgPtr, "constraintMinDistance", constraintMinDistance);
        nGame::SetNMMessageParam(msgPtr, "makeConstraintDistance", makeConstraintDistance);
        nGame::SetNMMessageParam(msgPtr, "reduceConstraintLengthVel", reduceConstraintLengthVel);
        nGame::SetNMMessageParam(msgPtr, "breakingStrength", breakingStrength);
        nGame::SetNMMessageParam(msgPtr, "brokenSupportTime", brokenSupportTime);
        nGame::SetNMMessageParam(msgPtr, "brokenToSideProb", brokenToSideProb);
        nGame::SetNMMessageParam(msgPtr, "connectAfter", connectAfter);
        nGame::SetNMMessageParam(msgPtr, "connectFor", connectFor);
        nGame::SetNMMessageParam(msgPtr, "oneHandedPointing", oneHandedPointing);
        nGame::SetNMMessageParam(msgPtr, "alwaysSupport", alwaysSupport);
        nGame::SetNMMessageParam(msgPtr, "poseUnusedGunArm", poseUnusedGunArm);
        nGame::SetNMMessageParam(msgPtr, "poseUnusedSupportArm", poseUnusedSupportArm);
        nGame::SetNMMessageParam(msgPtr, "poseUnusedOtherArm", poseUnusedOtherArm);
        nGame::SetNMMessageParam(msgPtr, "maxAngleAcross", maxAngleAcross);
        nGame::SetNMMessageParam(msgPtr, "maxAngleAway", maxAngleAway);
        nGame::SetNMMessageParam(msgPtr, "fallingLimits", fallingLimits);
        nGame::SetNMMessageParam(msgPtr, "acrossLimit", acrossLimit);
        nGame::SetNMMessageParam(msgPtr, "awayLimit", awayLimit);
        nGame::SetNMMessageParam(msgPtr, "upLimit", upLimit);
        nGame::SetNMMessageParam(msgPtr, "downLimit", downLimit);
        nGame::SetNMMessageParam(msgPtr, "rifleFall", rifleFall);
        nGame::SetNMMessageParam(msgPtr, "fallingSupport", fallingSupport);
        nGame::SetNMMessageParam(msgPtr, "fallingTypeSupport", fallingTypeSupport);
        nGame::SetNMMessageParam(msgPtr, "pistolNeutralType", pistolNeutralType);
        nGame::SetNMMessageParam(msgPtr, "neutralPoint4Pistols", neutralPoint4Pistols);
        nGame::SetNMMessageParam(msgPtr, "neutralPoint4Rifle", neutralPoint4Rifle);
        nGame::SetNMMessageParam(msgPtr, "checkNeutralPoint", checkNeutralPoint);
        nGame::SetNMMessageParam(msgPtr, "point2Side", point2Side.x, point2Side.y, point2Side.z);
        nGame::SetNMMessageParam(msgPtr, "add2WeaponDistSide", add2WeaponDistSide);
        nGame::SetNMMessageParam(msgPtr, "point2Connect", point2Connect.x, point2Connect.y, point2Connect.z);
        nGame::SetNMMessageParam(msgPtr, "add2WeaponDistConnect", add2WeaponDistConnect);
        nGame::SetNMMessageParam(msgPtr, "usePistolIK", usePistolIK);
        nGame::SetNMMessageParam(msgPtr, "useSpineTwist", useSpineTwist);
        nGame::SetNMMessageParam(msgPtr, "useTurnToTarget", useTurnToTarget);
        nGame::SetNMMessageParam(msgPtr, "useHeadLook", useHeadLook);
        nGame::SetNMMessageParam(msgPtr, "errorThreshold", errorThreshold);
        nGame::SetNMMessageParam(msgPtr, "fireWeaponRelaxTime", fireWeaponRelaxTime);
        nGame::SetNMMessageParam(msgPtr, "fireWeaponRelaxAmount", fireWeaponRelaxAmount);
        nGame::SetNMMessageParam(msgPtr, "fireWeaponRelaxDistance", fireWeaponRelaxDistance);
        nGame::SetNMMessageParam(msgPtr, "useIncomingTransforms", useIncomingTransforms);
        nGame::SetNMMessageParam(msgPtr, "measureParentOffset", measureParentOffset);
        nGame::SetNMMessageParam(msgPtr, "leftHandParentOffset", leftHandParentOffset.x, leftHandParentOffset.y, leftHandParentOffset.z);
        nGame::SetNMMessageParam(msgPtr, "leftHandParentEffector", leftHandParentEffector);
        nGame::SetNMMessageParam(msgPtr, "rightHandParentOffset", rightHandParentOffset.x, rightHandParentOffset.y, rightHandParentOffset.z);
        nGame::SetNMMessageParam(msgPtr, "rightHandParentEffector", rightHandParentEffector);
        nGame::SetNMMessageParam(msgPtr, "primaryHandWeaponDistance", primaryHandWeaponDistance);
        nGame::SetNMMessageParam(msgPtr, "constrainRifle", constrainRifle);
        nGame::SetNMMessageParam(msgPtr, "rifleConstraintMinDistance", rifleConstraintMinDistance);
        nGame::SetNMMessageParam(msgPtr, "disableArmCollisions", disableArmCollisions);
        nGame::SetNMMessageParam(msgPtr, "disableRifleCollisions", disableRifleCollisions);
        return;
    }
};

struct NMMessagePointGunExtra
{
    float constraintStrength = 2.0f;
    float constraintThresh = 0.1f;
    int weaponMask = 1024;
    bool timeWarpActive = false;
    float timeWarpStrengthScale = 1.0f;
    float oriStiff = 0.0f;
    float oriDamp = 0.0f;
    float posStiff = 0.0f;
    float posDamp = 0.0f;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "constraintStrength", constraintStrength);
        nGame::SetNMMessageParam(msgPtr, "constraintThresh", constraintThresh);
        nGame::SetNMMessageParam(msgPtr, "weaponMask", weaponMask);
        nGame::SetNMMessageParam(msgPtr, "timeWarpActive", timeWarpActive);
        nGame::SetNMMessageParam(msgPtr, "timeWarpStrengthScale", timeWarpStrengthScale);
        nGame::SetNMMessageParam(msgPtr, "oriStiff", oriStiff);
        nGame::SetNMMessageParam(msgPtr, "oriDamp", oriDamp);
        nGame::SetNMMessageParam(msgPtr, "posStiff", posStiff);
        nGame::SetNMMessageParam(msgPtr, "posDamp", posDamp);
        return;
    }
};

struct NMMessageRollDownStairs
{
    float stiffness = 6.0f;
    float damping = 0.0f;
    float forcemag = 1.1f;
    float m_useArmToSlowDown = -1.9f;
    bool useZeroPose = false;
    bool spinWhenInAir = false;
    float m_armReachAmount = 0.0f;
    float m_legPush = 0.0f;
    bool tryToAvoidHeadButtingGround = false;
    float armReachLength = 0.0f;
    Vector3 customRollDir{ 0.0f, 0.0f, 1.0f };
    bool useCustomRollDir = false;
    float stiffnessDecayTarget = 0.1f;
    float stiffnessDecayTime = 0.1f;
    float asymmetricalLegs = 0.0f;
    float zAxisSpinReduction = 0.0f;
    float targetLinearVelocityDecayTime = 2.0f;
    float targetLinearVelocity = 10.0f;
    bool onlyApplyHelperForces = false;
    bool useVelocityOfObjectBelow = false;
    bool useRelativeVelocity = false;
    bool applyFoetalToLegs = false;
    float movementLegsInFoetalPosition = 1.3f;
    float maxAngVelAroundFrontwardAxis = 10.0f;
    float minAngVel = 0.1f;
    bool applyNewRollingCheatingTorques = false;
    float maxAngVel = 10.0f;
    float magOfTorqueToRoll = 400.0f;
    bool applyHelPerTorqueToAlign = true;
    float delayToAlignBody = 0.2f;
    float magOfTorqueToAlign = 10.0f;
    float airborneReduction = 0.0f;
    bool applyMinMaxFriction = true;
    bool limitSpinReduction = false;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "stiffness", stiffness);
        nGame::SetNMMessageParam(msgPtr, "damping", damping);
        nGame::SetNMMessageParam(msgPtr, "forcemag", forcemag);
        nGame::SetNMMessageParam(msgPtr, "m_useArmToSlowDown", m_useArmToSlowDown);
        nGame::SetNMMessageParam(msgPtr, "useZeroPose", useZeroPose);
        nGame::SetNMMessageParam(msgPtr, "spinWhenInAir", spinWhenInAir);
        nGame::SetNMMessageParam(msgPtr, "m_armReachAmount", m_armReachAmount);
        nGame::SetNMMessageParam(msgPtr, "m_legPush", m_legPush);
        nGame::SetNMMessageParam(msgPtr, "tryToAvoidHeadButtingGround", tryToAvoidHeadButtingGround);
        nGame::SetNMMessageParam(msgPtr, "armReachLength", armReachLength);
        nGame::SetNMMessageParam(msgPtr, "customRollDir", customRollDir.x, customRollDir.y, customRollDir.z);
        nGame::SetNMMessageParam(msgPtr, "useCustomRollDir", useCustomRollDir);
        nGame::SetNMMessageParam(msgPtr, "stiffnessDecayTarget", stiffnessDecayTarget);
        nGame::SetNMMessageParam(msgPtr, "stiffnessDecayTime", stiffnessDecayTime);
        nGame::SetNMMessageParam(msgPtr, "asymmetricalLegs", asymmetricalLegs);
        nGame::SetNMMessageParam(msgPtr, "zAxisSpinReduction", zAxisSpinReduction);
        nGame::SetNMMessageParam(msgPtr, "targetLinearVelocityDecayTime", targetLinearVelocityDecayTime);
        nGame::SetNMMessageParam(msgPtr, "targetLinearVelocity", targetLinearVelocity);
        nGame::SetNMMessageParam(msgPtr, "onlyApplyHelperForces", onlyApplyHelperForces);
        nGame::SetNMMessageParam(msgPtr, "useVelocityOfObjectBelow", useVelocityOfObjectBelow);
        nGame::SetNMMessageParam(msgPtr, "useRelativeVelocity", useRelativeVelocity);
        nGame::SetNMMessageParam(msgPtr, "applyFoetalToLegs", applyFoetalToLegs);
        nGame::SetNMMessageParam(msgPtr, "movementLegsInFoetalPosition", movementLegsInFoetalPosition);
        nGame::SetNMMessageParam(msgPtr, "maxAngVelAroundFrontwardAxis", maxAngVelAroundFrontwardAxis);
        nGame::SetNMMessageParam(msgPtr, "minAngVel", minAngVel);
        nGame::SetNMMessageParam(msgPtr, "applyNewRollingCheatingTorques", applyNewRollingCheatingTorques);
        nGame::SetNMMessageParam(msgPtr, "maxAngVel", maxAngVel);
        nGame::SetNMMessageParam(msgPtr, "magOfTorqueToRoll", magOfTorqueToRoll);
        nGame::SetNMMessageParam(msgPtr, "applyHelPerTorqueToAlign", applyHelPerTorqueToAlign);
        nGame::SetNMMessageParam(msgPtr, "delayToAlignBody", delayToAlignBody);
        nGame::SetNMMessageParam(msgPtr, "magOfTorqueToAlign", magOfTorqueToAlign);
        nGame::SetNMMessageParam(msgPtr, "airborneReduction", airborneReduction);
        nGame::SetNMMessageParam(msgPtr, "applyMinMaxFriction", applyMinMaxFriction);
        nGame::SetNMMessageParam(msgPtr, "limitSpinReduction", limitSpinReduction);
        return;
    }
};

struct NMMessageShot
{
    float bodyStiffness = 6.0f;
    float spineDamping = 0.9f;
    float armStiffness = 6.0f;
    float initialNeckStiffness = 6.0f;
    float initialNeckDamping = 0.9f;
    float neckStiffness = 6.0f;
    float neckDamping = 0.9f;
    float kMultOnLoose = 0.0f;
    float kMult4Legs = 0.3f;
    float loosenessAmount = 1.0f;
    float looseness4Fall = 0.0f;
    float looseness4Stagger = 0.0f;
    float minArmsLooseness = 0.1f;
    float minLegsLooseness = 0.1f;
    float grabHoldTime = 10.0f;
    bool spineBlendExagCPain = false;
    float spineBlendZero = 0.6f;
    bool bulletProofVest = false;
    bool alwaysResetLooseness = true;
    bool alwaysResetNeckLooseness = true;
    float angVelScale = 1.0f;
    std::string angVelScaleMask = "fb";
    float flingWidth = 0.5f;
    float flingTime = 0.6f;
    float timeBeforeReachForWound = 0.05f;
    float exagDuration = 0.0f;
    float exagMag = 1.0f;
    float exagTwistMag = 0.5f;
    float exagSmooth2Zero = 0.0f;
    float exagZeroTime = 0.0f;
    float cpainSmooth2Time = 0.2f;
    float cpainDuration = 0.0f;
    float cpainMag = 1.0f;
    float cpainTwistMag = 0.5f;
    float cpainSmooth2Zero = 1.5f;
    bool crouching = false;
    bool chickenArms = false;
    bool reachForWound = true;
    bool fling = false;
    bool allowInjuredArm = true;
    bool allowInjuredLeg = true;
    bool allowInjuredLowerLegReach = false;
    bool allowInjuredThighReach = true;
    bool stableHandsAndNeck = false;
    bool melee = false;
    int fallingReaction = 0;
    bool useExtendedCatchFall = false;
    float initialWeaknessZeroDuration = 0.0f;
    float initialWeaknessRampDuration = 0.4f;
    float initialNeckDuration = 0.0f;
    float initialNeckRampDuration = 0.4f;
    bool useCStrModulation = false;
    float cStrUpperMin = 0.1f;
    float cStrUpperMax = 1.0f;
    float cStrLowerMin = 0.1f;
    float cStrLowerMax = 1.0f;
    float deathTime = -1.0f;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "bodyStiffness", bodyStiffness);
        nGame::SetNMMessageParam(msgPtr, "spineDamping", spineDamping);
        nGame::SetNMMessageParam(msgPtr, "armStiffness", armStiffness);
        nGame::SetNMMessageParam(msgPtr, "initialNeckStiffness", initialNeckStiffness);
        nGame::SetNMMessageParam(msgPtr, "initialNeckDamping", initialNeckDamping);
        nGame::SetNMMessageParam(msgPtr, "neckStiffness", neckStiffness);
        nGame::SetNMMessageParam(msgPtr, "neckDamping", neckDamping);
        nGame::SetNMMessageParam(msgPtr, "kMultOnLoose", kMultOnLoose);
        nGame::SetNMMessageParam(msgPtr, "kMult4Legs", kMult4Legs);
        nGame::SetNMMessageParam(msgPtr, "loosenessAmount", loosenessAmount);
        nGame::SetNMMessageParam(msgPtr, "looseness4Fall", looseness4Fall);
        nGame::SetNMMessageParam(msgPtr, "looseness4Stagger", looseness4Stagger);
        nGame::SetNMMessageParam(msgPtr, "minArmsLooseness", minArmsLooseness);
        nGame::SetNMMessageParam(msgPtr, "minLegsLooseness", minLegsLooseness);
        nGame::SetNMMessageParam(msgPtr, "grabHoldTime", grabHoldTime);
        nGame::SetNMMessageParam(msgPtr, "spineBlendExagCPain", spineBlendExagCPain);
        nGame::SetNMMessageParam(msgPtr, "spineBlendZero", spineBlendZero);
        nGame::SetNMMessageParam(msgPtr, "bulletProofVest", bulletProofVest);
        nGame::SetNMMessageParam(msgPtr, "alwaysResetLooseness", alwaysResetLooseness);
        nGame::SetNMMessageParam(msgPtr, "alwaysResetNeckLooseness", alwaysResetNeckLooseness);
        nGame::SetNMMessageParam(msgPtr, "angVelScale", angVelScale);
        nGame::SetNMMessageParam(msgPtr, "angVelScaleMask", angVelScaleMask.c_str());
        nGame::SetNMMessageParam(msgPtr, "flingWidth", flingWidth);
        nGame::SetNMMessageParam(msgPtr, "flingTime", flingTime);
        nGame::SetNMMessageParam(msgPtr, "timeBeforeReachForWound", timeBeforeReachForWound);
        nGame::SetNMMessageParam(msgPtr, "exagDuration", exagDuration);
        nGame::SetNMMessageParam(msgPtr, "exagMag", exagMag);
        nGame::SetNMMessageParam(msgPtr, "exagTwistMag", exagTwistMag);
        nGame::SetNMMessageParam(msgPtr, "exagSmooth2Zero", exagSmooth2Zero);
        nGame::SetNMMessageParam(msgPtr, "exagZeroTime", exagZeroTime);
        nGame::SetNMMessageParam(msgPtr, "cpainSmooth2Time", cpainSmooth2Time);
        nGame::SetNMMessageParam(msgPtr, "cpainDuration", cpainDuration);
        nGame::SetNMMessageParam(msgPtr, "cpainMag", cpainMag);
        nGame::SetNMMessageParam(msgPtr, "cpainTwistMag", cpainTwistMag);
        nGame::SetNMMessageParam(msgPtr, "cpainSmooth2Zero", cpainSmooth2Zero);
        nGame::SetNMMessageParam(msgPtr, "crouching", crouching);
        nGame::SetNMMessageParam(msgPtr, "chickenArms", chickenArms);
        nGame::SetNMMessageParam(msgPtr, "reachForWound", reachForWound);
        nGame::SetNMMessageParam(msgPtr, "fling", fling);
        nGame::SetNMMessageParam(msgPtr, "allowInjuredArm", allowInjuredArm);
        nGame::SetNMMessageParam(msgPtr, "allowInjuredLeg", allowInjuredLeg);
        nGame::SetNMMessageParam(msgPtr, "allowInjuredLowerLegReach", allowInjuredLowerLegReach);
        nGame::SetNMMessageParam(msgPtr, "allowInjuredThighReach", allowInjuredThighReach);
        nGame::SetNMMessageParam(msgPtr, "stableHandsAndNeck", stableHandsAndNeck);
        nGame::SetNMMessageParam(msgPtr, "melee", melee);
        nGame::SetNMMessageParam(msgPtr, "fallingReaction", fallingReaction);
        nGame::SetNMMessageParam(msgPtr, "useExtendedCatchFall", useExtendedCatchFall);
        nGame::SetNMMessageParam(msgPtr, "initialWeaknessZeroDuration", initialWeaknessZeroDuration);
        nGame::SetNMMessageParam(msgPtr, "initialWeaknessRampDuration", initialWeaknessRampDuration);
        nGame::SetNMMessageParam(msgPtr, "initialNeckDuration", initialNeckDuration);
        nGame::SetNMMessageParam(msgPtr, "initialNeckRampDuration", initialNeckRampDuration);
        nGame::SetNMMessageParam(msgPtr, "useCStrModulation", useCStrModulation);
        nGame::SetNMMessageParam(msgPtr, "cStrUpperMin", cStrUpperMin);
        nGame::SetNMMessageParam(msgPtr, "cStrUpperMax", cStrUpperMax);
        nGame::SetNMMessageParam(msgPtr, "cStrLowerMin", cStrLowerMin);
        nGame::SetNMMessageParam(msgPtr, "cStrLowerMax", cStrLowerMax);
        nGame::SetNMMessageParam(msgPtr, "deathTime", deathTime);
        return;
    }
};

struct NMMessageShotNewBullet
{
    int bodyPart = 0;
    bool localHitPointInfo = true;
    Vector3 normal{ 0.0f, 0.0f, -1.0f };
    Vector3 hitPoint{ 0.0f, 0.0f, 0.0f };
    Vector3 bulletVel{ 0.0f, 0.0f, 0.0f };
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "bodyPart", bodyPart);
        nGame::SetNMMessageParam(msgPtr, "localHitPointInfo", localHitPointInfo);
        nGame::SetNMMessageParam(msgPtr, "normal", normal.x, normal.y, normal.z);
        nGame::SetNMMessageParam(msgPtr, "hitPoint", hitPoint.x, hitPoint.y, hitPoint.z);
        nGame::SetNMMessageParam(msgPtr, "bulletVel", bulletVel.x, bulletVel.y, bulletVel.z);
        return;
    }
};

struct NMMessageShotSnap
{
    bool snap = true;
    float snapMag = 0.1f;
    float snapMovingMult = 1.0f;
    float snapBalancingMult = 1.0f;
    float snapAirborneMult = 1.0f;
    float snapMovingThresh = 1.0f;
    float snapDirectionRandomness = 0.3f;
    bool snapLeftArm = false;
    bool snapRightArm = false;
    bool snapLeftLeg = false;
    bool snapRightLeg = false;
    bool snapSpine = true;
    bool snapNeck = true;
    bool snapPhasedLegs = true;
    int snapHipType = 0;
    bool snapUseBulletDir = true;
    bool snapHitPart = false;
    float unSnapInterval = 0.010f;
    float unSnapRatio = 0.7f;
    bool snapUseTorques = true;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "snap", snap);
        nGame::SetNMMessageParam(msgPtr, "snapMag", snapMag);
        nGame::SetNMMessageParam(msgPtr, "snapMovingMult", snapMovingMult);
        nGame::SetNMMessageParam(msgPtr, "snapBalancingMult", snapBalancingMult);
        nGame::SetNMMessageParam(msgPtr, "snapAirborneMult", snapAirborneMult);
        nGame::SetNMMessageParam(msgPtr, "snapMovingThresh", snapMovingThresh);
        nGame::SetNMMessageParam(msgPtr, "snapDirectionRandomness", snapDirectionRandomness);
        nGame::SetNMMessageParam(msgPtr, "snapLeftArm", snapLeftArm);
        nGame::SetNMMessageParam(msgPtr, "snapRightArm", snapRightArm);
        nGame::SetNMMessageParam(msgPtr, "snapLeftLeg", snapLeftLeg);
        nGame::SetNMMessageParam(msgPtr, "snapRightLeg", snapRightLeg);
        nGame::SetNMMessageParam(msgPtr, "snapSpine", snapSpine);
        nGame::SetNMMessageParam(msgPtr, "snapNeck", snapNeck);
        nGame::SetNMMessageParam(msgPtr, "snapPhasedLegs", snapPhasedLegs);
        nGame::SetNMMessageParam(msgPtr, "snapHipType", snapHipType);
        nGame::SetNMMessageParam(msgPtr, "snapUseBulletDir", snapUseBulletDir);
        nGame::SetNMMessageParam(msgPtr, "snapHitPart", snapHitPart);
        nGame::SetNMMessageParam(msgPtr, "unSnapInterval", unSnapInterval);
        nGame::SetNMMessageParam(msgPtr, "unSnapRatio", unSnapRatio);
        nGame::SetNMMessageParam(msgPtr, "snapUseTorques", snapUseTorques);
        return;
    }
};

struct NMMessageShotShockSpin
{
    bool addShockSpin = true;
    bool randomizeShockSpinDirection = false;
    bool alwaysAddShockSpin = false;
    float shockSpinMin = 5.0f;
    float shockSpinMax = 10.0f;
    float shockSpinLiftForceMult = 0.0f;
    float shockSpinDecayMult = 4.0f;
    float shockSpinScalePerComponent = 0.5f;
    float shockSpinMaxTwistVel = -1.0f;
    bool shockSpinScaleByLeverArm = true;
    float shockSpinAirMult = 1.0f;
    float shockSpin1FootMult = 1.0f;
    float shockSpinFootGripMult = 1.0f;
    float bracedSideSpinMult = 1.0f;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "addShockSpin", addShockSpin);
        nGame::SetNMMessageParam(msgPtr, "randomizeShockSpinDirection", randomizeShockSpinDirection);
        nGame::SetNMMessageParam(msgPtr, "alwaysAddShockSpin", alwaysAddShockSpin);
        nGame::SetNMMessageParam(msgPtr, "shockSpinMin", shockSpinMin);
        nGame::SetNMMessageParam(msgPtr, "shockSpinMax", shockSpinMax);
        nGame::SetNMMessageParam(msgPtr, "shockSpinLiftForceMult", shockSpinLiftForceMult);
        nGame::SetNMMessageParam(msgPtr, "shockSpinDecayMult", shockSpinDecayMult);
        nGame::SetNMMessageParam(msgPtr, "shockSpinScalePerComponent", shockSpinScalePerComponent);
        nGame::SetNMMessageParam(msgPtr, "shockSpinMaxTwistVel", shockSpinMaxTwistVel);
        nGame::SetNMMessageParam(msgPtr, "shockSpinScaleByLeverArm", shockSpinScaleByLeverArm);
        nGame::SetNMMessageParam(msgPtr, "shockSpinAirMult", shockSpinAirMult);
        nGame::SetNMMessageParam(msgPtr, "shockSpin1FootMult", shockSpin1FootMult);
        nGame::SetNMMessageParam(msgPtr, "shockSpinFootGripMult", shockSpinFootGripMult);
        nGame::SetNMMessageParam(msgPtr, "bracedSideSpinMult", bracedSideSpinMult);
        return;
    }
};

struct NMMessageShotFallToKnees
{
    bool fallToKnees = true;
    bool ftkAlwaysChangeFall = false;
    float ftkBalanceTime = 1.2f;
    float ftkHelperForce = 100.0f;
    bool ftkHelperForceOnSpine = true;
    float ftkLeanHelp = 0.050f;
    float ftkSpineBend = -0.0f;
    bool ftkStiffSpine = false;
    float ftkImpactLooseness = 0.5f;
    float ftkImpactLoosenessTime = 0.2f;
    float ftkBendRate = 0.5f;
    float ftkHipBlend = 0.3f;
    float ftkLungeProb = 0.5f;
    bool ftkKneeSpin = false;
    float ftkFricMult = 1.0f;
    float ftkHipAngleFall = 0.5f;
    float ftkPitchForwards = 0.1f;
    float ftkPitchBackwards = 0.1f;
    float ftkFallBelowStab = 0.5f;
    float ftkBalanceAbortThreshold = 1.0f;
    int ftkOnKneesArmType = 2;
    float ftkReleaseReachForWound = 0.0f;
    bool ftkReachForWound = true;
    bool ftkReleasePointGun = false;
    bool ftkFailMustCollide = true;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "fallToKnees", fallToKnees);
        nGame::SetNMMessageParam(msgPtr, "ftkAlwaysChangeFall", ftkAlwaysChangeFall);
        nGame::SetNMMessageParam(msgPtr, "ftkBalanceTime", ftkBalanceTime);
        nGame::SetNMMessageParam(msgPtr, "ftkHelperForce", ftkHelperForce);
        nGame::SetNMMessageParam(msgPtr, "ftkHelperForceOnSpine", ftkHelperForceOnSpine);
        nGame::SetNMMessageParam(msgPtr, "ftkLeanHelp", ftkLeanHelp);
        nGame::SetNMMessageParam(msgPtr, "ftkSpineBend", ftkSpineBend);
        nGame::SetNMMessageParam(msgPtr, "ftkStiffSpine", ftkStiffSpine);
        nGame::SetNMMessageParam(msgPtr, "ftkImpactLooseness", ftkImpactLooseness);
        nGame::SetNMMessageParam(msgPtr, "ftkImpactLoosenessTime", ftkImpactLoosenessTime);
        nGame::SetNMMessageParam(msgPtr, "ftkBendRate", ftkBendRate);
        nGame::SetNMMessageParam(msgPtr, "ftkHipBlend", ftkHipBlend);
        nGame::SetNMMessageParam(msgPtr, "ftkLungeProb", ftkLungeProb);
        nGame::SetNMMessageParam(msgPtr, "ftkKneeSpin", ftkKneeSpin);
        nGame::SetNMMessageParam(msgPtr, "ftkFricMult", ftkFricMult);
        nGame::SetNMMessageParam(msgPtr, "ftkHipAngleFall", ftkHipAngleFall);
        nGame::SetNMMessageParam(msgPtr, "ftkPitchForwards", ftkPitchForwards);
        nGame::SetNMMessageParam(msgPtr, "ftkPitchBackwards", ftkPitchBackwards);
        nGame::SetNMMessageParam(msgPtr, "ftkFallBelowStab", ftkFallBelowStab);
        nGame::SetNMMessageParam(msgPtr, "ftkBalanceAbortThreshold", ftkBalanceAbortThreshold);
        nGame::SetNMMessageParam(msgPtr, "ftkOnKneesArmType", ftkOnKneesArmType);
        nGame::SetNMMessageParam(msgPtr, "ftkReleaseReachForWound", ftkReleaseReachForWound);
        nGame::SetNMMessageParam(msgPtr, "ftkReachForWound", ftkReachForWound);
        nGame::SetNMMessageParam(msgPtr, "ftkReleasePointGun", ftkReleasePointGun);
        nGame::SetNMMessageParam(msgPtr, "ftkFailMustCollide", ftkFailMustCollide);
        return;
    }
};

struct NMMessageShotFromBehind
{
    bool shotFromBehind = false;
    float sfbSpineAmount = 4.0f;
    float sfbNeckAmount = 1.0f;
    float sfbHipAmount = 1.0f;
    float sfbKneeAmount = 0.050f;
    float sfbPeriod = 0.7f;
    float sfbForceBalancePeriod = 0.05f;
    float sfbArmsOnset = 0.0f;
    float sfbKneesOnset = 0.0f;
    float sfbNoiseGain = 0.0f;
    int sfbIgnoreFail = 0;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "shotFromBehind", shotFromBehind);
        nGame::SetNMMessageParam(msgPtr, "sfbSpineAmount", sfbSpineAmount);
        nGame::SetNMMessageParam(msgPtr, "sfbNeckAmount", sfbNeckAmount);
        nGame::SetNMMessageParam(msgPtr, "sfbHipAmount", sfbHipAmount);
        nGame::SetNMMessageParam(msgPtr, "sfbKneeAmount", sfbKneeAmount);
        nGame::SetNMMessageParam(msgPtr, "sfbPeriod", sfbPeriod);
        nGame::SetNMMessageParam(msgPtr, "sfbForceBalancePeriod", sfbForceBalancePeriod);
        nGame::SetNMMessageParam(msgPtr, "sfbArmsOnset", sfbArmsOnset);
        nGame::SetNMMessageParam(msgPtr, "sfbKneesOnset", sfbKneesOnset);
        nGame::SetNMMessageParam(msgPtr, "sfbNoiseGain", sfbNoiseGain);
        nGame::SetNMMessageParam(msgPtr, "sfbIgnoreFail", sfbIgnoreFail);
        return;
    }
};

struct NMMessageShotInGuts
{
    bool shotInGuts = false;
    float sigSpineAmount = 2.0f;
    float sigNeckAmount = 1.0f;
    float sigHipAmount = 1.0f;
    float sigKneeAmount = 0.050f;
    float sigPeriod = 2.0f;
    float sigForceBalancePeriod = 0.0f;
    float sigKneesOnset = 0.0f;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "shotInGuts", shotInGuts);
        nGame::SetNMMessageParam(msgPtr, "sigSpineAmount", sigSpineAmount);
        nGame::SetNMMessageParam(msgPtr, "sigNeckAmount", sigNeckAmount);
        nGame::SetNMMessageParam(msgPtr, "sigHipAmount", sigHipAmount);
        nGame::SetNMMessageParam(msgPtr, "sigKneeAmount", sigKneeAmount);
        nGame::SetNMMessageParam(msgPtr, "sigPeriod", sigPeriod);
        nGame::SetNMMessageParam(msgPtr, "sigForceBalancePeriod", sigForceBalancePeriod);
        nGame::SetNMMessageParam(msgPtr, "sigKneesOnset", sigKneesOnset);
        return;
    }
};

struct NMMessageShotHeadLook
{
    bool useHeadLook = false;
    Vector3 headLook{ 0.0f, 0.0f, 0.0f };
    float headLookAtWoundMinTimer = 0.250f;
    float headLookAtWoundMaxTimer = 0.8f;
    float headLookAtHeadPosMaxTimer = 1.7f;
    float headLookAtHeadPosMinTimer = 0.6f;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "useHeadLook", useHeadLook);
        nGame::SetNMMessageParam(msgPtr, "headLook", headLook.x, headLook.y, headLook.z);
        nGame::SetNMMessageParam(msgPtr, "headLookAtWoundMinTimer", headLookAtWoundMinTimer);
        nGame::SetNMMessageParam(msgPtr, "headLookAtWoundMaxTimer", headLookAtWoundMaxTimer);
        nGame::SetNMMessageParam(msgPtr, "headLookAtHeadPosMaxTimer", headLookAtHeadPosMaxTimer);
        nGame::SetNMMessageParam(msgPtr, "headLookAtHeadPosMinTimer", headLookAtHeadPosMinTimer);
        return;
    }
};

struct NMMessageShotConfigureArms
{
    bool brace = false;
    bool pointGun = true;
    bool useArmsWindmill = false;
    int releaseWound = 0;
    int reachFalling = 1;
    int reachFallingWithOneHand = 3;
    int reachOnFloor = 1;
    float alwaysReachTime = 2.0f;
    float AWSpeedMult = 1.0f;
    float AWRadiusMult = 1.0f;
    float AWStiffnessAdd = 4.0f;
    int reachWithOneHand = 0;
    bool allowLeftPistolRFW = true;
    bool allowRightPistolRFW = true;
    bool rfwWithPistol = false;
    bool fling2 = false;
    bool fling2Left = true;
    bool fling2Right = true;
    bool fling2OverrideStagger = false;
    float fling2TimeBefore = 0.1f;
    float fling2Time = 0.5f;
    float fling2MStiffL = 0.950f;
    float fling2MStiffR = -1.0f;
    float fling2RelaxTimeL = 0.5f;
    float fling2RelaxTimeR = 0.5f;
    float fling2AngleMinL = -1.5f;
    float fling2AngleMaxL = 1.0f;
    float fling2AngleMinR = -1.5f;
    float fling2AngleMaxR = 1.0f;
    float fling2LengthMinL = 0.250f;
    float fling2LengthMaxL = 0.6f;
    float fling2LengthMinR = 0.250f;
    float fling2LengthMaxR = 0.6f;
    bool bust = false;
    float bustElbowLift = 0.7f;
    float cupSize = 0.1f;
    bool cupBust = false;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "brace", brace);
        nGame::SetNMMessageParam(msgPtr, "pointGun", pointGun);
        nGame::SetNMMessageParam(msgPtr, "useArmsWindmill", useArmsWindmill);
        nGame::SetNMMessageParam(msgPtr, "releaseWound", releaseWound);
        nGame::SetNMMessageParam(msgPtr, "reachFalling", reachFalling);
        nGame::SetNMMessageParam(msgPtr, "reachFallingWithOneHand", reachFallingWithOneHand);
        nGame::SetNMMessageParam(msgPtr, "reachOnFloor", reachOnFloor);
        nGame::SetNMMessageParam(msgPtr, "alwaysReachTime", alwaysReachTime);
        nGame::SetNMMessageParam(msgPtr, "AWSpeedMult", AWSpeedMult);
        nGame::SetNMMessageParam(msgPtr, "AWRadiusMult", AWRadiusMult);
        nGame::SetNMMessageParam(msgPtr, "AWStiffnessAdd", AWStiffnessAdd);
        nGame::SetNMMessageParam(msgPtr, "reachWithOneHand", reachWithOneHand);
        nGame::SetNMMessageParam(msgPtr, "allowLeftPistolRFW", allowLeftPistolRFW);
        nGame::SetNMMessageParam(msgPtr, "allowRightPistolRFW", allowRightPistolRFW);
        nGame::SetNMMessageParam(msgPtr, "rfwWithPistol", rfwWithPistol);
        nGame::SetNMMessageParam(msgPtr, "fling2", fling2);
        nGame::SetNMMessageParam(msgPtr, "fling2Left", fling2Left);
        nGame::SetNMMessageParam(msgPtr, "fling2Right", fling2Right);
        nGame::SetNMMessageParam(msgPtr, "fling2OverrideStagger", fling2OverrideStagger);
        nGame::SetNMMessageParam(msgPtr, "fling2TimeBefore", fling2TimeBefore);
        nGame::SetNMMessageParam(msgPtr, "fling2Time", fling2Time);
        nGame::SetNMMessageParam(msgPtr, "fling2MStiffL", fling2MStiffL);
        nGame::SetNMMessageParam(msgPtr, "fling2MStiffR", fling2MStiffR);
        nGame::SetNMMessageParam(msgPtr, "fling2RelaxTimeL", fling2RelaxTimeL);
        nGame::SetNMMessageParam(msgPtr, "fling2RelaxTimeR", fling2RelaxTimeR);
        nGame::SetNMMessageParam(msgPtr, "fling2AngleMinL", fling2AngleMinL);
        nGame::SetNMMessageParam(msgPtr, "fling2AngleMaxL", fling2AngleMaxL);
        nGame::SetNMMessageParam(msgPtr, "fling2AngleMinR", fling2AngleMinR);
        nGame::SetNMMessageParam(msgPtr, "fling2AngleMaxR", fling2AngleMaxR);
        nGame::SetNMMessageParam(msgPtr, "fling2LengthMinL", fling2LengthMinL);
        nGame::SetNMMessageParam(msgPtr, "fling2LengthMaxL", fling2LengthMaxL);
        nGame::SetNMMessageParam(msgPtr, "fling2LengthMinR", fling2LengthMinR);
        nGame::SetNMMessageParam(msgPtr, "fling2LengthMaxR", fling2LengthMaxR);
        nGame::SetNMMessageParam(msgPtr, "bust", bust);
        nGame::SetNMMessageParam(msgPtr, "bustElbowLift", bustElbowLift);
        nGame::SetNMMessageParam(msgPtr, "cupSize", cupSize);
        nGame::SetNMMessageParam(msgPtr, "cupBust", cupBust);
        return;
    }
};

struct NMMessageSmartFall
{
    float bodyStiffness = 6.0f;
    float bodydamping = 1.0f;
    float catchfalltime = 0.3f;
    float crashOrLandCutOff = 0.8680f;
    float pdStrength = 0.0f;
    float pdDamping = 1.0f;
    float armAngSpeed = 7.850f;
    float armAmplitude = 2.0f;
    float armPhase = 3.1f;
    bool armBendElbows = true;
    float legRadius = 0.4f;
    float legAngSpeed = 7.850f;
    float legAsymmetry = 4.0f;
    float arms2LegsPhase = 0.0f;
    int arms2LegsSync = 1;
    float armsUp = -3.1f;
    bool orientateBodyToFallDirection = false;
    bool orientateTwist = true;
    float orientateMax = 300.0f;
    bool alanRickman = false;
    bool fowardRoll = false;
    bool useZeroPose_withFowardRoll = false;
    float aimAngleBase = 0.180f;
    float fowardVelRotation = -0.020f;
    float footVelCompScale = 0.050f;
    float sideD = 0.2f;
    float fowardOffsetOfLegIK = 0.0f;
    float legL = 1.0f;
    float catchFallCutOff = 0.8780f;
    float legStrength = 12.0f;
    bool balance = true;
    bool ignorWorldCollisions = false;
    bool adaptiveCircling = true;
    bool hula = true;
    float maxSpeedForRecoverableFall = 15.0f;
    float minSpeedForBrace = 10.0f;
    float landingNormal = 0.6f;
    float rdsForceMag = 0.8f;
    float rdsTargetLinVeDecayTime = 0.5f;
    float rdsTargetLinearVelocity = 1.0f;
    bool rdsUseStartingFriction = true;
    float rdsStartingFriction = 1.0f;
    float rdsStartingFrictionMin = 1.0f;
    float rdsForceVelThreshold = 10.0f;
    int initialState = 0;
    bool changeExtremityFriction = false;
    bool teeter = true;
    float teeterOffset = 0.3f;
    float stopRollingTime = 10.0f;
    float reboundScale = 0.2f;
    std::string reboundMask = "uk";
    bool forceHeadAvoid = false;
    float cfZAxisSpinReduction = 0.5f;
    float splatWhenStopped = 0.0f;
    float blendHeadWhenStopped = 0.0f;
    float spreadLegs = 0.1f;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "bodyStiffness", bodyStiffness);
        nGame::SetNMMessageParam(msgPtr, "bodydamping", bodydamping);
        nGame::SetNMMessageParam(msgPtr, "catchfalltime", catchfalltime);
        nGame::SetNMMessageParam(msgPtr, "crashOrLandCutOff", crashOrLandCutOff);
        nGame::SetNMMessageParam(msgPtr, "pdStrength", pdStrength);
        nGame::SetNMMessageParam(msgPtr, "pdDamping", pdDamping);
        nGame::SetNMMessageParam(msgPtr, "armAngSpeed", armAngSpeed);
        nGame::SetNMMessageParam(msgPtr, "armAmplitude", armAmplitude);
        nGame::SetNMMessageParam(msgPtr, "armPhase", armPhase);
        nGame::SetNMMessageParam(msgPtr, "armBendElbows", armBendElbows);
        nGame::SetNMMessageParam(msgPtr, "legRadius", legRadius);
        nGame::SetNMMessageParam(msgPtr, "legAngSpeed", legAngSpeed);
        nGame::SetNMMessageParam(msgPtr, "legAsymmetry", legAsymmetry);
        nGame::SetNMMessageParam(msgPtr, "arms2LegsPhase", arms2LegsPhase);
        nGame::SetNMMessageParam(msgPtr, "arms2LegsSync", arms2LegsSync);
        nGame::SetNMMessageParam(msgPtr, "armsUp", armsUp);
        nGame::SetNMMessageParam(msgPtr, "orientateBodyToFallDirection", orientateBodyToFallDirection);
        nGame::SetNMMessageParam(msgPtr, "orientateTwist", orientateTwist);
        nGame::SetNMMessageParam(msgPtr, "orientateMax", orientateMax);
        nGame::SetNMMessageParam(msgPtr, "alanRickman", alanRickman);
        nGame::SetNMMessageParam(msgPtr, "fowardRoll", fowardRoll);
        nGame::SetNMMessageParam(msgPtr, "useZeroPose_withFowardRoll", useZeroPose_withFowardRoll);
        nGame::SetNMMessageParam(msgPtr, "aimAngleBase", aimAngleBase);
        nGame::SetNMMessageParam(msgPtr, "fowardVelRotation", fowardVelRotation);
        nGame::SetNMMessageParam(msgPtr, "footVelCompScale", footVelCompScale);
        nGame::SetNMMessageParam(msgPtr, "sideD", sideD);
        nGame::SetNMMessageParam(msgPtr, "fowardOffsetOfLegIK", fowardOffsetOfLegIK);
        nGame::SetNMMessageParam(msgPtr, "legL", legL);
        nGame::SetNMMessageParam(msgPtr, "catchFallCutOff", catchFallCutOff);
        nGame::SetNMMessageParam(msgPtr, "legStrength", legStrength);
        nGame::SetNMMessageParam(msgPtr, "balance", balance);
        nGame::SetNMMessageParam(msgPtr, "ignorWorldCollisions", ignorWorldCollisions);
        nGame::SetNMMessageParam(msgPtr, "adaptiveCircling", adaptiveCircling);
        nGame::SetNMMessageParam(msgPtr, "hula", hula);
        nGame::SetNMMessageParam(msgPtr, "maxSpeedForRecoverableFall", maxSpeedForRecoverableFall);
        nGame::SetNMMessageParam(msgPtr, "minSpeedForBrace", minSpeedForBrace);
        nGame::SetNMMessageParam(msgPtr, "landingNormal", landingNormal);
        nGame::SetNMMessageParam(msgPtr, "rdsForceMag", rdsForceMag);
        nGame::SetNMMessageParam(msgPtr, "rdsTargetLinVeDecayTime", rdsTargetLinVeDecayTime);
        nGame::SetNMMessageParam(msgPtr, "rdsTargetLinearVelocity", rdsTargetLinearVelocity);
        nGame::SetNMMessageParam(msgPtr, "rdsUseStartingFriction", rdsUseStartingFriction);
        nGame::SetNMMessageParam(msgPtr, "rdsStartingFriction", rdsStartingFriction);
        nGame::SetNMMessageParam(msgPtr, "rdsStartingFrictionMin", rdsStartingFrictionMin);
        nGame::SetNMMessageParam(msgPtr, "rdsForceVelThreshold", rdsForceVelThreshold);
        nGame::SetNMMessageParam(msgPtr, "initialState", initialState);
        nGame::SetNMMessageParam(msgPtr, "changeExtremityFriction", changeExtremityFriction);
        nGame::SetNMMessageParam(msgPtr, "teeter", teeter);
        nGame::SetNMMessageParam(msgPtr, "teeterOffset", teeterOffset);
        nGame::SetNMMessageParam(msgPtr, "stopRollingTime", stopRollingTime);
        nGame::SetNMMessageParam(msgPtr, "reboundScale", reboundScale);
        nGame::SetNMMessageParam(msgPtr, "reboundMask", reboundMask.c_str());
        nGame::SetNMMessageParam(msgPtr, "forceHeadAvoid", forceHeadAvoid);
        nGame::SetNMMessageParam(msgPtr, "cfZAxisSpinReduction", cfZAxisSpinReduction);
        nGame::SetNMMessageParam(msgPtr, "splatWhenStopped", splatWhenStopped);
        nGame::SetNMMessageParam(msgPtr, "blendHeadWhenStopped", blendHeadWhenStopped);
        nGame::SetNMMessageParam(msgPtr, "spreadLegs", spreadLegs);
        return;
    }
};

struct NMMessageStaggerFall
{
    float armStiffness = 2.0f;
    float armDamping = 1.0f;
    float spineDamping = 1.0f;
    float spineStiffness = 5.0f;
    float armStiffnessStart = 3.0f;
    float armDampingStart = 0.1f;
    float spineDampingStart = 0.1f;
    float spineStiffnessStart = 3.0f;
    float timeAtStartValues = 0.0f;
    float rampTimeFromStartValues = 0.0f;
    float staggerStepProb = 0.2f;
    int stepsTillStartEnd = 5;
    float timeStartEnd = 100.0f;
    float rampTimeToEndValues = 0.0f;
    float lowerBodyStiffness = 6.0f;
    float lowerBodyStiffnessEnd = 4.0f;
    float predictionTime = 0.25f;
    float perStepReduction1 = 0.5f;
    float leanInDirRate = 1.0f;
    float leanInDirMaxF = 0.5f;
    float leanInDirMaxB = 0.6f;
    float leanHipsMaxF = 0.0f;
    float leanHipsMaxB = 0.0f;
    float lean2multF = -1.0f;
    float lean2multB = -2.0f;
    float pushOffDist = 0.2f;
    float maxPushoffVel = 20.0f;
    float hipBendMult = 0.0f;
    bool alwaysBendForwards = false;
    float spineBendMult = 0.4f;
    bool useHeadLook = true;
    Vector3 headLookPos{ 0.0f, 0.0f, 0.0f };
    int headLookInstanceIndex = -1;
    float headLookAtVelProb = 1.0f;
    float turnOffProb = 0.0f;
    float turn2TargetProb = 0.2f;
    float turn2VelProb = 0.2f;
    float turnAwayProb = 0.2f;
    float turnLeftProb = 0.2f;
    float turnRightProb = 0.2f;
    bool useBodyTurn = false;
    bool upperBodyReaction = false;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "armStiffness", armStiffness);
        nGame::SetNMMessageParam(msgPtr, "armDamping", armDamping);
        nGame::SetNMMessageParam(msgPtr, "spineDamping", spineDamping);
        nGame::SetNMMessageParam(msgPtr, "spineStiffness", spineStiffness);
        nGame::SetNMMessageParam(msgPtr, "armStiffnessStart", armStiffnessStart);
        nGame::SetNMMessageParam(msgPtr, "armDampingStart", armDampingStart);
        nGame::SetNMMessageParam(msgPtr, "spineDampingStart", spineDampingStart);
        nGame::SetNMMessageParam(msgPtr, "spineStiffnessStart", spineStiffnessStart);
        nGame::SetNMMessageParam(msgPtr, "timeAtStartValues", timeAtStartValues);
        nGame::SetNMMessageParam(msgPtr, "rampTimeFromStartValues", rampTimeFromStartValues);
        nGame::SetNMMessageParam(msgPtr, "staggerStepProb", staggerStepProb);
        nGame::SetNMMessageParam(msgPtr, "stepsTillStartEnd", stepsTillStartEnd);
        nGame::SetNMMessageParam(msgPtr, "timeStartEnd", timeStartEnd);
        nGame::SetNMMessageParam(msgPtr, "rampTimeToEndValues", rampTimeToEndValues);
        nGame::SetNMMessageParam(msgPtr, "lowerBodyStiffness", lowerBodyStiffness);
        nGame::SetNMMessageParam(msgPtr, "lowerBodyStiffnessEnd", lowerBodyStiffnessEnd);
        nGame::SetNMMessageParam(msgPtr, "predictionTime", predictionTime);
        nGame::SetNMMessageParam(msgPtr, "perStepReduction1", perStepReduction1);
        nGame::SetNMMessageParam(msgPtr, "leanInDirRate", leanInDirRate);
        nGame::SetNMMessageParam(msgPtr, "leanInDirMaxF", leanInDirMaxF);
        nGame::SetNMMessageParam(msgPtr, "leanInDirMaxB", leanInDirMaxB);
        nGame::SetNMMessageParam(msgPtr, "leanHipsMaxF", leanHipsMaxF);
        nGame::SetNMMessageParam(msgPtr, "leanHipsMaxB", leanHipsMaxB);
        nGame::SetNMMessageParam(msgPtr, "lean2multF", lean2multF);
        nGame::SetNMMessageParam(msgPtr, "lean2multB", lean2multB);
        nGame::SetNMMessageParam(msgPtr, "pushOffDist", pushOffDist);
        nGame::SetNMMessageParam(msgPtr, "maxPushoffVel", maxPushoffVel);
        nGame::SetNMMessageParam(msgPtr, "hipBendMult", hipBendMult);
        nGame::SetNMMessageParam(msgPtr, "alwaysBendForwards", alwaysBendForwards);
        nGame::SetNMMessageParam(msgPtr, "spineBendMult", spineBendMult);
        nGame::SetNMMessageParam(msgPtr, "useHeadLook", useHeadLook);
        nGame::SetNMMessageParam(msgPtr, "headLookPos", headLookPos.x, headLookPos.y, headLookPos.z);
        nGame::SetNMMessageParam(msgPtr, "headLookInstanceIndex", headLookInstanceIndex);
        nGame::SetNMMessageParam(msgPtr, "headLookAtVelProb", headLookAtVelProb);
        nGame::SetNMMessageParam(msgPtr, "turnOffProb", turnOffProb);
        nGame::SetNMMessageParam(msgPtr, "turn2TargetProb", turn2TargetProb);
        nGame::SetNMMessageParam(msgPtr, "turn2VelProb", turn2VelProb);
        nGame::SetNMMessageParam(msgPtr, "turnAwayProb", turnAwayProb);
        nGame::SetNMMessageParam(msgPtr, "turnLeftProb", turnLeftProb);
        nGame::SetNMMessageParam(msgPtr, "turnRightProb", turnRightProb);
        nGame::SetNMMessageParam(msgPtr, "useBodyTurn", useBodyTurn);
        nGame::SetNMMessageParam(msgPtr, "upperBodyReaction", upperBodyReaction);
        return;
    }
};

struct NMMessageTeeter
{
    Vector3 edgeLeft{ 39.470f, 38.890f, 21.120f };
    Vector3 edgeRight{ 39.470f, 39.890f, 21.120f };
    bool useExclusionZone = true;
    bool useHeadLook = true;
    bool callHighFall = true;
    bool leanAway = true;
    float preTeeterTime = 2.0f;
    float leanAwayTime = 1.0f;
    float leanAwayScale = 0.5f;
    float teeterTime = 1.0f;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "edgeLeft", edgeLeft.x, edgeLeft.y, edgeLeft.z);
        nGame::SetNMMessageParam(msgPtr, "edgeRight", edgeRight.x, edgeRight.y, edgeRight.z);
        nGame::SetNMMessageParam(msgPtr, "useExclusionZone", useExclusionZone);
        nGame::SetNMMessageParam(msgPtr, "useHeadLook", useHeadLook);
        nGame::SetNMMessageParam(msgPtr, "callHighFall", callHighFall);
        nGame::SetNMMessageParam(msgPtr, "leanAway", leanAway);
        nGame::SetNMMessageParam(msgPtr, "preTeeterTime", preTeeterTime);
        nGame::SetNMMessageParam(msgPtr, "leanAwayTime", leanAwayTime);
        nGame::SetNMMessageParam(msgPtr, "leanAwayScale", leanAwayScale);
        nGame::SetNMMessageParam(msgPtr, "teeterTime", teeterTime);
        return;
    }
};

struct NMMessageUpperBodyFlinch
{
    float handDistanceLeftRight = 0.1f;
    float handDistanceFrontBack = 0.1f;
    float handDistanceVertical = 0.1f;
    float bodyStiffness = 6.0f;
    float bodyDamping = 1.0f;
    float backBendAmount = -0.55f;
    bool useRightArm = true;
    bool useLeftArm = true;
    float noiseScale = 0.1f;
    bool newHit = true;
    bool protectHeadToggle = false;
    bool dontBraceHead = false;
    bool applyStiffness = false;
    bool headLookAwayFromTarget = false;
    bool useHeadLook = true;
    int turnTowards = 1;
    Vector3 pos{ 0.0f, 0.0f, 0.0f };
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "handDistanceLeftRight", handDistanceLeftRight);
        nGame::SetNMMessageParam(msgPtr, "handDistanceFrontBack", handDistanceFrontBack);
        nGame::SetNMMessageParam(msgPtr, "handDistanceVertical", handDistanceVertical);
        nGame::SetNMMessageParam(msgPtr, "bodyStiffness", bodyStiffness);
        nGame::SetNMMessageParam(msgPtr, "bodyDamping", bodyDamping);
        nGame::SetNMMessageParam(msgPtr, "backBendAmount", backBendAmount);
        nGame::SetNMMessageParam(msgPtr, "useRightArm", useRightArm);
        nGame::SetNMMessageParam(msgPtr, "useLeftArm", useLeftArm);
        nGame::SetNMMessageParam(msgPtr, "noiseScale", noiseScale);
        nGame::SetNMMessageParam(msgPtr, "newHit", newHit);
        nGame::SetNMMessageParam(msgPtr, "protectHeadToggle", protectHeadToggle);
        nGame::SetNMMessageParam(msgPtr, "dontBraceHead", dontBraceHead);
        nGame::SetNMMessageParam(msgPtr, "applyStiffness", applyStiffness);
        nGame::SetNMMessageParam(msgPtr, "headLookAwayFromTarget", headLookAwayFromTarget);
        nGame::SetNMMessageParam(msgPtr, "useHeadLook", useHeadLook);
        nGame::SetNMMessageParam(msgPtr, "turnTowards", turnTowards);
        nGame::SetNMMessageParam(msgPtr, "pos", pos.x, pos.y, pos.z);
        return;
    }
};

struct NMMessageYanked
{
    float armStiffness = 6.0f;
    float armDamping = 1.0f;
    float spineDamping = 1.0f;
    float spineStiffness = 6.0f;
    float armStiffnessStart = 3.0f;
    float armDampingStart = 0.1f;
    float spineDampingStart = 0.1f;
    float spineStiffnessStart = 3.0f;
    float timeAtStartValues = 0.4f;
    float rampTimeFromStartValues = 0.1f;
    int stepsTillStartEnd = 6;
    float timeStartEnd = 100.0f;
    float rampTimeToEndValues = 0.0f;
    float lowerBodyStiffness = 6.0f;
    float lowerBodyStiffnessEnd = 2.0f;
    float perStepReduction = 1.5f;
    float hipPitchForward = 0.6f;
    float hipPitchBack = 1.0f;
    float spineBend = 0.7f;
    float footFriction = 1.0f;
    float turnThresholdMin = 0.6f;
    float turnThresholdMax = 0.6f;
    bool useHeadLook = false;
    Vector3 headLookPos{ 0.0f, 0.0f, 0.0f };
    int headLookInstanceIndex = -1;
    float headLookAtVelProb = -1.0f;
    float comVelRDSThresh = 1.0f;
    float hulaPeriod = 0.25f;
    float hipAmplitude = 1.0f;
    float spineAmplitude = 1.0f;
    float minRelaxPeriod = 0.3f;
    float maxRelaxPeriod = 1.5f;
    float rollHelp = 0.5f;
    float groundLegStiffness = 2.0f;
    float groundArmStiffness = 2.0f;
    float groundSpineStiffness = 2.0f;
    float groundLegDamping = 0.5f;
    float groundArmDamping = 0.5f;
    float groundSpineDamping = 0.5f;
    float groundFriction = 8.0f;
    void operator()(NmMessagePtr msgPtr)
    {
        nGame::SetNMMessageParam(msgPtr, "armStiffness", armStiffness);
        nGame::SetNMMessageParam(msgPtr, "armDamping", armDamping);
        nGame::SetNMMessageParam(msgPtr, "spineDamping", spineDamping);
        nGame::SetNMMessageParam(msgPtr, "spineStiffness", spineStiffness);
        nGame::SetNMMessageParam(msgPtr, "armStiffnessStart", armStiffnessStart);
        nGame::SetNMMessageParam(msgPtr, "armDampingStart", armDampingStart);
        nGame::SetNMMessageParam(msgPtr, "spineDampingStart", spineDampingStart);
        nGame::SetNMMessageParam(msgPtr, "spineStiffnessStart", spineStiffnessStart);
        nGame::SetNMMessageParam(msgPtr, "timeAtStartValues", timeAtStartValues);
        nGame::SetNMMessageParam(msgPtr, "rampTimeFromStartValues", rampTimeFromStartValues);
        nGame::SetNMMessageParam(msgPtr, "stepsTillStartEnd", stepsTillStartEnd);
        nGame::SetNMMessageParam(msgPtr, "timeStartEnd", timeStartEnd);
        nGame::SetNMMessageParam(msgPtr, "rampTimeToEndValues", rampTimeToEndValues);
        nGame::SetNMMessageParam(msgPtr, "lowerBodyStiffness", lowerBodyStiffness);
        nGame::SetNMMessageParam(msgPtr, "lowerBodyStiffnessEnd", lowerBodyStiffnessEnd);
        nGame::SetNMMessageParam(msgPtr, "perStepReduction", perStepReduction);
        nGame::SetNMMessageParam(msgPtr, "hipPitchForward", hipPitchForward);
        nGame::SetNMMessageParam(msgPtr, "hipPitchBack", hipPitchBack);
        nGame::SetNMMessageParam(msgPtr, "spineBend", spineBend);
        nGame::SetNMMessageParam(msgPtr, "footFriction", footFriction);
        nGame::SetNMMessageParam(msgPtr, "turnThresholdMin", turnThresholdMin);
        nGame::SetNMMessageParam(msgPtr, "turnThresholdMax", turnThresholdMax);
        nGame::SetNMMessageParam(msgPtr, "useHeadLook", useHeadLook);
        nGame::SetNMMessageParam(msgPtr, "headLookPos", headLookPos.x, headLookPos.y, headLookPos.z);
        nGame::SetNMMessageParam(msgPtr, "headLookInstanceIndex", headLookInstanceIndex);
        nGame::SetNMMessageParam(msgPtr, "headLookAtVelProb", headLookAtVelProb);
        nGame::SetNMMessageParam(msgPtr, "comVelRDSThresh", comVelRDSThresh);
        nGame::SetNMMessageParam(msgPtr, "hulaPeriod", hulaPeriod);
        nGame::SetNMMessageParam(msgPtr, "hipAmplitude", hipAmplitude);
        nGame::SetNMMessageParam(msgPtr, "spineAmplitude", spineAmplitude);
        nGame::SetNMMessageParam(msgPtr, "minRelaxPeriod", minRelaxPeriod);
        nGame::SetNMMessageParam(msgPtr, "maxRelaxPeriod", maxRelaxPeriod);
        nGame::SetNMMessageParam(msgPtr, "rollHelp", rollHelp);
        nGame::SetNMMessageParam(msgPtr, "groundLegStiffness", groundLegStiffness);
        nGame::SetNMMessageParam(msgPtr, "groundArmStiffness", groundArmStiffness);
        nGame::SetNMMessageParam(msgPtr, "groundSpineStiffness", groundSpineStiffness);
        nGame::SetNMMessageParam(msgPtr, "groundLegDamping", groundLegDamping);
        nGame::SetNMMessageParam(msgPtr, "groundArmDamping", groundArmDamping);
        nGame::SetNMMessageParam(msgPtr, "groundSpineDamping", groundSpineDamping);
        nGame::SetNMMessageParam(msgPtr, "groundFriction", groundFriction);
        return;
    }
};
#pragma endregion



/*
inline void TaskNMShotBase(Ped ped)
{
    NmMessage msgPtr = nGame::CreateNmMessage();
    NMMessageShot shot;
    shot.reachForWound = true;
    shot.allowInjuredLeg = true;
    shot(msgPtr.get());
    nGame::SetNMMessageParam(msgPtr.get(), "start", true);
    nGame::GivePedNMMessage(std::move(msgPtr), ped, "shot");

    msgPtr = nGame::CreateNmMessage();
    NMMessageShotConfigureArms shotConfigureArms;
    shotConfigureArms.pointGun = true;
    shotConfigureArms(msgPtr.get());
    nGame::SetNMMessageParam(msgPtr.get(), "start", true);
    nGame::GivePedNMMessage(std::move(msgPtr), ped, "shotConfigureArms");

    msgPtr = nGame::CreateNmMessage();
    NMMessageBodyBalance bodyBalance; bodyBalance(msgPtr.get());
    nGame::SetNMMessageParam(msgPtr.get(), "start", true);
    nGame::GivePedNMMessage(std::move(msgPtr), ped, "bodyBalance");
    return;
}

inline void TaskNMShotElectrocute(Ped ped)
{
    NmMessage msgPtr = nGame::CreateNmMessage();
    NMMessageElectrocute electrocute; electrocute(msgPtr.get());
    nGame::SetNMMessageParam(msgPtr.get(), "start", true);
    nGame::GivePedNMMessage(std::move(msgPtr), ped, "electrocute");

    msgPtr = nGame::CreateNmMessage();
    NMMessageForceLeanRandom forceLeanRandom; forceLeanRandom(msgPtr.get());
    nGame::SetNMMessageParam(msgPtr.get(), "start", true);
    nGame::GivePedNMMessage(std::move(msgPtr), ped, "forceLeanRandom");

    msgPtr = nGame::CreateNmMessage();
    NMMessageStaggerFall staggerFall; staggerFall(msgPtr.get());
    nGame::SetNMMessageParam(msgPtr.get(), "start", true);
    nGame::GivePedNMMessage(std::move(msgPtr), ped, "staggerFall");

    //MSG: CNmParameterResetMessage reset ???
    //msgPtr = nGame::CreateNmMessage();
    //NMMessageShotSnap shotSnap; shotSnap(msgPtr.get());
    //nGame::SetNMMessageParam(msgPtr.get(), "start", true);
    //nGame::GivePedNMMessage(std::move(msgPtr), ped, "shotSnap");


    msgPtr = nGame::CreateNmMessage();
    NMMessageConfigureBalance configureBalance; configureBalance(msgPtr.get());
    nGame::SetNMMessageParam(msgPtr.get(), "start", true);
    nGame::GivePedNMMessage(std::move(msgPtr), ped, "configureBalance");
    return;
}

inline void TaskNMAutomaticHeadShot(Ped ped)
{
    NmMessage msgPtr = nGame::CreateNmMessage();
    NMMessageCatchFall catchFall; catchFall(msgPtr.get());
    nGame::SetNMMessageParam(msgPtr.get(), "start", false);
    nGame::GivePedNMMessage(std::move(msgPtr), ped, "catchFall");

    msgPtr = nGame::CreateNmMessage();
    NMMessageStaggerFall staggerFall; staggerFall(msgPtr.get());
    nGame::SetNMMessageParam(msgPtr.get(), "start", false);
    nGame::GivePedNMMessage(std::move(msgPtr), ped, "staggerFall");

    msgPtr = nGame::CreateNmMessage();
    NMMessageHighFall highFall;
	highFall.catchfalltime = 0.0f;
    highFall.crashOrLandCutOff = 0.034f;
    highFall(msgPtr.get());
    nGame::SetNMMessageParam(msgPtr.get(), "start", true);
    nGame::GivePedNMMessage(std::move(msgPtr), ped, "highFall");

    msgPtr = nGame::CreateNmMessage();
    NMMessageConfigureBullets configureBullets;
    configureBullets.impulseReductionPerShot = 0.5f;
    configureBullets.impulseRecovery = 0.0f;
    configureBullets.impulseNoBalMult = 0.5f;
    configureBullets.impulseBalStabStart = 3.0f;
    configureBullets.impulseBalStabEnd = 10.0f;
    configureBullets.impulseBalStabMult = 1.0f;
    configureBullets.impulseSpineAngStart = 0.7f;
    configureBullets.impulseSpineAngEnd = 0.2f;
    configureBullets.impulseSpineAngMult = 1.0f;
    configureBullets.impulseVelStart = 1.0f;
    configureBullets.impulseVelEnd = 4.0f;
    configureBullets.impulseVelMult = 1.0f;
    configureBullets.impulseAirMult = 0.5f;
    configureBullets.impulseAirMultStart = 50.0f;
    configureBullets.impulseAirMax = 100.0f;
    configureBullets.impulseAirApplyAbove = 399.0f;
    configureBullets.impulseAirOn = true;
    configureBullets.impulseOneLegMult = 0.5f;
    configureBullets.impulseOneLegMultStart = 30.0f;
    configureBullets.impulseOneLegMax = 80.0f;
    configureBullets.impulseOneLegApplyAbove = 399.0f;
    configureBullets.impulseOneLegOn = true;
    configureBullets(msgPtr.get());
    //nGame::SetNMMessageParam(msgPtr.get(), "start", true);
    nGame::GivePedNMMessage(std::move(msgPtr), ped, "configureBullets");

    msgPtr = nGame::CreateNmMessage();
    NMMessageConfigureBalance configureBalance;
	configureBalance.extraSteps = 0;
    configureBalance.extraTime = 0.0f;
    configureBalance.maxSteps = 2;
    configureBalance.maxBalanceTime = 10.0f;
    configureBalance.rampHipPitchOnFail = true;
    configureBalance.stepIfInSupport = false;
    configureBalance.backwardsLeanCutoff = 0.3f;
    configureBalance.balanceAbortThreshold = 0.6f;
    configureBalance.failMustCollide = false;
    configureBalance.giveUpHeight = 0.5f;
    configureBalance(msgPtr.get());
    nGame::SetNMMessageParam(msgPtr.get(), "start", true);
    nGame::GivePedNMMessage(std::move(msgPtr), ped, "configureBalance");

    msgPtr = nGame::CreateNmMessage();
    NMMessageShotShockSpin shotShockSpin; shotShockSpin(msgPtr.get());
    //nGame::SetNMMessageParam(msgPtr.get(), "start", false);
    nGame::GivePedNMMessage(std::move(msgPtr), ped, "shotShockSpin");

    msgPtr = nGame::CreateNmMessage();
    NMMessageShotFallToKnees shotFallToKnees;
    shotFallToKnees.ftkOnKneesArmType = 1;
    shotFallToKnees(msgPtr.get());
    //nGame::SetNMMessageParam(msgPtr.get(), "start", false);
    nGame::GivePedNMMessage(std::move(msgPtr), ped, "shotFallToKnees");

    msgPtr = nGame::CreateNmMessage();
    NMMessageShot shot;
    shot.fallingReaction = 3;
    shot.fling = false;
    shot(msgPtr.get());
    nGame::SetNMMessageParam(msgPtr.get(), "start", true);
    nGame::GivePedNMMessage(std::move(msgPtr), ped, "shot");

    msgPtr = nGame::CreateNmMessage();
    NMMessageShotSnap shotSnap;
    shotSnap.snapHitPart = false;
    shotSnap(msgPtr.get());
    //nGame::SetNMMessageParam(msgPtr.get(), "start", false);
    nGame::GivePedNMMessage(std::move(msgPtr), ped, "shotSnap");
    return;
}
*/