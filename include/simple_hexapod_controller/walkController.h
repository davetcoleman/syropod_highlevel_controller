#pragma once
#include "model.h"

struct WalkController
{
  Model *model;
  Pose pose;
  int gaitType;
  double stepFrequency;
  double bodyClearance;
  double stepClearance;
  Vector3d footSpreadDistances;
  double minFootprintRadius;
  double stanceRadius; // affects turning circle
  Vector3d localStanceTipPositions[3][2];
  Vector2d localCentreVelocity;
  Vector2d localCentreAcceleration;
  double angularVelocity;
  double walkPhase;
  double maximumBodyHeight;
  
  struct LegStepper
  {
    double phase; // 0 to 2pi
    double phaseOffset; // 0 to 2pi
    Vector2d strideVector; // length gives stride length
    
    Vector3d getPosition(double liftHeight);
  } legSteppers[3][2];
  
  vector<Vector3d> targets;
  
  // Determines the basic stance pose which the hexapod will try to maintain, by finding the largest footprint radius that each leg can achieve for the specified level of clearance
  // stepFrequency- preferred step cycles per second
  // bodyClearance, stepClearance- 0 to 1, 1 is vertical legs
  // stanceLegYaws- natural yaw pose per leg
  // minYawLimits- the minimum yaw (or hip) joint limit around centre for each leg

  WalkController(Model *model, 
		 int gaitType,
		 double stepFrequency,
		 double stepClearance,
		 double bodyClearance = -1);
  
  // curvature is 0 to 1 so 1 is rotate on the spot, 0.5 rotates around leg stance pos
  // bodyOffset is body pose relative to the basic stance pose, note that large offsets may prevent achievable leg positions
  // call this function even when not walking (newLocalVelocity=0), otherwise joint angles will just freeze
  void update(Vector2d newLocalVelocity, double newCurvature, const Pose *bodyOffset = NULL);
  bool moveToStart();
};