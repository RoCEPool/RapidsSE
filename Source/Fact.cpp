/*============================================================================*/
/* Fact.cpp - Fact                                                            */
/*============================================================================*/

#include "Facts.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Facts,Fact,CN_FACT)
__aNewFunction(Fact,CN_FACT)
__vDeleteFunction(Fact,CN_FACT)
__aFooterFunction(Fact)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Fact::Fact(i8 iFactNo, Fact* pSuper, Stz* szNote)
: Object(CN_FACT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_pKbase  = nullptr;
  
  m_iFactNo = iFactNo;
  m_sNote   = szNote ? szNote : C_NO_NOTE;
  m_xProperty.m_pObject = nullptr;
    
  m_pSubclassOf    = pSuper;
  m_pAttributeOf   = nullptr;
  m_pElementOf     = nullptr;

  m_pAttributes    = nullptr;
  m_pAttributeNext = nullptr;

  m_pElements      = nullptr;
  m_pElementNext   = nullptr;
  
  _checkInitialised_(CN_FACT);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Fact::Fact(Kbase* pKbase, Fact* pSuper, Stz* szNote)
: Object(CN_FACT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_pKbase  = pKbase;
  
  m_iFactNo = m_pKbase->iNextFactNo();
  m_sNote   = szNote ? szNote : C_NO_NOTE;
  m_xProperty.m_pObject = nullptr;
    
  m_pSubclassOf    = pSuper;
  m_pAttributeOf   = nullptr;
  m_pElementOf     = nullptr;

  m_pAttributes    = nullptr;
  m_pAttributeNext = nullptr;

  m_pElements      = nullptr;
  m_pElementNext   = nullptr;
  
  _checkInitialised_(CN_FACT);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Fact::~Fact(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add attribute (a sub-fact)                                                 */
/*----------------------------------------------------------------------------*/
void Fact::vAttributeAdd(Fact* pAttribute)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  pAttribute->vAttributeOf(this);
  
  if (! m_pAttributeNext) {
    m_pAttributeNext = pAttribute;
    return;
  }
  
  Fact* pTail = m_pAttributeNext;
  while (pTail->pAttributeNext())
    pTail = pTail->pAttributeNext();
  
  pTail->vAttributeNext(pAttribute);
}
 
/*----------------------------------------------------------------------------*/
/* get next attribute                                                         */
/*----------------------------------------------------------------------------*/
Fact* Fact::pAttributeNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_pAttributeNext;
}

/*----------------------------------------------------------------------------*/
/* set next attribute                                                         */
/*----------------------------------------------------------------------------*/
void Fact::vAttributeNext(Fact* pAttribute)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_pAttributeNext = pAttribute;
}

/*----------------------------------------------------------------------------*/
/* get which fact this is an attribute of                                     */
/*----------------------------------------------------------------------------*/
Fact* Fact::pAttributeOf(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_pAttributeOf;
}

/*----------------------------------------------------------------------------*/
/* set which fact this is an attribute of                                     */
/*----------------------------------------------------------------------------*/
void Fact::vAttributeOf(Fact* pAttributeOf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_pAttributeOf = pAttributeOf;
}

/*----------------------------------------------------------------------------*/
/* get first attribute of this fact                                           */
/*----------------------------------------------------------------------------*/
Fact* Fact::pAttributes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_pAttributes;
}

/*----------------------------------------------------------------------------*/
/* set first attribute of this fact                                           */
/*----------------------------------------------------------------------------*/
void Fact::vAttributes(Fact* pAttributes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_pAttributes = pAttributes;
}

/*----------------------------------------------------------------------------*/
/* get attribute that is a sub-class of given super fact                      */
/*----------------------------------------------------------------------------*/
Fact* Fact::pAttributeThatIsSubClass(Fact* pOfSuperFact)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  Fact* pScan = m_pAttributes;
  while (pScan && ! pScan->bIsSubClass(pOfSuperFact))
    pScan = pScan->pAttributeNext();
    
  return pScan;
}

/*----------------------------------------------------------------------------*/
/* add given fact element to this set                                         */
/*----------------------------------------------------------------------------*/
void Fact::vElementAdd(Fact* pElement)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  pElement->vElementOf(this);
  
  if (! m_pElementNext) {
    m_pElementNext = pElement;
    return;
  }
  
  Fact* pTail = m_pElementNext;
  while (pTail->pElementNext())
    pTail = pTail->pElementNext();
  
  pTail->vElementNext(pElement);
}

