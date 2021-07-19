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
  
  vTrace("%s * %s "_Stz,spArgA.sz(*this),spArgB.sz(xArgB));

  i8 nToPlaces = m_nPlaces;
  vAssign(m_iValue  * xArgB.m_iValue,
          nToPlaces + xArgB.m_nPlaces);

  vTrace("= %s "_Stz,spResC.sz(*this));
  
  DecVal xrResC;
  vTrace("| to nPlaces %lld: "_Stz,nToPlaces);
  
  xrResC.vRoundDown(*this,nToPlaces);
  vTrace("roundDown %10s; "_Stz,spResC.sz(xrResC));
  
  xrResC.vRoundUp(*this,nToPlaces);

  return *this;
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
  
  xrResC.vRoundDown(*this,m_nPlaces);
  vTrace("roundDown %10s; "_Stz,spResC.sz(xrResC));
  
  xrResC.vRoundUp(*this,m_nPlaces);
  vTrace("roundUp %10s "_Stz,spResC.sz(xrResC));
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
void DecVal::vRoundUp(i8 nPlaces)
{
  // required number of places
  if (m_nPlaces == nPlaces)
    return;
  
  i8 iScaleDiff;
  i8 iMod ;
  i8 iCutVal;

  // too many decimal digits
  if (m_nPlaces > nPlaces) {
    iScaleDiff = m_nPlaces - nPlaces;
    m_nPlaces -= iScaleDiff;
    
    iMod = IntLib::iTenToPow(iScaleDiff);
    iCutVal = m_iValue % iMod;
    
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
