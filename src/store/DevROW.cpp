/*============================================================================*/
/* DevROW.hpp -- ROW Device                                                   */
/*============================================================================*/

#include "BASE.h"

#include "DevROW.hpp"
#include "StreamDef.hpp"
#include "StreamDefRht.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,DevROW,CN_DEVROW)
__aNewFunction(DevROW,CN_DEVROW)
__vDeleteFunction(DevROW,CN_DEVROW)
__aFooterFunction(DevROW)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
DevROW::DevROW(void)
: Device(CN_DEVROW)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DEVROW,_FOOTER_);
  
  m_sTableNm   = ""_Stz;
  m_i_Row      = C_UNKNOWN;
  m_nRows      = C_UNKNOWN;
  m_aRow       = nullptr;
  m_pRowBuffer = nullptr;
  m_pEachRowBuffer = nullptr;
  m_pStreamDef     = nullptr;

  _checkInitialised_(CN_DEVROW);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_DEVROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
DevROW::~DevROW(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DEVROW,_FOOTER_);

  if (m_pStreamDef) {
    if (m_pStreamDef->eGetClassNo() == CN_STREAMDEF)
      delete m_pStreamDef;
    else {
      StreamDefRht* pStreamDefRht = _pStreamDefRht(m_pStreamDef);
      delete pStreamDefRht;
    }
  }
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
