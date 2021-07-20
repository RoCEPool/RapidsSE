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
/* add operator                                                               */
/*----------------------------------------------------------------------------*/
DecVal DecVal::operator +(DecVal xArgB)
{
  SP_DecVal spArgA;
  SP_DecVal spArgB;
  SP_DecVal spResC;
  
  DecVal xResC;
  
  vTrace("(( %s + %s "_Stz,spArgA.sz(*this),spArgB.sz(xArgB));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // normalise values
  vNormalise(m_iValue,m_nPlaces,xArgB.m_iValue,xArgB.m_nPlaces);
  vTrace("; normalised %s + %s "_Stz,spArgA.sz(*this),spArgB.sz(xArgB));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compute result
  
  i16 iComputedValue  = m_iValue  + xArgB.m_iValue;
  i8  iComputedPlaces = m_nPlaces;
  
  xResC.vAssign(iComputedValue,iComputedPlaces);
  vTrace("= %s "_Stz,spResC.sz(xResC));
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // round up
  
  xResC.vRoundUp(m_nPlaces);
  vTrace("; round up (to %lld places) = %s "_Stz,m_nPlaces,spResC.sz(xResC));

  vTrace(")) "_Stz);

  return xResC;
}

/*----------------------------------------------------------------------------*/
/* subtract operator                                                           */
/*----------------------------------------------------------------------------*/
DecVal DecVal::operator -(DecVal xArgB)
{
  SP_DecVal spArgA;
  SP_DecVal spArgB;
  SP_DecVal spResC;
  
  DecVal xResC;
  
  vTrace("(( %s - %s "_Stz,spArgA.sz(*this),spArgB.sz(xArgB));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // normalise values
  vNormalise(m_iValue,m_nPlaces,xArgB.m_iValue,xArgB.m_nPlaces);
  vTrace("; normalised %s + %s "_Stz,spArgA.sz(*this),spArgB.sz(xArgB));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compute result
  
  i16 iComputedValue  = m_iValue  - xArgB.m_iValue;
  i8  iComputedPlaces = m_nPlaces; // + xArgB.m_nPlaces;
  
  xResC.vAssign(iComputedValue,iComputedPlaces);
  vTrace("= %s "_Stz,spResC.sz(xResC));
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // round up
  
  xResC.vRoundUp(m_nPlaces);
  vTrace("; round up (to %lld places) = %s "_Stz,m_nPlaces,spResC.sz(xResC));

  vTrace(")) "_Stz);

  return xResC;

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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compute result
  
  i16 iComputedValue  = m_iValue  * xArgB.m_iValue;
  i8  iComputedPlaces = m_nPlaces + xArgB.m_nPlaces;
  
  xResC.vAssign(iComputedValue,iComputedPlaces);
  vTrace("= %s "_Stz,spResC.sz(xResC));
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // round up
  
  xResC.vRoundUp(m_nPlaces);
  vTrace("; round up (to %lld places) = %s "_Stz,m_nPlaces,spResC.sz(xResC));

  vTrace(")) "_Stz);

  return xResC;
}

/*----------------------------------------------------------------------------*/
/* divide operator                                                            */
/*----------------------------------------------------------------------------*/
DecVal DecVal::operator /(DecVal xArgB)
{
  SP_DecVal spArgA;
  SP_DecVal spArgB;
  SP_DecVal spResC;
  
  DecVal xResC;
  
  vTrace("(( %s / %s "_Stz,spArgA.sz(*this),spArgB.sz(xArgB));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compute result
  
  i16 iComputedValue  = m_iValue  / xArgB.m_iValue;
  i8  iComputedPlaces = m_nPlaces; // + xArgB.m_nPlaces;
  
  xResC.vAssign(iComputedValue,iComputedPlaces);
  vTrace("= %s "_Stz,spResC.sz(xResC));
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // round up
  
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
  i16 iValue = iWhole * IntLib::iTenToPow(nPlaces) + iFraction;

  vAssign(iValue,nPlaces);
}

/*----------------------------------------------------------------------------*/
/* normalise                                                                  */
/*----------------------------------------------------------------------------*/
void DecVal::vNormalise(i16& isArgA, i8& isArgA_places,
                        i16& isArgB, i8& isArgB_places)
{
  // 1) arg a & b have the same scale
  if (isArgA_places == isArgB_places) {
    return;
  }
    
  i8 iScaleDiff;
  i8 iScaleCorrect;

  // 2) arg b needs to be scaled to arg a places
  if (isArgA_places > isArgB_places) {
    iScaleDiff = isArgA_places - isArgB_places;
    iScaleCorrect = IntLib::iTenToPow(iScaleDiff);
    isArgB = isArgB * iScaleCorrect;
    isArgB_places = isArgA_places;
    return;
  }

  // 2) arg a needs to be scaled to arg a places
  iScaleDiff = isArgB_places - isArgA_places;
  iScaleCorrect = IntLib::iTenToPow(iScaleDiff);
  isArgA = isArgA * iScaleCorrect;
  isArgA_places = isArgB_places;
}


