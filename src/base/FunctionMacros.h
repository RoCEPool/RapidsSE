/*============================================================================*/
/* FunctionMacros.h -- Function Macros                                        */
/*============================================================================*/
#pragma once

#define _FILTER_EXIT_ON_FALSE(sLabel)                   \
  bFilterResult = *(--paW);                             \
  if (aDynamicT->m_iErrorCode != (i8)EC_NONE) {         \
    return 0;                                           \
  }                                                     \
  if ( bFilterResult ) {                                \
    *paW++ = (i8)bFilterResult;                         \
  }                                                     \
  else {                                                \
    aDynamicT->m_bProcessRow = false;                   \
    goto sLabel;                                        \
  }

#define _FILTER_EXIT_ON_TRUE(sLabel)                    \
  bFilterResult = *(--paW);                             \
  if (aDynamicT->m_iErrorCode != (i8)EC_NONE) {         \
    return 0;                                           \
  }                                                     \
  if ( ! bFilterResult ) {                              \
    *paW++ = (i8)bFilterResult;                         \
  }                                                     \
  else {                                                \
    aDynamicT->m_bProcessRow = false;                   \
    goto sLabel;                                        \
  }


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// PUSHES

#define _PSH_COL(iArg) \
  *paW++ = (i8)iArg;

#define _PSH_BOL(iArg)  \
  *paW++ = (i8)iArg;

#define _PSH_BCD(iArg)  \
  *paW++ = iArg;

#define _PSH_DTE(iArg)  \
  *paW++ = iArg;

#define _PSH_FLT(dArg)        \
{                             \
  fltArg1.m_dFlt = dArg;      \
  *paW++ = fltArg1.m_iAsI8;   \
}

#define _PSH_INT(iArg) \
  *paW++ = iArg;

#define _PSH_IVD(iArg)  \
  *paW++ = iArg;

#define _PSH_IVY(iArg)  \
  *paW++ = iArg;

#define _PSH_NULL \
  *paW++ = C_DATUM_NULL;

#define _PSH_TME(iArg)  \
  *paW++ = iArg;

#define _PSH_TSP(iArg)  \
  *paW++ = iArg;

#define _PSH_UNK(iArg)  \
  *paW++ = iArg;

#define _PSH_VCH(szArg1)     \
  *paW++ = (i8)szArg1;

// strncpy(szDstArea, szArg1, nSrcSize);

