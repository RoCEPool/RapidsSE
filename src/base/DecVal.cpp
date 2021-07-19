/*============================================================================*/
/* DecVal.cpp - Decimal Value                                                 */
/*============================================================================*/

#include "BASE.h"

#include "DecVal.hpp"

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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
