/*============================================================================*/
/* Magnitudes.cpp -- Various Useful Magnitude functions                       */
/*============================================================================*/

#include "System.h"

/*----------------------------------------------------------------------------*/
/* pMagnitue -- convert bytes to its magnitude equivalent                     */
/*----------------------------------------------------------------------------*/
void Magnitudes::vMagnitude(i8 iValue, Stz* psBuf)
{
       if (iValue < C_1KB) psBuf->vSprint("%lldB"_Stz, iValue);
  else if (iValue < C_1MB) psBuf->vSprint("%lldKB"_Stz,iValue/C_1KB);
  else if (iValue < C_1GB) psBuf->vSprint("%lldMB"_Stz,iValue/C_1MB);
  else if (iValue < C_1TB) psBuf->vSprint("%lldGB"_Stz,iValue/C_1GB);
  else if (iValue < C_1PB) psBuf->vSprint("%lldTB"_Stz,iValue/C_1TB);
  else                     psBuf->vSprint("%lldPB"_Stz,iValue/C_1PB);
}

/*----------------------------------------------------------------------------*/
/* pMagnitue -- convert bytes to its magnitude equivalent upto n decimals     */
/*----------------------------------------------------------------------------*/
void Magnitudes::vMagnitude(i8 iValue, i8 iDecimals, Stz* psBuf)
{
  //ASSERT(iDecimals > 0);
  
  d8 dValue = (d8)iValue;
  int iDecimalPoints = (int)iDecimals;
  
       if (iValue < C_1KB) psBuf->vSprint("%.*fB"_Stz,  iDecimalPoints, dValue);
  else if (iValue < C_1MB) psBuf->vSprint("%.*fKB"_Stz, iDecimalPoints, dValue/C_1KB);
  else if (iValue < C_1GB) psBuf->vSprint("%.*fMB"_Stz, iDecimalPoints, dValue/C_1MB);
  else if (iValue < C_1TB) psBuf->vSprint("%.*fGB"_Stz, iDecimalPoints, dValue/C_1GB);
  else if (iValue < C_1PB) psBuf->vSprint("%.*fTB"_Stz, iDecimalPoints, dValue/C_1TB);
  else                     psBuf->vSprint("%.*fPB"_Stz, iDecimalPoints, dValue/C_1PB);
}

/*----------------------------------------------------------------------------*/
/* iBytes -- convert magnitude to equivalent bytes                            */
/*----------------------------------------------------------------------------*/
bool Magnitudes::bBytes(Stz* psBuf, i8& iBytes)
{
  s256 sNumber;
  s256 sString;
  i8 iMagnitude = 1;
  
  sString = (Stz*)psBuf;
  
  sString.vToLower();
  
  if (sString.bContains("kb"_Stz)) {
    sNumber.vCopyTill(sString,'k');
    iMagnitude = C_1KB;
  }
  else if (sString.bContains("mb"_Stz)) {
    sNumber.vCopyTill(sString,'m');
    iMagnitude = C_1MB;
  }
  else if (sString.bContains("gb"_Stz)) {
    sNumber.vCopyTill(sString,'g');
    iMagnitude = C_1GB;
  }
  else if (sString.bContains("tb"_Stz)) {
    sNumber.vCopyTill(sString,'t');
    iMagnitude = C_1TB;
  }
  else if (sString.bContains("pb"_Stz)) {
    sNumber.vCopyTill(sString,'p');
    iMagnitude = C_1PB;
  }
  else if (sString.bContains("b"_Stz)) {
    if (!StrLib::bEqualTo(StrLib::sLast(sString.aChar()), "b")) {
      return false;
    }
    sNumber.vCopyTill(sString,'b');
    iMagnitude = 1;
  }
  else {
    sNumber = sString;
  }
  
  char* ps = sNumber.aChar();
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
