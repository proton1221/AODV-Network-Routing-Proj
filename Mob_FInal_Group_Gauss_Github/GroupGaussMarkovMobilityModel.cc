/* -- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -- */
/* *...existing copyright and author information... */

#include <cmath>
#include <vector>
#include "ns3/simulator.h"
#include "ns3/double.h"
#include "ns3/pointer.h"
#include "ns3/string.h"
#include "ns3/vector.h"
#include "ns3/random-variable-stream.h"
#include "ns3/gauss-markov-mobility-model.h"
#include "ns3/aodv-routing-protocol.h"
#include "ns3/node-list.h"
#include "ns3/node-container.h"
#include "ns3/net-device-container.h"
#include "ns3/ipv4-interface-container.h"
#include "ns3/node-list.h"
#include "ns3/node.h"
#include "ns3/vector.h"
#include "ns3/mobility-model.h"
#include "ns3/attribute.h"
#include "ns3/uinteger.h"
#include "ns3/position-allocator.h"
#include "GroupGaussMarkovMobilityModel.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("GroupGaussMarkovMobilityModel");
NS_OBJECT_ENSURE_REGISTERED (GroupGaussMarkovMobilityModel);

// Required for NS_OBJECT_ENSURE_REGISTERED

TypeId GroupGaussMarkovMobilityModel::GetTypeId(void) {
  static TypeId tid = TypeId("ns3::GroupGaussMarkovMobilityModel")
    .SetParent<GaussMarkovMobilityModel>()
    .SetGroupName("Mobility")
    .AddConstructor<GroupGaussMarkovMobilityModel>()
    .AddAttribute("NumberOfGroupNodes",
                  "Number of nodes in the group.",
                  UintegerValue(1),
                  MakeUintegerAccessor(&GroupGaussMarkovMobilityModel::m_numberOfGroupNodes),
                  MakeUintegerChecker<uint32_t>(1))
    .AddAttribute("Stickiness",
                  "Stickiness of the group (0-1).",
                  DoubleValue(0.5),
                  MakeDoubleAccessor(&GroupGaussMarkovMobilityModel::m_stickiness),
                  MakeDoubleChecker<double>(0, 1));
  return tid;
}

GroupGaussMarkovMobilityModel::GroupGaussMarkovMobilityModel()
  : m_numberOfGroupNodes(2),
    m_stickiness(0.5) {
}

void GroupGaussMarkovMobilityModel::DoInitialize(void) {
  GaussMarkovMobilityModel::DoInitialize();
  UpdateNeighbors();
  UpdateCenterOfMass();
}

// void GroupGaussMarkovMobilityModel::DoInitializePrivate() {

// }

Vector divideScalar(Vector val1,size_t val2){
    if (val2 != 0) { // Prevent division by zero
        Vector val_new(
            val1.x / val2,
            val1.y / val2,
            val1.z / val2
        );
    return val_new;
    }
    else
    return val1;
}


Vector mulitplyScalar(Vector val1,size_t val2){

    Vector val_new(
        val1.x * val2,
        val1.y * val2,
        val1.z * val2
    );
    return val1;
}

void GroupGaussMarkovMobilityModel::Start(void) {

// GaussMarkovMobiltyModel Code
  if (m_meanVelocity == 0.0)
    {
      //Initialize the mean velocity, direction, and pitch variables
      m_meanVelocity = m_rndMeanVelocity->GetValue ();
      m_meanDirection = m_rndMeanDirection->GetValue ();
      m_meanPitch = m_rndMeanPitch->GetValue ();
      double cosD = std::cos (m_meanDirection);
      // double cosP = std::cos (m_meanPitch);
      double sinD = std::sin (m_meanDirection);
      // double sinP = std::sin (m_meanPitch);
      //Initialize the starting velocity, direction, and pitch to be identical to the mean ones
      m_Velocity = m_meanVelocity;
      m_Direction = m_meanDirection;
      // m_Pitch = m_meanPitch;

      //Set the velocity vector to give to the constant velocity helper
      m_helper.SetVelocity (Vector (m_Velocity*cosD, m_Velocity*sinD,0));
    }

  m_helper.Update ();

  //Get the next values from the gaussian distributions for velocity, direction, and pitch
  double rv = m_normalVelocity->GetValue ();
  double rd = m_normalDirection->GetValue ();
  // double rp = m_normalPitch->GetValue ();

  //Calculate the NEW velocity, direction, and pitch values using the Gauss-Markov formula:
  //newVal = alpha*oldVal + (1-alpha)*meanVal + sqrt(1-alpha^2)*rv
  //where rv is a random number from a normal (gaussian) distribution
  double one_minus_alpha = 1 - m_alpha;
  double sqrt_alpha = std::sqrt (1 - m_alpha*m_alpha);
  m_Velocity  = m_alpha * m_Velocity  + one_minus_alpha * m_meanVelocity  + sqrt_alpha * rv;
  m_Direction = m_alpha * m_Direction + one_minus_alpha * m_meanDirection + sqrt_alpha * rd;
  // m_Pitch     = m_alpha * m_Pitch     + one_minus_alpha * m_meanPitch     + sqrt_alpha * rp;

  //Calculate the linear velocity vector to give to the constant velocity helper
  double cosDir = std::cos (m_Direction);
  // double cosPit = std::cos (m_Pitch);
  double sinDir = std::sin (m_Direction);
  // double sinPit = std::sin (m_Pitch);
  double vx = m_Velocity * cosDir;
  double vy = m_Velocity * sinDir;
  // double vz = m_Velocity * sinPit;
  m_helper.SetVelocity (Vector (vx, vy, 0));

  m_helper.Unpause ();

// Gauss-Markov-Model-Code

  UpdateNeighbors();
  UpdateCenterOfMass();

  Vector currentPosition = GetPosition();
  Vector currentVelocity = GetVelocity();
  Vector toCenterOfMass = m_centerOfMass - currentPosition;
  double distanceToCenter = toCenterOfMass.GetLength();
  
  if (distanceToCenter > 0) {
    // Calculate a direction vector towards the center of mass.
    Vector directionToCenter = divideScalar(toCenterOfMass,distanceToCenter);
    
    // Apply stickiness to adjust velocity towards the group's average velocity.
    double temp = m_stickiness * distanceToCenter;
    Vector adjustedVelocity = currentVelocity + mulitplyScalar(directionToCenter,temp);
    m_helper.SetVelocity(adjustedVelocity);
  }


  GroupGaussMarkovMobilityModel::DoWalk(m_timeStep);
  
  

}

