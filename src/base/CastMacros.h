/*============================================================================*/
/* CastMacros.h -- Cast Macros                                                */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// CAST
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _ANY_TOUNK(iErrorCode)            \
{                                         \
  iArg1 = *(--paW);                       \
  *paW++ = C_DATUM_NULL;                  \
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  TO BCD
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _INT_TOBCD(iErrorCode)                                             \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    if (iErrorCode != EC_NONE ||                                           \
      iArg1 == C_DATUM_NULL) {                                             \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      if(iArg1 > C_BCD_MAX) {                                              \
        iErrorCode = EC_ARITHMETIC_OVERFLOW;                               \
        *paW++ = C_DATUM_NULL;                                             \
      }                                                                    \
      else if(iArg1 < (i8) C_BCD_MIN) {                                    \
        iErrorCode = EC_ARITHMETIC_UNDERFLOW;                              \
        *paW++ = C_DATUM_NULL;                                             \
      }                                                                    \
      else {                                                               \
        i8 max_col_decimal = TMP_TBLCOL_SCALE;                             \
        bcdArg1.m_iMantissa = iArg1;                                       \
        bcdArg2.m_iMantissa = aDynamicT->m_iaPow10[max_col_decimal];       \
        iErrorCode = _OF_BCDMUL;                                           \
        if(iErrorCode != EC_NONE) {                                        \
          *paW++ = C_DATUM_NULL;                                           \
        }                                                                  \
        else {                                                             \
          bcdArg1.m_iMantissa *= bcdArg2.m_iMantissa;                      \
          bcdArg1.m_uScale = max_col_decimal;                              \
          *paW++ = bcdArg1.m_iAsI8;                                        \
        }                                                                  \
      }                                                                    \
    }                                                                      \
  }

#define _VCH_TOBCD(iErrorCode) \
  iErrorCode = EC_NOT_IMPLEMENTED;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  TO FLOAT
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define _BCD_TOFLT(iErrorCode) \
{                                                                        \
  bcdArg1.m_iAsI8 = *(--paW);                                            \
  if (iErrorCode != EC_NONE ||                                           \
    bcdArg1.m_iAsI8 == C_DATUM_NULL) {                                   \
    *paW++ = C_DATUM_NULL;                                               \
  }                                                                      \
  else {                                                                 \
    fltArg1.m_dFlt = (d8)(bcdArg1.m_iMantissa);                          \
    fltArg1.m_dFlt = fltArg1.m_dFlt / aDynamicT->m_iaPow10[bcdArg1.m_uScale]; \
    *paW++ = fltArg1.m_iAsI8 ;                                           \
  }                                                                      \
}

// INT to Float
#define _INT_TOFLT(iErrorCode) \
{                                                                        \
  fltArg1.m_iAsI8 = *(--paW);                                            \
  if (iErrorCode != EC_NONE ||                                           \
  fltArg1.m_iAsI8 == C_DATUM_NULL) {                                     \
    *paW++ = C_DATUM_NULL;                                               \
  }                                                                      \
  else {                                                                 \
    if(fltArg1.m_dFlt > (d8)C_FLT_MAX) {                                 \
      iErrorCode = EC_ARITHMETIC_OVERFLOW;                               \
      *paW++ = C_DATUM_NULL;                                             \
    }                                                                    \
    else if(fltArg1.m_dFlt < (d8) C_FLT_MIN) {                           \
      iErrorCode = EC_ARITHMETIC_UNDERFLOW;                              \
      *paW++ = C_DATUM_NULL;                                             \
    }                                                                    \
    else {                                                               \
      fltArg2.m_dFlt = fltArg1.m_iAsI8;                                  \
      *paW++ = fltArg2.m_iAsI8;                                          \
    }                                                                    \
  }                                                                      \
}

