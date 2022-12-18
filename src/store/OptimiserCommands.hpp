/*============================================================================*/
/* OptimiserCommands.hpp -- the optimiser commands                            */
/*============================================================================*/

#include "BASE.h"
#include "Optimiser.hpp"
#include "OptimiserFun.hpp"
#include "eOA.h"

namespace STORE
{
  bool bOpt_default          (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);
  bool bOpt_expanded         (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);
  bool bOpt_help             (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);
  bool bOpt_longer           (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);
  bool bOpt_notation         (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);
  bool bOpt_readPipeline     (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);
  bool bOpt_shorter          (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);
  bool bOpt_toEnd            (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);
  bool bOpt_weave            (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);
  bool bOpt_writePipeline    (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
