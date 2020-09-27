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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* pose a question                                                            */
/*----------------------------------------------------------------------------*/
void Question::vAsk(Stz* szSetNm, Stz* m_sAtrNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUESTION,_FOOTER_);
  
  m_sSetNm = szSetNm;
  m_sAtrNm = m_sAtrNm;
  
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
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* get next answer                                                            */
/*----------------------------------------------------------------------------*/
bool Question::bNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUESTION,_FOOTER_);
  
  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
