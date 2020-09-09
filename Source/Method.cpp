/*============================================================================*/
/* Method.cpp - Kbase Method                                                  */
/*============================================================================*/

#include "Facts.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Facts,Method,CN_METHOD)
__aNewFunction(Method,CN_METHOD)
__vDeleteFunction(Method,CN_METHOD)
__aFooterFunction(Method)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Method::Method(void)
: Object(CN_METHOD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);
    
  _checkInitialised_(CN_METHOD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Method::~Method(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add code                                                                   */
/*----------------------------------------------------------------------------*/
void Method::vAddCode(eQC eCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);
    
  m_eaCode[m_i_Code++] = eCode;
}

/*----------------------------------------------------------------------------*/
/* add code with address argument                                             */
/*----------------------------------------------------------------------------*/
void Method::vAddCodeAdr(eQC eCode, Opaque* aOpaque)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);
      
  m_eaCode[m_i_Code++] = eCode;

  Opaque** aaOpaque = (Opaque**) & m_eaCode[m_i_Code];
  * aaOpaque = aOpaque;
  
  m_i_Code += sizeof(Opaque*);
}

/*----------------------------------------------------------------------------*/
/* add code with integer argument                                             */
/*----------------------------------------------------------------------------*/
void Method::vAddCodeInt(eQC eCode, i8 iInt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METHOD,_FOOTER_);
      
  m_eaCode[m_i_Code++] = eCode;

  i8* piInt = (i8*) & m_eaCode[m_i_Code];
  * piInt = iInt;
  
  m_i_Code += sizeof(i8);
}

/*----------------------------------------------------------------------------*/
/* add code with string argument                                              */
/*----------------------------------------------------------------------------*/
void Method::vAddCodeStz(eQC eCode, Stz* szStz)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
    
  m_eaCode[m_i_Code++] = eCode;

  for (char* acChr = szStz->azChar(); *acChr; acChr++)
    m_eaCode[m_i_Code++] = (eQC)*acChr;
  m_eaCode[m_i_Code++] = (eQC)0;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