// Float to int is just drop the decimal digits?
#define _FLT_TOINT(iErrorCode)                                             \
{                                                                          \
    iArg1 = *(--paW);                                                      \
    fltArg1.m_iAsI8 = iArg1;                                               \
    if (iErrorCode != EC_NONE ||                                           \
        iArg1 == C_DATUM_NULL) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
  }                                                                        \
  else {                                                                   \
    if(fltArg1.m_dFlt > (i8)C_I8_MAX) {                                    \
      iErrorCode = EC_ARITHMETIC_OVERFLOW;                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else if(fltArg1.m_dFlt < (i8) C_I8_MIN) {                              \
      iErrorCode = EC_ARITHMETIC_UNDERFLOW;                                \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      *paW++ = (i8)( fltArg1.m_dFlt >= 0.0 ?                               \
        (fltArg1.m_dFlt + 0.5) : (fltArg1.m_dFlt - 0.5) );                 \
    }                                                                      \
  }                                                                        \
}

#define _FLT_TOBCD(iErrorCode)                                             \
{                                                                          \
  fltArg1.m_iAsI8 = *(--paW);                                              \
                                                                           \
  if (iErrorCode != EC_NONE ||                                             \
    fltArg1.m_iAsI8 == C_DATUM_NULL) {                                     \
    *paW++ = C_DATUM_NULL;                                                 \
  }                                                                        \
  else {                                                                   \
    if (fltArg1.m_dFlt > (d8)C_BCD_MAX) {                                  \
      iErrorCode = EC_ARITHMETIC_OVERFLOW;                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else if (fltArg1.m_dFlt < (d8) C_BCD_MIN) {                            \
      iErrorCode = EC_ARITHMETIC_UNDERFLOW;                                \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      i8 cnt = 0;                                                          \
      d8 dt = fltArg1.m_dFlt - (i8)(fltArg1.m_dFlt);                       \
      if (fltArg1.m_dFlt < 0 ) dt = dt * -1;                               \
      while (dt >= 0.0000001 && cnt < 14) {                                \
        dt = dt * 10.0;                                                    \
        dt = dt - (int)(dt+0.0000001);                                     \
        cnt++;                                                             \
      }                                                                    \
                                                                           \
      bcdArg1.m_iMantissa = fltArg1.m_iAsI8;                               \
      bcdArg2.m_iMantissa = aDynamicT->m_iaPow10[cnt];                     \
                                                                           \
      i8 iMantissa = (i8)(fltArg1.m_dFlt * (d8)bcdArg2.m_iMantissa);       \
      while (iMantissa > C_BCD_MAX) {                                      \
        iMantissa = iMantissa / 10;                                        \
        cnt--;                                                             \
      }                                                                    \
                                                                           \
      bcdArg1.m_iMantissa = iMantissa;                                     \
      bcdArg1.m_uScale = cnt;                                              \
                                                                           \
      *paW++ = bcdArg1.m_iAsI8;                                            \
    }                                                                      \
  }                                                                        \
}

static inline void vFltToBcdToScale(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Flt_T fltArg1;
  i8 iScale;
  union Bcd_T bcdArg1;
  union Bcd_T bcdArg2;

  fltArg1.m_iAsI8 = *(--(*ppaW));
  iScale = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    fltArg1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    if (fltArg1.m_dFlt > (d8)C_BCD_MAX) {
      aDynamicT->m_iErrorCode = EC_ARITHMETIC_OVERFLOW;
      *(*ppaW)++ = C_DATUM_NULL;
    }
    else if (fltArg1.m_dFlt < (d8) C_BCD_MIN) {
      aDynamicT->m_iErrorCode = EC_ARITHMETIC_UNDERFLOW;
      *(*ppaW)++ = C_DATUM_NULL;
    }
    else {
      i8 cnt = 0;
      d8 dt = fltArg1.m_dFlt - (i8)(fltArg1.m_dFlt);
      if (fltArg1.m_dFlt < 0 ) dt = dt * -1;
      while (dt >= 0.0000001 && cnt < iScale) {
        dt = dt * 10.0;
        dt = dt - (int)(dt+0.0000001);
        cnt++;
      }

      bcdArg1.m_iMantissa = fltArg1.m_iAsI8;
      bcdArg2.m_iMantissa = aDynamicT->m_iaPow10[iScale];

      i8 iMantissa = (i8)(fltArg1.m_dFlt * (d8)bcdArg2.m_iMantissa);
      while (iMantissa > C_BCD_MAX) {
        iMantissa = iMantissa / 10;
        cnt--;
      }

      bcdArg1.m_iMantissa = iMantissa;
      bcdArg1.m_uScale = iScale;

      *(*ppaW)++ = bcdArg1.m_iAsI8;
    }
  }
}