#define _ALLOC_VCH(szDstArea, nAreaRemaining)   \
{                                               \
  szArg1 = (char*)*(--paW);                     \
  if ((i8)szArg1 == C_DATUM_NULL) {             \
    *paW++ = C_DATUM_NULL;                      \
  }                                             \
  else {                                        \
    nSrcSize = strlen(szArg1);                  \
    char *d = szDstArea;                        \
    const char *s = szArg1;                     \
                                                \
	 i8 iBytesToCopy = nSrcSize;                  \
   while (iBytesToCopy--)                       \
     *d++ = *s++;                               \
                                                \
    *d = '\0';                                  \
                                                \
    *paW++ = (i8)szDstArea;                     \
    szDstArea += nSrcSize + 1;                  \
    nAreaRemaining -= nSrcSize + 1;             \
  }                                             \
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// ASSIGNS

#define _ASSIGN(iArgTo) \
  iArgTo = *(--paW);

#define _ASSIGN_IFTRUE(iArgTo)                                              \
 {                                                                          \
  iArg1 = *(--paW);                                                         \
  if (iArg1 && iArg1 != C_DATUM_NULL)    {                                  \
    iArgTo = *(--paW);                                                      \
    }                                                                       \
  else                                                                      \
    --paW;                                                                  \
 }

#define _ASSIGN_TO_ENTINT(iArgTo)                                           \
  iArg1 = *(--paW);                                                         \
  vExtern_EntWrkAppendElementInt(iArgTo,iArg1);

#define _ASSIGN_TO_ENTSTZ(iArgTo) \
  szArg1 = (char*)*(--paW);                                                 \
  vExtern_EntWrkAppendElementStz(iArgTo,szArg1);


#define _COPYSTR_IFTRUE(szAreaTo, iArgTo)                                   \
  {                                                                         \
    iArg1 = *(--paW);                                                       \
    iArg2 = *(--paW);                                                       \
    if (iArg1 && iArg2 != C_DATUM_NULL)    {                                \
		  nSrcSize = strlen((char*)iArg2);                                      \
			char *d = szAreaTo;                                                   \
			const char *s = (char*)iArg2;                                         \
                                                                            \
			i8 iBytesToCopy = nSrcSize;                                           \
      while (iBytesToCopy--)                                                \
			  *d++ = *s++;                                                        \
                                                                            \
      *d = '\0';                                                            \
      iArgTo =  (i8)szAreaTo;                                               \
      szAreaTo += nSrcSize + 1;                                             \
    }                                                                       \
  }

#define _COPYSTR_PL(iArg2)                                                  \
  {                                                                         \
    iArg1 = *(--paW);                                                       \
    if (iArg1 != C_DATUM_NULL)    {                                         \
      iArg2 = (i8)aExtern_IndexTreeAllotPayStr((void*)aDynamicT->m_xIndexInfoPL.m_aIndexTree, (char*)iArg1); \
    }                                                                       \
  }

#define _COPYSTR_IFTRUE_PL(iArg3)                                           \
  {                                                                         \
    iArg1 = *(--paW);                                                       \
    iArg2 = *(--paW);                                                       \
    if (iArg1 && iArg2 != C_DATUM_NULL)    {                                \
      iArg3 = (i8)aExtern_IndexTreeAllotPayStr((void*)aDynamicT->m_xIndexInfoPL.m_aIndexTree, (char*)iArg2); \
    }                                                                       \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// BETWEEN
static inline void _BET_BCDBCD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Bcd_T bcdArg1;
  union Bcd_T bcdArg2;
  union Bcd_T bcdArg3;

  bcdArg1.m_iAsI8 = *(--(*ppaW));
  bcdArg2.m_iAsI8 = *(--(*ppaW));
  bcdArg3.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      bcdArg1.m_iAsI8 == C_DATUM_NULL ||
      bcdArg2.m_iAsI8 ==  C_DATUM_NULL ||
      bcdArg3.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {

      if (bcdArg1.m_uScale != bcdArg2.m_uScale) {
        *(*ppaW)++ = bcdArg1.m_iAsI8;
        *(*ppaW)++ = bcdArg2.m_iAsI8;
        vRescaleBcdMaxOfTwo(ppaW, aDynamicT);
        bcdArg2.m_iAsI8 = *(--(*ppaW));
        bcdArg1.m_iAsI8 = *(--(*ppaW));
      }

      if (bcdArg1.m_uScale != bcdArg3.m_uScale) {
        *(*ppaW)++ = bcdArg1.m_iAsI8;
        *(*ppaW)++ = bcdArg3.m_iAsI8;
        vRescaleBcdMaxOfTwo(ppaW, aDynamicT);
        bcdArg3.m_iAsI8 = *(--(*ppaW));
        bcdArg1.m_iAsI8 = *(--(*ppaW));
      }

      if (bcdArg2.m_uScale != bcdArg3.m_uScale) {
        *(*ppaW)++ = bcdArg2.m_iAsI8;
        *(*ppaW)++ = bcdArg3.m_iAsI8;
        vRescaleBcdMaxOfTwo(ppaW, aDynamicT);
        bcdArg3.m_iAsI8 = *(--(*ppaW));
        bcdArg2.m_iAsI8 = *(--(*ppaW));
        }

      if (bcdArg1.m_iMantissa > bcdArg2.m_iMantissa) {
        *((*ppaW)++) = 0;
      }
    else {
      if (bcdArg1.m_iMantissa < bcdArg3.m_iMantissa)    {
        *((*ppaW)++) = 0;
      }
      else {
        *((*ppaW)++) = 1;
      }
    }
 }
}

static inline void _BET_FLTFLT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Flt_T fltArg1;
  union Flt_T fltArg2;
  union Flt_T fltArg3;

  fltArg1.m_iAsI8 = *(--(*ppaW));
  fltArg2.m_iAsI8 = *(--(*ppaW));
  fltArg3.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      fltArg1.m_iAsI8 == C_DATUM_NULL ||
      fltArg2.m_iAsI8 ==  C_DATUM_NULL ||
      fltArg3.m_iAsI8 == C_DATUM_NULL) {
    *((*ppaW)++) = C_DATUM_NULL;
  }
  else {
    if (fltArg1.m_dFlt > fltArg2.m_dFlt) {
      *((*ppaW)++) = 0;
    }
    else {
      if (fltArg1.m_dFlt < fltArg3.m_dFlt)
        *((*ppaW)++) = 0;
      else
        *((*ppaW)++) = 1;
    }
  }
}

