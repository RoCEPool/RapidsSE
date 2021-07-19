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
  vAssign(xDecVal);
}

/*----------------------------------------------------------------------------*/
/* multiply operator                                                          */
/*----------------------------------------------------------------------------*/
DecVal DecVal::operator *(DecVal xArgB)
{
  SP_DecVal spArgA;
  SP_DecVal spArgB;
  SP_DecVal spResC;
  
  DecVal xResC;
  
  vTrace("(( %s * %s "_Stz,spArgA.sz(*this),spArgB.sz(xArgB));

  xResC.vAssign(m_iValue  * xArgB.m_iValue,
                m_nPlaces + xArgB.m_nPlaces);

  vTrace("= %s "_Stz,spResC.sz(xResC));
    
  xResC.vRoundUp(m_nPlaces);
  vTrace("; round up (to %lld places) = %s "_Stz,m_nPlaces,spResC.sz(xResC));

  vTrace(")) "_Stz);

  return xResC;
}

/*----------------------------------------------------------------------------*/
/* assign                                                                     */
/*----------------------------------------------------------------------------*/
void DecVal::vAssign(DecVal xArg)
{
  vAssign(xArg.m_iValue,xArg.m_nPlaces);
}

/*----------------------------------------------------------------------------*/
/* assign                                                                     */
/*----------------------------------------------------------------------------*/
void DecVal::vAssign(i16 iValue, i8 nPlaces)
{
  m_iValue  = iValue;
  m_nPlaces = nPlaces;
}

/*----------------------------------------------------------------------------*/
/* assign                                                                     */
/*----------------------------------------------------------------------------*/
void DecVal::vAssign(i8 iWhole, i8 iFraction, i8 nPlaces)
{
  vAssign(iWhole * IntLib::iTenToPow(nPlaces) + iFraction,nPlaces);
}

/*----------------------------------------------------------------------------*/
/* multiply                                                                   */
/*----------------------------------------------------------------------------*/
void DecVal::vMultiply(DecVal xsArgB)
{
  SP_DecVal spArgA;
  SP_DecVal spArgB;
  SP_DecVal spResC;
  
  vTrace("%s * %s "_Stz,spArgA.sz(*this),spArgB.sz(xsArgB));

  i8 nToPlaces = m_nPlaces;
  
  vAssign(m_iValue  * xsArgB.m_iValue,
          m_nPlaces + xsArgB.m_nPlaces);

  vTrace("= %s "_Stz,spResC.sz(*this));
  
  DecVal xrResC;
  vTrace("| to nPlaces %lld: "_Stz,m_nPlaces);
  
  xrResC.vRoundDown(m_nPlaces);
  vTrace("roundDown %10s; "_Stz,spResC.sz(xrResC));
  
  xrResC.vRoundUp(m_nPlaces);
  vTrace("roundUp %10s "_Stz,spResC.sz(xrResC));
}

/*----------------------------------------------------------------------------*/
/* multiply                                                                   */
/*----------------------------------------------------------------------------*/
void DecVal::vMultiply(DecVal xArgA, DecVal xArgB)
{
  vAssign(xArgA * xArgB);
}

/*----------------------------------------------------------------------------*/
/* round down                                                                 */
/*----------------------------------------------------------------------------*/
void DecVal::vRoundDown(i8 nPlaces)
{
  // required number of places
  if (m_nPlaces == nPlaces)
    return;
  
  i8 iScaleDiff;
  
  // too many decimal digits
  if (m_nPlaces > nPlaces) {
    iScaleDiff = m_nPlaces - nPlaces;
    m_nPlaces -= iScaleDiff;
    m_iValue /= IntLib::iTenToPow(iScaleDiff);
    return;
  }
  
  // too few decimal digits
  iScaleDiff = nPlaces - m_nPlaces;
  m_nPlaces += iScaleDiff;
  m_iValue *= IntLib::iTenToPow(iScaleDiff);
}

/*----------------------------------------------------------------------------*/
/* round up                                                                   */
/*----------------------------------------------------------------------------*/
void DecVal::vRoundUp(i8 nPlaces)
{
  // required number of places
  if (m_nPlaces == nPlaces)
    return;
  
  i8 iScaleDiff;

  // too many decimal digits
  if (m_nPlaces > nPlaces) {
    iScaleDiff = m_nPlaces - nPlaces;
    m_nPlaces -= iScaleDiff;
    
    i8 iMod = IntLib::iTenToPow(iScaleDiff);
    i8 iCutVal = m_iValue % iMod;
    
    m_iValue /= IntLib::iTenToPow(iScaleDiff);
    
    if (iCutVal >= iMod / 2)
      m_iValue += 1;
    return;
  }
  
  // too few decimal digits
  iScaleDiff = nPlaces - m_nPlaces;
  m_nPlaces += iScaleDiff;
  m_iValue *= IntLib::iTenToPow(iScaleDiff);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* _Dec literal suffix (which converts const char* to DecVal)                 */
/*----------------------------------------------------------------------------*/
DecVal BASE::operator "" _Dec(const char* azLiteral, size_t nSize)
{
  UNUSED(nSize);

  DecVal xDecVal;
  
  // extract the whole.fraction parts
  i8 iWhole;
  i8 iFraction;
  i8 nParts = sscanf(azLiteral,"%lld.%lld",&iWhole,&iFraction);

  if (1 == nParts) {
    // whole part only, no fraction (e.g. "2000."_Dec)
    xDecVal.vAssign(iWhole,0);
  }
  else {
    // both whole and fraction parts (e.g. "12.34."_Dec)
    s32 sFractionDigits;
    sFractionDigits.vSprint("%lld"_Stz,iFraction);
    i8 nFractionDigits = sFractionDigits.nLength();

    xDecVal.vAssign(iWhole,iFraction,nFractionDigits);
  }
    
  return xDecVal;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