#define _FLT_TOVCH(iErrorCode, szStrArea)                             \
{                                                                     \
  fltArg1.m_iAsI8 = *(--paW);                                         \
  if (iErrorCode != EC_NONE ||                                        \
    fltArg1.m_iAsI8 == C_DATUM_NULL) {                                \
    *paW++ = C_DATUM_NULL;                                            \
  }                                                                   \
  else {                                                              \
    sprintf(szStrArea, "%.9e", fltArg1.m_dFlt);                       \
    *paW++ = (i8)szStrArea;                                           \
    szStrArea += strlen(szStrArea) + 1;                               \
  }                                                                   \
}

/*
 * Does not take care of min/max flt to varchar
 *
// how many decimal places , 10?
//WARNING do we need to length of string?

#define _FLT_TOVCH(iErrorCode, szStrArea)                             \
{                                                                     \
  fltArg1.m_iAsI8 = *(--paW);                                         \
  if (iErrorCode != EC_NONE ||                                        \
    fltArg1.m_iAsI8 == C_DATUM_NULL) {                                \
    *paW++ = C_DATUM_NULL;                                            \
  }                                                                   \
  else {                                                              \
    char *pS = szStrArea;                                             \
    char *pS1 = szStrArea;                                            \
    i8 iVal;                                                          \
    i8 iDecs;                                                         \
    if (fltArg1.m_iAsI8 < 0) {                                        \
      fltArg1.m_dFlt -= 0.00000000005;                                \
      iDecs = (i8)(fltArg1.m_dFlt * -10000000000) % 10000000000;      \
      iVal = (i8)(-1 * fltArg1.m_dFlt);                               \
    }                                                                 \
    else {                                                            \
      fltArg1.m_dFlt += 0.00000000005;                                \
      iDecs = (i8)(fltArg1.m_dFlt * 10000000000) % 10000000000;       \
      iVal = (i8)fltArg1.m_dFlt;                                      \
    }                                                                 \
    for (i8 i=0; i< 10; i++) {                                        \
      *pS++ = (iDecs % 10) + '0';                                     \
      iDecs /= 10;                                                    \
    }                                                                 \
    *pS++ = '.';                                                      \
    if (iVal == 0)                                                    \
      *pS++ = '0' ;                                                   \
    else {                                                            \
      while (iVal > 0) {                                              \
      *pS++ = (iVal % 10) + '0';                                      \
        iVal /= 10;                                                   \
      }                                                               \
    }                                                                 \
    if (fltArg1.m_dFlt < 0.0) *pS++ = '-';                            \
    *pS = '\0';                                                       \
    while (--pS > pS1) {                                              \
      char c = *pS;                                                   \
      *pS = *pS1;                                                     \
      *pS1++ = c;                                                     \
    }                                                                 \
    *paW++ = (i8)szStrArea;                                           \
    szStrArea += strlen(szStrArea) + 1;                               \
  }                                                                   \
}
*/

