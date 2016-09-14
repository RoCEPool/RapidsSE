/*============================================================================*/
/* Magnitudes.cpp -- Various Useful Magnitude functions                       */
/*============================================================================*/

#include "Common.h"

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
i8 Magnitudes::iBytes(char* psBuf)
{
  s256 sNumber;
  i8 iMagnitude = 1;
  
  if (String::bContains(psBuf, "KB") ) {
    String::vCopyTill(sNumber.a(), psBuf, 'K');
    iMagnitude = C_1KB;
    
  } else if (String::bContains(psBuf, "MB") ) {
    String::vCopyTill(sNumber.a(), psBuf, 'M');
    iMagnitude = C_1MB;
    
  } else if (String::bContains(psBuf, "GB") ) {
    String::vCopyTill(sNumber.a(), psBuf, 'G');
    iMagnitude = C_1GB;
    
  } else if (String::bContains(psBuf, "TB") ) {
    String::vCopyTill(sNumber.a(), psBuf, 'T');
    iMagnitude = C_1TB;
    
  } else if (String::bContains(psBuf, "PB") ) {
    String::vCopyTill(sNumber.a(), psBuf, 'P');
    iMagnitude = C_1PB;
    
  } else if (String::bContains(psBuf, "B") ) {
    if (!String::bEqualTo(String::sLast(psBuf), "B")) {
      THROW_ERROR(EC_INVALID_ARGUMENT);

    }
    String::vCopyTill(sNumber.a(), psBuf, 'B');
    iMagnitude = 1;
  } else {
    sNumber = psBuf;
  }
  
  char* ps = sNumber.a();
  i8 iVal;
  
  if (!String::bIsNumeric(ps)){
     THROW_ERROR(EC_INVALID_ARGUMENT);
  }
  
  if (String::bExtractI8(&ps, &iVal)) {
    return iVal*iMagnitude;
  }
  else {
    THROW_ERROR(EC_INVALID_ARGUMENT);
  }
  
  /*
  while (iLen--) {
    if (*s && isdigit(*s)) {
      if (bDigit)
        String::vCopy(sNumber.a(), *s);
      else {
        bDigit = true;
        String::vConcat(sNumber.a(), *s);
      }
    }
    else {
      if (*s == 'B') {
        bTrailingB = true;
      }
      else {
        if (*s == 'K') {
          iMagnitude = C_1KB;
        }
        else if (*s == 'M') {
          iMagnitude = C_1MB;
        }
        else if (*s == 'G') {
          iMagnitude = C_1GB;
        }
        else if (*s == 'T') {
          iMagnitude = C_1TB;
        }
        else if (*s == 'T') {
          iMagnitude = C_1PB;
        }
        else {
          if (bTrailingB) {
            THROW_ERROR(EC_INVALID_ARGUMENT);
          }
        }
      }
    }
  }
  
  if (bExtractInw(&sNumeric.a(), &iValue)) {
    return iValue*iMagnitude;
  }
  else {
    THROW_ERROR(EC_INVALID_ARGUMENT);
  }
   
  */
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/