static inline void _BET_INTINT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  i8 iArg2 = *(--(*ppaW));
  i8 iArg3 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL ||
      iArg2 ==  C_DATUM_NULL ||
      iArg3 == C_DATUM_NULL) {
    *((*ppaW)++) = C_DATUM_NULL;
  }
  else {
    if (iArg1 > iArg2) {
      *((*ppaW)++) = 0;
    }
    else {
      if (iArg1 < iArg3)
        *((*ppaW)++) = 0;
      else
        *((*ppaW)++) = 1;
    }
  }
}

static inline void _BET_IVDIVD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg1;
  union Ivl_T ivlArg2;
  union Ivl_T ivlArg3;

  ivlArg1.m_iAsI8 = *(--(*ppaW));
  ivlArg2.m_iAsI8 = *(--(*ppaW));
  ivlArg3.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      ivlArg1.m_iAsI8 == C_DATUM_NULL ||
      ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    i8 iArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;
    i8 iArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;
    i8 iArg3 = ivlArg3.m_xIvl.u.m_xIvlDT.m_uMicros;

    if (ivlArg1.m_xIvl.m_bIsNeg)
      iArg1 = (-1) * iArg1;

    if (ivlArg2.m_xIvl.m_bIsNeg)
      iArg2 = (-1) * iArg2;

    if (ivlArg3.m_xIvl.m_bIsNeg)
      iArg3 = (-1) * iArg3;

    if (iArg1 > iArg2) {
      *((*ppaW)++) = 0;
    }
    else {
      if (iArg1 < iArg3)
        *((*ppaW)++) = 0;
      else
        *((*ppaW)++) = 1;
    }
  }
}

static inline void _BET_IVYIVY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg1;
  union Ivl_T ivlArg2;
  union Ivl_T ivlArg3;

  ivlArg1.m_iAsI8 = *(--(*ppaW));
  ivlArg2.m_iAsI8 = *(--(*ppaW));
  ivlArg3.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      ivlArg1.m_iAsI8 == C_DATUM_NULL ||
      ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    i8 iArg1 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;
    i8 iArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;
    i8 iArg3 = ivlArg3.m_xIvl.u.m_xIvlYM.m_uMonths;

    if (ivlArg1.m_xIvl.m_bIsNeg)
      iArg1 = (-1) * iArg1;

    if (ivlArg2.m_xIvl.m_bIsNeg)
      iArg2 = (-1) * iArg2;

   if (ivlArg3.m_xIvl.m_bIsNeg)
     iArg3 = (-1) * iArg3;

    if (iArg1 > iArg2) {
      *((*ppaW)++) = 0;
    }
    else {
      if (iArg1 < iArg3)
        *((*ppaW)++) = 0;
      else
        *((*ppaW)++) = 1;
    }
  }
}

static inline void _BET_DTEDTE(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Dte_T xDte1;
  union Dte_T xDte2;
  union Dte_T xDte3;

  xDte1.m_iAsI8 = *(--(*ppaW));
  xDte2.m_iAsI8 = *(--(*ppaW));
  xDte3.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xDte1.m_iAsI8 == C_DATUM_NULL ||
      xDte2.m_iAsI8 ==  C_DATUM_NULL ||
      xDte3.m_iAsI8 == C_DATUM_NULL) {
    *((*ppaW)++) = C_DATUM_NULL;
  }
  else {
    if (xDte1.m_iAsI8 > xDte2.m_iAsI8) {
      *((*ppaW)++) = 0;
    }
    else {
      if (xDte1.m_iAsI8 < xDte3.m_iAsI8)
        *((*ppaW)++) = 0;
      else
        *((*ppaW)++) = 1;
    }
  }
}


static inline void _BET_TMETME(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tme_T xTme1;
  union Tme_T xTme2;
  union Tme_T xTme3;

  xTme1.m_iAsI8 = *(--(*ppaW));
  xTme2.m_iAsI8 = *(--(*ppaW));
  xTme3.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTme1.m_iAsI8 == C_DATUM_NULL ||
      xTme2.m_iAsI8 ==  C_DATUM_NULL ||
      xTme3.m_iAsI8 == C_DATUM_NULL) {
    *((*ppaW)++) = C_DATUM_NULL;
  }
  else {
    if (xTme1.m_iAsI8 > xTme2.m_iAsI8) {
      *((*ppaW)++) = 0;
    }
    else {
      if (xTme1.m_iAsI8 < xTme3.m_iAsI8)
        *((*ppaW)++) = 0;
      else
        *((*ppaW)++) = 1;
    }
  }
}


