/*============================================================================*/
/* ParamsSpec.cpp -- Parameters (list) Specification                          */
/*============================================================================*/

#include "BASE.h"

#include "ParamsSpec.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(STORE,ParamsSpec,CN_PARAMSSPEC)
__aNewFunction(ParamsSpec,CN_PARAMSSPEC)
__vDeleteFunction(ParamsSpec,CN_PARAMSSPEC)
__aFooterFunction(ParamsSpec)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ParamsSpec::ParamsSpec(void)
: ListNL_t<ParamSpec>(CN_PARAMSSPEC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARAMSSPEC,_FOOTER_);

  _checkInitialised_(CN_PARAMSSPEC);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PARAMSSPEC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ParamsSpec::~ParamsSpec(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARAMSSPEC,_FOOTER_);

  while (ParamSpec* pParamSpec = pPop())
    delete pParamSpec;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