// varchar TO FLT
#define _VCH_TOFLT(iErrorCode)                                        \
{                                                                     \
  iArg1 = *(--paW);                                                   \
  char *pS = (char*)iArg1;                                            \
  if (iErrorCode != EC_NONE ||                                        \
    pS == C_DATUM_NULL) {                                             \
    *paW++ = C_DATUM_NULL;                                            \
  }                                                                   \
  else {                                                              \
                                                                      \
    i8 len=0, n=0,i=0;                                                \
    d8 f=1.0,val=0.0;                                                 \
    while(pS[len])len++;                                              \
    if(!len) {                                                        \
      *paW++ = C_DATUM_NULL;                                          \
    }                                                                 \
    else {                                                            \
      while(i<len && pS[i]!='.')                                      \
        n=10*n +(pS[i++]-'0');                                        \
      i++;                                                            \
      while(i<len)                                                    \
      {                                                               \
        f*=0.1;                                                       \
        val+=f*(pS[i++]-'0');                                         \
      }                                                               \
      fltArg1.m_dFlt = (val+n);                                       \
      *paW++ = fltArg1.m_iAsI8;                                       \
   }                                                                  \
 }                                                                    \
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TO INTEGER
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define _BCD_TOINT(iErrorCode)                                             \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    if (iErrorCode != EC_NONE ||                                           \
      bcdArg1.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      *paW++ = bcdArg1.m_iMantissa / aDynamicT->m_iaPow10[bcdArg1.m_uScale]; \
    }                                                                      \
  }

#define _VCH_TOINT(iErrorCode) \
  iErrorCode = EC_NOT_IMPLEMENTED;



// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TO DATE
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define _TSP_TODTE(iErrorCode)                                             \
  {                                                                        \
    xTsp1.m_iAsI8 = *(--paW);                                              \
    if (iErrorCode != EC_NONE ||                                           \
      xTsp1.m_iAsI8 == C_DATUM_NULL) {                                     \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      xDte1.m_iAsI8 = 0;                                                   \
                                                                           \
      xDte1.m_uDay = xTsp1.m_uDay;                                         \
      xDte1.m_uMonth = xTsp1.m_uMonth;                                     \
      xDte1.m_uYearDelta = xTsp1.m_uYearDelta;                             \
                                                                           \
      *paW++ = xDte1.m_iAsI8;                                              \
    }                                                                      \
  }

#define _VCH_TODTE(iErrorCode) \
  iErrorCode = EC_NOT_IMPLEMENTED;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TO IVL
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// INT to DAY
#define _INT_TODAY(iErrorCode) \
{                                                                        \
  iArg1 = *(--paW);                                                      \
  if (iErrorCode != EC_NONE ||                                           \
    iArg1 == C_DATUM_NULL) {                                             \
    *paW++ = C_DATUM_NULL;                                               \
  }                                                                      \
  else {                                                                 \
    wIvlResult.m_iAsI8 = 0;                                              \
    wIvlResult.m_xIvl.m_eType = IVL_DAY;                                 \
                                                                         \
    if (iArg1 < 0) {                                                     \
      wIvlResult.m_xIvl.m_bIsNeg = 1;                                    \
		}                                                                    \
		wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros = iArg1 * _C_SECS_PER_DAY * _C_1M; \
																                                         \
		*paW++ = wIvlResult.m_iAsI8;                                         \
  }                                                                      \
}

// INT to HOUR
#define _INT_TOHOUR(iErrorCode)                                          \
{                                                                        \
  iArg1 = *(--paW);                                                      \
  if (iErrorCode != EC_NONE ||                                           \
    iArg1 == C_DATUM_NULL) {                                             \
    *paW++ = C_DATUM_NULL;                                               \
  }                                                                      \
  else {                                                                 \
    wIvlResult.m_iAsI8 = 0;                                              \
    wIvlResult.m_xIvl.m_eType = IVL_HOUR;                                \
                                                                         \
    if (iArg1 < 0) {                                                     \
      wIvlResult.m_xIvl.m_bIsNeg = 1;                                    \
		}                                                                    \
		wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros = iArg1 * _C_SECS_PER_HOUR * _C_1M; \
																                                         \
		*paW++ = wIvlResult.m_iAsI8;                                         \
  }                                                                      \
}

// INT to MINUTE
#define _INT_TOMINUTE(iErrorCode) \
{                                                                        \
  iArg1 = *(--paW);                                                      \
  if (iErrorCode != EC_NONE ||                                           \
    iArg1 == C_DATUM_NULL) {                                             \
    *paW++ = C_DATUM_NULL;                                               \
  }                                                                      \
  else {                                                                 \
    wIvlResult.m_iAsI8 = 0;                                              \
    wIvlResult.m_xIvl.m_eType = IVL_MINUTE;                              \
                                                                         \
    if (iArg1 < 0) {                                                     \
      wIvlResult.m_xIvl.m_bIsNeg = 1;                                    \
		}                                                                    \
		wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros = iArg1 * _C_SECS_PER_MINUTE * _C_1M; \
																                                         \
		*paW++ = wIvlResult.m_iAsI8;                                         \
  }                                                                      \
}

