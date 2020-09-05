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
/* is this fact an assocation?                                                */
/*----------------------------------------------------------------------------*/
bool Fact::bIsAssociation(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  return bIsSubClass(m_pKbase->m_pAssociation);
}

/*----------------------------------------------------------------------------*/
/* is this fact an int?                                                       */
/*----------------------------------------------------------------------------*/
bool Fact::bIsInt(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  return bIsSubClass(m_pKbase->m_pInt);
}

/*----------------------------------------------------------------------------*/
/* is this fact an object?                                                    */
/*----------------------------------------------------------------------------*/
bool Fact::bIsObject(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  return bIsSubClass(m_pKbase->m_pObject);
}

/*----------------------------------------------------------------------------*/
/* is this fact an opaque?                                                    */
/*----------------------------------------------------------------------------*/
bool Fact::bIsOpaque(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  return bIsSubClass(m_pKbase->m_pOpaque);
}

/*----------------------------------------------------------------------------*/
/* is this fact a text?                                                       */
/*----------------------------------------------------------------------------*/
bool Fact::bIsText(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  return bIsSubClass(m_pKbase->m_pText);
}

/*----------------------------------------------------------------------------*/
/* is this fact a word?                                                       */
/*----------------------------------------------------------------------------*/
bool Fact::bIsWord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  return bIsSubClass(m_pKbase->m_pWord);
}

/*----------------------------------------------------------------------------*/
/* create a new instance of this fact                                         */
/*----------------------------------------------------------------------------*/
Fact* Fact::pNewInstance(Stz* szNote)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  Fact* pFact = new Fact(m_pKbase, this, szNote);
  
  if (! szNote)
    pFact->m_sNote.vSprint("isa-%s"_Stz,m_sNote.azChar());
    
  return pFact;
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
/* print fact (recursive)                                                     */
/*----------------------------------------------------------------------------*/
void Fact::vPrint_r(i8 iDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  vTrace("[%lld] %s\n"_Stz,
         m_iFactNo,
         m_sNote.azChar()
         );
  
       if (bIsAssociation()) { }
  else if (bIsInt())         { }
  else if (bIsObject())      { }
  else if (bIsOpaque())      { }
  else if (bIsText())        { }
  else if (bIsWord())        { }
  else { }

  for (Fact* pAttribute = m_pAttributes;
       pAttribute;
       pAttribute = pAttribute->m_pAttributeNext)
    pAttribute->vPrint_r(iDepth + 1);

  for (Fact* pElement = m_pElements;
       pElement;
       pElement = pElement->m_pElementNext)
    pElement->vPrint_r(iDepth + 1);
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
