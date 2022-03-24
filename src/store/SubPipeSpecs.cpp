/*============================================================================*/
/* SubPipeSpecs.cpp -- List of Sub-Pipeline Specifications                    */
/*============================================================================*/

#include "BASE.h"
#include "SubPipeSpecs.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, SubPipeSpecs, CN_SUBPIPESPECS)
__aNewFunction(SubPipeSpecs, CN_SUBPIPESPECS)
__vDeleteFunction(SubPipeSpecs, CN_SUBPIPESPECS)
__aFooterFunction(SubPipeSpecs)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SubPipeSpecs::SubPipeSpecs(void)
    
: ListNL_t<PipeSpec>(CN_SUBPIPESPECS)

{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBPIPESPECS, _FOOTER_);


  _checkInitialised_(CN_SUBPIPESPECS);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_SUBPIPESPECS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SubPipeSpecs::~SubPipeSpecs(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBPIPESPECS, _FOOTER_);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
