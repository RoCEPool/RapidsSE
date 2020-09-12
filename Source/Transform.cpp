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
Transform::Transform(Kbase* pKbase, Fact* pSelf)
: Object(CN_TRANSFORM)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORM,_FOOTER_);
  
  m_pKbase = pKbase;
  m_pSelf  = pSelf;
    
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

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add facet to fact                                                          */
/*----------------------------------------------------------------------------*/
//void Transform::vAddAttribute(Fact* pToFact, Fact* pAttribute)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORM,_FOOTER_);
//  
//  pToFact->vAttributeAppend(pAttribute);
//}

/*----------------------------------------------------------------------------*/
/* new facet of a fact                                                        */
/*----------------------------------------------------------------------------*/
//Fact* Transform::pNewAttribute(Fact* pToFact, Fact* pFacetKind,
//                           Fact* pFactKind, Stz* szFactNm)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORM,_FOOTER_);
//  
//
//  return nullptr;
//}

/*----------------------------------------------------------------------------*/
/* new fact                                                                   */
/*----------------------------------------------------------------------------*/
//Fact* Transform::pNewFact(Fact* pSuper, Stz* szNote)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORM,_FOOTER_);
//  
//  Fact* pFact = m_pKbase->pFactNew(pSuper,szNote);
//    
//  return pFact;
//}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
