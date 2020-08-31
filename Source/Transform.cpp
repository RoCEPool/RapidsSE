/*============================================================================*/
/* Transform.cpp - Transform                                                  */
/*============================================================================*/

#include "Facts.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Facts,Transform,CN_TRANSFORM)
__aNewFunction(Transform,CN_TRANSFORM)
__vDeleteFunction(Transform,CN_TRANSFORM)
__aFooterFunction(Transform)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Transform::Transform(Kbase* pKbase)
: Object(CN_TRANSFORM)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORM,_FOOTER_);
  
  m_pKbase = pKbase;
    
  _checkInitialised_(CN_TRANSFORM);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Transform::~Transform(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORM,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