/*----------------------------------------------------------------------------*/
/* round down to given number of places                                       */
/*----------------------------------------------------------------------------*/
void DecVal::vRoundDown(i8 nPlaces)
{
  // required number of places
  if (m_nPlaces == nPlaces)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // too many decimal digits (e.g with nPlaces=1: 34.56 => 34.5)
  
  if (m_nPlaces > nPlaces) {
    i8 nReduce = m_nPlaces - nPlaces;         // number of places to reduce
    m_iValue /= IntLib::iTenToPow(nReduce);   // m_iValue /= 10^nReduce
    m_nPlaces -= nReduce;                     // reduce m_nPlaces
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // too few decimal digits (e.g with nPlaces=3: 12.34 => 12.340)
  
  i8 nIncrease = nPlaces - m_nPlaces;         // number of places to increase
  m_iValue *= IntLib::iTenToPow(nIncrease);   // m_iValue /*= 10^nIncrease
  m_nPlaces += nIncrease;                     // increase m_nPlaces
}

/*----------------------------------------------------------------------------*/
/* round up to given number of places                                         */
/*----------------------------------------------------------------------------*/
void DecVal::vRoundUp(i8 nPlaces)
{
  // required number of places
  if (m_nPlaces == nPlaces)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // too many decimal digits (e.g with nPlaces=1: 12.34 => 12.3 ; 34.56 => 34.6)
  
  if (m_nPlaces > nPlaces) {
    i8 nReduce = m_nPlaces - nPlaces;        // number of places to reduce
    m_nPlaces -= nReduce;                    // reduce m_nPlaces

    i8 iMod = IntLib::iTenToPow(nReduce);    // calc what gets cut off
    i8 iCutVal = m_iValue % iMod;
    
    m_iValue /= IntLib::iTenToPow(nReduce);  // m_iValue /= 10^nReduce
    
    if (iCutVal >= iMod / 2)                 // round up if cut is over half
      m_iValue += 1;
    
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // too few decimal digits (e.g with nPlaces=3: 34.56 => 34.560)
  
  i8 nIncrease = nPlaces - m_nPlaces;        // number of places to increase
  m_nPlaces += nIncrease;                    // increase m_nPlaces
  m_iValue *= IntLib::iTenToPow(nIncrease);  // m_iValue /*= 10^nIncrease
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* _Dec literal suffix (which converts const char* to DecVal)                 */
/*----------------------------------------------------------------------------*/
DecVal BASE::operator "" _Dec(const char* azLiteral, size_t nSize)
{
  UNUSED(nSize);

  i8 iValue;
  i8 nFractionDigits;

  char* azDecLit = (char*)azLiteral;
  StrLib::bExtractDecLit(azDecLit,iValue,nFractionDigits);
  
  DecVal xDecVal;
  xDecVal.vAssign(iValue,nFractionDigits);
    
  return xDecVal;
}
/*----------------------------------------------------------------------------*/
//DecVal BASE::operator "" _Dec(const char* azLiteral, size_t nSize)
//{
//  UNUSED(nSize);
//
//  DecVal xDecVal;
//
//  // extract the whole.fraction parts
//  i8 iWhole;
//  i8 iFraction;
//  i8 nPlaces;
//  i8 nParts = sscanf(azLiteral,"%lld.%lld",&iWhole,&iFraction);
//
//  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
//
//  char* azDecLit = (char*)azLiteral;
//  StrLib::bExtractDecVal(azDecLit,iWhole,iFraction,nPlaces);
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // whole part only, no fraction (e.g. "2000."_Dec)
//
//  if (1 == nParts) {
//    xDecVal.vAssign(iWhole,0);
//    return xDecVal;
//  }
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // both whole and fraction parts (e.g. "12.34."_Dec)
//
//  s32 sFractionDigits;
//  sFractionDigits.vSprint("%lld"_Stz,iFraction);
//  i8 nFractionDigits = sFractionDigits.nLength();
//
//  xDecVal.vAssign(iWhole,iFraction,nFractionDigits);
//
//  return xDecVal;
//}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
