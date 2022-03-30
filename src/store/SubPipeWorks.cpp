/*============================================================================*/
/* SubPipeWorks.cpp -- List of Sub-Pipeline Specifications                    */
/*============================================================================*/

#include "BASE.h"
#include "SubPipeWorks.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, SubPipeWorks, CN_SUBPIPEWORKS)
__aNewFunction(SubPipeWorks, CN_SUBPIPEWORKS)
__vDeleteFunction(SubPipeWorks, CN_SUBPIPEWORKS)
__aFooterFunction(SubPipeWorks)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SubPipeWorks::SubPipeWorks(void)
    
: ListNL_t<PipeWork>(CN_SUBPIPEWORKS)

{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBPIPEWORKS, _FOOTER_);


  _checkInitialised_(CN_SUBPIPEWORKS);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_SUBPIPEWORKS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SubPipeWorks::~SubPipeWorks(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBPIPEWORKS, _FOOTER_);

  while (PipeWork* pSubPipeWork = pPop())
    delete pSubPipeWork;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
