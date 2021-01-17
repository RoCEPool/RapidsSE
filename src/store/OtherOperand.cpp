/*============================================================================*/
/* OtherOperand.cpp -- Other Operand                                          */
/*============================================================================*/

#include "BASE.h"

#include "OtherOperand.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,OtherOperand,CN_OTHEROPERAND)
__aNewFunction(OtherOperand,CN_OTHEROPERAND)
__vDeleteFunction(OtherOperand,CN_OTHEROPERAND)
__aFooterFunction(OtherOperand)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
OtherOperand::OtherOperand(Object* pObject)
: Operand(CN_OTHEROPERAND)
{
  _precondition_(_MAGIC_,CN_OTHEROPERAND);
  
  m_pObject = pObject;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
OtherOperand::~OtherOperand(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* get the object                                                             */
/*----------------------------------------------------------------------------*/
Object* OtherOperand::pObject(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_pObject;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
