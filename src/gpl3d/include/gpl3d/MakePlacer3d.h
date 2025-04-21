#pragma once

namespace gpl3d {
class Placer3d;
}

namespace ord {

class OpenRoad;

gpl3d::Placer3d* makePlacer3d();

void initPlacer3d(OpenRoad* openroad);

void deletePlacer3d(gpl3d::Placer3d* placer3d);

}  // namespace ord
