/*============================================================================*/
/* Kbase.cpp - Kbase                                                          */
/*============================================================================*/

#include "Facts.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Facts,Kbase,CN_KBASE)
__aNewFunction(Kbase,CN_KBASE)
__vDeleteFunction(Kbase,CN_KBASE)
__aFooterFunction(Kbase)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Kbase::Kbase(Stz* szKbName)
: Object(CN_KBASE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  vAprioriFacts();
  
  _checkInitialised_(CN_KBASE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Kbase::~Kbase(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  // more tbd ...
  
  delete m_pRoot;
  
  delete m_pFact;
  delete m_pFirst;
  delete m_pInt;
  delete m_pObject;
  delete m_pName;
  delete m_pNext;
  delete m_pPrev;
  delete m_pText;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* new fact                                                                   */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pFactNew(Fact* pFactKind, Stz* szFactNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pFact = new Fact(++ m_ixFactNo, pFactKind, szFactNm);

  return pFact;
}

/*----------------------------------------------------------------------------*/
/* delete a fact                                                              */
/*----------------------------------------------------------------------------*/
void Kbase::vFactDelete(Fact* pFact)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
    
  delete pFact;
}

/*----------------------------------------------------------------------------*/
/* borrow/new a question                                                      */
/*----------------------------------------------------------------------------*/
Question* Kbase::pQuestionBorrow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Question* pQuestion = new Question(this);
  
  return pQuestion;
}

/*----------------------------------------------------------------------------*/
/* return/delete a question                                                   */
/*----------------------------------------------------------------------------*/
void Kbase::vQuestionReturn(Question* pQuestion)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
    
  delete pQuestion;
}

/*----------------------------------------------------------------------------*/
/* borrow/new a transform                                                     */
/*----------------------------------------------------------------------------*/
Transform* Kbase::pTransformBorrow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Transform* pTransform = new Transform(this);
  
  return pTransform;
}

/*----------------------------------------------------------------------------*/
/* return/delete a transform                                                  */
/*----------------------------------------------------------------------------*/
void Kbase::vTransformReturn(Transform* pTransform)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
    
  delete pTransform;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add facet to fact                                                          */
/*----------------------------------------------------------------------------*/
void Kbase::vAddFacet(Fact* pToFact, Facet* pFacet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  pToFact->vAddFacet(pFacet);
}


/*----------------------------------------------------------------------------*/
/* new fact                                                                   */
/*----------------------------------------------------------------------------*/
Fact* Kbase::pNewFact(Fact* pFactKind, Stz* szFactNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);
  
  Fact* pFact = pFactNew(pFactKind,szFactNm);
  
  return pFact;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add apriori facts                                                          */
/*----------------------------------------------------------------------------*/
void Kbase::vAprioriFacts(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_KBASE,_FOOTER_);

  m_pRoot   = pFactNew(nullptr,nullptr);
  
  // the primitives
  m_pFact   = pFactNew(nullptr,nullptr);

  m_pFirst  = pFactNew(nullptr,nullptr);
  m_pPrev   = pFactNew(nullptr,nullptr);
  m_pNext   = pFactNew(nullptr,nullptr);
  m_pNode   = pFactNew(nullptr,nullptr);

  m_pName   = pFactNew(nullptr,nullptr);
  m_pInt    = pFactNew(nullptr,nullptr);
  m_pObject = pFactNew(nullptr,nullptr);
  m_pText   = pFactNew(nullptr,nullptr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
