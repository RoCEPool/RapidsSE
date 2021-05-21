/*============================================================================*/
/* MathLib.h -- math utility                                                  */
/*============================================================================*/
#pragma once

union MathLib_Dte {
  struct {
    u8 m_uDay: 5; // 0..31       same as _Tsp.dte
    u8 m_uMonth: 4; // 0..15
    u8 m_uYearDelta: 8; // 0..255  17b (1900+8b == 2155)

    u8 m_uSpare: 47; //         47b
    //               ---
    //                64
    //               ===
  };
  i8 m_iAsI8;
};

union MathLib_Tsp {
  struct {
    u8 m_uLocal     :  1 ; //        1b

    u8 m_uMicro     : 20 ; // 0..1M
    u8 m_uSecond    :  6 ; // 0..63
    u8 m_uMinute    :  6 ; // 0..63
    u8 m_uHour      :  5 ; // 0..31   37b

    u8 m_uDay       :  5 ; // 0..31
    u8 m_uMonth     :  4 ; // 0..15
    u8 m_uYearDelta :  8 ; // 0..255  17b  (1900+8b == 2155)

    u8 m_uSpare     :  9 ; //        9b
    //               ---
    //                64
    //               ===
  };
  i8 m_iAsI8;
};

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
  i8 iResult = C_DATUM_NULL;

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
  iResult = *(i8 *) &dResult;

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

  fValue1 = *(d8*)&iFltValue1;
  fValue2 = (d8)iIntValue2;

  d8 dResult = fValue1 + fValue2;
  iResult = *(i8 *) &dResult;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Add_INTBCD: int + bcd                                             */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Add_INTBCD(i8 iIntValue, i8 iBcdValue)
{
  i8 iResult;

  if (iIntValue == C_DATUM_NULL || iBcdValue == C_DATUM_NULL)
    iResult = C_DATUM_NULL;
  else
    iResult = (iIntValue*100) + iBcdValue;

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
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Div_BCDINT:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Div_BCDINT(i8 iBcdValue, i8 iIntValue)
{
  i8 iResult;

  if (iBcdValue == C_DATUM_NULL || iIntValue == C_DATUM_NULL)
    iResult = C_DATUM_NULL;
  else
    iResult = (iIntValue*100) / iBcdValue;

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
  i8 iResult = C_DATUM_NULL;

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
  iResult = *(i8 *) &dResult;

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
  fValue2 = (d8)iIntValue2;

  d8 dResult = fValue1 / fValue2;
  iResult = *(i8 *) &dResult;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Div_INTBCD:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Div_INTBCD(i8 iIntValue, i8 iBcdValue)
{
  i8 iResult;

  if (iIntValue == C_DATUM_NULL || iBcdValue == C_DATUM_NULL)
    iResult = C_DATUM_NULL;
  else
    iResult = (iIntValue*100) / iBcdValue;

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

  fValue1 = (d8)iIntValue1;
  fValue2 = *(d8*)&iFltValue2;

  d8 dResult = fValue1 / fValue2;
  iResult = *(i8 *) &dResult;

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
  i8 iResult;

  if (iBcdValue == C_DATUM_NULL || iIntValue == C_DATUM_NULL)
    iResult = C_DATUM_NULL;
  else
    iResult = (iIntValue*100) * iBcdValue;

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

  d8 dResult = fValue1 * fValue2;
  iResult = *(i8 *) &dResult;

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
  fValue2 = (d8)iIntValue;

  d8 dResult = fValue1 * fValue2;
  iResult = *(i8 *) &dResult;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Mul_INTBCD:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Mul_INTBCD(i8 iIntValue, i8 iBcdValue)
{
  i8 iResult;

  if (iIntValue == C_DATUM_NULL || iBcdValue == C_DATUM_NULL)
    iResult = C_DATUM_NULL;
  else
    iResult = (iIntValue*100) * iBcdValue;

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

/*----------------------------------------------------------------------------*/
/* iMathLib_Neg_FLT:                                                          */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Neg_FLT(i8 iFltValue)
{
  if (iFltValue == C_DATUM_NULL)
    return C_DATUM_NULL;

  d8 fValue1 = *(d8*)&iFltValue;

  d8 dResult = - fValue1;
  i8 iResult = *(i8 *) &dResult;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Neg_INT:                                                          */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Neg_INT(i8 iIntValue)
{
  if (iIntValue == C_DATUM_NULL)
    return C_DATUM_NULL;

  return -iIntValue;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Neg_IVL:                                                          */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Neg_IVL(i8 iIntValue)
{
  return C_DATUM_NULL;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Sub_BCDBCD:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Sub_BCDBCD(i8 iBcdValue1, i8 iBcdValue2)
{
  // COME BACK TO THIS
  i8 iResult;

  if (iBcdValue1 == C_DATUM_NULL || iBcdValue2 == C_DATUM_NULL)
    iResult = C_DATUM_NULL;
  else
    iResult = iBcdValue1 - iBcdValue2;


  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Sub_BCDFLT:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Sub_BCDFLT(i8 iBcdValue, i8 iFloatValue)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Sub_BCDINT:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Sub_BCDINT(i8 iBcdValue, i8 iIntValue)
{
  // COME BACK TO THIS !!!
  i8 iResult;

  if (iBcdValue == C_DATUM_NULL || iIntValue == C_DATUM_NULL)
    iResult = C_DATUM_NULL;
  else
    iResult = (iIntValue*100) - iBcdValue;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Sub_DTEIVL:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Sub_DTEIVL(i8 iDteValue, i8 iIvlValue)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Sub_FLTBCD:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Sub_FLTBCD(i8 iFloatValue, i8 iBcdValue)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Sub_FLTFLT:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Sub_FLTFLT(i8 iFltValue1, i8 iFltValue2)
{
  i8 iResult;
  d8 fValue1;
  d8 fValue2;

  if (iFltValue1 ==  C_DATUM_NULL || iFltValue2 == C_DATUM_NULL)
    return C_DATUM_NULL;

  fValue1 = *(d8*)&iFltValue1;
  fValue2 = *(d8*)&iFltValue2;

  d8 dResult = fValue1 - fValue2;
  iResult = *(i8 *) &dResult;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Sub_FLTFLT:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Sub_FLTINT(i8 iFltValue1, i8 iIntValue2)
{
  i8 iResult;
  d8 fValue1;
  d8 fValue2;

  if (iFltValue1 == C_DATUM_NULL || iIntValue2 == C_DATUM_NULL)
    return C_DATUM_NULL;

  fValue1 = *(d8*)&iFltValue1;
  fValue2 = (d8)iIntValue2;

  d8 dResult = fValue1 - fValue2;
  iResult = *(i8 *) &dResult;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Sub_INTBCD:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Sub_INTBCD(i8 iIntValue, i8 iBcdValue)
{
  i8 iResult;

  if (iIntValue == C_DATUM_NULL || iBcdValue == C_DATUM_NULL)
    iResult = C_DATUM_NULL;
  else
    iResult = (iIntValue*100) - iBcdValue;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Sub_INTFLT:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Sub_INTFLT(i8 iIntValue1, i8 iFltValue2)
{
  i8 iResult;
  d8 fValue1;
  d8 fValue2;

  if (iIntValue1 == C_DATUM_NULL || iFltValue2 == C_DATUM_NULL)
    return C_DATUM_NULL;

  fValue1 = (d8)iIntValue1;
  fValue2 = *(d8*)&iFltValue2;

  d8 dResult = fValue1 - fValue2;
  iResult = *(i8 *) &dResult;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Sub_INTINT:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Sub_INTINT(i8 iIntValue1, i8 iIntValue2)
{
  i8 iResult;

  if (iIntValue1 == C_DATUM_NULL || iIntValue2 == C_DATUM_NULL)
    iResult = C_DATUM_NULL;
  else
    iResult = iIntValue1 - iIntValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Sub_IVLDTE:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Sub_IVLDTE(i8 iIvlValue, i8 iDteValue)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Sub_IVLIVL:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Sub_IVLIVL(i8 iIvlValue1, i8 iIvlValue2)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_v_IVLTME:                                                         */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Sub_IVLTME(i8 iIvlValue, i8 iTmeValue)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Sub_IVLTSP:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Sub_IVLTSP(i8 iIvlValue, i8 iTspValue)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Sub_TMEIVL:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Sub_TMEIVL(i8 iTmeValue, i8 iIvlValue)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Sub_TSPIVL:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Sub_TSPIVL(i8 iTspValue, i8 iIvlValue)
{
  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Lgl_AND:                                                          */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Lgl_AND(i8 iBolValue1, i8 iBolValue2)
{
  if ( (iBolValue1 == C_DATUM_NULL) && (iBolValue2 == C_DATUM_NULL))
    return C_DATUM_NULL;

  if (iBolValue1 == C_DATUM_NULL) {
    if (iBolValue2 == 0)
      return 0;
    else
      return C_DATUM_NULL;
  }

  if (iBolValue2 == C_DATUM_NULL) {
    if (iBolValue1 == 0)
      return 0;
    else
      return C_DATUM_NULL;
  }

  i8 iResult =  (iBolValue1 && iBolValue2) ? 1 : 0;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Lgl_OR:                                                          */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Lgl_OR(i8 iBolValue1, i8 iBolValue2)
{
  if ( (iBolValue1 == C_DATUM_NULL) && (iBolValue2 == C_DATUM_NULL))
    return C_DATUM_NULL;

  if (iBolValue1 == C_DATUM_NULL) {
    if (iBolValue2 == 1)
      return 1;
    else
      return C_DATUM_NULL;
  }

  if (iBolValue2 == C_DATUM_NULL) {
    if (iBolValue1 == 1)
      return 1;
    else
      return C_DATUM_NULL;
  }

  i8 iResult =  (iBolValue1 || iBolValue2) ? 1 : 0;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Lgl_NOT:                                                          */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Lgl_NOT(i8 iBolValue)
{
  if (iBolValue == C_DATUM_NULL)
    return C_DATUM_NULL;

  i8 iResult =  ( !iBolValue ) ? 1 : 0;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Lgl_XOR:                                                          */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Lgl_XOR(i8 iBolValue1, i8 iBolValue2)
{
  if ( (iBolValue1 == C_DATUM_NULL) && (iBolValue2 == C_DATUM_NULL))
    return C_DATUM_NULL;

  if (iBolValue1 == C_DATUM_NULL) {
    return 0;
  }

  if (iBolValue2 == C_DATUM_NULL) {
    return 0;
  }

  i8 iResult =  (!iBolValue1 && iBolValue2) || (iBolValue1 && !iBolValue2) ? 1 : 0;

  return iResult;
}



/*----------------------------------------------------------------------------*/
/* iMathLib_Eq_BCDBCD:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Eq_BCDBCD(i8 iBcdValue1, i8 iBcdValue2)
{
  // COME BACK TO THIS
  
  if (iBcdValue1 == C_DATUM_NULL || iBcdValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult  = iBcdValue1 == iBcdValue2;
  
  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Eq_BOLBOL:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Eq_BOLBOL(i8 iBolValue1, i8 iBolValue2)
{
  if (iBolValue1 == C_DATUM_NULL || iBolValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult  = iBolValue1 == iBolValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Eq_DTEDTE:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Eq_DTEDTE(i8 iDteValue1, i8 iDteValue2)
{
  if (iDteValue1 == C_DATUM_NULL || iDteValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult  = iDteValue1 == iDteValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Eq_DTETSP:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Eq_DTETSP(i8 iDteValue, i8 iTspValue)
{
  if (iDteValue == C_DATUM_NULL || iTspValue == C_DATUM_NULL)
    return 0;

  union MathLib_Dte xDte;
  xDte.m_iAsI8 = iDteValue;

  union MathLib_Tsp xTsp;
  xTsp.m_iAsI8 = iTspValue;

  union MathLib_Tsp xDteTsp;
  xDteTsp.m_iAsI8 = 0;

  xDteTsp.m_uDay       = xDte.m_uDay;
  xDteTsp.m_uMonth     = xDte.m_uMonth;
  xDteTsp.m_uYearDelta = xDte.m_uYearDelta;

  if (xDteTsp.m_iAsI8 == xTsp.m_iAsI8)
    return 1;
  else
    return 0;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Eq_FLTBCD:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Eq_FLTBCD(i8 iFltValue, i8 iBcdValue)
{
  if (iFltValue ==  C_DATUM_NULL || iBcdValue == C_DATUM_NULL)
    return 0;

  i8 iResult = 0;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Eq_FLTFLT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Eq_FLTFLT(i8 iFltValue1, i8 iFltValue2)
{
  if (iFltValue1 ==  C_DATUM_NULL || iFltValue2 == C_DATUM_NULL)
    return 0;

  d8 fValue1 = *(d8*)&iFltValue1;
  d8 fValue2 = *(d8*)&iFltValue2;

  i8 iResult  = fValue1 == fValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Eq_FLTINT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Eq_FLTINT(i8 iFltValue, i8 iIntValue)
{
  if (iFltValue == C_DATUM_NULL || iIntValue == C_DATUM_NULL)
    return 0;

  d8 fValue1 = *(d8*)&iFltValue;
  d8 fValue2 = (d8)iIntValue;

  i8 iResult  = fValue1 == fValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Eq_INTBCD:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Eq_INTBCD(i8 iIntValue, i8 iBcdValue)
{
  // COME BACK TO THIS

  if (iIntValue == C_DATUM_NULL || iBcdValue == C_DATUM_NULL)
    return 0;

  i8 iResult  = iIntValue*100 == iBcdValue;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Eq_INTINT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Eq_INTINT(i8 iIntValue1, i8 iIntValue2)
{
  if (iIntValue1 == C_DATUM_NULL || iIntValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult = iIntValue1 == iIntValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Eq_IVLIVL:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Eq_IVLIVL(i8 iIvlValue1, i8 iIvlValue2)
{
  if (iIvlValue1 == C_DATUM_NULL || iIvlValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult = 0;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Eq_TMETME:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Eq_TMETME(i8 iTmeValue1, i8 iTmeValue2)
{
  if (iTmeValue1 == C_DATUM_NULL || iTmeValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult = iTmeValue1 == iTmeValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Eq_TSPDTE:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Eq_TSPDTE(i8 iTspValue, i8 iDteValue)
{
  if (iTspValue == C_DATUM_NULL || iDteValue == C_DATUM_NULL)
    return 0;

  union MathLib_Tsp xTsp;
  xTsp.m_iAsI8 = iTspValue;

  union MathLib_Dte xDte;
  xDte.m_iAsI8 = iDteValue;

  union MathLib_Tsp xDteTsp;
  xDteTsp.m_iAsI8 = 0;

  xDteTsp.m_uDay       = xDte.m_uDay;
  xDteTsp.m_uMonth     = xDte.m_uMonth;
  xDteTsp.m_uYearDelta = xDte.m_uYearDelta;

  if (xTsp.m_iAsI8 == xDteTsp.m_iAsI8)
    return 1;
  else
    return 0;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Eq_TSPTSP:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Eq_TSPTSP(i8 iTspValue1, i8 iTspValue2)
{
  if (iTspValue1 == C_DATUM_NULL || iTspValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult = iTspValue1 == iTspValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Eq_VCHVCH:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Eq_VCHVCH(char* azString1, char* azString2)
{
  if ((i8)azString1 == C_DATUM_NULL || (i8)azString2 == C_DATUM_NULL)
    return 0;

  i8 iResult = strcmp(azString1,azString2);

  return iResult == 0 ? 1 : 0;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ge_BCDBCD:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ge_BCDBCD(i8 iBcdValue1, i8 iBcdValue2)
{
  // COME BACK TO THIS

  if (iBcdValue1 == C_DATUM_NULL || iBcdValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult  = iBcdValue1 >= iBcdValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ge_BCDFLT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ge_BCDFLT(i8 iBcdValue, i8 iFltValue)
{
  // COME BACK TO THIS

  if (iBcdValue == C_DATUM_NULL || iFltValue == C_DATUM_NULL)
    return 0;

  i8 iResult  = 0;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ge_BCDINT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ge_BCDINT(i8 iBcdValue, i8 iIntValue)
{
  // COME BACK TO THIS

  if (iBcdValue == C_DATUM_NULL || iIntValue == C_DATUM_NULL)
    return 0;

  i8 iResult  = iBcdValue >= iIntValue*100;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ge_BOLBOL:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ge_BOLBOL(i8 iBolValue1, i8 iBolValue2)
{
  if (iBolValue1 == C_DATUM_NULL || iBolValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult  = iBolValue1 >= iBolValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ge_DTEDTE:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ge_DTEDTE(i8 iDteValue1, i8 iDteValue2)
{
  if (iDteValue1 == C_DATUM_NULL || iDteValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult  = iDteValue1 >= iDteValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ge_DTEDTE:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ge_DTETSP(i8 iDteValue, i8 iTspValue)
{
  if (iDteValue == C_DATUM_NULL || iTspValue == C_DATUM_NULL)
    return 0;

  union MathLib_Dte xDte;
  xDte.m_iAsI8 = iDteValue;

  union MathLib_Tsp xTsp;
  xTsp.m_iAsI8 = iTspValue;

  union MathLib_Tsp xDteTsp;
  xDteTsp.m_iAsI8 = 0;

  xDteTsp.m_uDay       = xDte.m_uDay;
  xDteTsp.m_uMonth     = xDte.m_uMonth;
  xDteTsp.m_uYearDelta = xDte.m_uYearDelta;

  if (xDteTsp.m_iAsI8 >= xTsp.m_iAsI8)
    return 1;
  else
    return 0;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ge_FLTBCD:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ge_FLTBCD(i8 iFltValue, i8 iBcdValue)
{
  if (iFltValue ==  C_DATUM_NULL || iBcdValue == C_DATUM_NULL)
    return 0;

  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ge_FLTFLT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ge_FLTFLT(i8 iFltValue1, i8 iFltValue2)
{
  if (iFltValue1 ==  C_DATUM_NULL || iFltValue2 == C_DATUM_NULL)
    return 0;

  d8 fValue1 = *(d8*)&iFltValue1;
  d8 fValue2 = *(d8*)&iFltValue2;

  i8 iResult  = fValue1 >= fValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ge_FLTINT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ge_FLTINT(i8 iFltValue, i8 iIntValue)
{
  if (iFltValue == C_DATUM_NULL || iIntValue == C_DATUM_NULL)
    return 0;

  d8 fValue1 = *(d8*)&iFltValue;
  d8 fValue2 = (d8)iIntValue;

  i8 iResult  = fValue1 >= fValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ge_INTBCD:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ge_INTBCD(i8 iIntValue, i8 iBcdValue)
{
  // COME BACK TO THIS

  if (iIntValue == C_DATUM_NULL || iBcdValue == C_DATUM_NULL)
    return 0;

  i8 iResult  = iIntValue*100 >= iBcdValue;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ge_INTFLT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ge_INTFLT(i8 iIntValue, i8 iFltValue)
{
  if (iIntValue == C_DATUM_NULL || iFltValue == C_DATUM_NULL)
    return 0;

  d8 fValue1 = (d8)iIntValue;
  d8 fValue2 = *(d8*)&iFltValue;

  i8 iResult  = fValue1 >= fValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ge_INTINT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ge_INTINT(i8 iIntValue1, i8 iIntValue2)
{
  if (iIntValue1 == C_DATUM_NULL || iIntValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult = iIntValue1 >= iIntValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ge_IVLIVL:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ge_IVLIVL(i8 iIvlValue1, i8 iIvlValue2)
{
  if (iIvlValue1 == C_DATUM_NULL || iIvlValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult = 0;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ge_TMETME:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ge_TMETME(i8 iTmeValue1, i8 iTmeValue2)
{
  if (iTmeValue1 == C_DATUM_NULL || iTmeValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult = iTmeValue1 >= iTmeValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ge_TSPDTE:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ge_TSPDTE(i8 iTspValue, i8 iDteValue)
{
  if (iTspValue == C_DATUM_NULL || iDteValue == C_DATUM_NULL)
    return 0;

  union MathLib_Tsp xTsp;
  xTsp.m_iAsI8 = iTspValue;

  union MathLib_Dte xDte;
  xDte.m_iAsI8 = iDteValue;

  union MathLib_Tsp xDteTsp;
  xDteTsp.m_iAsI8 = 0;

  xDteTsp.m_uDay       = xDte.m_uDay;
  xDteTsp.m_uMonth     = xDte.m_uMonth;
  xDteTsp.m_uYearDelta = xDte.m_uYearDelta;

  if (xTsp.m_iAsI8 >= xDteTsp.m_iAsI8)
    return 1;
  else
    return 0;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ge_TSPTSP:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ge_TSPTSP(i8 iTspValue1, i8 iTspValue2)
{
  if (iTspValue1 == C_DATUM_NULL || iTspValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult = iTspValue1 >= iTspValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ge_VCHVCH:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ge_VCHVCH(char* azString1, char* azString2)
{
  if ((i8)azString1 == C_DATUM_NULL || (i8)azString2 == C_DATUM_NULL)
    return 0;

  i8 iResult = strcmp(azString1,azString2);

  return iResult >= 0 ? 1 : 0;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Gt_BCDBCD:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Gt_BCDBCD(i8 iBcdValue1, i8 iBcdValue2)
{
  // COME BACK TO THIS

  if (iBcdValue1 == C_DATUM_NULL || iBcdValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult  = iBcdValue1 > iBcdValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Gt_BCDFLT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Gt_BCDFLT(i8 iBcdValue, i8 iFltValue)
{
  // COME BACK TO THIS

  if (iBcdValue == C_DATUM_NULL || iFltValue == C_DATUM_NULL)
    return 0;

  i8 iResult  = 0;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Gt_BCDINT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Gt_BCDINT(i8 iBcdValue, i8 iIntValue)
{
  // COME BACK TO THIS

  if (iBcdValue == C_DATUM_NULL || iIntValue == C_DATUM_NULL)
    return 0;

  i8 iResult  = iBcdValue > iIntValue * 100;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Gt_BOLBOL:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Gt_BOLBOL(i8 iBolValue1, i8 iBolValue2)
{
  if (iBolValue1 == C_DATUM_NULL || iBolValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult  = iBolValue1 > iBolValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Gt_DTEDTE:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Gt_DTEDTE(i8 iDteValue1, i8 iDteValue2)
{
  if (iDteValue1 == C_DATUM_NULL || iDteValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult  = iDteValue1 > iDteValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Gt_DTEDTE:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Gt_DTETSP(i8 iDteValue, i8 iTspValue)
{
  if (iDteValue == C_DATUM_NULL || iTspValue == C_DATUM_NULL)
    return 0;

  union MathLib_Dte xDte;
  xDte.m_iAsI8 = iDteValue;

  union MathLib_Tsp xTsp;
  xTsp.m_iAsI8 = iTspValue;

  union MathLib_Tsp xDteTsp;
  xDteTsp.m_iAsI8 = 0;

  xDteTsp.m_uDay       = xDte.m_uDay;
  xDteTsp.m_uMonth     = xDte.m_uMonth;
  xDteTsp.m_uYearDelta = xDte.m_uYearDelta;

  if (xDteTsp.m_iAsI8 > xTsp.m_iAsI8)
    return 1;
  else
    return 0;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Gt_FLTBCD:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Gt_FLTBCD(i8 iFltValue, i8 iBcdValue)
{
  if (iFltValue ==  C_DATUM_NULL || iBcdValue == C_DATUM_NULL)
    return 0;

  i8 iResult = 0;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Gt_FLTFLT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Gt_FLTFLT(i8 iFltValue1, i8 iFltValue2)
{
  if (iFltValue1 ==  C_DATUM_NULL || iFltValue2 == C_DATUM_NULL)
    return 0;

  d8 fValue1 = *(d8*)&iFltValue1;
  d8 fValue2 = *(d8*)&iFltValue2;

  i8 iResult  = fValue1 >= fValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Gt_FLTINT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Gt_FLTINT(i8 iFltValue, i8 iIntValue)
{
  if (iFltValue == C_DATUM_NULL || iIntValue == C_DATUM_NULL)
    return 0;

  d8 fValue1 = *(d8*)&iFltValue;
  d8 fValue2 = (d8)iIntValue;

  i8 iResult  = fValue1 >= fValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Gt_INTBCD:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Gt_INTBCD(i8 iIntValue, i8 iBcdValue)
{
  // COME BACK TO THIS

  if (iIntValue == C_DATUM_NULL || iBcdValue == C_DATUM_NULL)
    return 0;

  i8 iResult  = iIntValue*100 > iBcdValue;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Gt_INTFLT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Gt_INTFLT(i8 iIntValue, i8 iFltValue)
{
  if (iIntValue == C_DATUM_NULL || iFltValue == C_DATUM_NULL)
    return 0;

  d8 fValue1 = (d8)iIntValue;
  d8 fValue2 = *(d8*)&iFltValue;

  i8 iResult  = fValue1 > fValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Gt_INTINT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Gt_INTINT(i8 iIntValue1, i8 iIntValue2)
{
  if (iIntValue1 == C_DATUM_NULL || iIntValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult = iIntValue1 > iIntValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Gt_IVLIVL:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Gt_IVLIVL(i8 iIvlValue1, i8 iIvlValue2)
{
  if (iIvlValue1 == C_DATUM_NULL || iIvlValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult = 0;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Gt_TMETME:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Gt_TMETME(i8 iTmeValue1, i8 iTmeValue2)
{
  if (iTmeValue1 == C_DATUM_NULL || iTmeValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult = iTmeValue1 > iTmeValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Gt_TSPDTE:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Gt_TSPDTE(i8 iTspValue, i8 iDteValue)
{
  if (iTspValue == C_DATUM_NULL || iDteValue == C_DATUM_NULL)
    return 0;

  union MathLib_Tsp xTsp;
  xTsp.m_iAsI8 = iTspValue;

  union MathLib_Dte xDte;
  xDte.m_iAsI8 = iDteValue;

  union MathLib_Tsp xDteTsp;
  xDteTsp.m_iAsI8 = 0;

  xDteTsp.m_uDay       = xDte.m_uDay;
  xDteTsp.m_uMonth     = xDte.m_uMonth;
  xDteTsp.m_uYearDelta = xDte.m_uYearDelta;

  if (xTsp.m_iAsI8 > xDteTsp.m_iAsI8)
    return 1;
  else
    return 0;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Gt_TSPTSP:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Gt_TSPTSP(i8 iTspValue1, i8 iTspValue2)
{
  if (iTspValue1 == C_DATUM_NULL || iTspValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult = iTspValue1 > iTspValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Gt_VCHVCH:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Gt_VCHVCH(char* azString1, char* azString2)
{
  if ((i8)azString1 == C_DATUM_NULL || (i8)azString2 == C_DATUM_NULL)
    return 0;

  i8 iResult = strcmp(azString1,azString2);

  return iResult > 0 ? 1 : 0;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Le_BCDBCD:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Le_BCDBCD(i8 iBcdValue1, i8 iBcdValue2)
{
  // COME BACK TO THIS

  if (iBcdValue1 == C_DATUM_NULL || iBcdValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult  = iBcdValue1 <= iBcdValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Le_BCDINT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Le_BCDFLT(i8 iBcdValue, i8 FltValue)
{
  // COME BACK TO THIS

  if (iBcdValue == C_DATUM_NULL || FltValue == C_DATUM_NULL)
    return 0;

  i8 iResult  = 0;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Le_BCDINT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Le_BCDINT(i8 iBcdValue, i8 iIntValue)
{
  // COME BACK TO THIS

  if (iBcdValue == C_DATUM_NULL || iIntValue == C_DATUM_NULL)
    return 0;

  i8 iResult  = iBcdValue <= iIntValue*100;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Le_BOLBOL:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Le_BOLBOL(i8 iBolValue1, i8 iBolValue2)
{
  if (iBolValue1 == C_DATUM_NULL || iBolValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult  = iBolValue1 <= iBolValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Le_DTEDTE:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Le_DTEDTE(i8 iDteValue1, i8 iDteValue2)
{
  if (iDteValue1 == C_DATUM_NULL || iDteValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult  = iDteValue1 <= iDteValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Le_DTEDTE:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Le_DTETSP(i8 iDteValue, i8 iTspValue)
{
  if (iDteValue == C_DATUM_NULL || iTspValue == C_DATUM_NULL)
    return 0;

  union MathLib_Dte xDte;
  xDte.m_iAsI8 = iDteValue;

  union MathLib_Tsp xTsp;
  xTsp.m_iAsI8 = iTspValue;

  union MathLib_Tsp xDteTsp;
  xDteTsp.m_iAsI8 = 0;

  xDteTsp.m_uDay       = xDte.m_uDay;
  xDteTsp.m_uMonth     = xDte.m_uMonth;
  xDteTsp.m_uYearDelta = xDte.m_uYearDelta;

  if (xDteTsp.m_iAsI8 <= xTsp.m_iAsI8)
    return 1;
  else
    return 0;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Le_FLTBCD:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Le_FLTBCD(i8 iFltValue, i8 iBcdValue)
{
  if (iFltValue ==  C_DATUM_NULL || iBcdValue == C_DATUM_NULL)
    return 0;

  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Le_FLTFLT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Le_FLTFLT(i8 iFltValue1, i8 iFltValue2)
{
  if (iFltValue1 ==  C_DATUM_NULL || iFltValue2 == C_DATUM_NULL)
    return 0;

  d8 fValue1 = *(d8*)&iFltValue1;
  d8 fValue2 = *(d8*)&iFltValue2;

  i8 iResult  = fValue1 <= fValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Le_FLTINT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Le_FLTINT(i8 iFltValue, i8 iIntValue)
{
  if (iFltValue == C_DATUM_NULL || iIntValue == C_DATUM_NULL)
    return 0;

  d8 fValue1 = *(d8*)&iFltValue;
  d8 fValue2 = (d8)iIntValue;

  i8 iResult  = fValue1 <= fValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Le_INTBCD:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Le_INTBCD(i8 iIntValue, i8 iBcdValue)
{
  // COME BACK TO THIS

  if (iIntValue == C_DATUM_NULL || iBcdValue == C_DATUM_NULL)
    return 0;

  i8 iResult  = iIntValue*100 <= iBcdValue;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Le_INTFLT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Le_INTFLT(i8 iIntValue, i8 iFltValue)
{
  if (iIntValue == C_DATUM_NULL || iFltValue == C_DATUM_NULL)
    return 0;

  d8 fValue1 = (d8)iIntValue;
  d8 fValue2 = *(d8*)&iFltValue;

  i8 iResult  = fValue1 <= fValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Le_INTINT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Le_INTINT(i8 iIntValue1, i8 iIntValue2)
{
  if (iIntValue1 == C_DATUM_NULL || iIntValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult = iIntValue1 <= iIntValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Le_IVLIVL:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Le_IVLIVL(i8 iIvlValue1, i8 iIvlValue2)
{
  if (iIvlValue1 == C_DATUM_NULL || iIvlValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult = 0;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Le_TMETME:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Le_TMETME(i8 iTmeValue1, i8 iTmeValue2)
{
  if (iTmeValue1 == C_DATUM_NULL || iTmeValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult = iTmeValue1 <= iTmeValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Le_TSPDTE:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Le_TSPDTE(i8 iTspValue, i8 iDteValue)
{
  if (iTspValue == C_DATUM_NULL || iDteValue == C_DATUM_NULL)
    return 0;

  union MathLib_Tsp xTsp;
  xTsp.m_iAsI8 = iTspValue;

  union MathLib_Dte xDte;
  xDte.m_iAsI8 = iDteValue;

  union MathLib_Tsp xDteTsp;
  xDteTsp.m_iAsI8 = 0;

  xDteTsp.m_uDay       = xDte.m_uDay;
  xDteTsp.m_uMonth     = xDte.m_uMonth;
  xDteTsp.m_uYearDelta = xDte.m_uYearDelta;

  if (xTsp.m_iAsI8 <= xDteTsp.m_iAsI8)
    return 1;
  else
    return 0;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Le_TSPTSP:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Le_TSPTSP(i8 iTspValue1, i8 iTspValue2)
{
  if (iTspValue1 == C_DATUM_NULL || iTspValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult = iTspValue1 <= iTspValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Le_VCHVCH:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Le_VCHVCH(char* azString1, char* azString2)
{
  if ((i8)azString1 == C_DATUM_NULL || (i8)azString2 == C_DATUM_NULL)
    return 0;

  i8 iResult = strcmp(azString1,azString2);

  return iResult <= 0 ? 1 : 0;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Lt_BCDBCD:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Lt_BCDBCD(i8 iBcdValue1, i8 iBcdValue2)
{
  // COME BACK TO THIS

  if (iBcdValue1 == C_DATUM_NULL || iBcdValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult  = iBcdValue1 < iBcdValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Lt_BCDFLT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Lt_BCDFLT(i8 iBcdValue, i8 iFltValue)
{
  // COME BACK TO THIS

  if (iBcdValue == C_DATUM_NULL || iFltValue == C_DATUM_NULL)
    return 0;

  i8 iResult  = 0;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Lt_BCDINT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Lt_BCDINT(i8 iBcdValue, i8 iIntValue)
{
  // COME BACK TO THIS

  if (iBcdValue == C_DATUM_NULL || iIntValue == C_DATUM_NULL)
    return 0;

  i8 iResult  = iBcdValue < iIntValue*100;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Lt_BOLBOL:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Lt_BOLBOL(i8 iBolValue1, i8 iBolValue2)
{
  if (iBolValue1 == C_DATUM_NULL || iBolValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult  = iBolValue1 < iBolValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Lt_DTEDTE:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Lt_DTEDTE(i8 iDteValue1, i8 iDteValue2)
{
  if (iDteValue1 == C_DATUM_NULL || iDteValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult  = iDteValue1 < iDteValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Lt_DTEDTE:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Lt_DTETSP(i8 iDteValue, i8 iTspValue)
{
  if (iDteValue == C_DATUM_NULL || iTspValue == C_DATUM_NULL)
    return 0;

  union MathLib_Dte xDte;
  xDte.m_iAsI8 = iDteValue;

  union MathLib_Tsp xTsp;
  xTsp.m_iAsI8 = iTspValue;

  union MathLib_Tsp xDteTsp;
  xDteTsp.m_iAsI8 = 0;

  xDteTsp.m_uDay       = xDte.m_uDay;
  xDteTsp.m_uMonth     = xDte.m_uMonth;
  xDteTsp.m_uYearDelta = xDte.m_uYearDelta;

  if (xDteTsp.m_iAsI8 < xTsp.m_iAsI8)
    return 1;
  else
    return 0;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Lt_FLTBCD:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Lt_FLTBCD(i8 iFltValue, i8 iBcdValue)
{
  if (iFltValue ==  C_DATUM_NULL || iBcdValue == C_DATUM_NULL)
    return 0;

  i8 iResult = 0;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Lt_FLTFLT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Lt_FLTFLT(i8 iFltValue1, i8 iFltValue2)
{
  if (iFltValue1 ==  C_DATUM_NULL || iFltValue2 == C_DATUM_NULL)
    return 0;

  d8 fValue1 = *(d8*)&iFltValue1;
  d8 fValue2 = *(d8*)&iFltValue2;

  i8 iResult  = fValue1 < fValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Lt_FLTINT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Lt_FLTINT(i8 iFltValue1, i8 iIntValue2)
{
  if (iFltValue1 == C_DATUM_NULL || iIntValue2 == C_DATUM_NULL)
    return 0;

  d8 fValue1 = *(d8*)&iFltValue1;
  d8 fValue2 = (d8)iIntValue2;

  i8 iResult  = fValue1 < fValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Lt_INTBCD:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Lt_INTBCD(i8 iIntValue, i8 iBcdValue)
{
  // COME BACK TO THIS

  if (iIntValue == C_DATUM_NULL || iBcdValue == C_DATUM_NULL)
    return 0;

  i8 iResult  = iIntValue*100 < iBcdValue;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Lt_INTFLT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Lt_INTFLT(i8 iIntValue, i8 iFltValue)
{
  if (iIntValue == C_DATUM_NULL || iFltValue == C_DATUM_NULL)
    return 0;

  d8 fValue1 = (d8)iIntValue;
  d8 fValue2 = *(d8*)&iFltValue;

  i8 iResult  = fValue1 < fValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Lt_INTINT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Lt_INTINT(i8 iIntValue1, i8 iIntValue2)
{
  if (iIntValue1 == C_DATUM_NULL || iIntValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult = iIntValue1 < iIntValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Lt_IVLIVL:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Lt_IVLIVL(i8 iIvlValue1, i8 iIvlValue2)
{
  if (iIvlValue1 == C_DATUM_NULL || iIvlValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult = 0;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Lt_TMETME:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Lt_TMETME(i8 iTmeValue1, i8 iTmeValue2)
{
  if (iTmeValue1 == C_DATUM_NULL || iTmeValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult = iTmeValue1 < iTmeValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Lt_TSPDTE:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Lt_TSPDTE(i8 iTspValue, i8 iDteValue)
{
  if (iTspValue == C_DATUM_NULL || iDteValue == C_DATUM_NULL)
    return 0;

  union MathLib_Tsp xTsp;
  xTsp.m_iAsI8 = iTspValue;

  union MathLib_Dte xDte;
  xDte.m_iAsI8 = iDteValue;

  union MathLib_Tsp xDteTsp;
  xDteTsp.m_iAsI8 = 0;

  xDteTsp.m_uDay       = xDte.m_uDay;
  xDteTsp.m_uMonth     = xDte.m_uMonth;
  xDteTsp.m_uYearDelta = xDte.m_uYearDelta;

  if (xTsp.m_iAsI8 < xDteTsp.m_iAsI8)
    return 1;
  else
    return 0;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Lt_TSPTSP:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Lt_TSPTSP(i8 iTspValue1, i8 iTspValue2)
{
  if (iTspValue1 == C_DATUM_NULL || iTspValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult = iTspValue1 < iTspValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Lt_VCHVCH:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Lt_VCHVCH(char* azString1, char* azString2)
{
  if ((i8)azString1 == C_DATUM_NULL || (i8)azString2 == C_DATUM_NULL)
    return 0;

  i8 iResult = strcmp(azString1,azString2);

  return iResult < 0 ? 1 : 0;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ne_BCDBCD:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ne_BCDBCD(i8 iValue1, i8 iValue2)
{
  // COME BACK TO THIS

  if (iValue1 == C_DATUM_NULL || iValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult  = iValue1 != iValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ne_BCDFLT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ne_BCDFLT(i8 iBcdValue, i8 iFltValue)
{
  // COME BACK TO THIS

  if (iBcdValue == C_DATUM_NULL || iFltValue == C_DATUM_NULL)
    return 0;

  i8 iResult  = 0;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ne_BCDINT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ne_BCDINT(i8 iBcdValue, i8 iIntValue)
{
  // COME BACK TO THIS

  if (iBcdValue == C_DATUM_NULL || iIntValue == C_DATUM_NULL)
    return 0;

  i8 iResult  = iBcdValue != iIntValue*100;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ne_BOLBOL:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ne_BOLBOL(i8 iBolValue1, i8 iBolValue2)
{
  if (iBolValue1 == C_DATUM_NULL || iBolValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult  = iBolValue1 != iBolValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ne_DTEDTE:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ne_DTEDTE(i8 iDteValue1, i8 iDteValue2)
{
  if (iDteValue1 == C_DATUM_NULL || iDteValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult  = iDteValue1 != iDteValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ne_DTEDTE:                                                       */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ne_DTETSP(i8 iDteValue, i8 iTspValue)
{
  if (iDteValue == C_DATUM_NULL || iTspValue == C_DATUM_NULL)
    return 0;

  union MathLib_Dte xDte;
  xDte.m_iAsI8 = iDteValue;

  union MathLib_Tsp xTsp;
  xTsp.m_iAsI8 = iTspValue;

  union MathLib_Tsp xDteTsp;
  xDteTsp.m_iAsI8 = 0;

  xDteTsp.m_uDay       = xDte.m_uDay;
  xDteTsp.m_uMonth     = xDte.m_uMonth;
  xDteTsp.m_uYearDelta = xDte.m_uYearDelta;

  if (xDteTsp.m_iAsI8 != xTsp.m_iAsI8)
    return 1;
  else
    return 0;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ne_FLTBCD:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ne_FLTBCD(i8 iFltValue, i8 iBcdValue)
{
  if (iFltValue ==  C_DATUM_NULL || iBcdValue == C_DATUM_NULL)
    return 0;

  i8 iResult = C_DATUM_NULL;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ne_FLTFLT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ne_FLTFLT(i8 iFltValue1, i8 iFltValue2)
{
  if (iFltValue1 ==  C_DATUM_NULL || iFltValue2 == C_DATUM_NULL)
    return 0;

  d8 fValue1 = *(d8*)&iFltValue1;
  d8 fValue2 = *(d8*)&iFltValue2;

  i8 iResult  = fValue1 != fValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ne_FLTINT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ne_FLTINT(i8 iValue1, i8 iValue2)
{
  if (iValue1 == C_DATUM_NULL || iValue2 == C_DATUM_NULL)
    return 0;

  d8 fValue1 = *(d8*)&iValue1;
  d8 fValue2 = (d8)iValue2;

  i8 iResult  = fValue1 != fValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ne_INTBCD:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ne_INTBCD(i8 iIntValue, i8 iBcdValue)
{
  // COME BACK TO THIS

  if (iIntValue == C_DATUM_NULL || iBcdValue == C_DATUM_NULL)
    return 0;

  i8 iResult  = iIntValue*100 != iBcdValue;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ne_INTFLT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ne_INTFLT(i8 iIntValue, i8 iFltValue)
{
  if (iIntValue == C_DATUM_NULL || iFltValue == C_DATUM_NULL)
    return 0;

  d8 fValue1 = (d8)iIntValue;
  d8 fValue2 = *(d8*)&iFltValue;

  i8 iResult  = fValue1 != fValue2;

  return iResult;
}
/*----------------------------------------------------------------------------*/
/* iMathLib_Ne_INTINT:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ne_INTINT(i8 iIntValue1, i8 iIntValue2)
{
  if (iIntValue1 == C_DATUM_NULL || iIntValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult = iIntValue1 != iIntValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ne_IVLIVL:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ne_IVLIVL(i8 iIvlValue1, i8 iIvlValue2)
{
  if (iIvlValue1 == C_DATUM_NULL || iIvlValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult = 0;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ne_TMETME:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ne_TMETME(i8 iTmeValue1, i8 iTmeValue2)
{
  if (iTmeValue1 == C_DATUM_NULL || iTmeValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult = iTmeValue1 != iTmeValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ne_TSPDTE:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ne_TSPDTE(i8 iTspValue, i8 iDteValue)
{
  if (iTspValue == C_DATUM_NULL || iDteValue == C_DATUM_NULL)
    return 0;

  union MathLib_Tsp xTsp;
  xTsp.m_iAsI8 = iTspValue;

  union MathLib_Dte xDte;
  xDte.m_iAsI8 = iDteValue;

  union MathLib_Tsp xDteTsp;
  xDteTsp.m_iAsI8 = 0;

  xDteTsp.m_uDay       = xDte.m_uDay;
  xDteTsp.m_uMonth     = xDte.m_uMonth;
  xDteTsp.m_uYearDelta = xDte.m_uYearDelta;

  if (xTsp.m_iAsI8 != xDteTsp.m_iAsI8)
    return 1;
  else
    return 0;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ne_TSPTSP:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ne_TSPTSP(i8 iTspValue1, i8 iTspValue2)
{
  if (iTspValue1 == C_DATUM_NULL || iTspValue2 == C_DATUM_NULL)
    return 0;

  i8 iResult = iTspValue1 != iTspValue2;

  return iResult;
}

/*----------------------------------------------------------------------------*/
/* iMathLib_Ne_VCHVCH:                                                        */
/*----------------------------------------------------------------------------*/
inline i8 iMathLib_Ne_VCHVCH(char* azString1, char* azString2)
{
  if ((i8)azString1 == C_DATUM_NULL || (i8)azString2 == C_DATUM_NULL)
    return 0;

  i8 iResult = strcmp(azString1,azString2);

  return iResult != 0 ? 1 : 0;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