static inline void _BET_TSPTSP(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tsp_T xTsp1;
  union Tsp_T xTsp2;
  union Tsp_T xTsp3;

  xTsp1.m_iAsI8 = *(--(*ppaW));
  xTsp2.m_iAsI8 = *(--(*ppaW));
  xTsp3.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTsp1.m_iAsI8 == C_DATUM_NULL ||
      xTsp2.m_iAsI8 ==  C_DATUM_NULL ||
      xTsp3.m_iAsI8 == C_DATUM_NULL) {
    *((*ppaW)++) = C_DATUM_NULL;
  }
  else {
    if (xTsp1.m_iAsI8 > xTsp2.m_iAsI8) {
      *((*ppaW)++) = 0;
    }
    else {
      if (xTsp1.m_iAsI8 < xTsp3.m_iAsI8)
        *((*ppaW)++) = 0;
      else
        *((*ppaW)++) = 1;
    }
  }
}

static inline void _BET_VCHVCH(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  i8 iArg2 = *(--(*ppaW));
  i8 iArg3 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL ||
      iArg2 ==  C_DATUM_NULL ||
      iArg3 == C_DATUM_NULL) {
    *((*ppaW)++) = C_DATUM_NULL;
  }
  else {
    if (strcmp((char*)iArg1, (char*)iArg2) > 0) {
      *((*ppaW)++) = 0;
    }
    else {
      if (strcmp((char*)iArg1, (char*)iArg3) < 0)
        *((*ppaW)++) = 0;
      else
        *((*ppaW)++) = 1;
    }
  }
}

static inline void _BET_UNKANY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  --(*ppaW);
  --(*ppaW);
  *(*ppaW)++ = C_DATUM_NULL;
}

static inline void _BET_ANYUNK(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  --(*ppaW);
  --(*ppaW);
  *(*ppaW)++ = C_DATUM_NULL;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// INEXPR

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// INEXPR

static inline void _INX_BCDBCD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 bFilterResult = 0;
  i8 iArg1 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL) {
    *((*ppaW)++) = C_DATUM_NULL;
  }
  else {
    i8 iInCount = *(--(*ppaW));
    while(iInCount--) {
      *((*ppaW)++) = iArg1;
      _EQ_BCDBCD(ppaW, aDynamicT);
      bFilterResult = *(--(*ppaW));
      if (bFilterResult) {
        break;
        }
    }
    while(iInCount-- > 0) {
      --(*ppaW);
    }
    *((*ppaW)++) = bFilterResult;
  }
}

static inline void _INX_FLTFLT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 bFilterResult = 0;
  i8 iArg1 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL) {
    *((*ppaW)++) = C_DATUM_NULL;
  }
  else {
    i8 iInCount = *(--(*ppaW));
    while(iInCount--) {
      *((*ppaW)++) = iArg1;
      _EQ_FLTFLT(ppaW, aDynamicT);
      bFilterResult = *(--(*ppaW));
      if (bFilterResult) {
        break;
        }
    }
    while(iInCount-- > 0) {
      --(*ppaW);
    }
    *((*ppaW)++) = bFilterResult;
  }
}

static inline void _INX_INTINT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 bFilterResult = 0;

  i8 iArg1 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL) {
//    while(iInCount-- > 0) {
//      --(*ppaW);
//    }
    *((*ppaW)++) = C_DATUM_NULL;
  }
  else {
    i8 iInCount = *(--(*ppaW));
    while(iInCount--) {
      i8 iArg2 = *(--(*ppaW));
      if (iArg1 == iArg2) {
         bFilterResult = 1;
         break;
      }
    }
    while(iInCount-- > 0) {
      --(*ppaW);
    }
    *((*ppaW)++) = bFilterResult;
  }
}

static inline void _INX_IVDIVD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 bFilterResult = 0;
  union Ivl_T ivlArg1;
  union Ivl_T ivlArg2;

  ivlArg1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      ivlArg1.m_iAsI8 == C_DATUM_NULL) {
    *((*ppaW)++) = 0;
  }
  else {
    i8 iInCount = *(--(*ppaW));
    while(iInCount--) {
      ivlArg2.m_iAsI8 = *(--(*ppaW));
      if ( (ivlArg1.m_xIvl.m_bIsNeg == ivlArg2.m_xIvl.m_bIsNeg) &&
           (ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros == ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros) )
      {
         bFilterResult = 1;
         break;
      }
    }
    while(iInCount-- > 0) {
      ivlArg2.m_iAsI8 = *(--(*ppaW));
    }
    *((*ppaW)++) = bFilterResult;
  }
}


