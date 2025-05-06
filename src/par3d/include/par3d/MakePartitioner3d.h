#pragma once

namespace par3d {
class Partitioner3d;
}

namespace ord {

class OpenRoad;

par3d::Partitioner3d* makePartitioner3d();

void initPartitioner3d(OpenRoad* openroad);

void deletePartitioner3d(par3d::Partitioner3d* partitioner3d);

}  // namespace ord
