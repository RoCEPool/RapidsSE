/*============================================================================*/
/* Magnitudes.cpp -- Various Useful Magnitude functions                       */
/*============================================================================*/
#pragma once

#include "Magic.h"

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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/