// INT to MONTH
#define _INT_TOMONTH(iErrorCode)                                         \
{                                                                        \
  iArg1 = *(--paW);                                                      \
  if (iErrorCode != EC_NONE ||                                           \
    iArg1 == C_DATUM_NULL) {                                             \
    *paW++ = C_DATUM_NULL;                                               \
  }                                                                      \
  else {                                                                 \
    wIvlResult.m_iAsI8 = 0;                                              \
    wIvlResult.m_xIvl.m_eType = IVL_MONTH;                               \
                                                                         \
    if (iArg1 < 0) {                                                     \
      wIvlResult.m_xIvl.m_bIsNeg = 1;                                    \
		}                                                                    \
		wIvlResult.m_xIvl.u.m_xIvlYM.m_uMonths = iArg1;                      \
																                                         \
		*paW++ = wIvlResult.m_iAsI8;                                         \
  }                                                                      \
}

// INT to SECOND
#define _INT_TOSECOND(iErrorCode) \
{                                                                        \
  iArg1 = *(--paW);                                                      \
  if (iErrorCode != EC_NONE ||                                           \
    iArg1 == C_DATUM_NULL) {                                             \
    *paW++ = C_DATUM_NULL;                                               \
  }                                                                      \
  else {                                                                 \
    wIvlResult.m_iAsI8 = 0;                                              \
    wIvlResult.m_xIvl.m_eType = IVL_SECOND;                              \
                                                                         \
    if (iArg1 < 0) {                                                     \
      wIvlResult.m_xIvl.m_bIsNeg = 1;                                    \
		}                                                                    \
		wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros = iArg1 * _C_1M;              \
																                                         \
		*paW++ = wIvlResult.m_iAsI8;                                         \
  }                                                                      \
}

// INT to YEAR
#define _INT_TOYEAR(iErrorCode)                                          \
{                                                                        \
  iArg1 = *(--paW);                                                      \
  if (iErrorCode != EC_NONE ||                                           \
    iArg1 == C_DATUM_NULL) {                                             \
    *paW++ = C_DATUM_NULL;                                               \
  }                                                                      \
  else {                                                                 \
    wIvlResult.m_iAsI8 = 0;                                              \
    wIvlResult.m_xIvl.m_eType = IVL_DAY;                                 \
                                                                         \
    if (iArg1 < 0) {                                                     \
      wIvlResult.m_xIvl.m_bIsNeg = 1;                                    \
		}                                                                    \
		wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros = iArg1 * _C_MONTHS_PER_YEAR; \
																                                         \
		*paW++ = wIvlResult.m_iAsI8;                                         \
  }                                                                      \
}

#define _VCH_TOIVL(iErrorCode) \
  iErrorCode = EC_NOT_IMPLEMENTED;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TO IVY
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define _VCH_TOIVY(iErrorCode) \
  iErrorCode = EC_NOT_IMPLEMENTED;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TO TME
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define _TSP_TOTME(iErrorCode)                                             \
  {                                                                        \
    xTsp1.m_iAsI8 = *(--paW);                                              \
    if (iErrorCode != EC_NONE ||                                           \
      xTsp1.m_iAsI8 == C_DATUM_NULL) {                                     \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      xTme1.m_iAsI8 = 0;                                                   \
                                                                           \
      xTme1.m_uMicro = xTsp1.m_uMicro;                                     \
      xTme1.m_uSecond = xTsp1.m_uSecond;                                   \
      xTme1.m_uMinute = xTsp1.m_uMinute;                                   \
      xTme1.m_uHour = xTsp1.m_uHour;                                       \
                                                                           \
      *paW++ = xTme1.m_iAsI8;                                              \
    }                                                                      \
  }

