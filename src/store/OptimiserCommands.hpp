/*============================================================================*/
/* OptimiserCommands.hpp -- the optimiser commands                            */
/*============================================================================*/

#include "BASE.h"
#include "Optimiser.hpp"
#include "OptimiserFun.hpp"
#include "eOA.h"

namespace STORE
{
  bool bOpt_clear   (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);

  bool bOpt_help    (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);

  bool bOpt_load    (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);

  bool bOpt_name    (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);

  bool bOpt_reprint (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);

  bool bOpt_weave   (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);

  bool bOpt_write   (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
