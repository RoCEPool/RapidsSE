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
  
  m_eQuestion = QN_NONE;
  
  m_afbMatch = nullptr;

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

  m_eQuestion = QN_NONE;
  
  m_afbMatch = nullptr;

  _checkInitialised_(CN_QUESTION);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_QUESTION,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Question::Question(eQN eQuestion, Kbase* pKbase, Fact* pOrigin, ...)
: Object(CN_QUESTION)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUESTION,_FOOTER_);
  
  va_list xArgs;
  
  m_pKbase = pKbase;
  m_pOrigin = pOrigin ? pOrigin : pKbase->m_pContents;
  
  m_eQuestion = eQuestion;

  switch (m_eQuestion) {
      
    case QN_ELEMENTS_BY_ATR_NAME:
      va_start(xArgs,pOrigin);
      m_sOriNm = va_arg(xArgs,Stz*);
      m_sAtrNm = va_arg(xArgs,Stz*);
      va_end(xArgs);
      m_afbMatch = &Question::bMatchAtrNm;
      break;
      
    case QN_ELEMENTS_BY_ATR_NAME_VALUE:
      va_start(xArgs,pOrigin);
      m_sOriNm = va_arg(xArgs,Stz*);
      m_sAtrNm  = va_arg(xArgs,Stz*);
      m_sAtrVal = va_arg(xArgs,Stz*);
      va_end(xArgs);
      m_afbMatch = &Question::bMatchAtrNmVal;
      break;
      
    case QN_ELEMENTS_BY_ELM_NAME:
      va_start(xArgs,pOrigin);
      m_sOriNm = va_arg(xArgs,Stz*);
      m_sElmNm = va_arg(xArgs,Stz*);
      va_end(xArgs);
      m_afbMatch = &Question::bMatchElmNm;
      break;
      
    default:
      NEVER_REACH;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_bInited = false;
  
  m_pStart = nullptr;
  m_pElement = nullptr;
  m_pAttribute = nullptr;

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
/* get next answer                                                            */
/*----------------------------------------------------------------------------*/
bool Question::bEachMatch(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUESTION,_FOOTER_);
  
  bool bResult;
  
  m_pElement   = nullptr;
  m_pAttribute = nullptr;

  if (! m_bInited)
    bResult = bFirstMatch();
  else
    bResult = bNextMatch();
  
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
/* get first answer                                                           */
/*----------------------------------------------------------------------------*/
bool Question::bFirstMatch(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUESTION,_FOOTER_);
  
  m_bInited = true;
  
  m_pOrigin = m_pStart = m_pKbase->pFactFind(m_sOriNm);
  
  m_pPrev = m_pOrigin;
  
  vTrace("m_pOrigin: %s\n"_Stz,m_pOrigin->szName());

  Fact* pFound = Question::pFirstMatchingElm();
  
  return pFound ? true : false;
}

/*----------------------------------------------------------------------------*/
/* get next matching answer                                                   */
/*----------------------------------------------------------------------------*/
bool Question::bNextMatch(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUESTION,_FOOTER_);
  
  Fact* pFound = Question::pNextMatchingElm();

  return pFound ? true : false;
}

/*----------------------------------------------------------------------------*/
/* does given element have an attribute which matchs required name ?          */
/*----------------------------------------------------------------------------*/
bool Question::bMatchAtrNm(Fact* pElm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUESTION,_FOOTER_);
  
  vTrace("bMatchAtrNm: %s ?hasAtrNm(%s)\n"_Stz,pElm->szName(),m_sAtrNm.azChar());
  
  if ( (m_pAttribute = pElm->pAttributeFindNm(m_sAtrNm)) ) {
    m_pElement = pElm;
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* does given element have an attribute which matchs required name & value ?  */
/*----------------------------------------------------------------------------*/
bool Question::bMatchAtrNmVal(Fact* pElm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUESTION,_FOOTER_);
    
  vTrace("bMatchAtrNm: %s ?hasAtrNmVal(%s.%s)\n"_Stz,
         pElm->szName(),m_sAtrNm.azChar(),m_sAtrVal.azChar());
  
  if ( (m_pAttribute = pElm->pAttributeFindNmVal(m_sAtrNm,m_sAtrVal)) ) {
    m_pElement = pElm;
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* does the given element match required element name                         */
/*----------------------------------------------------------------------------*/
bool Question::bMatchElmNm(Fact* pElm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUESTION,_FOOTER_);
  
  vTrace("bMatchElmNm: %s ?same(%s)\n"_Stz,pElm->szName(),m_sElmNm.azChar());

  if (pElm->bSameName(m_sElmNm)) {
    m_pElement = pElm;
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* position on first element which satisfies the call back match function     */
/*----------------------------------------------------------------------------*/
Fact* Question::pFirstMatchingElm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUESTION,_FOOTER_);
  
  Fact* pElm = m_pOrigin->pElements();
  
  while (pElm) {
    
    vTrace("pFirstMatchingElm: %s\n"_Stz,pElm->szName());
           
    // does this element match?
    if ((this->*m_afbMatch)(pElm)) {
      m_pPrev = pElm;
      return pElm;
    }
    
    // -- -- -- -- -- -- -- -- --
    
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

/*----------------------------------------------------------------------------*/
/* advance to next element which satisfies the call back match function       */
/*----------------------------------------------------------------------------*/
Fact* Question::pNextMatchingElm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUESTION,_FOOTER_);
  
  Fact* pElm = m_pPrev;
  
  while (pElm) {
    
    vTrace("pNextMatchingElm: %s\n"_Stz,pElm->szName());
    
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
      if (pElm == m_pOrigin)
        break;
      
      if (pElm->pElementNext()) {
        pElm = pElm->pElementNext();
        break;
      }
    }
    
    // have we exhausted the search?
    if (pElm == m_pOrigin)
      break;
    
    // -- -- -- -- -- -- -- -- --

    // does this element match?
    if ((this->*m_afbMatch)(pElm)) {
      m_pPrev = pElm;
      return pElm;
    }
  }
 
  m_pPrev = nullptr;
  return nullptr;
}
/*----------------------------------------------------------------------------*/
//Fact* Question::pNextMatchingElm(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_QUESTION,_FOOTER_);
//
//  Fact* pElm = m_pPrev;
//
//  while (pElm) {
//
//    // does this element match?
//    if (m_afbMatch(pElm)) {
//      m_pPrev = pElm;
//      return pElm;
//    }
//
//    // does the element have sub-elements?
//    if (pElm->pElements()) {
//      pElm = pElm->pElements();
//      continue;
//    }
//
//    // are there more elements in the set the element is in?
//    if (pElm->pElementNext()) {
//      pElm = pElm->pElementNext();
//      continue;
//    }
//
//    // step up/left/outwards till we can go to next element of a set
//    while (pElm != m_pOrigin) {
//      pElm = pElm->pElementOf();
//      if (pElm->pElementNext()) {
//        pElm = pElm->pElementNext();
//        break;
//      }
//    }
//
//    // have we exhausted the search?
//    if (pElm == m_pOrigin)
//      break;
//  }
//
//  m_pPrev = nullptr;
//  return nullptr;
//}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
