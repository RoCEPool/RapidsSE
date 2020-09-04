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
  
  m_iFactNo = iFactNo;
  m_sNote   = szNote ? szNote : C_NO_NOTE;
  m_xProperty.m_pObject = nullptr;
  
m_pAttributes  = new FactVec;
  
  m_pSuper = pSuper;

  m_pAttrs         = nullptr;
  m_pAttributeOf   = nullptr;
  m_pNextAttribute = nullptr;

  m_pElements      = nullptr;
  m_pElementOf     = nullptr;
  m_pNextElement   = nullptr;
  
  _checkInitialised_(CN_FACT);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Fact::~Fact(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  delete m_pAttributes;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add attribute (a sub-fact)                                                 */
/*----------------------------------------------------------------------------*/
void Fact::vAddAttribute(Fact* pAttribute)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  pAttribute->vAttributeOf(this);
  
  if (! m_pNextAttribute) {
    m_pNextAttribute = pAttribute;
    return;
  }
  
  Fact* pTail = m_pNextAttribute;
  while (pTail->pNextAttribute())
    pTail = pTail->pNextAttribute();
  
  pTail->vNextAttribute(pAttribute);
}
 
/*----------------------------------------------------------------------------*/
/* append given fact element to this set                                      */
/*----------------------------------------------------------------------------*/
void Fact::vAppendElement(Fact* pElement)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  pElement->vElementOf(this);
  
  if (! m_pNextElement) {
    m_pNextElement = pElement;
    return;
  }
  
  Fact* pTail = m_pNextElement;
  while (pTail->pNextElement())
    pTail = pTail->pNextElement();
  
  pTail->vNextElement(pElement);
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
/* get attributes (the set of sub-facts)                                      */
/*----------------------------------------------------------------------------*/
Fact* Fact::pAttributes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  return m_pAttrs;
}
 
/*----------------------------------------------------------------------------*/
/* get attribute that is a sub-class of given fact                            */
/*----------------------------------------------------------------------------*/
Fact* Fact::pAttributeThatIsSubClass(Fact* pOfFact)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  i8 nAtr = m_pAttributes->nItems();
  
  for (i8 i_Atr = 0; i_Atr < nAtr; i_Atr++) {
    Fact* pAttribute = m_pAttributes->pGet(i_Atr);
    if (pAttribute->bIsSubClass(pOfFact))
      return pAttribute;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get first attribute of this fact                                           */
/*----------------------------------------------------------------------------*/
Fact* Fact::pAttrs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_pAttrs;
}

/*----------------------------------------------------------------------------*/
/* set first attribute of this fact                                           */
/*----------------------------------------------------------------------------*/
void Fact::vAttrs(Fact* pAttrs)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_pAttrs = pAttrs;
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
/* has an attribute which specifically given fact                             */
/*----------------------------------------------------------------------------*/
bool Fact::bHasSpecificAttribute(Fact* pFact)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  i8 nAtr = m_pAttributes->nItems();
  
  for (i8 i_Atr = 0; i_Atr < nAtr; i_Atr++) {
    Fact* pAttribute = m_pAttributes->pGet(i_Atr);
    if (pAttribute == pFact)
      return true;
  }

  return false;
}
 
/*----------------------------------------------------------------------------*/
/* is this fact a sub-class of given fact                         (recursive) */
/*----------------------------------------------------------------------------*/
bool Fact::bIsSubClass(Fact* pOfFact)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  if (this == pOfFact)
    return true;
  
  if (! m_pSuper)
    return false;
  
  return m_pSuper->bIsSubClass(pOfFact);
}

/*----------------------------------------------------------------------------*/
/* get next attribute                                                         */
/*----------------------------------------------------------------------------*/
Fact* Fact::pNextAttribute(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_pNextAttribute;
}

/*----------------------------------------------------------------------------*/
/* set next attribute                                                         */
/*----------------------------------------------------------------------------*/
void Fact::vNextAttribute(Fact* pAttribute)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_pNextAttribute = pAttribute;
}

/*----------------------------------------------------------------------------*/
/* get next element                                                           */
/*----------------------------------------------------------------------------*/
Fact* Fact::pNextElement(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_pNextElement;
}

/*----------------------------------------------------------------------------*/
/* set next fact element                                                      */
/*----------------------------------------------------------------------------*/
void Fact::vNextElement(Fact* pElement)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_pNextElement = pElement;
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
Fact* Fact::pSuper(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_pSuper;
}

/*----------------------------------------------------------------------------*/
/* set super-class fact                                                       */
/*----------------------------------------------------------------------------*/
void Fact::vSuper(Fact* pSuper)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_pSuper = pSuper;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
