/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: [Your Name] <[Your Email]>
 */

#ifndef GROUP_GAUSS_MARKOV_MOBILITY_MODEL_H
#define GROUP_GAUSS_MARKOV_MOBILITY_MODEL_H

#include "ns3/gauss-markov-mobility-model.h"
#include "ns3/constant-velocity-helper.h"
#include "ns3/vector.h"
#include <vector>

namespace ns3 {

/**
 * \ingroup mobility
 * \brief Group Gauss Markov mobility model.
 */
class GroupGaussMarkovMobilityModel : public GaussMarkovMobilityModel
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId(void);

  GroupGaussMarkovMobilityModel();

  // Override virtual methods from the base class
  virtual void Start(void);
  virtual Vector DoGetPosition(void) const override;
  virtual void DoSetPosition(const Vector &position) override;
  virtual Vector DoGetVelocity(void) const override;
  virtual void DoInitialize(void) override;
  virtual void DoDispose(void) override;
  void DoWalk (Time delayLeft);
  ConstantVelocityHelper m_helper; ///< Helper to maintain constant velocity
  uint32_t m_numberOfGroupNodes; ///< Number of nodes in the group
  double m_stickiness; ///< Stickiness of the group (0-1)
  mutable Vector m_centerOfMass; ///< Center of mass of the group
  mutable std::vector<Vector> m_neighborPositions; ///< Positions of neighbors
  mutable std::vector<Vector> m_neighborVelocities; ///< Velocities of neighbors

  void UpdateCenterOfMass(); ///< Update the center of mass of the group
  void UpdateNeighbors(); ///< Update the neighbors' positions and velocities
};

} // namespace ns3

#endif /* GROUP_GAUSS_MARKOV_MOBILITY_MODEL_H */
