/******************************************************************************
 * Copyright 2017 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

/**
 * @file obstacle.h
 **/

#ifndef MODULES_PLANNING_COMMON_OBSTACLE_H_
#define MODULES_PLANNING_COMMON_OBSTACLE_H_

#include <list>
#include <vector>

#include "modules/perception/proto/perception_obstacle.pb.h"
#include "modules/planning/proto/decision.pb.h"
#include "modules/prediction/proto/prediction_obstacle.pb.h"

#include "modules/common/math/box2d.h"
#include "modules/common/math/vec2d.h"

namespace apollo {
namespace planning {

class Obstacle;
using ConstObstacleList = std::vector<const Obstacle *>;

class Obstacle {
 public:
  Obstacle() = default;

  Obstacle(const std::string &id,
           const perception::PerceptionObstacle &perception,
           const prediction::Trajectory &trajectory);

  const std::string &Id() const;

  bool IsStatic() const;

  common::TrajectoryPoint GetPointAtTime(const double time) const;
  common::math::Box2d GetBoundingBox(
      const common::TrajectoryPoint &point) const;
  /**
   * @brief get the perception bounding box
   */
  const common::math::Box2d &PerceptionBoundingBox() const;

  const prediction::Trajectory &Trajectory() const;

  const perception::PerceptionObstacle &Perception() const;

  // FIXME fake functions, will be migrate out soon.
  const std::vector<ObjectDecisionType> &Decisions() const {
    return decisions_;
  }
  std::vector<ObjectDecisionType> *MutableDecisions() { return &decisions_; }

  /**
   * @brief This is a helper function that can create obstacles from prediction
   * data.  The original prediction may have multiple trajectories for each
   * obstacle. But this function will create one obstacle for each trajectory.
   * @param predictions The prediction results
   * @param obstacles The output obstacles saved in a list of unique_ptr.
   */
  static void CreateObstacles(
      const prediction::PredictionObstacles &predictions,
      std::list<std::unique_ptr<Obstacle>> *obstacles);

 private:
  static bool IsStaticObstacle(
      const perception::PerceptionObstacle &perception_obstacle);
  std::string id_;
  bool is_static_;
  prediction::Trajectory trajectory_;
  perception::PerceptionObstacle perception_obstacle_;
  // FIXME move out later
  std::vector<ObjectDecisionType> decisions_;
  common::math::Box2d perception_bounding_box_;
};

}  // namespace planning
}  // namespace apollo

#endif  // MODULES_PLANNING_COMMON_OBSTACLE_H_
