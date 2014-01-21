#ifndef included_PflotranJacobianMultilevelOperatorParameters
#define included_PflotranJacobianMultilevelOperatorParameters

extern "C" {
#include "petscmat.h"
}

#include <string>

#include "tbox/Database.h"
#include "PatchHierarchy.h"
#include "MultilevelOperatorParameters.h"

namespace SAMRAI{
  namespace SAMRSolvers{
    
class PflotranJacobianMultilevelOperatorParameters: public MultilevelOperatorParameters
{
public:
   PflotranJacobianMultilevelOperatorParameters(const tbox::Pointer<tbox::Database> &database );
   ~PflotranJacobianMultilevelOperatorParameters();
   Mat *d_pMatrix;
protected:
private:
   PflotranJacobianMultilevelOperatorParameters();
};

  }
}
#endif //  included_PflotranJacobianMultilevelOperatorParameters