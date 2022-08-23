/*============================================================================*/
/* PayStrBuffer.cpp -- Payload String Buffer                                  */
/*============================================================================*/

#include "BASE.h"

#include "PayStrBuffer.hpp"
#include "PayStr.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,PayStrBuffer,CN_PAYSTRBUFFER)
__aNewFunction(PayStrBuffer,CN_PAYSTRBUFFER)
__vDeleteFunction(PayStrBuffer,CN_PAYSTRBUFFER)
__aFooterFunction(PayStrBuffer)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PayStrBuffer::PayStrBuffer(void)
: Object(CN_PAYSTRBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PAYSTRBUFFER,_FOOTER_);

  m_pFrom = this;
  m_pNext = nullptr;

  #ifdef WITH_CHECK_INITIALISED
    m_xWsp.vZero(CN_CSVBUFFER);
  #endif

_postcondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot a new paystr                                                         */
/*----------------------------------------------------------------------------*/
byte* PayStrBuffer::aAllot(Stz* szStz)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PAYSTRBUFFER, _FOOTER_);

  i8 jSizeStz = szStz->nSize();

  byte* ayBytes = aAllot(jSizeStz);

  return ayBytes;
}

/*----------------------------------------------------------------------------*/
/* allot a new paystr                                                         */
/*----------------------------------------------------------------------------*/
byte* PayStrBuffer::aAllot(i8 jSizeStz)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PAYSTRBUFFER, _FOOTER_);

  i8 jSize = PayStr::jFullSize(jSizeStz);

  byte* ayBytes = m_xWsp.ayAllot(jSize);

  PayStr* aPayStr_allotted = (PayStr*)ayBytes;
  aPayStr_allotted->m_jArea = jSizeStz;

  return ayBytes;
}

/*----------------------------------------------------------------------------*/
/* get from                                                                   */
/*----------------------------------------------------------------------------*/
PayStrBuffer* PayStrBuffer::pFrom(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PAYSTRBUFFER, _FOOTER_);

  return m_pFrom;
}

/*----------------------------------------------------------------------------*/
/* set from                                                                   */
/*----------------------------------------------------------------------------*/
void PayStrBuffer::vFrom(PayStrBuffer* pFrom)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PAYSTRBUFFER, _FOOTER_);

  m_pFrom = pFrom;
}

/*----------------------------------------------------------------------------*/
/* get next                                                                   */
/*----------------------------------------------------------------------------*/
PayStrBuffer* PayStrBuffer::pNext(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PAYSTRBUFFER, _FOOTER_);

  return m_pNext;
}

/*----------------------------------------------------------------------------*/
/* set next                                                                   */
/*----------------------------------------------------------------------------*/
void PayStrBuffer::vNext(PayStrBuffer* pNext)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PAYSTRBUFFER, _FOOTER_);

  m_pNext = pNext;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
