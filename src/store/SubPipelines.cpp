/*============================================================================*/
/* SubPipelines.cpp -- List of Sub-Pipeline Specifications                    */
/*============================================================================*/

#include "BASE.h"
#include "SubPipelines.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, SubPipelines, CN_SUBPIPELINES)
__aNewFunction(SubPipelines, CN_SUBPIPELINES)
__vDeleteFunction(SubPipelines, CN_SUBPIPELINES)
__aFooterFunction(SubPipelines)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SubPipelines::SubPipelines(void)
    
: ListNL_t<PipeSpec>(CN_SUBPIPELINES)

{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBPIPELINES, _FOOTER_);


  _checkInitialised_(CN_SUBPIPELINES);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_SUBPIPELINES, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SubPipelines::~SubPipelines(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBPIPELINES, _FOOTER_);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
