/*============================================================================*/
/* OptimiserCommands.hpp -- the optimiser commands                            */
/*============================================================================*/

#include "BASE.h"
#include "Optimiser.hpp"
#include "OptimiserFun.hpp"
#include "eOA.h"

namespace STORE
{
  bool bOpt_add     (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);

  bool bOpt_below   (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);

  bool bOpt_clear   (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);

  bool bOpt_cmp     (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);

  bool bOpt_cut     (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);

  bool bOpt_depth   (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);

  bool bOpt_form    (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);

  bool bOpt_help    (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);

  bool bOpt_hide    (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);

  bool bOpt_load    (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);

  bool bOpt_name    (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);

  bool bOpt_reprint (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);

  bool bOpt_set     (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);

  bool bOpt_sub     (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);

  bool bOpt_weave   (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);

  bool bOpt_write   (Optimiser* pOpt, TokenVec* pTknVec, eOA& reAction);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
