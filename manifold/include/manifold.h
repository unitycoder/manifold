// Copyright 2021 Emmett Lalish
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once
#include <functional>
#include <memory>

#include "structs.h"

namespace manifold {

/** @defgroup Core
 *  @brief The central classes of the library
 *  @{
 */
class Manifold {
 public:
  /** @name Creation
   *  Constructors
   */
  ///@{
  Manifold();

  Manifold(
      const Mesh&,
      const std::vector<glm::ivec3>& triProperties = std::vector<glm::ivec3>(),
      const std::vector<float>& properties = std::vector<float>(),
      const std::vector<float>& propertyTolerance = std::vector<float>());

  static Manifold Smooth(const Mesh&,
                         const std::vector<Smoothness>& sharpenedEdges = {});
  static Manifold Tetrahedron();
  static Manifold Cube(glm::vec3 size = glm::vec3(1.0f), bool center = false);
  static Manifold Cylinder(float height, float radiusLow,
                           float radiusHigh = -1.0f, int circularSegments = 0,
                           bool center = false);
  static Manifold Sphere(float radius, int circularSegments = 0);
  static Manifold Extrude(Polygons crossSection, float height,
                          int nDivisions = 0, float twistDegrees = 0.0f,
                          glm::vec2 scaleTop = glm::vec2(1.0f));
  static Manifold Revolve(const Polygons& crossSection,
                          int circularSegments = 0);
  ///@}

  /** @name Topological
   *  No geometric calculations.
   */
  ///@{
  static Manifold Compose(const std::vector<Manifold>&);
  std::vector<Manifold> Decompose() const;
  ///@}

  /** @name Defaults
   * These static properties control how circular shapes are quantized by
   * default on construction. If circularSegments is specified, it takes
   * precedence. If it is zero, then instead the minimum is used of the segments
   * calculated based on edge length and angle, rounded up to the nearest
   * multiple of four. To get numbers not divisible by four, circularSegements
   * must be specified.
   */
  ///@{
  static void SetMinCircularAngle(float degrees);
  static void SetMinCircularEdgeLength(float length);
  static void SetCircularSegments(int number);
  static int GetCircularSegments(float radius);
  ///@}

  /** @name Information
   *  Details of the manifold
   */
  ///@{
  Mesh GetMesh() const;
  bool IsEmpty() const;
  int NumVert() const;
  int NumEdge() const;
  int NumTri() const;
  Box BoundingBox() const;
  float Precision() const;
  int Genus() const;
  Properties GetProperties() const;
  Curvature GetCurvature() const;
  ///@}

  /** @name Relation
   *  Details of the manifold's relation to its input meshes, for the purposes
   * of reapplying mesh properties.
   */
  ///@{
  MeshRelation GetMeshRelation() const;
  std::vector<int> GetMeshIDs() const;
  int SetAsOriginal();
  static std::vector<int> MeshID2Original();
  ///@}

  /** @name Modification
   *  Change this manifold in-place.
   */
  ///@{
  Manifold& Translate(glm::vec3);
  Manifold& Scale(glm::vec3);
  Manifold& Rotate(float xDegrees, float yDegrees = 0.0f,
                   float zDegrees = 0.0f);
  Manifold& Transform(const glm::mat4x3&);
  Manifold& Warp(std::function<void(glm::vec3&)>);
  Manifold& Refine(int);
  // Manifold RefineToLength(float);
  // Manifold RefineToPrecision(float);
  ///@}

  /** @name Boolean
   *  Combine two manifolds
   */
  ///@{
  enum class OpType { ADD, SUBTRACT, INTERSECT };
  Manifold Boolean(const Manifold& second, OpType op) const;
  // Boolean operation shorthand
  Manifold operator+(const Manifold&) const;  // ADD (Union)
  Manifold& operator+=(const Manifold&);
  Manifold operator-(const Manifold&) const;  // SUBTRACT (Difference)
  Manifold& operator-=(const Manifold&);
  Manifold operator^(const Manifold&) const;  // INTERSECT
  Manifold& operator^=(const Manifold&);
  std::pair<Manifold, Manifold> Split(const Manifold&) const;
  std::pair<Manifold, Manifold> SplitByPlane(glm::vec3 normal,
                                             float originOffset) const;
  Manifold TrimByPlane(glm::vec3 normal, float originOffset) const;
  ///@}

  /** @name Testing hooks
   *  These are just for internal testing.
   */
  ///@{
  bool IsManifold() const;
  bool MatchesTriNormals() const;
  int NumDegenerateTris() const;
  int NumOverlaps(const Manifold& second) const;
  ///@}

  ~Manifold();
  Manifold(const Manifold& other);
  Manifold& operator=(const Manifold& other);
  Manifold(Manifold&&) noexcept;
  Manifold& operator=(Manifold&&) noexcept;
  struct Impl;

 private:
  std::unique_ptr<Impl> pImpl_;
  static int circularSegments_;
  static float circularAngle_;
  static float circularEdgeLength_;
};
/** @} */
}  // namespace manifold