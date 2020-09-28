/*============================================================================*/
/* Question.cpp - Question                                                    */
/*============================================================================*/

#include "Facts.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Facts,Question,CN_QUESTION)
__aNewFunction(Question,CN_QUESTION)
__vDeleteFunction(Question,CN_QUESTION)
__aFooterFunction(Question)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Question::Question(Kbase* pKbase)
: Object(CN_QUESTION)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUESTION,_FOOTER_);
  
  m_pKbase = pKbase;
  
  // m_sSetNm
  // m_sAtrNm
  
  m_bInited = false;
  
  m_pStart = nullptr;
  m_pElement = nullptr;
  m_pAttribute = nullptr;
  
  m_pOrigin = nullptr;
  m_pPrev = nullptr;

  _checkInitialised_(CN_QUESTION);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_QUESTION,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Question::Question(Kbase* pKbase, Stz* szSetNm, Stz* szAtrNm)
: Object(CN_QUESTION)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUESTION,_FOOTER_);
  
  m_pKbase = pKbase;
  
  m_sSetNm = szSetNm;
  m_sAtrNm = szAtrNm;

  m_bInited = false;
  
  m_pStart = nullptr;
  m_pElement = nullptr;
  m_pAttribute = nullptr;

  m_pOrigin = nullptr;
  m_pPrev = nullptr;

  _checkInitialised_(CN_QUESTION);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_QUESTION,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Question::~Question(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUESTION,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* find named element within named set                                        */
/*----------------------------------------------------------------------------*/
Fact* Question::pFindElm(Kbase* pKbase, Stz* szSetNm, Stz* szElmNm)
{
  _static_
  
  Fact* pSet = pKbase->pElementFind(szSetNm);
  
  if (!pSet)
    return nullptr;
  
  Fact* pElm = pFindElm(pKbase,pSet,szSetNm);
  
  return pElm;
}

/*----------------------------------------------------------------------------*/
/* find named element within named set                                        */
/*----------------------------------------------------------------------------*/
Fact* Question::pFindElm(Kbase* pKbase, Fact* pSet, Stz* szElmNm)
{
  _static_
  
  
  
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* pose a question                                                            */
/*----------------------------------------------------------------------------*/
void Question::vAsk(Stz* szSetNm, Stz* szAtrNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUESTION,_FOOTER_);
  
  m_sSetNm = szSetNm;
  m_sAtrNm = szAtrNm;
  
  m_bInited = false;
}

/*----------------------------------------------------------------------------*/
/* get current attribute                                                      */
/*----------------------------------------------------------------------------*/
Fact* Question::pAttribute(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUESTION,_FOOTER_);
  
  return m_pAttribute;
}

/*----------------------------------------------------------------------------*/
/* get next answer                                                            */
/*----------------------------------------------------------------------------*/
bool Question::bEach(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUESTION,_FOOTER_);
  
  bool bResult;
  
  if (! m_bInited)
    bResult = bFirst();
  else
    bResult = bNext();
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* get current element                                                        */
/*----------------------------------------------------------------------------*/
Fact* Question::pElement(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUESTION,_FOOTER_);
  
  return m_pElement;
}

/*----------------------------------------------------------------------------*/
/* get int value                                                              */
/*----------------------------------------------------------------------------*/
i8 Question::iValue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUESTION,_FOOTER_);
  
  if (! m_pAttribute)
    return C_UNKNOWN;
  
  i8 iVal = m_pAttribute->rxValue().m_iInt;
  
  return iVal;
}

/*----------------------------------------------------------------------------*/
/* get text value                                                             */
/*----------------------------------------------------------------------------*/
Stz* Question::szValue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUESTION,_FOOTER_);
  
  if (! m_pAttribute)
    return nullptr;
  
  Stz*  szVal = m_pAttribute->rxValue().m_pText->szGet();
  
  return szVal;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get first answer                                                           */
/*----------------------------------------------------------------------------*/
bool Question::bFirst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUESTION,_FOOTER_);
  
  m_bInited = true;
  
  m_pStart = m_pKbase->pFactFind(m_sSetNm);
  
  m_pElement = pSearch(m_pStart,&m_pAttribute);
  
  return m_pElement ? true : false;
}

