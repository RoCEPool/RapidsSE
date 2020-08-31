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

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add facet to fact                                                          */
/*----------------------------------------------------------------------------*/
void Transform::vAddFacet(Fact* pParent, Fact* pKind, Fact* pKid)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORM,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/* new facet of a fact                                                        */
/*----------------------------------------------------------------------------*/
Fact* Transform::pNewFacet(Fact* pParent, Fact* pKind, Stz* szFactNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORM,_FOOTER_);
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* new fact                                                                   */
/*----------------------------------------------------------------------------*/
Fact* Transform::pNewFact(Fact* pKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORM,_FOOTER_);
  
  Fact* pFact = m_pKbase->pNewFact();
  
  pFact->vKind(pKind);
  
  return pFact;
}

/*----------------------------------------------------------------------------*/
/* new fact                                                                   */
/*----------------------------------------------------------------------------*/
Fact* Transform::pNewFact(Stz* szFactNm, Fact* pKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORM,_FOOTER_);
  
  Fact* pFact = m_pKbase->pNewFact(szFactNm);
  
  pFact->vKind(pKind);
  
  return pFact;
}

/*----------------------------------------------------------------------------*/
/* new fact (variable args)                                                   */
/*----------------------------------------------------------------------------*/
Fact* Transform::pNewFactv(Fact* pKind, ...)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORM,_FOOTER_);
  
  Fact* pFact = m_pKbase->pNewFact();
  
  pFact->vKind(pKind);
  
  return pFact;
}

/*----------------------------------------------------------------------------*/
/* new fact                                                                   */
/*----------------------------------------------------------------------------*/
Fact* Transform::pNewFactv(Stz* szFactNm, Fact* pKind, ...)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORM,_FOOTER_);
  
  Fact* pFact = m_pKbase->pNewFact(szFactNm);
  
  pFact->vKind(pKind);
  
  return pFact;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
