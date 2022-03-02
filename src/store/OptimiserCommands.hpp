/*============================================================================*/
/* OptimiserCommands.hpp -- the optimiser commands                            */
/*============================================================================*/

#include "BASE.h"
#include "Optimiser.hpp"
#include "OptimiserFun.hpp"

namespace STORE
{
  bool bOpt_add    (Optimiser* pOptimiser, TokenVec* pTknVec);

  bool bOpt_depth  (Optimiser* pOptimiser, TokenVec* pTknVec);

  bool bOpt_sub    (Optimiser* pOptimiser, TokenVec* pTknVec);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
