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

#include "samples.h"

namespace manifold {

Manifold RoundedFrame(float edgeLength, float radius) {
  Manifold edge = Manifold::Cylinder(edgeLength, radius);
  Manifold corner = Manifold::Sphere(radius);

  Manifold edge1 = corner + edge;
  edge1.Rotate(-90).Translate({-edgeLength / 2, -edgeLength / 2, 0});

  Manifold edge2 = edge1;
  edge2.Rotate(0, 0, 180);
  edge2 += edge1;
  edge2 += edge.Translate({-edgeLength / 2, -edgeLength / 2, 0});

  Manifold edge4 = edge2;
  edge4.Rotate(0, 0, 90);
  edge4 += edge2;

  Manifold frame = edge4.Translate({0, 0, -edgeLength / 2});
  frame += edge4.Rotate(180);

  return frame;
}
}  // namespace manifold