/*============================================================================*/
/* LocalStrs.cpp -- Local Variables                                           */
/*============================================================================*/

#include "BASE.h"

#include "LocalStrs.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,LocalStrs,CN_LOCALSTRS)
__aNewFunction(LocalStrs,CN_LOCALSTRS)
__vDeleteFunction(LocalStrs,CN_LOCALSTRS)
__aFooterFunction(LocalStrs)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LocalStrs::LocalStrs(void)
: Object(CN_LOCALSTRS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSTRS,_FOOTER_);
 
  m_pLocalVars = nullptr;

  for (i8 i_LclStr = 0; i_LclStr < C_LOCAL_STRS; i_LclStr++) {
    _LclStr& rxLvlStr = m_xaLclStr[i_LclStr];
    
    rxLvlStr.m__dummy0 = C_UNKNOWN;
    rxLvlStr.m__dummy1 = C_UNKNOWN;
    
    MemLib::vZero(rxLvlStr.m__strspace,
                  sizeof(rxLvlStr.m__strspace),
                  CN_LOCALSTRS);
  }
  
  _checkInitialised_(CN_LOCALSTRS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOCALSTRS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LocalStrs::~LocalStrs(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSTRS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
