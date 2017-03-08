/*============================================================================*/
/* Magnitudes.cpp -- Various Useful Magnitude functions                       */
/*============================================================================*/

#include "System.h"

/*----------------------------------------------------------------------------*/
/* pMagnitue -- convert bytes to its magnitude equivalent                     */
/*----------------------------------------------------------------------------*/
void Magnitudes::vMagnitude(i8 iValue, Str* psBuf)
{
  if (iValue < C_1KB) StrLib::vSprint(psBuf,"%lldB"_Str,iValue);
  else if (iValue < C_1MB) StrLib::vSprint(psBuf,"%lldKB"_Str,iValue/C_1KB);
  else if (iValue < C_1GB) StrLib::vSprint(psBuf,"%lldMB"_Str,iValue/C_1MB);
  else if (iValue < C_1TB) StrLib::vSprint(psBuf,"%lldGB"_Str,iValue/C_1GB);
  else if (iValue < C_1PB) StrLib::vSprint(psBuf,"%lldTB"_Str,iValue/C_1TB);
  else StrLib::vSprint(psBuf,"%lldPB"_Str,iValue/C_1PB);
}

/*----------------------------------------------------------------------------*/
/* pMagnitue -- convert bytes to its magnitude equivalent upto n decimals     */
/*----------------------------------------------------------------------------*/
void Magnitudes::vMagnitude(i8 iValue, i8 iDecimals, Str* psBuf)
{
  ASSERT(iDecimals > 0);
  
  d8 dValue = (d8)iValue;
  int iDecimalPoints = (int)iDecimals;
  
  if (iValue < C_1KB) StrLib::vSprint(psBuf,"%.*fB"_Str, iDecimalPoints, dValue);
  else if (iValue < C_1MB) StrLib::vSprint(psBuf,"%.*fKB"_Str, iDecimalPoints, dValue/C_1KB);
  else if (iValue < C_1GB) StrLib::vSprint(psBuf,"%.*fMB"_Str, iDecimalPoints, dValue/C_1MB);
  else if (iValue < C_1TB) StrLib::vSprint(psBuf,"%.*fGB"_Str, iDecimalPoints, dValue/C_1GB);
  else if (iValue < C_1PB) StrLib::vSprint(psBuf,"%.*fTB"_Str, iDecimalPoints, dValue/C_1TB);
  else StrLib::vSprint(psBuf,"%.*fPB"_Str, iDecimalPoints, dValue/C_1PB);
}

/*----------------------------------------------------------------------------*/
/* iBytes -- convert magnitude to equivalent bytes                            */
/*----------------------------------------------------------------------------*/
bool Magnitudes::bBytes(Str* psBuf, i8& iBytes)
{
  s256 sNumber;
  s256 sString;
  i8 iMagnitude = 1;
  
  sString = (Str*)psBuf;
  
  StrLib::vToLower(sString.aStr());
  
  if (StrLib::bContains(sString.aStr(), "kb")) {
    StrLib::vCopyTill(sNumber.aStr(), sString.aStr(), 'k');
    iMagnitude = C_1KB;
    
  } else if (StrLib::bContains(sString.aStr(), "mb")) {
    StrLib::vCopyTill(sNumber.aStr(), sString.aStr(), 'm');
    iMagnitude = C_1MB;
    
  } else if (StrLib::bContains(sString.aStr(), "gb")) {
    StrLib::vCopyTill(sNumber.aStr(), sString.aStr(), 'g');
    iMagnitude = C_1GB;
    
  } else if (StrLib::bContains(sString.aStr(), "tb")) {
    StrLib::vCopyTill(sNumber.aStr(), sString.aStr(), 't');
    iMagnitude = C_1TB;
    
  } else if (StrLib::bContains(sString.aStr(), "pb")) {
    StrLib::vCopyTill(sNumber.aStr(), sString.aStr(), 'p');
    iMagnitude = C_1PB;
    
  } else if (StrLib::bContains(sString.aStr(), "b")) {
    if (!StrLib::bEqualTo(StrLib::sLast(sString.aStr()), "b")) {
      return false;
    }
    StrLib::vCopyTill(sNumber.aStr(), sString.aStr(), 'b');
    iMagnitude = 1;
  } else {
    sNumber = sString;
  }
  
  char* ps = sNumber.aStr();
  i8 iVal;
  
  if (!StrLib::bIsNumeric(ps)){
    return false;
  }
  
  if (StrLib::bExtractI8(&ps, &iVal)) {
    iBytes = iVal*iMagnitude;
  }
  else {
    return false;
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
