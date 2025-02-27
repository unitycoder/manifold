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

Manifold TetPuzzle(float edgeLength, float gap, int nDivisions) {
  const glm::vec3 scale(edgeLength / (2 * sqrt(2)));

  Manifold tet = Manifold::Tetrahedron().Scale(scale);

  Polygons box;
  box.push_back({{glm::vec2(2, -2), 0}, {glm::vec2(2, 2), 1}});

  for (int i = 0; i <= nDivisions; ++i) {
    box[0].push_back({glm::vec2(gap / 2, 2 - i * 4.0f / nDivisions), 2 + i});
  }

  Manifold screw = Manifold::Extrude(box, 2, nDivisions, 270)
                       .Rotate(0, 0, -45)
                       .Translate({0, 0, -1})
                       .Scale(scale);

  return tet ^ screw;
}
}  // namespace manifold