#define _VCH_TOTME(iErrorCode) \
  iErrorCode = EC_NOT_IMPLEMENTED;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TO TSP
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define _DTE_TOTSP(iErrorCode)                                             \
  {                                                                        \
    xDte1.m_iAsI8 = *(--paW);                                              \
    if (iErrorCode != EC_NONE ||                                           \
      xDte1.m_iAsI8 == C_DATUM_NULL) {                                     \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      xTsp1.m_iAsI8 = 0;                                                   \
                                                                           \
      xTsp1.m_uDay = xDte1.m_uDay;                                         \
      xTsp1.m_uMonth = xDte1.m_uMonth;                                     \
      xTsp1.m_uYearDelta = xDte1.m_uYearDelta;                             \
      xTsp1.m_uHour      = 0;                                              \
      xTsp1.m_uMinute    = 0;                                              \
      xTsp1.m_uSecond    = 0;                                              \
      xTsp1.m_uMicro     = 0;                                              \
      xTsp1.m_uLocal     = _C_TSP_LOCAL;                                   \
      *paW++ = xTsp1.m_iAsI8;                                              \
    }                                                                      \
  }

#define _TME_TOTSP(iErrorCode)                                             \
  {                                                                        \
    xTme1.m_iAsI8 = *(--paW);                                              \
    if (iErrorCode != EC_NONE ||                                           \
      xTme1.m_iAsI8 == C_DATUM_NULL) {                                     \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      time_t iSecondsSincePosixEpoch = time(0);                            \
      struct tm* aLocalDateTime = localtime(&iSecondsSincePosixEpoch);     \
                                                                           \
      xTsp1.m_iAsI8 = 0;                                                   \
                                                                           \
      xTsp1.m_uYearDelta = aLocalDateTime->tm_year;                        \
      xTsp1.m_uMonth = aLocalDateTime->tm_mon + 1;                         \
      xTsp1.m_uDay = aLocalDateTime->tm_mday;                              \
                                                                           \
      xTsp1.m_uMicro = xTme1.m_uMicro;                                     \
      xTsp1.m_uSecond = xTme1.m_uSecond;                                   \
      xTsp1.m_uMinute = xTme1.m_uMinute;                                   \
      xTsp1.m_uHour = xTme1.m_uHour;                                       \
      xTsp1.m_uLocal = _C_TSP_LOCAL;                                       \
                                                                           \
      *paW++ = xTsp1.m_iAsI8;                                              \
    }                                                                      \
  }

#define _VCH_TOTSP(iErrorCode) \
  iErrorCode = EC_NOT_IMPLEMENTED;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TO VCH
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define _BOL_TOVCH(iErrorCode, szDstArea)                                  \
{                                                                          \
  iArg1 = *(--paW);                                                        \
  if (iErrorCode != EC_NONE ||                                             \
      iArg1 == C_DATUM_NULL) {                                             \
    *paW++ = C_DATUM_NULL;                                                 \
  }                                                                        \
  else {                                                                   \
    if (iArg1)                                                             \
      strcpy(szDstArea,"true");                                            \
    else                                                                   \
      strcpy(szDstArea,"false");                                           \
    *paW++ = (i8)szDstArea;                                                \
    szDstArea += strlen(szDstArea) + 1;                                    \
  }                                                                        \
}