void
GroupGaussMarkovMobilityModel::DoWalk (Time delayLeft){
  m_event = Simulator::Schedule (m_timeStep, &GroupGaussMarkovMobilityModel::Start, this);
}

Vector GroupGaussMarkovMobilityModel::DoGetPosition(void) const {
  return GaussMarkovMobilityModel::DoGetPosition();
}

void GroupGaussMarkovMobilityModel::DoSetPosition(const Vector &position) {
  GaussMarkovMobilityModel::DoSetPosition(position);
}

Vector GroupGaussMarkovMobilityModel::DoGetVelocity(void) const {
  return GaussMarkovMobilityModel::DoGetVelocity();
}

void GroupGaussMarkovMobilityModel::UpdateCenterOfMass() {
  m_centerOfMass = Vector(0, 0, 0);


  Vector sumOfPositions(0, 0, 0);
  Vector sumOfVelocities(0, 0, 0);
  uint32_t groupSize = m_neighborPositions.size(); // Assuming m_neighborPositions has been populated.

  for (uint32_t i = 0; i < groupSize; ++i) {
    sumOfPositions = sumOfPositions + m_neighborPositions[i];
    sumOfVelocities = sumOfPositions + m_neighborVelocities[i]; // Assuming m_neighborVelocities follow the said positions.
  }

  if (groupSize > 0) {
    m_centerOfMass = divideScalar(sumOfPositions,groupSize);
    Vector averageVelocity = divideScalar(sumOfVelocities,groupSize);
    // you can set the closest node to center of mass as well 
    // uint64_t closestToCenterOfMassDistance = 0;
    // for (uint32_t i = 0; i < groupSize; ++i) {
    //   if (CalculateDistance(m_centerOfMass,m_neighborPositions[i]) > closestToCenterOfMassDistance){
    //       closestToCenterOfMassDistance = CalculateDistance(m_centerOfMass,m_neighborPositions[i]);
    //       m_centerOfMass = m_neighborPositions[i];
    //   }
    // }
    // Assuming m_neighborVelocities follow the said positions.
    // Now apply this average velocity to influence the node's motion.
    m_helper.SetVelocity(averageVelocity); // This sets the node's velocity to the group's average.
  }



}


void GroupGaussMarkovMobilityModel::UpdateNeighbors() {
    m_neighborPositions.clear();
    m_neighborVelocities.clear();
    
    Ptr<Node> currentNode = GetObject<Node>(); // Get the current node
    if (currentNode == nullptr) return;
    
    Ptr<MobilityModel> currentMobilityModel = currentNode->GetObject<MobilityModel>();
    if (currentMobilityModel == nullptr) return;

    // Define the maximum distance for considering another node a neighbor
    const double maxNeighborDistance = 10.0; // meters

    for (uint32_t i = 0; i < NodeList::GetNNodes() && m_neighborPositions.size() < m_numberOfGroupNodes; ++i) {
        Ptr<Node> node = NodeList::GetNode(i);
        if (node == currentNode) continue; // Skip the current node
        
        Ptr<MobilityModel> mobilityModel = node->GetObject<MobilityModel>();
        double distance = currentMobilityModel->GetDistanceFrom(mobilityModel);
        
        if (distance <= maxNeighborDistance) {
            // fetch position
            m_neighborPositions.push_back(mobilityModel->GetPosition());
            // fetch velocity
            m_neighborVelocities.push_back(mobilityModel->GetVelocity());
        }
    }
}


void GroupGaussMarkovMobilityModel::DoDispose(void) {
  GaussMarkovMobilityModel::DoDispose();
}



} // namespace ns3
