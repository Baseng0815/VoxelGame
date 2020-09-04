#include "../../include/Systems/CloudSystem.h"

CloudModule::CloudModule()
    : module::Module { GetSourceModuleCount() }
{  }

int CloudModule::GetSourceModuleCount() const
{ return 1; }

double CloudModule::GetValue(double x, double y, double z) const
{
    double val = m_pSourceModule[0]->GetValue(x, y, z);
    /* return val; */
    if (val < 0) return -1;
    else return 1;
}

void CloudSystem::_update(int dt)
{

}

CloudSystem::CloudSystem(Registry_T &registry)
    : System {registry, 10}
{

}
