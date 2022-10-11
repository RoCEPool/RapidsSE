/*============================================================================*/
/* OptimiserCommands.hpp -- the optimiser commands                            */
/*============================================================================*/

#include "BASE.h"
#include "Optimiser.hpp"
#include "OptimiserFun.hpp"
#include "eOA.h"

namespace STORE
{
  bool bOpt_help    (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);

  bool bOpt_weave   (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
