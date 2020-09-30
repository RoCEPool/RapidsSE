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
Fact::Fact(Kbase* pKbase, i8 iFactNo, Stz* szName)
: Object(CN_FACT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  ASSERT(pKbase->pFact());
  
  m_pKbase  = pKbase;
  
  m_iFactNo = C_UNKNOWN == iFactNo ? m_pKbase->iNextFactNo() : iFactNo;
  m_sFactNm   = szName ? szName : C_NO_NOTE;
  m_xValue.m_pObject = nullptr;
    
  m_pSubclassOf    = nullptr;
  m_pAttributeOf   = nullptr;
  m_pElementOf     = nullptr;

  m_pAttributes    = nullptr;
  m_pAttributeNext = nullptr;

  m_pElements      = nullptr;
  m_pElementNext   = nullptr;
  
  m_bDeleteMethod  = false;
  
  _checkInitialised_(CN_FACT);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Fact::Fact(Kbase* pKbase, Fact* pSuper, Stz* szName)
: Object(CN_FACT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  ASSERT(!pSuper || pKbase->pFact());
  
  m_pKbase  = pKbase;
  
  m_iFactNo = pSuper ? m_pKbase->iNextFactNo() : 0;
  m_sFactNm   = szName ? szName : C_NO_NOTE;
  m_xValue.m_pObject = nullptr;
    
  m_pSubclassOf    = pSuper;
  m_pAttributeOf   = nullptr;
  m_pElementOf     = nullptr;

  m_pAttributes    = nullptr;
  m_pAttributeNext = nullptr;

  m_pElements      = nullptr;
  m_pElementNext   = nullptr;
  
  m_bDeleteMethod  = false;
  
  _checkInitialised_(CN_FACT);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Fact::~Fact(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  //vTrace("~Fact %p ... %s\n"_Stz,this,m_sFactNm.azChar());
  
  if (m_bDeleteMethod)
    delete m_xValue.m_pMethod;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* delete fact recursively                                                    */
/*----------------------------------------------------------------------------*/
void Fact::vDeleteRecursively(Fact** ppFact)
{
  _static_
  
  Fact* pFact = *ppFact;
  *ppFact = nullptr;
  
  if (pFact->m_pAttributeOf)
    pFact->m_pAttributeOf->vAttributeCut(pFact);
  
  if (pFact->m_pElementOf)
    pFact->m_pElementOf->vElementCut(pFact);
  
  Fact* pNext = nullptr;
  for (Fact* pAttribute = pFact->m_pAttributes;
       pAttribute;
       pAttribute = pNext) {
    pNext = pAttribute->m_pAttributeNext;
    vDeleteRecursively(&pAttribute);
  }

  pNext = nullptr;
  for (Fact* pElement = pFact->m_pElements;
       pElement;
       pElement = pNext) {
    pNext = pElement->m_pElementNext;
    vDeleteRecursively(&pElement);
  }

  delete pFact;
}

/*----------------------------------------------------------------------------*/
/* delete facts below given fact recursively                                  */
/*----------------------------------------------------------------------------*/
void Fact::vDeleteBelowRecursively(Fact* pFact)
{
  _static_
    
  for (Fact* pScan = pFact->m_pAttributes;
       pScan;
       pScan = pScan->m_pAttributeNext)
    vDeleteRecursively(&pScan);

  for (Fact* pScan = pFact->m_pElements;
       pScan;
       pScan = pScan->m_pElementNext)
    vDeleteRecursively(&pScan);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* build ancestry string (recursive)                                          */
/*----------------------------------------------------------------------------*/
void Fact::vAncestry(Stz* szDst)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  if (* szDst->azChar()) {
    szDst->vConcat("."_Stz);
  }
  
  szDst->vCatSprint("[%lld]%s"_Stz,m_iFactNo,m_sFactNm.azChar());
    
  if (m_pSubclassOf)
    m_pSubclassOf->vAncestry(szDst);
}

/*----------------------------------------------------------------------------*/
/* append attribute to end of this facts attributes                           */
/*----------------------------------------------------------------------------*/
void Fact::vAttributeAppend(Fact* pAttribute)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  pAttribute->vAttributeOf(this);
  
  if (! m_pAttributes) {
    m_pAttributes = pAttribute;
    return;
  }
  
  Fact* pTail = m_pAttributes;
  while (pTail->pAttributeNext())
    pTail = pTail->pAttributeNext();
  
  pTail->vAttributeNext(pAttribute);
}
 
/*----------------------------------------------------------------------------*/
/* cut given element from this fact                                           */
/*----------------------------------------------------------------------------*/
void Fact::vAttributeCut(Fact* pAttribute)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  ASSERT(pAttribute->m_pAttributeOf == this);
  pAttribute->m_pAttributeOf = nullptr;

  // is it the head attribute?
  if (pAttribute == m_pAttributes) {
    m_pAttributes = m_pAttributes->m_pAttributeNext;
    
    pAttribute->m_pAttributeNext = nullptr;
    
    return;
  }
  
  // find the prior attribute
  Fact* pPrior;
  for (pPrior = m_pAttributes;
       pPrior;
       pPrior = pPrior->m_pAttributeNext)
    if (pPrior->m_pAttributeNext == pAttribute)
      break;

  // cut the attribute
  pPrior->m_pAttributeNext = pAttribute->m_pAttributeNext;
  pAttribute->m_pAttributeNext = nullptr;
}

/*----------------------------------------------------------------------------*/
/* find attribute fact                                                        */
/*----------------------------------------------------------------------------*/
Fact* Fact::pAttributeFindNm(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
      
  for (Fact* pAtr = m_pAttributes;
       pAtr;
       pAtr = pAtr->m_pAttributeNext) {
    
    if (pAtr->bSameName(szName))
      return pAtr;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find attribute fact                                                        */
/*----------------------------------------------------------------------------*/
Fact* Fact::pAttributeFindNmVal(Stz* szName, Stz* szValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
    
  for (Fact* pAtr = m_pAttributes;
       pAtr;
       pAtr = pAtr->m_pAttributeNext) {
    
    if (! pAtr->bSameName(szName))
      continue;

    // has to be a text
    if (! pAtr->bIsSubClass(m_pKbase->m_p_Text_))
      continue;
    
    if (! pAtr->m_xValue.m_pText->szGet()->bSame(szValue))
      continue;

    return pAtr;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* insert attribute at head of this facts attributes                          */
/*----------------------------------------------------------------------------*/
void Fact::vAttributeInsert(Fact* pAttribute)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  pAttribute->vAttributeOf(this);
  
  if (! m_pAttributes) {
    m_pAttributes = pAttribute;
    return;
  }

  pAttribute->m_pAttributeNext = m_pAttributes;
  m_pAttributes = pAttribute;
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
/* get int value of named attribute                                           */
/*----------------------------------------------------------------------------*/
i8 Fact::iAttributeValNm(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
    
  Fact* pAttribute = pAttributeFindNm(szName);
  
  if (! pAttribute)
    return C_UNKNOWN;
  
  i8 iVal = pAttribute->iValue();
  
  return iVal;
}

/*----------------------------------------------------------------------------*/
/* append given element to end of this facts elements                         */
/*----------------------------------------------------------------------------*/
void Fact::vElementAppend(Fact* pElement)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  pElement->vElementOf(this);
  
  if (! m_pElements) {
    m_pElements = pElement;
    return;
  }
  
  Fact* pTail = m_pElements;
  while (pTail->pElementNext())
    pTail = pTail->pElementNext();
  
  pTail->vElementNext(pElement);
}

/*----------------------------------------------------------------------------*/
/* cut given element from this fact                                           */
/*----------------------------------------------------------------------------*/
void Fact::vElementCut(Fact* pElement)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  ASSERT(pElement->m_pElementOf == this);
  pElement->m_pElementOf = nullptr;
  
  // is it the head element?
  if (pElement == m_pElements) {
    m_pElements = m_pElements->m_pElementNext;
    
    pElement->m_pElementNext = nullptr;
    
    return;
  }
  
  // find the prior element
  Fact* pPrior;
  for (pPrior = m_pElements;
       pPrior;
       pPrior = pPrior->m_pElementNext)
    if (pPrior->m_pElementNext == pElement)
      break;

  // cut the element
  pPrior->m_pElementNext = pElement->m_pElementNext;
  pElement->m_pElementNext = nullptr;
}

/*----------------------------------------------------------------------------*/
/* find element fact (recursively)                                            */
/*----------------------------------------------------------------------------*/
Fact* Fact::pElementFindNm(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  if (m_sFactNm.bSame(szName))
    return this;
  
  Fact* pElement = nullptr;
  
  for (Fact* pScan = m_pElements;
       pScan && !pElement;
       pScan = pScan->m_pElementNext)
    pElement = pScan->pElementFindNm(szName);

  return pElement;
}

/*----------------------------------------------------------------------------*/
/* find element fact (recursively)                                            */
/*----------------------------------------------------------------------------*/
Fact* Fact::pElementFindNmVal(Stz* szName, Stz* szValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  vTrace("pElementSearch %s\n"_Stz,m_sFactNm.azChar());
  
  if (pAttributeFindNmVal(szName,szValue))
    return this;

  Fact* pElement = nullptr;
  
  for (Fact* pScan = m_pElements;
       pScan && !pElement;
       pScan = pScan->m_pElementNext)
    pElement = pScan->pElementFindNmVal(szName,szValue);

  return pElement;
}

/*----------------------------------------------------------------------------*/
/* insert element as first element of this fact                               */
/*----------------------------------------------------------------------------*/
void Fact::vElementInsert(Fact* pElement)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  pElement->m_pElementNext = m_pElements;
  
  m_pElements = pElement;
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
/* find fact matching given fact no                                           */
/*----------------------------------------------------------------------------*/
Fact* Fact::pFind(i8 iFactNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  if (iFactNo == m_iFactNo)
    return this;
    
  for (Fact* pAttribute = m_pAttributes;
       pAttribute;
       pAttribute = pAttribute->m_pAttributeNext) {
    Fact* pFound = pAttribute->pFind(iFactNo);
    if (pFound)
      return pFound;
  }

  for (Fact* pElement = m_pElements;
       pElement;
       pElement = pElement->m_pElementNext) {
    Fact* pFound = pElement->pFind(iFactNo);
    if (pFound)
      return pFound;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find fact matching given fact name                                         */
/*----------------------------------------------------------------------------*/
Fact* Fact::pFind(Stz* szFactNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  if (m_sFactNm.bSame(szFactNm))
    return this;
    
  for (Fact* pAttribute = m_pAttributes;
       pAttribute;
       pAttribute = pAttribute->m_pAttributeNext) {
    Fact* pFound = pAttribute->pFind(szFactNm);
    if (pFound)
      return pFound;
  }

  for (Fact* pElement = m_pElements;
       pElement;
       pElement = pElement->m_pElementNext) {
    Fact* pFound = pElement->pFind(szFactNm);
    if (pFound)
      return pFound;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get fact that is immediate child of given fact (recursively)               */
/*----------------------------------------------------------------------------*/
Fact* Fact::pGetSuperChild(Fact* pOfFact)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  if (m_pSubclassOf == pOfFact)
    return this;
  
  if (!m_pSubclassOf)
    return nullptr;
  
  return m_pSubclassOf->pGetSuperChild(pOfFact);
}

/*----------------------------------------------------------------------------*/
/* does this element have an attribute with given name?                       */
/*----------------------------------------------------------------------------*/
bool Fact::bHasAtrNm(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
    
//  for (Fact* pAtr = m_pAttributes;
//       pAtr;
//       pAtr = pAtr->m_pAttributeNext) {
//
//    if (pAtr->bSameName(szName))
//      return true;
//  }
//
//  return false;
  
  if (pAttributeFindNm(szName))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* does this element have an attribute with given name and text value?        */
/*----------------------------------------------------------------------------*/
bool Fact::bHasAtrNmVal(Stz* szName, Stz* szValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  if (pAttributeFindNmVal(szName,szValue))
    return true;
  else
    return false;
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
/* find call with highest priority                                            */
/*----------------------------------------------------------------------------*/
void Fact::vHighestCall(Fact*& rpCall)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  if (m_sFactNm.bSame("call"_Stz)) {
    if (rpCall) {
      i8 iThisPriority = iAttributeValNm("priority"_Stz);
      i8 iCurrPriority = rpCall->iAttributeValNm("priority"_Stz);

      if (iThisPriority > iCurrPriority)
        rpCall = this;
    }
    else {
      rpCall = this;
    }
  }
  
  for (Fact* pElement = m_pElements;
       pElement;
       pElement = pElement->m_pElementNext)
    pElement->vHighestCall(rpCall);
}

/*----------------------------------------------------------------------------*/
/* is this fact an immediate sub-element of given fact                        */
/*----------------------------------------------------------------------------*/
bool Fact::bIsElementOf(Fact* pOfFact)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  if (m_pElementOf && m_pElementOf == pOfFact)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this fact an immediate sub-element of given fact                        */
/*----------------------------------------------------------------------------*/
bool Fact::bIsElementOf(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  if (m_pElementOf && m_pElementOf->m_sFactNm.bSame(szName))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* does this fact/attribute have given name and text value ?                  */
/*----------------------------------------------------------------------------*/
bool Fact::bIsNmVal(Stz* szName, Stz* szValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
    
  if (! bSameName(szName))
    return false;

  // has to be a text
  if (! bIsSubClass(m_pKbase->m_p_Text_))
    return false;

  if (! m_xValue.m_pText->szGet()->bSame(szValue))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* is this fact a transitive sub-class of given fact              (recursive) */
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
/* is this fact a transititive sub-class of given fact            (recursive) */
/*----------------------------------------------------------------------------*/
bool Fact::bIsSubClass(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  if (m_sFactNm.bSame(szName))
    return true;
  
  if (! m_pSubclassOf)
    return false;
  
  return m_pSubclassOf->bIsSubClass(szName);
}

/*----------------------------------------------------------------------------*/
/* is this fact a transative sub-element of given fact            (recursive) */
/*----------------------------------------------------------------------------*/
bool Fact::bIsSubElement(Fact* pOfFact)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  if (this == pOfFact)
    return true;
  
  if (! m_pElementOf)
    return false;
  
  return m_pElementOf->bIsSubElement(pOfFact);
}

/*----------------------------------------------------------------------------*/
/* is this fact a transative sub-element of given fact            (recursive) */
/*----------------------------------------------------------------------------*/
bool Fact::bIsSubElement(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  if (m_sFactNm.bSame(szName))
    return true;
  
  if (! m_pElementOf)
    return false;
  
  return m_pSubclassOf->bIsSubElement(szName);
}

/*----------------------------------------------------------------------------*/
/* is this fact an int?                                                       */
/*----------------------------------------------------------------------------*/
bool Fact::bIsInt(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  return bIsSubClass(m_pKbase->m_p_Int_) && this != m_pKbase->m_p_Int_;
}

/*----------------------------------------------------------------------------*/
/* is this fact an object?                                                    */
/*----------------------------------------------------------------------------*/
bool Fact::bIsObject(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  return bIsSubClass(m_pKbase->m_p_Object_) && this != m_pKbase->m_p_Object_;
}

/*----------------------------------------------------------------------------*/
/* is this fact an opaque?                                                    */
/*----------------------------------------------------------------------------*/
bool Fact::bIsOpaque(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  return bIsSubClass(m_pKbase->m_p_Opaque_) && this != m_pKbase->m_p_Opaque_;
}

/*----------------------------------------------------------------------------*/
/* is this fact a text?                                                       */
/*----------------------------------------------------------------------------*/
bool Fact::bIsText(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  return bIsSubClass(m_pKbase->m_p_Text_) && this != m_pKbase->m_p_Text_;
}

/*----------------------------------------------------------------------------*/
/* is this fact a word?                                                       */
/*----------------------------------------------------------------------------*/
bool Fact::bIsWord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  return bIsSubClass(m_pKbase->m_p_Word_) && this != m_pKbase->m_p_Word_;
}

/*----------------------------------------------------------------------------*/
/* is this fact an cross-ref?                                                 */
/*----------------------------------------------------------------------------*/
bool Fact::bIsXref(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  return bIsSubClass(m_pKbase->m_p_Xref_) && this != m_pKbase->m_p_Xref_;
}

/*----------------------------------------------------------------------------*/
/* add a new method to this fact                                              */
/*----------------------------------------------------------------------------*/
void Fact::vMethodNew(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);

  ASSERT(! m_xValue.m_pMethod);
  
  m_xValue.m_pMethod = new Method(m_pKbase,this);
  
  m_bDeleteMethod = true;
}

/*----------------------------------------------------------------------------*/
/* create a new instance of this fact                                         */
/*----------------------------------------------------------------------------*/
Fact* Fact::pNewInstance(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  Fact* pFact = new Fact(m_pKbase, this, szName);
  
  if (! szName) {
    pFact->m_sFactNm.vSprint("%s"_Stz,m_sFactNm.azChar());
//    StrLib::vRemovePrefix(pFact->m_sFactNm.azChar(),'_');
//    StrLib::vRemoveSuffix(pFact->m_sFactNm.azChar(),'_');
    StrLib::vTopTail(pFact->m_sFactNm.azChar(),'_');
  }
  
  return pFact;
}

/*----------------------------------------------------------------------------*/
/* get fact name                                                              */
/*----------------------------------------------------------------------------*/
Stz* Fact::szName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_sFactNm;
}

/*----------------------------------------------------------------------------*/
/* set fact name                                                              */
/*----------------------------------------------------------------------------*/
void Fact::vName(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_sFactNm = szName;
}

/*----------------------------------------------------------------------------*/
/* print fact (recursive)                                                     */
/*----------------------------------------------------------------------------*/
void Fact::vPrint_r(File* pFile, i8 iDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  i8 iTab = iDepth;
  
  s32 sPrefix;
  
  if (! m_iFactNo) {
    sPrefix = "Bud{"_Stz;
    iTab = 0;
  }
  else if (m_sFactNm.bSame("contents"_Stz)) {
    sPrefix = "Set{"_Stz;
    iTab = 0;
    iDepth = 0;
  }
  else if (bIsElementOf("fact"_Stz) || bIsElementOf("_concept_"_Stz)) {
    sPrefix = "Aha{"_Stz;
    iTab = 0;
  }
  else if (m_pElements) //if (bIsSubElement("_folder_"_Stz))
    sPrefix = "Set{"_Stz;
  else if (m_pElementOf)
    sPrefix = "Tpl{"_Stz;
  else if (m_pAttributeOf)
    sPrefix = "Atr{"_Stz;
  else
    ///sPrefix = "Has{"_Stz;
    sPrefix = "???{"_Stz;

  if (m_sFactNm.bSame("question"_Stz))
    vDebugPoint();
  
  vTraceF(pFile,"%s%s [%lld] %p %s"_Stz,
         Stz::szTab(iTab),
         sPrefix.azChar(),

         m_iFactNo,
        this,
         m_sFactNm.azChar()
         );
  
  Fact* pSubValue = pGetSuperChild(m_pKbase->pValue());
  if (pSubValue) {
    if (this == pSubValue) {
      vTraceF(pFile," = eFU %lld %s"_Stz,m_xValue.m_eUsage,aFU[m_xValue.m_eUsage]);
    }
    else {
      switch (pSubValue->rxValue().m_eUsage) {
        case FU_INT:
          vTraceF(pFile," = Int %lld"_Stz,m_xValue.m_iInt);
          break;
        case FU_METHOD:
          vTraceF(pFile," = Method %p"_Stz,m_xValue.m_pMethod);
          break;
        case FU_OBJECT:
          vTraceF(pFile," = Object %p"_Stz,m_xValue.m_pObject);
          break;
        case FU_OPAQUE:
          vTraceF(pFile," = Opaque %p"_Stz,m_xValue.m_aOpaque);
          break;
        case FU_TEXT:
          vTraceF(pFile," = Text \"%s\""_Stz,
                 m_xValue.m_pText ? m_xValue.m_pText->szGet()
                                     : ""_Stz);
          break;
        case FU_USAGE:
          vTraceF(pFile," = Usage ???"_Stz);
          break;
        case FU_WORD:
          vTraceF(pFile," = Word %s"_Stz,
                 m_xValue.m_pWord ? m_xValue.m_pWord->szWordNm()
                                    : ""_Stz);
          break;
        case FU_XREF:
          vTraceF(pFile," = Xref %s"_Stz,
                 m_xValue.m_pXref ? m_xValue.m_pXref->szName()
                                     : ""_Stz);
          break;
        default:
          NEVER_REACH;
          break;
      }
    }
  }
  
  vTraceF(pFile," }"_Stz);
  
  if (m_pAttributeOf)
    vTraceF(pFile," AtrOf: [%lld] %s"_Stz,
            m_pAttributeOf->iFactNo(),m_pAttributeOf->szName());
  
  if (m_pElementOf)
    vTraceF(pFile," ElmOf: [%lld] %s"_Stz,
            m_pElementOf->iFactNo(),m_pElementOf->szName());

  if (m_pSubclassOf) {
    vTraceF(pFile," SubOf: [%lld] %s"_Stz,
            m_pSubclassOf->iFactNo(),m_pSubclassOf->szName());
    s2K sAncestry;
    vAncestry(sAncestry);
    //vTraceF(pFile," // Ancestry: %s"_Stz,sAncestry.azChar());
  }
  
  vTraceF(pFile,"\n"_Stz);

  for (Fact* pAttribute = m_pAttributes;
       pAttribute;
       pAttribute = pAttribute->m_pAttributeNext)
    pAttribute->vPrint_r(pFile, iDepth + 1);
  
  for (Fact* pElement = m_pElements;
       pElement;
       pElement = pElement->m_pElementNext)
    pElement->vPrint_r(pFile, iDepth + 1);
}
/*----------------------------------------------------------------------------*/
//void Fact::vPrint_r(File* pFile, i8 iDepth, i8 iAs)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
//
//  s32 sPrefix;
//
//  if (! m_iFactNo)
//    sPrefix = "Bud{"_Stz;
//  else if (! iAs)
//    sPrefix = "Aha{"_Stz;
//  else if (bIsSubClass("_folder_"_Stz))
//    sPrefix = "Has{"_Stz;
//  else if (m_pElementOf)
//    sPrefix = "Elm{"_Stz;
//  else if (m_pAttributeOf)
//    sPrefix = "Atr{"_Stz;
//  else
//    ///sPrefix = "Has{"_Stz;
//    sPrefix = "???{"_Stz;
//
//  if (m_sFactNm.bSame("question"_Stz))
//    vDebugPoint();
//
//  switch (iAs) {
//    case 0: // aha list
//
//      vTraceF(pFile,"%s%s [%lld] %s"_Stz,
//             Stz::szTab(iDepth),
//             sPrefix.azChar(),
//
//             m_iFactNo,
//             m_sFactNm.azChar()
//             );
//      break;
//
//    case 1: // has
//
//      vTraceF(pFile,"%s%s [%lld] %s"_Stz,
//             Stz::szTab(iDepth),
//             sPrefix.azChar(),
//
//             m_iFactNo,
//             m_sFactNm.azChar()
//             );
//      break;
//
//    case 2: // atr
//
//      vTraceF(pFile,"%s%s [%lld] %s"_Stz,
//             Stz::szTab(iDepth),
//             sPrefix.azChar(),
//
//             m_iFactNo,
//             m_sFactNm.azChar()
//             );
//      break;
//
//    case 3: // elm
//
//      vTraceF(pFile,"%s%s [%lld] %s"_Stz,
//             Stz::szTab(iDepth),
//             sPrefix.azChar(),
//
//             m_iFactNo,
//             m_sFactNm.azChar()
//             );
//      break;
//  }
//
//  Fact* pSubValue = pGetSuperChild(m_pKbase->pValue());
//  if (pSubValue) {
//    if (this == pSubValue) {
//      vTraceF(pFile," = eFU %lld %s"_Stz,m_xValue.m_eUsage,aFU[m_xValue.m_eUsage]);
//    }
//    else {
//      switch (pSubValue->rxValue().m_eUsage) {
//        case FU_INT:
//          vTraceF(pFile," = Int %lld"_Stz,m_xValue.m_iInt);
//          break;
//        case FU_METHOD:
//          vTraceF(pFile," = Method %p"_Stz,m_xValue.m_pMethod);
//          break;
//        case FU_OBJECT:
//          vTraceF(pFile," = Object %p"_Stz,m_xValue.m_pObject);
//          break;
//        case FU_OPAQUE:
//          vTraceF(pFile," = Opaque %p"_Stz,m_xValue.m_aOpaque);
//          break;
//        case FU_TEXT:
//          vTraceF(pFile," = Text \"%s\""_Stz,
//                 m_xValue.m_pText ? m_xValue.m_pText->szGet()
//                                     : ""_Stz);
//          break;
//        case FU_USAGE:
//          vTraceF(pFile," = Usage ???"_Stz);
//          break;
//        case FU_WORD:
//          vTraceF(pFile," = Word %s"_Stz,
//                 m_xValue.m_pWord ? m_xValue.m_pWord->szWordNm()
//                                    : ""_Stz);
//          break;
//        case FU_XREF:
//          vTraceF(pFile," = Xref %s"_Stz,
//                 m_xValue.m_pXref ? m_xValue.m_pXref->szName()
//                                     : ""_Stz);
//          break;
//        default:
//          NEVER_REACH;
//          break;
//      }
//    }
//  }
//
//  //if (m_pAttributeOf || m_pElementOf)
//    vTraceF(pFile," }"_Stz);
//
//  if (m_pAttributeOf)
//    vTraceF(pFile," AtrOf: [%lld] %s"_Stz,
//            m_pAttributeOf->iFactNo(),m_pAttributeOf->szName());
//
//  if (m_pElementOf)
//    vTraceF(pFile," ElmOf: [%lld] %s"_Stz,
//            m_pElementOf->iFactNo(),m_pElementOf->szName());
//
//  if (m_pSubclassOf) {
//    vTraceF(pFile," SubOf: [%lld] %s"_Stz,
//            m_pSubclassOf->iFactNo(),m_pSubclassOf->szName());
//    s2K sAncestry;
//    vAncestry(sAncestry);
//    vTraceF(pFile," // Ancestry: %s"_Stz,sAncestry.azChar());
//  }
//
//  vTraceF(pFile,"\n"_Stz);
//
//  for (Fact* pAttribute = m_pAttributes;
//       pAttribute;
//       pAttribute = pAttribute->m_pAttributeNext)
//    pAttribute->vPrint_r(pFile, iDepth + 1, iAs ? 2 : 0);
//
//  for (Fact* pElement = m_pElements;
//       pElement;
//       pElement = pElement->m_pElementNext)
//    pElement->vPrint_r(pFile, iAs ? iDepth + 1 : 0, iAs ? 3 : 0);
//}
/*----------------------------------------------------------------------------*/
//void Fact::vPrint_r(i8 iDepth, i8 iAs)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
//
//  s32 sPrefix;
//
//  if (m_pElementOf)
//    sPrefix = "Elm"_Stz;
//  else if (m_pAttributeOf)
//    sPrefix = "Atr"_Stz;
//  else
//    sPrefix = "-+-"_Stz;
//
//  if (m_sFactNm.bSame("question"_Stz))
//    vDebugPoint();
//
//  if (iAs) {
//    if (bIsSubClass("_folder_"_Stz))
//      vTrace("f:%s"_Stz,Stz::szTab(iDepth));
//    else
//    if (bIsSubClass("_tuple_"_Stz))
//      vTrace("t:%s"_Stz,Stz::szTab(iDepth));
//    else
//    if (bIsSubClass("_questions_"_Stz))
//      vTrace("e:%s"_Stz,Stz::szTab(iDepth));
//  }
//
//  switch (iAs) {
//    case 0:
//
//      vTrace("%lld [%lld] %s"_Stz,
//             iAs,
//
//             m_iFactNo,
//             m_sFactNm.azChar()
//             );
//      break;
//
//    case 1:
//
//      vTrace("%s %lld [%lld] %s"_Stz,
//             sPrefix.azChar(),
//             iAs,
//
//             m_iFactNo,
//             m_sFactNm.azChar()
//             );
//      break;
//
//    case 2:
//
//      vTrace("a:%s %s %lld [%lld] %s"_Stz,
//             Stz::szTab(iDepth),
//             sPrefix.azChar(),
//             iAs,
//
//             m_iFactNo,
//             m_sFactNm.azChar()
//             );
//      break;
//
//    case 3:
//
//      vTrace("%s %lld [%lld] %s"_Stz,
//             sPrefix.azChar(),
//             iAs,
//
//             m_iFactNo,
//             m_sFactNm.azChar()
//             );
//      break;
//  }
//  /*
//  if (bIsInt())
//    vTrace(" = Int %lld"_Stz,m_xValue.m_iInt);
//  else if (bIsObject())
//    vTrace(" = Object %p"_Stz,m_xValue.m_pObject);
//  else if (bIsOpaque())
//    vTrace(" = Opaque %p"_Stz,m_xValue.m_aOpaque);
//  else if (bIsText())
//    vTrace(" = Text \"%s\""_Stz,
//           m_xValue.m_pText ? m_xValue.m_pText->szGet()
//                               : ""_Stz);
//  else if (bIsWord())
//    vTrace(" = Word %s"_Stz,
//           m_xValue.m_pWord ? m_xValue.m_pWord->szWordNm()
//                              : ""_Stz);
//  else if (bIsXref())
//    vTrace(" = Xref %s"_Stz,
//           m_xValue.m_pXref ? m_xValue.m_pXref->szName()
//                               : ""_Stz);
//  else { }
//  */
//
//  Fact* pSubValue = pGetSuperChild(m_pKbase->pValue());
//  if (pSubValue) {
//    if (this == pSubValue) {
//      vTrace(" = eFU %lld %s"_Stz,m_xValue.m_eUsage,aFU[m_xValue.m_eUsage]);
//    }
//    else {
//      switch (pSubValue->rxValue().m_eUsage) {
//        case FU_INT:
//          vTrace(" = Int %lld"_Stz,m_xValue.m_iInt);
//          break;
//        case FU_METHOD:
//          vTrace(" = Method %p"_Stz,m_xValue.m_pMethod);
//          break;
//        case FU_OBJECT:
//          vTrace(" = Object %p"_Stz,m_xValue.m_pObject);
//          break;
//        case FU_OPAQUE:
//          vTrace(" = Opaque %p"_Stz,m_xValue.m_aOpaque);
//          break;
//        case FU_TEXT:
//          vTrace(" = Text \"%s\""_Stz,
//                 m_xValue.m_pText ? m_xValue.m_pText->szGet()
//                                     : ""_Stz);
//          break;
//        case FU_USAGE:
//          vTrace(" = Usage ???"_Stz);
//          break;
//        case FU_WORD:
//          vTrace(" = Word %s"_Stz,
//                 m_xValue.m_pWord ? m_xValue.m_pWord->szWordNm()
//                                    : ""_Stz);
//          break;
//        case FU_XREF:
//          vTrace(" = Xref %s"_Stz,
//                 m_xValue.m_pXref ? m_xValue.m_pXref->szName()
//                                     : ""_Stz);
//          break;
//        default:
//          NEVER_REACH;
//          break;
//      }
//    }
//  }
//
//  if (m_pAttributeOf || m_pElementOf || m_pSubclassOf)
//    vTrace(" //"_Stz);
//  if (m_pAttributeOf)
//    vTrace(" AtrOf: [%lld]%s"_Stz,m_pAttributeOf->iFactNo(),m_pAttributeOf->szName());
//  if (m_pElementOf)
//    vTrace(" ElmOf: [%lld]%s"_Stz,m_pElementOf->iFactNo(),m_pElementOf->szName());
//  if (m_pSubclassOf) {
//    s2K sAncestry;
//    vAncestry(sAncestry);
//    vTrace(" Ancestry: %s"_Stz,sAncestry.azChar());
//  }
//
//  switch (iAs) {
//    case 0: vTrace("\n"_Stz); break;
//    case 1: vTrace("\n"_Stz); break;
//    case 2: vTrace("\n"_Stz); break;
//    case 3: vTrace("\n"_Stz); break;
//  }
//
//  for (Fact* pAttribute = m_pAttributes;
//       pAttribute;
//       pAttribute = pAttribute->m_pAttributeNext)
//    pAttribute->vPrint_r(iDepth + 1, iAs ? 2 : 0);
//
//  switch (iAs) {
//    case 0: break;
//    case 1: break;
//    case 2: break;
//    case 3: break;
//  }
//
//  for (Fact* pElement = m_pElements;
//       pElement;
//       pElement = pElement->m_pElementNext)
//    pElement->vPrint_r(iDepth + 1, iAs ? 3 : 0);
//
//  switch (iAs) {
//    case 0: break;
//    case 1: break;
//    case 2: break;
//    case 3: break;
//  }
//}

/*----------------------------------------------------------------------------*/
/* process a question                                                         */
/*----------------------------------------------------------------------------*/
bool Fact::bQuestion(Fact* pQuestion, Opaque* aArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  if (! pQuestion->bIsSubClass("_question_"_Stz))
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "Fact::bQuestion: %s is not a subclass of _question_\n"_Stz,
           pQuestion->m_sFactNm.azChar());

  Fact* pMethodAttr = pQuestion->pAttributeFindNm("method"_Stz);
  if (! pMethodAttr)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "Fact::bQuestion: failed to find method of question\n"_Stz);

  Method* pMethod = pMethodAttr->m_xValue.m_pMethod;
  
  eTQ eCode = pMethod->eGetCode(0);
  
  bool bResult = false;
  
  switch (eCode) {
    case TQ_IS_SUBCLASS_OF:
      bResult = this->bIsSubClass((Stz*)aArg);
      break;
      
    default:
      NEVER_REACH;
  }
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* process a question                                                         */
/*----------------------------------------------------------------------------*/
bool Fact::bQuestion(Stz* szQuestion, Opaque* aArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  Fact* pQuestion =
    m_pKbase->m_pContents->pElementFindNmVal("questionNm"_Stz,szQuestion);
    
  if (! pQuestion)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "Fact::bQuestion: failed to find %s\n"_Stz,
           szQuestion->azChar());
  
  bool bResult = bQuestion(pQuestion,aArg);
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* test if fact has same name as given argument                               */
/*----------------------------------------------------------------------------*/
bool Fact::bSameName(Stz* szName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  bool bSame = m_sFactNm.bSame(szName);
  
  return bSame;
}

/*----------------------------------------------------------------------------*/
/* set int value                                                              */
/*----------------------------------------------------------------------------*/
void Fact::vSetInt(i8 iInt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_xValue.m_iInt = iInt;
}

/*----------------------------------------------------------------------------*/
/* set int value                                                              */
/*----------------------------------------------------------------------------*/
void Fact::vSetText(Stz* szText)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_xValue.m_pText = new Text(szText);
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

/*----------------------------------------------------------------------------*/
/* get text value                                                             */
/*----------------------------------------------------------------------------*/
Stz* Fact::szText(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  Stz* szText = m_xValue.m_pText->szGet();
  return szText;
}

/*----------------------------------------------------------------------------*/
/* get int value                                                              */
/*----------------------------------------------------------------------------*/
i8 Fact::iValue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_xValue.m_iInt;
}

/*----------------------------------------------------------------------------*/
/* set int value                                                              */
/*----------------------------------------------------------------------------*/
void Fact::vValue(i8 iValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  m_xValue.m_iInt = iValue;
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
Value& Fact::rxValue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FACT,_FOOTER_);
  
  return m_xValue;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
