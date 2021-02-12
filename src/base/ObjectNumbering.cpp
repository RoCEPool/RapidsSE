/*============================================================================*/
/* ObjectNumbering.cpp -- Object Numbering                                    */
/*============================================================================*/

#include "BASE.h"

#include "ObjectNumbering.hpp"

/*----------------------------------------------------------------------------*/
/* -- LifeCycle --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,ObjectNumbering,CN_OBJECTNUMBERING)
__aNewFunction(ObjectNumbering,CN_OBJECTNUMBERING)
__vDeleteFunction(ObjectNumbering,CN_OBJECTNUMBERING)
__aFooterFunction(ObjectNumbering)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ObjectNumbering::ObjectNumbering(void)
: Object(CN_OBJECTNUMBERING)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OBJECTNUMBERING);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_OBJECTNUMBERING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ObjectNumbering::~ObjectNumbering(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OBJECTNUMBERING,_FOOTER_);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* -- Public -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get next object no for given class                                         */
/*----------------------------------------------------------------------------*/
i8 ObjectNumbering::iNextNo(eCN eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OBJECTNUMBERING);
  
  i8 iObjectNo;
  
  if (CN_JOB == eClassNo)
    iObjectNo = AM::iNextJobId();
  else
    iObjectNo = ++ m_ixaNextObjectId[eClassNo];
  
  return iObjectNo;
}

/*----------------------------------------------------------------------------*/
/* set node no's where appropriate                                            */
/*----------------------------------------------------------------------------*/
void ObjectNumbering::vNodeNo(i8 iThisNodeNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OBJECTNUMBERING);

  m_ixaNextObjectId[CN_OBJECTNUMBERING] = iThisNodeNo << 48;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