static inline void _INX_IVYIVY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 bFilterResult = 0;
  union Ivl_T ivlArg1;
  union Ivl_T ivlArg2;

  ivlArg1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      ivlArg1.m_iAsI8 == C_DATUM_NULL) {
    *((*ppaW)++) = 0;
  }
  else {
    i8 iInCount = *(--(*ppaW));
    while(iInCount--) {
      ivlArg2.m_iAsI8 = *(--(*ppaW));
      if ( (ivlArg1.m_xIvl.m_bIsNeg == ivlArg2.m_xIvl.m_bIsNeg) &&
           (ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths == ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths) )
      {
         bFilterResult = 1;
         break;
      }
    }
    while(iInCount-- > 0) {
      ivlArg2.m_iAsI8 = *(--(*ppaW));
    }
    *((*ppaW)++) = bFilterResult;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// INEXPR

static inline void _INX_VCHVCH(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 bFilterResult = 0;
  i8 iArg1 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL) {
    *((*ppaW)++) = C_DATUM_NULL;
  }
  else {
    i8 iInCount = *(--(*ppaW));
    while(iInCount--) {
      *((*ppaW)++) = iArg1;
      _EQ_VCHVCH(ppaW, aDynamicT);
      bFilterResult = *(--(*ppaW));
      if (bFilterResult) {
        break;
        }
    }
    while(iInCount-- > 0) {
      --(*ppaW);
    }
    *((*ppaW)++) = bFilterResult;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// DUPS

#define _DUP_FLT(eErrorCode)                                                \
  iArg1 = *(--paW);                                                         \
  *(paW++) = iArg1;                                                         \
  *(paW++) = iArg1;

#define _DUP_BCD(eErrorCode)                                                \
  iArg1 = *(--paW);                                                         \
  *(paW++) = iArg1;                                                         \
  *(paW++) = iArg1;

#define _DUP_DTE(eErrorCode)                                                \
  iArg1 = *(--paW);                                                         \
  *(paW++) = iArg1;                                                         \
  *(paW++) = iArg1;

#define _DUP_INT(eErrorCode)                                                \
  iArg1 = *(--paW);                                                         \
  *(paW++) = iArg1;                                                         \
  *(paW++) = iArg1;

#define _DUP_IVD(eErrorCode)                                                \
  iArg1 = *(--paW);                                                         \
  *(paW++) = iArg1;                                                         \
  *(paW++) = iArg1;

#define _DUP_IVY(eErrorCode)                                                \
  iArg1 = *(--paW);                                                         \
  *(paW++) = iArg1;                                                         \
  *(paW++) = iArg1;

#define _DUP_TME(eErrorCode)                                                \
  iArg1 = *(--paW);                                                         \
  *(paW++) = iArg1;                                                         \
  *(paW++) = iArg1;

#define _DUP_TSP(eErrorCode)                                                \
  iArg1 = *(--paW);                                                         \
  *(paW++) = iArg1;                                                         \
  *(paW++) = iArg1;

#define _DUP_UNK(eErrorCode)                                                \
  iArg1 = *(--paW);                                                         \
  *(paW++) = iArg1;                                                         \
  *(paW++) = iArg1;

#define _DUP_VCH(eErrorCode)                                                \
  iArg1 = *(--paW);                                                         \
  *(paW++) = iArg1;                                                         \
  *(paW++) = iArg1;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// EXTRACT DAY

static inline void _DAY_DTE(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Dte_T xDte1;

  xDte1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xDte1.m_iAsI8 == C_DATUM_NULL) {
      *((*ppaW)++) = C_DATUM_NULL;
  }
  else {
    *((*ppaW)++) = xDte1.m_uDay;
  }
}

static inline void _DAY_TSP(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tsp_T xTsp1;

  xTsp1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTsp1.m_iAsI8 == C_DATUM_NULL) {
      *((*ppaW)++) = C_DATUM_NULL;
  }
  else {
    *((*ppaW)++) = xTsp1.m_uDay;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// EXTRACT HOUR

static inline void _HOUR_TME(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tme_T xTme1;

  xTme1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTme1.m_iAsI8 == C_DATUM_NULL) {
      *((*ppaW)++) = C_DATUM_NULL;
  }
  else {
    *((*ppaW)++) = xTme1.m_uHour;
  }
}

static inline void _HOUR_TSP(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tsp_T xTsp1;

  xTsp1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTsp1.m_iAsI8 == C_DATUM_NULL) {
      *((*ppaW)++) = C_DATUM_NULL;
  }
  else {
    *((*ppaW)++) = xTsp1.m_uHour;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// EXTRACT MINUTE

static inline void _MINUTE_TME(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tme_T xTme1;

  xTme1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTme1.m_iAsI8 == C_DATUM_NULL) {
      *((*ppaW)++) = C_DATUM_NULL;
  }
  else {
    *((*ppaW)++) = xTme1.m_uMinute;
  }
}

static inline void _MINUTE_TSP(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tsp_T xTsp1;

  xTsp1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTsp1.m_iAsI8 == C_DATUM_NULL) {
      *((*ppaW)++) = C_DATUM_NULL;
  }
  else {
    *((*ppaW)++) = xTsp1.m_uMinute;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// EXTRACT SECOND

static inline void _SECOND_TME(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tme_T xTme1;

  xTme1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTme1.m_iAsI8 == C_DATUM_NULL) {
      *((*ppaW)++) = C_DATUM_NULL;
  }
  else {
    *((*ppaW)++) = xTme1.m_uSecond;
  }
}

static inline void _SECOND_TSP(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tsp_T xTsp1;

  xTsp1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTsp1.m_iAsI8 == C_DATUM_NULL) {
      *((*ppaW)++) = C_DATUM_NULL;
  }
  else {
    *((*ppaW)++) = xTsp1.m_uSecond;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// EXTRACT MONTH

static inline void _MONTH_DTE(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Dte_T xDte1;

  xDte1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xDte1.m_iAsI8 == C_DATUM_NULL) {
      *((*ppaW)++) = C_DATUM_NULL;
  }
  else {
    *((*ppaW)++) = xDte1.m_uMonth;
  }
}

static inline void _MONTH_TSP(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tsp_T xTsp1;

  xTsp1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTsp1.m_iAsI8 == C_DATUM_NULL) {
      *((*ppaW)++) = C_DATUM_NULL;
  }
  else {
    *((*ppaW)++) = xTsp1.m_uMonth;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// EXTRACT YEAR

static inline void _YEAR_DTE(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Dte_T xDte1;

  xDte1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xDte1.m_iAsI8 == C_DATUM_NULL) {
      *((*ppaW)++) = C_DATUM_NULL;
  }
  else {
    *((*ppaW)++) = C_YEAR_BASE + xDte1.m_uYearDelta;
  }
}

static inline void _YEAR_TSP(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tsp_T xTsp1;

  xTsp1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTsp1.m_iAsI8 == C_DATUM_NULL) {
      *((*ppaW)++) = C_DATUM_NULL;
  }
  else {
    *((*ppaW)++) = C_YEAR_BASE + xTsp1.m_uYearDelta;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// NULL TESTS
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// do we need separate macros for all datatypes??? Not sure

static inline void _ISNOTNULL(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
   if (aDynamicT->m_iErrorCode != EC_NONE)
    (*(*ppaW)++) = C_DATUM_NULL;
  else
    (*(*ppaW)++) = (iArg1 != C_DATUM_NULL ? 1 : 0);
}

static inline void _ISNULL(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE)
    (*(*ppaW)++) = C_DATUM_NULL;
  else {
    (*(*ppaW)++) = (iArg1 == C_DATUM_NULL ? 1 : 0);
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// STRING FUNCTIONS
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//static inline void _LIKE2(i8** ppaW, struct Dynamic_T* aDynamicT)
//{
//  char* szArg1 = (char*)*(--(*ppaW));
//  char* szArg2 = (char*)*(--(*ppaW));
//  if (aDynamicT->m_iErrorCode != EC_NONE ||
//      szArg1  == (char*)C_DATUM_NULL ||
//      szArg2  == (char*)C_DATUM_NULL) {
//    *((*ppaW)++) = C_DATUM_NULL;
//  }
//  else {
//    *((*ppaW)++) = StrLib_iLike2(szArg2, szArg1);
//  }
//}
//
//static inline void _LIKE3(i8** ppaW, struct Dynamic_T* aDynamicT)
//{
//  char* szArg1 = (char*)*(--(*ppaW));
//  char* szArg2 = (char*)*(--(*ppaW));
//  char* szArg3 = (char*)*(--(*ppaW));
//  if (aDynamicT->m_iErrorCode != EC_NONE ||
//      szArg1  == (char*)C_DATUM_NULL ||
//      szArg2  == (char*)C_DATUM_NULL) {
//    *((*ppaW)++) = C_DATUM_NULL;
//  }
//  else {
//    *((*ppaW)++) = StrLib_iLike3(szArg3, szArg2, *szArg1);
//  }
//}

static inline void _LIKE2_V2(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  char* szArg1 = (char*)*(--(*ppaW));
  char* szArg2 = (char*)*(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      szArg1  == (char*)C_DATUM_NULL ||
      szArg2  == (char*)C_DATUM_NULL) {
    *((*ppaW)++) = C_DATUM_NULL;
  }
  else {
    *((*ppaW)++) = iExtern_StrLibLike2(szArg2, szArg1);
  }
}

static inline void _LIKE3_V2(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  char* szArg1 = (char*)*(--(*ppaW));
  char* szArg2 = (char*)*(--(*ppaW));
  char* szArg3 = (char*)*(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      szArg1  == (char*)C_DATUM_NULL ||
      szArg2  == (char*)C_DATUM_NULL) {
    *((*ppaW)++) = C_DATUM_NULL;
  }
  else {
    *((*ppaW)++) = iExtern_StrLibLike3(szArg3, szArg2, *szArg1);
  }
}

#define _MATCH_STRSTR(str,substr,len,bMatch)                                \
  {                                                                         \
    char* a = str;                                                          \
    char* b = substr;                                                       \
    i8 l = len;                                                             \
                                                                            \
    while(*a != '\0') {                                                     \
      if (*a != *b) {                                                       \
        a++;                                                                \
        continue;                                                           \
      }                                                                     \
                                                                            \
      bMatch = 1;                                                           \
                                                                            \
      char* s = a;                                                          \
      while (l--) {                                                         \
        if (*s++ != *b++) {                                                 \
          bMatch = 0;                                                       \
          break;                                                            \
        }                                                                   \
      }                                                                     \
      if (bMatch) {                                                         \
        break;                                                              \
      }                                                                     \
      b = substr;                                                           \
      l = len;                                                              \
      a++;                                                                  \
    }                                                                       \
                                                                            \
    if ( bMatch) {                                                          \
      str = a+len;                                                          \
    }                                                                       \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// _MATCH_LEADING
// matches given string with leading characters (before %) of the pattern
// e.g pattern is : ABC% ; matches first 3 characters with ABC
// parameters:
// szArg2 : string
// szArg1 : regular expression
// iArg1  : length of the regular expression
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static inline void _MATCH_LEADING(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1  = (i8)*(--(*ppaW)) - 1;
  char* szArg1 = (char*)*(--(*ppaW));
  char* szArg2 = (char*)*(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      szArg1  == (char*)C_DATUM_NULL ||
      szArg2  == (char*)C_DATUM_NULL) {
    *((*ppaW)++) = C_DATUM_NULL;
  }
  else {
    i8 bMatch = 1;
    char* r = szArg1;
    char* s = szArg2;

    while (iArg1--)
    {
      if (*r++ != *s++) {
        bMatch = 0;
        break;
      }
    }
    if (bMatch)
      *((*ppaW)++) = 1;
    else
      *((*ppaW)++) = 0;
  }
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// _MATCH_MIDDLE
// matches given string with middle characters (between 2 %) of the pattern
// e.g pattern is : %ABC% ; matches middle 3 characters with ABC
// parameters:
// szArg2 : string
// szArg1 : regular expression
// iArg1  : length of the regular expression between the two %'s
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static inline void _MATCH_MIDDLE(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1  = (i8)*(--(*ppaW));
  char* szArg1 = (char*)*(--(*ppaW));
  char* szArg2 = (char*)*(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      szArg1  == (char*)C_DATUM_NULL ||
      szArg2  == (char*)C_DATUM_NULL) {
    *((*ppaW)++) = C_DATUM_NULL;
  }
  else {
    i8 bMatch = 0;
    i8 iLen = iArg1;
    char* sub = szArg1 + 1;
    char* str = szArg2;
    _MATCH_STRSTR(str, sub, iLen, bMatch)
    *((*ppaW)++) = bMatch;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// _MATCH_MIDDLE1
// matches given string with middle characters (with one % in middle) of the pattern
// e.g pattern is : %ABC%DEF% ; matches middle 3 characters with ABC
// parameters:
// szArg2 : string
// szArg1 : regular expression
// iArg1  : length of the regular expression between first two %'s
// iArg2  : length of the regular expression between last two %'s
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static inline void _MATCH_MIDDLE1(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1  = (i8)*(--(*ppaW));
  i8 iArg2  = (i8)*(--(*ppaW));
  char* szArg1 = (char*)*(--(*ppaW));
  char* szArg2 = (char*)*(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      szArg1  == (char*)C_DATUM_NULL ||
      szArg2  == (char*)C_DATUM_NULL) {
    *((*ppaW)++) = C_DATUM_NULL;
  }
  else {
    i8 bMatch = 0;
    i8 iLen = iArg2;
    char* sub = szArg1 + 1;
    char* str = szArg2;
    _MATCH_STRSTR(str, sub, iLen, bMatch)
    if (bMatch) {
      bMatch = 0;
      sub = szArg1 + iLen + 2;
      iLen = iArg1;
      _MATCH_STRSTR(str, sub, iLen, bMatch)
    }
    *((*ppaW)++) = bMatch;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// _MATCH_TRAILING
// matches given string with trailing characters (following %) of the pattern
// e.g pattern is : %ABC ; matches last 3 characters with ABC
// parameters:
// szArg2 : string
// szArg1 : regular expression
// iArg1  : length of the regular expression
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static inline void _MATCH_TRAILING(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1  = (i8)*(--(*ppaW));
  char* szArg1 = (char*)*(--(*ppaW));
  char* szArg2 = (char*)*(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      szArg1  == (char*)C_DATUM_NULL ||
      szArg2  == (char*)C_DATUM_NULL) {
    *((*ppaW)++) = C_DATUM_NULL;
  }
  else {
    i8 bMatch = 1;
    char* r = szArg1 + iArg1 - 1;
    char* s = szArg2;

    while (*s)
      s++;
    s--;

    while (*r != '%')
    {
      if (*r-- != *s--) {
        bMatch = 0;
        break;
      }
    }
    if (bMatch)
      *((*ppaW)++) = 1;
    else
      *((*ppaW)++) = 0;
  }
}

static inline void _STR_SUBSTRING2(i8** ppaW, struct Dynamic_T* aDynamicT, char** ppszDstArea, i8* piAreaRemaining)
{
  i8 iStart = *(--(*ppaW));
  char* szArg1 = (char*)*(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iStart  == C_DATUM_NULL ||
      szArg1  == (char*)C_DATUM_NULL) {
    *((*ppaW)++) = C_DATUM_NULL;
  }
  else {
    char* szDstStz;
    if (iStart == 0) {
      iStart = 1;
    }
    i8 nSrcSize = strlen(szArg1);
    i8 nDstSize = 0;
    szDstStz = *ppszDstArea;
    if (nSrcSize >= iStart) {
      char* szSrcStz = (char*)(szArg1 + iStart - 1);
      while (*szSrcStz) {
        *szDstStz++ = *szSrcStz++;
        nDstSize++;
      }
    }
    *szDstStz = '\0';
    *((*ppaW)++) = (i8)*ppszDstArea;
    (*ppszDstArea) += nDstSize + 1;
    (*piAreaRemaining) -= (i8)(nDstSize) + 1;
  }
}

static inline void _STR_SUBSTRING3(i8** ppaW, struct Dynamic_T* aDynamicT, char** ppszDstArea, i8* piAreaRemaining)
{
  i8 iLength = *(--(*ppaW));
  i8 iStart = *(--(*ppaW));
  char* szArg1 = (char*)*(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iLength == C_DATUM_NULL ||
      iStart  == C_DATUM_NULL ||
      szArg1  == (char*)C_DATUM_NULL) {
    *((*ppaW)++) = C_DATUM_NULL;
  }
  else {
    char* szDstStz;
    if (iStart == 0) {
      iStart = 1;
      iLength--;
    }
    char* szSrcStz = (char*)(szArg1 + iStart - 1);
    i8 nSrcSize = strlen(szArg1);
    i8 nDstSize = 0;
    szDstStz = *ppszDstArea;
    if (nSrcSize >= iStart) {
       while (*szSrcStz && iLength-- && nSrcSize > nDstSize) {
         *szDstStz++ = *szSrcStz++;
         nDstSize++;
      }
    }
    *szDstStz = '\0';
    *((*ppaW)++) = (i8)*ppszDstArea;
    (*ppszDstArea) += (i8)(nDstSize) + 1;
    (*piAreaRemaining) -= (i8)(nDstSize) + 1;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  NOT SUPPORTED
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static inline void _OPR_NOTNOT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  --(*ppaW);
  aDynamicT->m_iErrorCode = EC_NOT_IMPLEMENTED;
  *(*ppaW)++ = C_DATUM_NULL;
}

static inline void _OPR_NOT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  aDynamicT->m_iErrorCode = EC_NOT_IMPLEMENTED;
  *(*ppaW)++ = C_DATUM_NULL;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  INVALID TYPES
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static inline void _OPR_INVINV(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  --(*ppaW);
  aDynamicT->m_iErrorCode = EC_INVALID_TYPES;
  *(*ppaW)++ = C_DATUM_NULL;
}

static inline void _OPR_INV(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  aDynamicT->m_iErrorCode = EC_INVALID_TYPES;
  *(*ppaW)++ = C_DATUM_NULL;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