#define _BCD_TOVCH(iErrorCode, szDstArea)                                  \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    if (iErrorCode != EC_NONE ||                                           \
      bcdArg1.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      i8 max_col_decimal = TMP_TBLCOL_SCALE;                               \
      i8 bNegative = 0;                                                    \
                                                                           \
      if (bcdArg1.m_iMantissa < 0) {                                       \
        bcdArg1.m_iMantissa = -bcdArg1.m_iMantissa;                        \
        bNegative = 1;                                                     \
      }                                                                    \
                                                                           \
      char *p = szDstArea, *q = szDstArea;                                 \
      if(bcdArg1.m_uScale == 0) {                                          \
        while(max_col_decimal) {                                           \
          *p++ = '0';                                                      \
          max_col_decimal--;                                               \
        }                                                                  \
      }                                                                    \
      else {                                                               \
        while (bcdArg1.m_uScale) {                                         \
          *p++ = (bcdArg1.m_iMantissa % 10) + '0';                         \
          bcdArg1.m_iMantissa /= 10ll;                                     \
          bcdArg1.m_uScale--;                                              \
        }                                                                  \
      }                                                                    \
      *p++ = '.';                                                          \
      while (bcdArg1.m_iMantissa) {                                        \
        *p++ = (bcdArg1.m_iMantissa % 10) + '0';                           \
        bcdArg1.m_iMantissa /= 10ll;                                       \
      }                                                                    \
      if (bNegative)                                                       \
        *p++ = '-';                                                        \
                                                                           \
        *p = '\0';                                                         \
        while (--p > q) {                                                  \
          char c = *p;                                                     \
          *p = *q;                                                         \
          *q++ = c;                                                        \
        }                                                                  \
      *paW++ = (i8)szDstArea;                                              \
      szDstArea += strlen(szDstArea) + 1;                                  \
    }                                                                      \
  }

#define _TME_TOVCH(iErrorCode, szDstArea)                                  \
  {                                                                        \
    xTme1.m_iAsI8 = *(--paW);                                              \
    if (iErrorCode != EC_NONE ||                                           \
      xTme1.m_iAsI8 == C_DATUM_NULL) {                                     \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      char *p = szDstArea;                                                 \
                                                                           \
      u8 tmp = xTme1.m_uHour;                                              \
      tmp /= 10llu;                                                        \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      tmp = xTme1.m_uHour;                                                 \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      *p++ = ':';                                                          \
                                                                           \
      tmp = xTme1.m_uMinute;                                               \
      tmp /= 10llu;                                                        \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      tmp = xTme1.m_uMinute;                                               \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      *p++ = ':';                                                          \
                                                                           \
      tmp = xTme1.m_uSecond;                                               \
      tmp /= 10llu;                                                        \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      tmp = xTme1.m_uSecond;                                               \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      if(xTme1.m_uMicro != 0) {                                            \
        *p++ = ':';                                                        \
                                                                           \
        tmp = xTme1.m_uMicro;                                              \
        tmp /= 10llu;                                                      \
        *p++ = (tmp % 10) + '0';                                           \
                                                                           \
        tmp = xTme1.m_uMicro;                                              \
        *p++ = (tmp % 10) + '0';                                           \
      }                                                                    \
      *paW++ = (i8)szDstArea;                                              \
      szDstArea += strlen(szDstArea) + 1;                                  \
    }                                                                      \
}

#define _DTE_TOVCH(iErrorCode, szDstArea)                                  \
  {                                                                        \
    xDte1.m_iAsI8 = *(--paW);                                              \
    if (iErrorCode != EC_NONE ||                                           \
      xDte1.m_iAsI8 == C_DATUM_NULL) {                                     \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      u8 tmpYear = C_YEAR_BASE + xDte1.m_uYearDelta;                       \
      char *p = szDstArea;                                                 \
                                                                           \
      u8 tmp = tmpYear;                                                    \
      tmp /= 1000llu;                                                      \
      *p++ = tmp + '0';                                                    \
                                                                           \
      tmp = tmpYear;                                                       \
      tmp /= 100llu;                                                       \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      tmp = tmpYear;                                                       \
      tmp /= 10llu;                                                        \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      tmp = tmpYear;                                                       \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      *p++ = '-';                                                          \
                                                                           \
      tmp = xDte1.m_uMonth;                                                \
      tmp /= 10llu;                                                        \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      tmp = xDte1.m_uMonth;                                                \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      *p++ = '-';                                                          \
                                                                           \
      tmp = xDte1.m_uDay;                                                  \
      tmp /= 10llu;                                                        \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      tmp = xDte1.m_uDay;                                                  \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      *paW++ = (i8)szDstArea;                                              \
      szDstArea += strlen(szDstArea) + 1;                                  \
    }                                                                      \
}


