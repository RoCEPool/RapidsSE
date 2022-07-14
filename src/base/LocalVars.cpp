/*============================================================================*/
/* LocalVars.cpp -- Local Variables                                           */
/*============================================================================*/

#include "BASE.h"

#include "LocalVars.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,LocalVars,CN_LOCALVARS)
__aNewFunction(LocalVars,CN_LOCALVARS)
__vDeleteFunction(LocalVars,CN_LOCALVARS)
__aFooterFunction(LocalVars)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LocalVars::LocalVars(void)
: Object(CN_LOCALVARS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALVARS,_FOOTER_);
  
  m_pLocalStrs = nullptr;
  
  for (i8 i_LclVar = 0; i_LclVar < C_LOCAL_VARS; i_LclVar++) {
    _LclVar& rxLvlVar = m_xaLclVar[i_LclVar];

    rxLvlVar.m_i_TopLocal = C_UNKNOWN;
    rxLvlVar.m__dummy0    = C_UNKNOWN;
  }
  
  _checkInitialised_(CN_LOCALVARS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOCALVARS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LocalVars::~LocalVars(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALVARS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* delete frame of locals                                                     */
/*----------------------------------------------------------------------------*/
void LocalVars::vDeleteFrame(void*)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALVARS,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* get given local                                                            */
/*----------------------------------------------------------------------------*/
_LclVar* LocalVars::aGetLocal(i8 i_Local)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALVARS,_FOOTER_);

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* create new frame of locals                                                 */
/*----------------------------------------------------------------------------*/
void LocalVars::vNewFrame(void*, i8 nLocals)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALVARS,_FOOTER_);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
