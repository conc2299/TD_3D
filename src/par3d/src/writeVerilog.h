#include <ostream>
#include <fstream>
#include <string>
#include <vector>


namespace odb{
class dbDatabase;
}

namespace par3d{




class VerilogWriter
{
public:
    VerilogWriter(std::ostream* os, odb::dbDatabase* db) : os_(os),db_(db)  {}
    bool write_to_file(std::string file_prefix, std::string module_prefix, int num_parts);
private:
    bool write_pins();
    bool write_netlist();
    odb::dbDatabase* db_;
    std::ostream* os_;
};


}
