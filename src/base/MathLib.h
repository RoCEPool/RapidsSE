/*============================================================================*/
/* MathLib.h -- math utility                                                  */
/*============================================================================*/
#pragma once

/*----------------------------------------------------------------------------*/
/* iMathLib_Add_BCDBCD:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Add_BCDBCD(i8 iBcdValue1, i8 iBcdValue2)
{

  // COME BACK TO THIS
  i8 iResult;

  if (iBcdValue1 == C_DATUM_NULL || iBcdValue2 == C_DATUM_NULL)
    iResult = C_DATUM_NULL;
  else
    iResult = iBcdValue1 + iBcdValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Add_DTEIVL:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Add_DTEIVL(i8 iDteValue, i8 iIvlValue)
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
inline i8 iMathLib_Add_FLTFLT(i8 iFltValue1, i8 iFltValue2)
{
  i8 iResult;
  d8 fValue1;
  d8 fValue2;

  if (iFltValue1 ==  C_DATUM_NULL || iFltValue2 == C_DATUM_NULL)
    return C_DATUM_NULL;

  fValue1 = *(d8*)&iFltValue1;
  fValue2 = *(d8*)&iFltValue2;

  d8 dResult = fValue1 + fValue2;
  iResult = *(d8 *) &dResult;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Add_FLTINT: float + int                                           */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Add_FLTINT(i8 iFltValue1, i8 iIntValue2)
{
  i8 iResult;
  d8 fValue1;
  d8 fValue2;

  if (iFltValue1 == C_DATUM_NULL || iIntValue2 == C_DATUM_NULL)
    return C_DATUM_NULL;

  fValue1 = iFltValue1;
  fValue2 = *(d8*)&iIntValue2;

  d8 dResult = fValue1 + fValue2;
  iResult = *(d8 *) &dResult;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Add_INTBCD: int + bcd                                             */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Add_INTBCD(i8 iIntValue, i8 iBcdValue)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Add_INTINT: integer + integer                                     */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Add_INTINT(i8 iIntValue1, i8 iIntValue2)
{
  i8 iResult;

  if (iIntValue1 == C_DATUM_NULL || iIntValue2 == C_DATUM_NULL)
    iResult = C_DATUM_NULL;
  else
    iResult = iIntValue1 + iIntValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Add_IVLDTE:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Add_IVLDTE(i8 iIvlValue, i8 iBcdValue)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Add_IVLIVL:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Add_IVLIVL(i8 iFltValue, i8 iBcdValue)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Add_IVLTME:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Add_IVLTME(i8 iIvlValue, i8 iTmeValue)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Add_IVLTSP:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Add_IVLTSP(i8 iIvlValue, i8 iTspValue)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Div_BCDBCD:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Div_BCDBCD(i8 iBcdValue1, i8 iBcdValue2)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Div_BCDFLT:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Div_BCDFLT(i8 iBcdValue, i8 iFloatValue)
{
  i8 iResult = C_DATUM_NULL;;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Div_BCDINT:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Div_BCDINT(i8 iBcdValue, i8 iIntValue)
{
  i8 iResult = C_DATUM_NULL;;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Div_DTEIVL:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Div_DTEIVL(i8 iDteValue, i8 iIvlValue)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Div_FLTBCD:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Div_FLTBCD(i8 iFloatValue, i8 iBcdValue)
{
  i8 iResult = C_DATUM_NULL;;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Div_FLTFLT:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Div_FLTFLT(i8 iFltValue1, i8 iFltValue2)
{
  i8 iResult;
  d8 fValue1;
  d8 fValue2;

  if (iFltValue1 ==  C_DATUM_NULL || iFltValue2 == C_DATUM_NULL)
    return C_DATUM_NULL;

  fValue1 = *(d8*)&iFltValue1;
  fValue2 = *(d8*)&iFltValue2;

  d8 dResult = fValue1 / fValue2;
  iResult = *(d8 *) &dResult;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Div_FLTFLT:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Div_FLTINT(i8 iFltValue1, i8 iIntValue2)
{
  i8 iResult;
  d8 fValue1;
  d8 fValue2;

  if (iFltValue1 == C_DATUM_NULL || iIntValue2 == C_DATUM_NULL)
    return C_DATUM_NULL;

  fValue1 = *(d8*)&iFltValue1;
  fValue2 = iIntValue2;

  d8 dResult = fValue1 / fValue2;
  iResult = *(d8 *) &dResult;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Div_INTBCD:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Div_INTBCD(i8 iIntValue, i8 iBcdValue)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Div_INTFLT:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Div_INTFLT(i8 iIntValue1, i8 iFltValue2)
{
  i8 iResult;
  d8 fValue1;
  d8 fValue2;

  if (iIntValue1 == C_DATUM_NULL || iFltValue2 == C_DATUM_NULL)
    return C_DATUM_NULL;

  fValue1 = iIntValue1;
  fValue2 = *(d8*)&iFltValue2;

  d8 dResult = fValue1 / fValue2;
  iResult = *(d8 *) &dResult;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Div_INTINT:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Div_INTINT(i8 iIntValue1, i8 iIntValue2)
{
  i8 iResult;

  if (iIntValue1 == C_DATUM_NULL || iIntValue2 == C_DATUM_NULL)
    iResult = C_DATUM_NULL;
  else
    iResult = iIntValue1 / iIntValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Div_IVLDTE:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Div_IVLDTE(i8 iIvlValue, i8 iDteValue)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Div_IVLIVL:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Div_IVLIVL(i8 iIvlValue1, i8 iIvlValue2)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_v_IVLTME:                                                         */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Div_IVLTME(i8 iIvlValue, i8 iTmeValue)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Div_IVLTSP:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Div_IVLTSP(i8 iIvlValue, i8 iTspValue)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Div_TMEIVL:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Div_TMEIVL(i8 iTmeValue, i8 iIvlValue)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Div_TSPIVL:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Div_TSPIVL(i8 iTspValue, i8 iIvlValue)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Mod_INTINT:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Mod_INTINT(i8 iIntValue1, i8 iIntValue2)
{
  i8 iResult;

  if (iIntValue1 == C_DATUM_NULL || iIntValue2 == C_DATUM_NULL)
    iResult = C_DATUM_NULL;
  else
    iResult = iIntValue1 % iIntValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Mul_BCDBCD:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Mul_BCDBCD(i8 iBcdValue1, i8 iBcdValue2)
{
  // COME BACK TO THIS!!!
  i8 iResult  = iBcdValue1 * iBcdValue2 / 100;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Mul_BCDFLT:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Mul_BCDFLT(i8 iBcdValue1, i8 iFltValue2)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Mul_BCDINT:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Mul_BCDINT(i8 iBcdValue, i8 iIntValue)
{

  // COME BACK TO THIS !!!
  i8 iResult  = iBcdValue * iIntValue;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Mul_DTEIVL:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Mul_DTEIVL(i8 iDteValue, i8 iIvlValue)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Mul_FLTFLT:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Mul_FLTFLT(i8 iFltValue1, i8 iFltValue2)
{
  i8 iResult;
  d8 fValue1;
  d8 fValue2;

  if (iFltValue1 ==  C_DATUM_NULL || iFltValue2 == C_DATUM_NULL)
    return C_DATUM_NULL;

  fValue1 = *(d8*)&iFltValue1;
  fValue2 = *(d8*)&iFltValue2;

  d8 dResult = fValue1 / fValue2;
  iResult = *(d8 *) &dResult;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Mul_FLTINT:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Mul_FLTINT(i8 iFltValue, i8 iIntValue)
{
  i8 iResult;
  d8 fValue1;
  d8 fValue2;

  if (iFltValue == C_DATUM_NULL || iIntValue == C_DATUM_NULL)
    return C_DATUM_NULL;

  fValue1 = *(d8*)&iFltValue;
  fValue2 = iIntValue;

  d8 dResult = fValue1 / fValue2;
  iResult = *(d8 *) &dResult;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Mul_INTBCD:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Mul_INTBCD(i8 iInttValue, i8 iBcdValue)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Mul_INTINT:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Mul_INTINT(i8 iIntValue1, i8 iIntValue2)
{
  i8 iResult;

  if (iIntValue1 == C_DATUM_NULL || iIntValue2 == C_DATUM_NULL)
    iResult = C_DATUM_NULL;
  else
    iResult = iIntValue1 * iIntValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Mul_IVLDTE:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Mul_IVLDTE(i8 iIvlValue, i8 iDteValue)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Mul_IVLIVL:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Mul_IVLIVL(i8 iIvlValue1, i8 iIvlValue2)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Nul_IVLTME:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Mul_IVLTME(i8 iIvlValue, i8 iTmeValue)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Mul_IVLTSP:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Mul_IVLTSP(i8 iIvlValue, i8 iTspValue)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Neg_BCD:                                                          */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Neg_BCD(i8 iBcdValue)
{
  if (iBcdValue == C_DATUM_NULL)
    return C_DATUM_NULL;

  return -iBcdValue;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
