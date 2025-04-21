#pragma once



namespace odb {
class dbDatabase;
class dbInst;

}  // namespace odb
namespace sta {
class dbSta;
class dbNetwork;
}

namespace utl {
class Logger;
}

namespace gpl3d {

class Placer3d
{
public:
    Placer3d() = default;
    ~Placer3d() = default;

    void init(odb::dbDatabase* db,
              sta::dbNetwork* db_network,
              sta::dbSta* sta,
              utl::Logger* logger);
    void run(int argc);

private:
    odb::dbDatabase* db_ = nullptr;
    sta::dbNetwork* db_network_ = nullptr;
    sta::dbSta* sta_ = nullptr;
    utl::Logger* logger_ = nullptr;
};

}  // namespace gpl3d