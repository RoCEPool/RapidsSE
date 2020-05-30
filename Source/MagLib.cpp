/*============================================================================*/
/* MagLib.cpp -- Magnitude Conversions                                        */
/*============================================================================*/

#include "Library.h"

/*----------------------------------------------------------------------------*/
/* convert magnitude to equivalent bytes                                      */
/*----------------------------------------------------------------------------*/
bool MagLib::bToBytes(Stz* psBuf, i8& iBytes)
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
    if (!StrLib::bEqualTo(StrLib::acLast(sString.azChar()), "b")) {
      return false;
    }
    sNumber.vCopyTill(sString,'b');
    iMagnitude = 1;
  }
  else {
    sNumber = sString;
  }
  
  char* ps = sNumber.azChar();
  i8 iVal;
  
  if (!StrLib::bIsNumeric(ps)){
    return false;
  }
  
  if (StrLib::bExtractInteger(&ps, &iVal)) {
    iBytes = iVal*iMagnitude;
  }
  else {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert bytes to its magnitude equivalent                                  */
/*----------------------------------------------------------------------------*/
void MagLib::vByteMag(i8 iValue, Stz* psBuf)
{
       if (iValue < C_1KB) psBuf->vSprint("%lldB"_Stz, iValue);
  else if (iValue < C_1MB) psBuf->vSprint("%lldKB"_Stz,iValue/C_1KB);
  else if (iValue < C_1GB) psBuf->vSprint("%lldMB"_Stz,iValue/C_1MB);
  else if (iValue < C_1TB) psBuf->vSprint("%lldGB"_Stz,iValue/C_1GB);
  else if (iValue < C_1PB) psBuf->vSprint("%lldTB"_Stz,iValue/C_1TB);
  else if (iValue < C_1EB) psBuf->vSprint("%lldPB"_Stz,iValue/C_1PB);
  else                     psBuf->vSprint("%lldEB"_Stz,iValue/C_1EB);
}

/*----------------------------------------------------------------------------*/
/* convert bytes to its magnitude equivalent upto n decimals                  */
/*----------------------------------------------------------------------------*/
void MagLib::vByteMag(i8 iValue, i8 nDecimals, Stz* psBuf)
{
  //ASSERT(iDecimals > 0);
  
  d8 dValue = (d8)iValue;
  int nPoints = (int)nDecimals;
  
       if (iValue < C_1KB) psBuf->vSprint("%.*fB"_Stz, nPoints,dValue);
  else if (iValue < C_1MB) psBuf->vSprint("%.*fKB"_Stz,nPoints,dValue/C_1KB);
  else if (iValue < C_1GB) psBuf->vSprint("%.*fMB"_Stz,nPoints,dValue/C_1MB);
  else if (iValue < C_1TB) psBuf->vSprint("%.*fGB"_Stz,nPoints,dValue/C_1GB);
  else if (iValue < C_1PB) psBuf->vSprint("%.*fTB"_Stz,nPoints,dValue/C_1TB);
  else if (iValue < C_1EB) psBuf->vSprint("%.*fPB"_Stz,nPoints,dValue/C_1PB);
  else                     psBuf->vSprint("%.*fEB"_Stz,nPoints,dValue/C_1EB);
}

/*----------------------------------------------------------------------------*/
/* convert bytes to its magnitude equivalent                                  */
/*----------------------------------------------------------------------------*/
void MagLib::vHzMag(i8 iValue, Stz* psBuf)
{
  d8 dValue = (d8)iValue;
  int nPoints = 3;
  
       if (iValue < C_1K) psBuf->vSprint("%.*fhz"_Stz, nPoints,dValue);
  else if (iValue < C_1M) psBuf->vSprint("%.*fKhz"_Stz,nPoints,dValue/C_1K);
  else if (iValue < C_1G) psBuf->vSprint("%.*fMhz"_Stz,nPoints,dValue/C_1M);
  else if (iValue < C_1T) psBuf->vSprint("%.*fGhz"_Stz,nPoints,dValue/C_1G);
  else if (iValue < C_1P) psBuf->vSprint("%.*fThz"_Stz,nPoints,dValue/C_1T);
  else if (iValue < C_1E) psBuf->vSprint("%.*fPhz"_Stz,nPoints,dValue/C_1P);
  else                    psBuf->vSprint("%.*fEhz"_Stz,nPoints,dValue/C_1E);
}

/*----------------------------------------------------------------------------*/
/* convert timer nanoseconds to its magnitude equivalent (ns, us, ms, s)      */
/*----------------------------------------------------------------------------*/
void MagLib::vTimeMag(i8 iValue, Stz* psBuf)
{
       if (iValue < C_1K) psBuf->vSprint("%lldns"_Stz,iValue);
  else if (iValue < C_1M) psBuf->vSprint("%lldus"_Stz,iValue/C_1K);
  else if (iValue < C_1G) psBuf->vSprint("%lldms"_Stz,iValue/C_1M);
  else                    psBuf->vSprint("%llds"_Stz, iValue/C_1G);
}

/*----------------------------------------------------------------------------*/
/* convert bytes to its magnitude equivalent                                  */
/*----------------------------------------------------------------------------*/
void MagLib::vUnitMag(i8 iValue, Stz* psBuf)
{
       if (iValue < C_1K) psBuf->vSprint("%lld"_Stz, iValue);
  else if (iValue < C_1M) psBuf->vSprint("%lldK"_Stz,iValue/C_1K);
  else if (iValue < C_1G) psBuf->vSprint("%lldM"_Stz,iValue/C_1M);
  else if (iValue < C_1T) psBuf->vSprint("%lldG"_Stz,iValue/C_1G);
  else if (iValue < C_1P) psBuf->vSprint("%lldT"_Stz,iValue/C_1T);
  else if (iValue < C_1E) psBuf->vSprint("%lldP"_Stz,iValue/C_1P);
  else                    psBuf->vSprint("%lldE"_Stz,iValue/C_1E);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