/*----------------------------------------------------------------------------*/
/* get next element                                                           */
/*----------------------------------------------------------------------------*/
Fact* Fact::pElementNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_pElementNext;
}

/*----------------------------------------------------------------------------*/
/* set next fact element                                                      */
/*----------------------------------------------------------------------------*/
void Fact::vElementNext(Fact* pElement)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_pElementNext = pElement;
}

/*----------------------------------------------------------------------------*/
/* get element of which set                                                   */
/*----------------------------------------------------------------------------*/
Fact* Fact::pElementOf(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_pElementOf;
}

/*----------------------------------------------------------------------------*/
/* set element of which set                                                   */
/*----------------------------------------------------------------------------*/
void Fact::vElementOf(Fact* pElementOf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_pElementOf = pElementOf;
}

/*----------------------------------------------------------------------------*/
/* get first element of this fact set                                         */
/*----------------------------------------------------------------------------*/
Fact* Fact::pElements(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_pElements;
}

/*----------------------------------------------------------------------------*/
/* set first element of this fact set                                         */
/*----------------------------------------------------------------------------*/
void Fact::vElements(Fact* pElements)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_pElements = pElements;
}

/*----------------------------------------------------------------------------*/
/* get element that is a sub-class of given super fact                        */
/*----------------------------------------------------------------------------*/
Fact* Fact::pElementThatIsSubClass(Fact* pOfSuperFact)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  Fact* pScan = m_pElements;
  while (pScan && ! pScan->bIsSubClass(pOfSuperFact))
    pScan = pScan->pElementNext();
    
  return pScan;
}

/*----------------------------------------------------------------------------*/
/* get fact no                                                                */
/*----------------------------------------------------------------------------*/
i8 Fact::iFactNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_iFactNo;
}

/*----------------------------------------------------------------------------*/
/* set fact no                                                                */
/*----------------------------------------------------------------------------*/
void Fact::vFactNo(i8 iFactNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_iFactNo = iFactNo;
}

/*----------------------------------------------------------------------------*/
/* has an attribute which is specifically given fact                          */
/*----------------------------------------------------------------------------*/
bool Fact::bHasSpecificAttribute(Fact* pFact)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  Fact* pScan = m_pAttributes;
  while (pScan && pScan != pFact)
    pScan = pScan->pAttributeNext();
    
  return pScan;
}
 
/*----------------------------------------------------------------------------*/
/* has an element which is specifically given fact                            */
/*----------------------------------------------------------------------------*/
bool Fact::bHasSpecificElement(Fact* pFact)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  Fact* pScan = m_pElements;
  while (pScan && pScan != pFact)
    pScan = pScan->pElementNext();
    
  return pScan;
}
 
/*----------------------------------------------------------------------------*/
/* is this fact a sub-class of given fact                         (recursive) */
/*----------------------------------------------------------------------------*/
bool Fact::bIsSubClass(Fact* pOfFact)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  if (this == pOfFact)
    return true;
  
  if (! m_pSubclassOf)
    return false;
  
  return m_pSubclassOf->bIsSubClass(pOfFact);
}

/*----------------------------------------------------------------------------*/
/* get fact name                                                              */
/*----------------------------------------------------------------------------*/
Stz* Fact::szNote(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_sNote;
}

/*----------------------------------------------------------------------------*/
/* set fact note                                                              */
/*----------------------------------------------------------------------------*/
void Fact::vNote(Stz* szNote)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_sNote = szNote;
}

/*----------------------------------------------------------------------------*/
/* get property                                                               */
/*----------------------------------------------------------------------------*/
Property& Fact::rxProperty(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_xProperty;
}

/*----------------------------------------------------------------------------*/
/* get super-class fact                                                       */
/*----------------------------------------------------------------------------*/
Fact* Fact::pSubclassOf(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_pSubclassOf;
}

/*----------------------------------------------------------------------------*/
/* set super-class fact                                                       */
/*----------------------------------------------------------------------------*/
void Fact::vSubclassOf(Fact* pSubclassOf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_pSubclassOf = pSubclassOf;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
