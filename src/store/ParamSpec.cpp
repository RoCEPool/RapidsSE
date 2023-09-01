/*============================================================================*/
/* ParamSpec.cpp -- Parameter Specification                                   */
/*============================================================================*/

#include "BASE.h"

#include "ParamSpec.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(STORE,ParamSpec,CN_PARAMSPEC)
__aNewFunction(ParamSpec,CN_PARAMSPEC)
__vDeleteFunction(ParamSpec,CN_PARAMSPEC)
__aFooterFunction(ParamSpec)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ParamSpec::ParamSpec(Stz* szParamNm)
: Object(CN_PARAMSPEC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARAMSPEC,_FOOTER_);
  
  m_sParamNm = szParamNm;

  _checkInitialised_(CN_PARAMSPEC);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PARAMSPEC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ParamSpec::~ParamSpec(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARAMSPEC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get parameter name                                                         */
/*----------------------------------------------------------------------------*/
Stz* ParamSpec::szParamNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARAMSPEC,_FOOTER_);

  return m_sParamNm;
}
/*----------------------------------------------------------------------------*/
/* get the size of the parameter name buffer                                  */
/*----------------------------------------------------------------------------*/
i8 ParamSpec::iParamNmBufSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARAMSPEC,_FOOTER_);

  return m_sParamNm.nAreaBytes();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