#define _INT_TOVCH(iErrorCode, szDstArea)                                  \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    if (iErrorCode != EC_NONE ||                                           \
      iArg1 == C_DATUM_NULL) {                                             \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      i8 bNegative = 0;                                                    \
                                                                           \
      if (iArg1 < 0) {                                                     \
        iArg1 = -iArg1;                                                    \
        bNegative = 1;                                                     \
      }                                                                    \
                                                                           \
      char *p = szDstArea, *q = szDstArea;                                 \
      do {                                                                 \
        *p++ = (iArg1 % 10) + '0';                                         \
        iArg1 /= 10ll;                                                     \
      } while (iArg1);                                                     \
                                                                           \
      if (bNegative)                                                       \
        *p++ = '-';                                                        \
                                                                           \
        *p = '\0';                                                         \
        while (--p > q) {                                                  \
          char c = *p;                                                     \
          *p = *q;                                                         \
          *q++ = c;                                                        \
        }                                                                  \
      *paW++ = (i8)szDstArea;                                              \
      szDstArea += strlen(szDstArea) + 1;                                  \
    }                                                                      \
  }

#define _IVL_TOVCH(iErrorCode) \
  iErrorCode = EC_NOT_IMPLEMENTED;

#define _IVY_TOVCH(iErrorCode) \
  iErrorCode = EC_NOT_IMPLEMENTED;

#define _TSP_TOVCH(iErrorCode, szDstArea)                                  \
  {                                                                        \
    xTsp1.m_iAsI8 = *(--paW);                                              \
    if (iErrorCode != EC_NONE ||                                           \
      xTsp1.m_iAsI8 == C_DATUM_NULL) {                                     \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      u8 tmpYear = C_YEAR_BASE + xTsp1.m_uYearDelta;                       \
      char *p = szDstArea;                                                 \
                                                                           \
      u8 tmp = tmpYear;                                                    \
      tmp /= 1000llu;                                                      \
      *p++ = tmp + '0';                                                    \
                                                                           \
      tmp = tmpYear;                                                       \
      tmp /= 100llu;                                                       \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      tmp = tmpYear;                                                       \
      tmp /= 10llu;                                                        \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      tmp = tmpYear;                                                       \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      *p++ = '-';                                                          \
                                                                           \
      tmp = xTsp1.m_uMonth;                                                \
      tmp /= 10llu;                                                        \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      tmp = xTsp1.m_uMonth;                                                \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      *p++ = '-';                                                          \
                                                                           \
      tmp = xTsp1.m_uDay;                                                  \
      tmp /= 10llu;                                                        \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      tmp = xTsp1.m_uDay;                                                  \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      *p++ = ' ';                                                          \
                                                                           \
      tmp = xTsp1.m_uHour;                                                 \
      tmp /= 10llu;                                                        \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      tmp = xTsp1.m_uHour;                                                 \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      *p++ = ':';                                                          \
                                                                           \
      tmp = xTsp1.m_uMinute;                                               \
      tmp /= 10llu;                                                        \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      tmp = xTsp1.m_uMinute;                                               \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      *p++ = ':';                                                          \
                                                                           \
      tmp = xTsp1.m_uSecond;                                               \
      tmp /= 10llu;                                                        \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      tmp = xTsp1.m_uSecond;                                               \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      if(xTsp1.m_uMicro != 0) {                                            \
        *p++ = ':';                                                        \
                                                                           \
        tmp = xTsp1.m_uMicro;                                              \
        tmp /= 10llu;                                                      \
        *p++ = (tmp % 10) + '0';                                           \
                                                                           \
        tmp = xTsp1.m_uMicro;                                              \
        *p++ = (tmp % 10) + '0';                                           \
      }                                                                    \
                                                                           \
      *paW++ = (i8)szDstArea;                                              \
      szDstArea += strlen(szDstArea) + 1;                                  \
    }                                                                      \
}

// nothing to do
// unk remains unk
#define _UNK_TOANY(iErrorCode)                                             \
{                                                                          \
                                                                           \
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
