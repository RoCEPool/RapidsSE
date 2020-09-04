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
void Transform::vAddAttribute(Fact* pToFact, Fact* pAttribute)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORM,_FOOTER_);
  
  pToFact->vAddAttribute(pAttribute);
}

/*----------------------------------------------------------------------------*/
/* new facet of a fact                                                        */
/*----------------------------------------------------------------------------*/
Fact* Transform::pNewAttribute(Fact* pToFact, Fact* pFacetKind,
                           Fact* pFactKind, Stz* szFactNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORM,_FOOTER_);
  

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* new fact                                                                   */
/*----------------------------------------------------------------------------*/
Fact* Transform::pNewFact(Fact* pFactKind, Stz* szFactNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORM,_FOOTER_);
  
  Fact* pFact = m_pKbase->pFactNew(szFactNm,pFactKind);
    
  return pFact;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add element to a set                                                       */
/*----------------------------------------------------------------------------*/
void Transform::vAddElement(Fact* pToSet, Fact* pElement)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORM,_FOOTER_);
  
  Fact* pFirst = pToSet->pAttributeThatIsSubClass(m_pKbase->m_pFirst);
  Fact* pLast  = pToSet->pAttributeThatIsSubClass(m_pKbase->m_pLast);

  if (! pFirst) {
    
    ASSERT(! pLast);
    
    pFirst = m_pKbase->pFactNew("a-first"_Stz,m_pKbase->m_pFirst);
    pFirst->vAddAttribute(pElement);
    pToSet->vAddAttribute(pFirst);

    pLast  = m_pKbase->pFactNew("a-last"_Stz, m_pKbase->m_pLast);
    pLast->vAddAttribute(pElement);
    pToSet->vAddAttribute(pLast);

    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Fact* pPrev = pLast->pAttributeThatIsSubClass(m_pKbase->m_pPrev);
  Fact* pNext = pLast->pAttributeThatIsSubClass(m_pKbase->m_pNext);

  if (! pPrev) {
    
    ASSERT(! pNext);
    
    pPrev = m_pKbase->pFactNew("a-prev"_Stz,m_pKbase->m_pPrev);
    pNext = m_pKbase->pFactNew("a-next"_Stz,m_pKbase->m_pNext);

    
    
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