/*----------------------------------------------------------------------------*/
/* get next answer                                                            */
/*----------------------------------------------------------------------------*/
bool Question::bNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUESTION,_FOOTER_);
  
  m_pElement = pSearch(m_pElement,&m_pAttribute);
  
  return m_pElement ? true : false;
}

/*----------------------------------------------------------------------------*/
/* advance thru each element which has an attribute with given name           */
/*----------------------------------------------------------------------------*/
Fact* Question::pSearch(Fact* pPrev, Fact** ppAttribute)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUESTION,_FOOTER_);
  
  Fact* pElement = pPrev;
  
  while (pElement) {
    
    if (pElement->pElements()) {
      
      pElement = pElement->pElements();
      
      Fact* pAttribute = pElement->pAttributeFindNm(m_sAtrNm);

      if (pAttribute) {
        *ppAttribute = pAttribute;
        return pElement;
      }
    
      continue;
    }

    if (pElement->pElementNext()) {
        
      pElement = pElement->pElementNext();

      Fact* pAttribute = pElement->pAttributeFindNm(m_sAtrNm);

      if (pAttribute) {
        *ppAttribute = pAttribute;
        return pElement;
      }
      
      continue;
    }
    
    if (pElement == m_pStart)
      break;
    
    pElement = pElement->pElementOf();
    
    if (pElement == m_pStart)
      break;
  }
    
  *ppAttribute = nullptr;
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* advance to next element which has an attribute with given attribute name   */
/*----------------------------------------------------------------------------*/
Fact* Question::pEachElmByAtrNm(Fact** ppPrev, Fact* pOrigin, Stz* szAtrNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUESTION,_FOOTER_);
  
  *ppPrev = nullptr;
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* advance to next element which has given element name                       */
/*----------------------------------------------------------------------------*/
Fact* Question::pEachElmByElmNm(Fact* pOrigin, Stz* szElmNm, Fact** ppPrev)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUESTION,_FOOTER_);
  
  Fact* pElm = *ppPrev;
  
  while (pElm) {
    
    // does the element have a matching name?
    if (pElm->bSameName(szElmNm)) {
      *ppPrev = pElm;
      return pElm;
    }
    
    // does the element have sub-elements?
    if (pElm->pElements()) {
      pElm = pElm->pElements();
      continue;
    }
    
    // are there more elements in the set the element is in?
    if (pElm->pElementNext()) {
      pElm = pElm->pElementNext();
      continue;
    }

    // step up/left/outwards till we can go to next element of a set
    while (pElm != pOrigin) {
      pElm = pElm->pElementOf();
      if (pElm->pElementNext()) {
        pElm = pElm->pElementNext();
        break;
      }
    }
    
    // have we exhausted the search?
    if (pElm == pOrigin)
      break;
  }
 
  *ppPrev = nullptr;
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* does the given element match required element name                         */
/*----------------------------------------------------------------------------*/
bool Question::bMatchElmNm(Fact* pElm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUESTION,_FOOTER_);
  
  if (pElm->bSameName(m_sSetNm))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* advance to next element which satisfies the call back match function       */
/*----------------------------------------------------------------------------*/
Fact* Question::pNextMatchingElm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUESTION,_FOOTER_);
  
  Fact* pElm = m_pPrev;
  
  while (pElm) {
    
    // does the element have a matching name?
    if (m_afbMatch(pElm)) {
      m_pPrev = pElm;
      return pElm;
    }
    
    // does the element have sub-elements?
    if (pElm->pElements()) {
      pElm = pElm->pElements();
      continue;
    }
    
    // are there more elements in the set the element is in?
    if (pElm->pElementNext()) {
      pElm = pElm->pElementNext();
      continue;
    }

    // step up/left/outwards till we can go to next element of a set
    while (pElm != m_pOrigin) {
      pElm = pElm->pElementOf();
      if (pElm->pElementNext()) {
        pElm = pElm->pElementNext();
        break;
      }
    }
    
    // have we exhausted the search?
    if (pElm == m_pOrigin)
      break;
  }
 
  m_pPrev = nullptr;
  return nullptr;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
