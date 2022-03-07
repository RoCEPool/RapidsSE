/*============================================================================*/
/* OptimiserCommands.hpp -- the optimiser commands                            */
/*============================================================================*/

#include "BASE.h"
#include "Optimiser.hpp"
#include "OptimiserFun.hpp"

namespace STORE
{
  bool bOpt_add     (Optimiser* pOpt, TokenVec* pTknVec, i8& riAction);

  bool bOpt_below   (Optimiser* pOpt, TokenVec* pTknVec, i8& riAction);

  bool bOpt_clear   (Optimiser* pOpt, TokenVec* pTknVec, i8& riAction);

  bool bOpt_depth   (Optimiser* pOpt, TokenVec* pTknVec, i8& riAction);

  bool bOpt_form    (Optimiser* pOpt, TokenVec* pTknVec, i8& riAction);

  bool bOpt_help    (Optimiser* pOpt, TokenVec* pTknVec, i8& riAction);

  bool bOpt_hide    (Optimiser* pOpt, TokenVec* pTknVec, i8& riAction);

  bool bOpt_load    (Optimiser* pOpt, TokenVec* pTknVec, i8& riAction);

  bool bOpt_name    (Optimiser* pOpt, TokenVec* pTknVec, i8& riAction);

  bool bOpt_reprint (Optimiser* pOpt, TokenVec* pTknVec, i8& riAction);

  bool bOpt_set     (Optimiser* pOpt, TokenVec* pTknVec, i8& riAction);

  bool bOpt_sub     (Optimiser* pOpt, TokenVec* pTknVec, i8& riAction);

  bool bOpt_weave   (Optimiser* pOpt, TokenVec* pTknVec, i8& riAction);

  bool bOpt_write   (Optimiser* pOpt, TokenVec* pTknVec, i8& riAction);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
