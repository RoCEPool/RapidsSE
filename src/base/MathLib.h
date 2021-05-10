/*============================================================================*/
/* MathLib.h -- math utility                                                  */
/*============================================================================*/
#pragma once

/*----------------------------------------------------------------------------*/
/* iMathLib_Add_BCDBCD:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Add_BCDBCD(i8 iValue1, i8 iValue2)
{

  // COME BACK TO THIS
  i8 iResult;

  if (iValue1 == C_DATUM_NULL || iValue2 == C_DATUM_NULL)
    iResult = C_DATUM_NULL;
  else
    iResult = iValue1 + iValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Add_DTEIVL:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Add_DTEIVL(i8 iFloatValue, i8 iBcdValue)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Add_FLTBCD: float + bcd                                           */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Add_FLTBCD(i8 iFloatValue, i8 iBcdValue)
{
  i8 iResult = C_DATUM_NULL;;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Add_FLTFLT: float + float                                         */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Add_FLTFLT(i8 iValue1, i8 iValue2)
{
  i8 iResult;
  d8 fValue1;
  d8 fValue2;

  fValue1 = *(d8*)&iValue1;
  fValue2 = *(d8*)&iValue2;

  if (fValue1==  C_DATUM_NULL || fValue2 == C_DATUM_NULL)
    iResult = C_DATUM_NULL;
  else {
    d8 dResult = fValue1 + fValue2;
    iResult = *(d8 *) &dResult;
  }

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Add_FLTFLT: float + int                                           */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Add_FLTINT(i8 iValue1, i8 iValue2)
{
  i8 iResult;
  d8 fValue1;
  d8 fValue2;

  if (fValue1 == C_DATUM_NULL || fValue2 == C_DATUM_NULL)
    return C_DATUM_NULL;

  fValue1 = *(d8*)&iValue1;
  fValue2 = iValue2;

  d8 dResult = fValue1 + fValue2;
  iResult = *(d8 *) &dResult;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Add_INTBCD: int + bcd                                             */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Add_INTBCD(i8 iFloatValue, i8 iBcdValue)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Add_INTINT: integer + integer                                     */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Add_INTINT(i8 iValue1, i8 iValue2)
{
  i8 iResult;

  if (iValue1 == C_DATUM_NULL || iValue2 == C_DATUM_NULL)
    iResult = C_DATUM_NULL;
  else
    iResult = iValue1 + iValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Add_IVLDTE:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Add_IVLDTE(i8 iFloatValue, i8 iBcdValue)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Add_IVLIVL:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Add_IVLIVL(i8 iFloatValue, i8 iBcdValue)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Add_IVLTME:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Add_IVLTME(i8 iFloatValue, i8 iBcdValue)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Add_IVLTSP:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Add_IVLTSP(i8 iFloatValue, i8 iBcdValue)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Div_BCDBCD:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Div_BCDBCD(i8 iValue1, i8 iValue2)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Mul_BCDBCD:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Mul_BCDBCD(i8 iValue1, i8 iValue2)
{
  // COME BACK TO THIS!!!
  i8 iResult  = iValue1 * iValue2 / 100;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Neg_BCD:                                                          */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Neg_BCD(i8 iValue)
{
  if (iValue == C_DATUM_NULL)
    return C_DATUM_NULL;

  return -iValue;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
