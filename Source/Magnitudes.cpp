/*============================================================================*/
/* Magnitudes.cpp -- Various Useful Magnitude functions                       */
/*============================================================================*/

#include "System.h"

/*----------------------------------------------------------------------------*/
/* pMagnitue -- convert bytes to its magnitude equivalent                     */
/*----------------------------------------------------------------------------*/
void Magnitudes::vMagnitude(i8 iValue, char* psBuf)
{
  if (iValue < C_1KB) sprintf(psBuf,"%lldB",iValue);
  else if (iValue < C_1MB) sprintf(psBuf,"%lldKB",iValue/C_1KB);
  else if (iValue < C_1GB) sprintf(psBuf,"%lldMB",iValue/C_1MB);
  else if (iValue < C_1TB) sprintf(psBuf,"%lldGB",iValue/C_1GB);
  else if (iValue < C_1PB) sprintf(psBuf,"%lldTB",iValue/C_1TB);
  else sprintf(psBuf,"%lldPB",iValue/C_1PB);
}

/*----------------------------------------------------------------------------*/
/* pMagnitue -- convert bytes to its magnitude equivalent upto n decimals     */
/*----------------------------------------------------------------------------*/
void Magnitudes::vMagnitude(i8 iValue, i8 iDecimals, char* psBuf)
{
  ASSERT(iDecimals > 0);
  
  d8 dValue = (d8)iValue;
  int iDecimalPoints = (int)iDecimals;
  
  if (iValue < C_1KB) sprintf(psBuf,"%.*fB", iDecimalPoints, dValue);
  else if (iValue < C_1MB) sprintf(psBuf,"%.*fKB", iDecimalPoints, dValue/C_1KB);
  else if (iValue < C_1GB) sprintf(psBuf,"%.*fMB", iDecimalPoints, dValue/C_1MB);
  else if (iValue < C_1TB) sprintf(psBuf,"%.*fGB", iDecimalPoints, dValue/C_1GB);
  else if (iValue < C_1PB) sprintf(psBuf,"%.*fTB", iDecimalPoints, dValue/C_1TB);
  else sprintf(psBuf,"%.*fPB", iDecimalPoints, dValue/C_1PB);
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
