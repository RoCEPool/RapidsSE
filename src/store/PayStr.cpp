/*============================================================================*/
/* PayStr.cpp -- Payload String                                               */
/*============================================================================*/

#include "BASE.h"

#include "PayStr.hpp"

#include "PayStrBuffer.hpp"

/*----------------------------------------------------------------------------*/
/* new a PayStr                                                               */
/*----------------------------------------------------------------------------*/
void* PayStr::operator new(size_t, PayStrBuffer* pPayStrBuffer, Stz* szStz)
{
  i8 iSizeAllocated = 0;
  PayStr* aPayStr_allotted = (PayStr*)pPayStrBuffer->aAllot(szStz, &iSizeAllocated);

  aPayStr_allotted->vSetMagic();
  aPayStr_allotted->vSetClassNo(CN_PAYSTR);
  aPayStr_allotted->aFooter()->vSetFooter();

  StrLib::vCopy(aPayStr_allotted->m_caArea, iSizeAllocated, szStz->azChar());

  return aPayStr_allotted;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PayStr::PayStr(void)
: Object(CN_PAYSTR)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PAYSTR, _FOOTER_);

  _checkInitialised_(CN_PAYSTR);

  _precondition_(_ADDRESS_, _MAGIC_, CN_PAYSTR, _FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* calculate dynamic size of a paystr given size of stz                       */
/*----------------------------------------------------------------------------*/
i8 PayStr::jFullSize(i8 jSizeStz)
{
  _static_

  i8 jSize = sizeof(PayStr) - 8 + MemLib::nRoundUp(jSizeStz,8) + sizeof(Footer);

  return jSize;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* calculate dynamic size of this paystr                                      */
/*----------------------------------------------------------------------------*/
i8 PayStr::jSizeOf(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PAYSTR);

  i8 jSizeOf = sizeof(PayStr) - 8 + m_jArea + sizeof(Footer);

  return jSizeOf;
}

/*----------------------------------------------------------------------------*/
/* calculate dynamic size of this paystr                                      */
/*----------------------------------------------------------------------------*/
Stz* PayStr::szStz(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PAYSTR);

  return (Stz*) m_caArea;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* PayStr::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PAYSTR);

  Footer* aFooter = aAdd<Footer*>(this,jSizeOf()-sizeof(Footer));

  return aFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
