#include "sta/Parasitics.hh"

namespace par3d
{

class VCModel
{
public:
    VCModel();
    ~VCModel();

    void setModel(const std::string& modelPath);
    sta::Parasitics* getParasitics() const;

private:

};

}