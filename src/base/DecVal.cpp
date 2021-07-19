/*============================================================================*/
/* DecVal.cpp - Decimal Value                                                 */
/*============================================================================*/

#include "BASE.h"

#include "DecVal.hpp"
#include "SP_DecVal.hpp"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
DecVal::DecVal(void)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
void DecVal::operator =(DecVal xDecVal)
{
  m_iValue  = xDecVal.m_iValue;
  m_nPlaces = xDecVal.m_nPlaces;
}

/*----------------------------------------------------------------------------*/
/* multiply operator                                                          */
/*----------------------------------------------------------------------------*/
DecVal DecVal::operator *(DecVal xDecVal)
{
  m_iValue  = xDecVal.m_iValue;
  m_nPlaces = xDecVal.m_nPlaces;
  
  return *this;
}

/*----------------------------------------------------------------------------*/
/* assign                                                                     */
/*----------------------------------------------------------------------------*/
void DecVal::vAssign(DecVal xArg)
{
  m_iValue  = xArg.m_iValue;
  m_nPlaces = xArg.m_nPlaces;
}

/*----------------------------------------------------------------------------*/
/* assign                                                                     */
/*----------------------------------------------------------------------------*/
void DecVal::vAssign(i16 iValue,i8 nPlaces)
{
  m_iValue  = iValue;
  m_nPlaces = nPlaces;
}

/*----------------------------------------------------------------------------*/
/* multiply                                                                   */
/*----------------------------------------------------------------------------*/
void DecVal::vMultiply(DecVal xsArgA, DecVal xsArgB)
{
  SP_DecVal spArgA;
  SP_DecVal spArgB;
  SP_DecVal spResC;
  
  vTrace("%s * %s "_Stz,spArgA.sz(xsArgA),spArgB.sz(xsArgB));

  vAssign(xsArgA.m_iValue  * xsArgB.m_iValue,
          xsArgA.m_nPlaces + xsArgB.m_nPlaces);

  vTrace("= %s "_Stz,spResC.sz(*this));
  
  DecVal xrResC;
  vTrace("| to nPlaces %lld: "_Stz,xsArgA.m_nPlaces);
  
  xrResC.vRoundDown(*this,xsArgA.m_nPlaces);
  vTrace("roundDown %10s; "_Stz,spResC.sz(xrResC));
  
  xrResC.vRoundUp(*this,xsArgA.m_nPlaces);
  vTrace("roundUp %10s "_Stz,spResC.sz(xrResC));
}

/*----------------------------------------------------------------------------*/
/* round down                                                                 */
/*----------------------------------------------------------------------------*/
void DecVal::vRoundDown(DecVal xsArg, i8 nPlaces)
{
  vAssign(xsArg.m_iValue,xsArg.m_nPlaces);
  
  // required number of places
  if (xsArg.m_nPlaces == nPlaces)
    return;
  
  i8 iScaleDiff;
  
  // too many decimal digits
  if (xsArg.m_nPlaces > nPlaces) {
    iScaleDiff = xsArg.m_nPlaces - nPlaces;
    m_nPlaces -= iScaleDiff;
    m_iValue /= IntLib::iTenToPow(iScaleDiff);
    return;
  }
  
  // too few decimal digits
  iScaleDiff = nPlaces - xsArg.m_nPlaces;
  m_nPlaces += iScaleDiff;
  m_iValue *= IntLib::iTenToPow(iScaleDiff);
}

/*----------------------------------------------------------------------------*/
/* round up                                                                   */
/*----------------------------------------------------------------------------*/
void DecVal::vRoundUp(DecVal xsArg, i8 nPlaces)
{
  vAssign(xsArg.m_iValue,xsArg.m_nPlaces);
  
  // required number of places
  if (xsArg.m_nPlaces == nPlaces)
    return;
  
  i8 iScaleDiff;
  i8 iMod ;
  i8 iCutVal;

  // too many decimal digits
  if (xsArg.m_nPlaces > nPlaces) {
    iScaleDiff = xsArg.m_nPlaces - nPlaces;
    m_nPlaces -= iScaleDiff;
    
    iMod = IntLib::iTenToPow(iScaleDiff);
    iCutVal = m_iValue % iMod;
    
    m_iValue /= IntLib::iTenToPow(iScaleDiff);
    
    if (iCutVal >= iMod / 2)
      m_iValue += 1;
    return;
  }
  
  // too few decimal digits
  iScaleDiff = nPlaces - xsArg.m_nPlaces;
  m_nPlaces += iScaleDiff;
  m_iValue *= IntLib::iTenToPow(iScaleDiff);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* _Dec literal suffix (which converts const char* to DecVal)                 */
/*----------------------------------------------------------------------------*/
DecVal BASE::operator "" _Dec(const char* szArg, size_t nSize)
{
  UNUSED(nSize);
  
  DecVal xDecVal;
  
  i8 iIntDigits = 0;
  i8 iDecDigits = -1;
  i8 nSeen = sscanf(szArg,"%lld.%lld",&iIntDigits,&iDecDigits);
  
  
  s32 sDecDigits;
  sDecDigits.vSprint("%lld"_Stz,iDecDigits);
  
  i8 nDecDigits;
  if (-1 == iDecDigits) {
    nDecDigits = 0;
    iDecDigits = 0;
  }
  else {
    nDecDigits = sDecDigits.nLength();
  }
  
  xDecVal.vAssign(iIntDigits * IntLib::iTenToPow(nDecDigits) + iDecDigits,
                  nDecDigits);
  
  return xDecVal;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
