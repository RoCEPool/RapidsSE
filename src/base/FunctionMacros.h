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

#define _ALLOC_VCH(szDstArea)                   \
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
  }                                             \
}

#define _ALLOC_VCH_TUB(azStringDyn)             \
{                                               \
  szArg1 = (char*)*(--paW);                     \
  if ((i8)szArg1 == C_DATUM_NULL) {             \
    *paW++ = C_DATUM_NULL;                      \
  }                                             \
  else {                                        \
    nSrcSize = strlen(szArg1) + 1;              \
    char *d = azStringDyn;                      \
    const char *s = szArg1;                     \
                                                \
	 i8 iBytesToCopy = nSrcSize - 1;              \
   while (iBytesToCopy--)                       \
     *d++ = *s++;                               \
                                                \
    *d = '\0';                                  \
		                                            \
   *paW++ = (i8)azStringDyn;                    \
	 aDynamicT->m_fp_vExtern_TubRowStrSize(aDynamicT->m_pTub,nSrcSize); \
	 azStringDyn = azStringDyn + nSrcSize;        \
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

#define _ASSIGN_TO_TUB(iArgTo)                                              \
  char* szDstArea = *(--paW);                                               \
	amCellAddress = (m8*) &iArgTo;                                            \
	iOffset = - nDifference(szDstArea, amCellAddress);                        \
	iArgTo = iOffset;


#define _ASSIGN_TO_ENTINT(iArgTo)                                           \
  iArg1 = *(--paW);                                                         \
  vExtern_EntWrkAppendElementInt(iArgTo,iArg1);

#define _ASSIGN_TO_ENTSTZ(iArgTo) \
  szArg1 = (char*)*(--paW);                                                 \
  aDynamicT->m_fp_vExtern_EntWrkAppendElementStz(iArgTo,szArg1);


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
#define _BET_BCDBCD(eErrorCode)                                            \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    bcdArg3.m_iAsI8 = *(--paW);                                            \
    if (eErrorCode != EC_NONE ||                                           \
        bcdArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
        bcdArg2.m_iAsI8 ==  C_DATUM_NULL ||                                \
        bcdArg3.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
                                                                           \
        if (bcdArg1.m_iScale != bcdArg2.m_iScale) {                        \
          *paW++ = bcdArg1.m_iAsI8;                                        \
          *paW++ = bcdArg2.m_iAsI8;                                        \
          _RESCALE_BCD_MAX_OF_TWO(eErrorCode)                              \
          bcdArg2.m_iAsI8 = *(--paW);                                      \
          bcdArg1.m_iAsI8 = *(--paW);                                      \
        }                                                                  \
        \
        if (bcdArg1.m_iScale != bcdArg3.m_iScale) {                        \
          *paW++ = bcdArg1.m_iAsI8;                                        \
          *paW++ = bcdArg3.m_iAsI8;                                        \
          _RESCALE_BCD_MAX_OF_TWO(eErrorCode)                              \
          bcdArg3.m_iAsI8 = *(--paW);                                      \
          bcdArg1.m_iAsI8 = *(--paW);                                      \
        }                                                                  \
                                                                           \
        if (bcdArg2.m_iScale != bcdArg3.m_iScale) {                        \
          *paW++ = bcdArg2.m_iAsI8;                                        \
          *paW++ = bcdArg3.m_iAsI8;                                        \
          _RESCALE_BCD_MAX_OF_TWO(eErrorCode)                              \
          bcdArg3.m_iAsI8 = *(--paW);                                      \
          bcdArg2.m_iAsI8 = *(--paW);                                      \
          }                                                                \
                                                                           \
        if (bcdArg1.m_iMantissa > bcdArg2.m_iMantissa) {                   \
          *(paW++) = 0;                                                    \
        }                                                                  \
      else {                                                               \
        if (bcdArg1.m_iMantissa < bcdArg3.m_iMantissa)    {                \
          *(paW++) = 0;                                                    \
        }                                                                  \
        else {                                                             \
          *(paW++) = 1;                                                    \
        }                                                                  \
      }                                                                    \
   }                                                                       \
}

#define _BET_FLTFLT(eErrorCode)                                             \
  {                                                                         \
    fltArg1.m_iAsI8 = *(--paW);                                             \
    fltArg2.m_iAsI8 = *(--paW);                                             \
    fltArg3.m_iAsI8 = *(--paW);                                             \
                                                                            \
    if (eErrorCode != EC_NONE ||                                            \
        fltArg1.m_iAsI8 == C_DATUM_NULL ||                                  \
        fltArg2.m_iAsI8 ==  C_DATUM_NULL ||                                 \
        fltArg3.m_iAsI8 == C_DATUM_NULL) {                                  \
      *(paW++) = C_DATUM_NULL;                                              \
    }                                                                       \
    else {                                                                  \
      if (fltArg1.m_dFlt > fltArg2.m_dFlt) {                                \
        *(paW++) = 0;                                                       \
      }                                                                     \
      else {                                                                \
        if (fltArg1.m_dFlt < fltArg3.m_dFlt)                                \
          *(paW++) = 0;                                                     \
        else                                                                \
          *(paW++) = 1;                                                     \
      }                                                                     \
    }                                                                       \
  }

#define _BET_INTINT(eErrorCode)                                             \
  {                                                                         \
    iArg1 = *(--paW);                                                       \
    iArg2 = *(--paW);                                                       \
    iArg3 = *(--paW);                                                       \
    if (eErrorCode != EC_NONE ||                                            \
        iArg1 == C_DATUM_NULL ||                                            \
        iArg2 ==  C_DATUM_NULL ||                                           \
        iArg3 == C_DATUM_NULL) {                                            \
      *(paW++) = C_DATUM_NULL;                                              \
    }                                                                       \
    else {                                                                  \
      if (iArg1 > iArg2) {                                                  \
        *(paW++) = 0;                                                       \
      }                                                                     \
      else {                                                                \
        if (iArg1 < iArg3)                                                  \
          *(paW++) = 0;                                                     \
        else                                                                \
          *(paW++) = 1;                                                     \
      }                                                                     \
    }                                                                       \
  }

#define _BET_IVDIVD(eErrorCode)                                             \
  {                                                                         \
    ivlArg1.m_iAsI8 = *(--paW);                                             \
    ivlArg2.m_iAsI8 = *(--paW);                                             \
    ivlArg3.m_iAsI8 = *(--paW);                                             \
                                                                            \
    if (eErrorCode != EC_NONE ||                                            \
        ivlArg1.m_iAsI8 == C_DATUM_NULL ||                                  \
        ivlArg2.m_iAsI8 == C_DATUM_NULL) {                                  \
      *paW++ = C_DATUM_NULL;                                                \
    }                                                                       \
    else {                                                                  \
      iArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;                          \
      iArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;                          \
      iArg3 = ivlArg3.m_xIvl.u.m_xIvlDT.m_uMicros;                          \
                                                                            \
      if (ivlArg1.m_xIvl.m_bIsNeg)                                          \
        iArg1 = (-1) * iArg1;                                               \
                                                                            \
      if (ivlArg2.m_xIvl.m_bIsNeg)                                          \
        iArg2 = (-1) * iArg2;                                               \
                                                                            \
      if (ivlArg3.m_xIvl.m_bIsNeg)                                          \
        iArg3 = (-1) * iArg3;                                               \
                                                                            \
      if (iArg1 > iArg2) {                                                  \
        *(paW++) = 0;                                                       \
      }                                                                     \
      else {                                                                \
        if (iArg1 < iArg3)                                                  \
          *(paW++) = 0;                                                     \
        else                                                                \
          *(paW++) = 1;                                                     \
      }                                                                     \
    }                                                                       \
  }

#define _BET_IVYIVY(eErrorCode)                                             \
  {                                                                         \
    ivlArg1.m_iAsI8 = *(--paW);                                             \
    ivlArg2.m_iAsI8 = *(--paW);                                             \
    ivlArg3.m_iAsI8 = *(--paW);                                             \
                                                                            \
    if (eErrorCode != EC_NONE ||                                            \
        ivlArg1.m_iAsI8 == C_DATUM_NULL ||                                  \
        ivlArg2.m_iAsI8 == C_DATUM_NULL) {                                  \
      *paW++ = C_DATUM_NULL;                                                \
    }                                                                       \
    else {                                                                  \
      iArg1 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;                          \
      iArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;                          \
      iArg3 = ivlArg3.m_xIvl.u.m_xIvlYM.m_uMonths;                          \
                                                                            \
      if (ivlArg1.m_xIvl.m_bIsNeg)                                          \
        iArg1 = (-1) * iArg1;                                               \
                                                                            \
      if (ivlArg2.m_xIvl.m_bIsNeg)                                          \
        iArg2 = (-1) * iArg2;                 \
                                                                            \
     if (ivlArg3.m_xIvl.m_bIsNeg)                                           \
       iArg3 = (-1) * iArg3;                  \
                                                                            \
      if (iArg1 > iArg2) {                                                  \
        *(paW++) = 0;                                                       \
      }                                                                     \
      else {                                                                \
        if (iArg1 < iArg3)                                                  \
          *(paW++) = 0;                                                     \
        else                                                                \
          *(paW++) = 1;                                                     \
      }                                                                     \
    }                                                                       \
  }

#define _BET_DTEDTE(eErrorCode)                                             \
  {                                                                         \
    xDte1.m_iAsI8 = *(--paW);                                               \
    xDte2.m_iAsI8 = *(--paW);                                               \
    xDte3.m_iAsI8 = *(--paW);                                               \
    if (eErrorCode != EC_NONE ||                                            \
        xDte1.m_iAsI8 == C_DATUM_NULL ||                                    \
        xDte2.m_iAsI8 ==  C_DATUM_NULL ||                                   \
        xDte3.m_iAsI8 == C_DATUM_NULL) {                                    \
      *(paW++) = C_DATUM_NULL;                                              \
    }                                                                       \
    else {                                                                  \
      if (xDte1.m_iAsI8 > xDte2.m_iAsI8) {                                  \
        *(paW++) = 0;                                                       \
      }                                                                     \
      else {                                                                \
        if (xDte1.m_iAsI8 < xDte3.m_iAsI8)                                  \
          *(paW++) = 0;                                                     \
        else                                                                \
          *(paW++) = 1;                                                     \
      }                                                                     \
    }                                                                       \
  }


#define _BET_TMETME(eErrorCode)                                             \
  {                                                                         \
    xTme1.m_iAsI8 = *(--paW);                                               \
    xTme2.m_iAsI8 = *(--paW);                                               \
    xTme3.m_iAsI8 = *(--paW);                                               \
    if (eErrorCode != EC_NONE ||                                            \
        xTme1.m_iAsI8 == C_DATUM_NULL ||                                    \
        xTme2.m_iAsI8 ==  C_DATUM_NULL ||                                   \
        xTme3.m_iAsI8 == C_DATUM_NULL) {                                    \
      *(paW++) = C_DATUM_NULL;                                              \
    }                                                                       \
    else {                                                                  \
      if (xTme1.m_iAsI8 > xTme2.m_iAsI8) {                                  \
        *(paW++) = 0;                                                       \
      }                                                                     \
      else {                                                                \
        if (xTme1.m_iAsI8 < xTme3.m_iAsI8)                                  \
          *(paW++) = 0;                                                     \
        else                                                                \
          *(paW++) = 1;                                                     \
      }                                                                     \
    }                                                                       \
  }


#define _BET_TSPTSP(eErrorCode)                                             \
  {                                                                         \
    xTsp1.m_iAsI8 = *(--paW);                                               \
    xTsp2.m_iAsI8 = *(--paW);                                               \
    xTsp3.m_iAsI8 = *(--paW);                                               \
    if (eErrorCode != EC_NONE ||                                            \
        xTsp1.m_iAsI8 == C_DATUM_NULL ||                                    \
        xTsp2.m_iAsI8 ==  C_DATUM_NULL ||                                   \
        xTsp3.m_iAsI8 == C_DATUM_NULL) {                                    \
      *(paW++) = C_DATUM_NULL;                                              \
    }                                                                       \
    else {                                                                  \
      if (xTsp1.m_iAsI8 > xTsp2.m_iAsI8) {                                  \
        *(paW++) = 0;                                                       \
      }                                                                     \
      else {                                                                \
        if (xTsp1.m_iAsI8 < xTsp3.m_iAsI8)                                  \
          *(paW++) = 0;                                                     \
        else                                                                \
          *(paW++) = 1;                                                     \
      }                                                                     \
    }                                                                       \
  }

#define _BET_VCHVCH(eErrorCode)                                             \
  {                                                                         \
    iArg1 = *(--paW);                                                       \
    iArg2 = *(--paW);                                                       \
    iArg3 = *(--paW);                                                       \
    if (eErrorCode != EC_NONE ||                                            \
        iArg1 == C_DATUM_NULL ||                                            \
        iArg2 ==  C_DATUM_NULL ||                                           \
        iArg3 == C_DATUM_NULL) {                                            \
      *(paW++) = C_DATUM_NULL;                                              \
    }                                                                       \
    else {                                                                  \
      if (strcmp((char*)iArg1, (char*)iArg2) > 0) {                         \
        *(paW++) = 0;                                                       \
      }                                                                     \
      else {                                                                \
        if (strcmp((char*)iArg1, (char*)iArg3) < 0)                         \
          *(paW++) = 0;                                                     \
        else                                                                \
          *(paW++) = 1;                                                     \
      }                                                                     \
    }                                                                       \
  }

#define _BET_UNKANY(eErrorCode)                                            \
  {                                                                        \
    iArg3 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    iArg1 = *(--paW);                                                      \
    *paW++ = C_DATUM_NULL;                                                 \
  }

#define _BET_ANYUNK(eErrorCode)                                            \
  {                                                                        \
    iArg3 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    iArg1 = *(--paW);                                                      \
    *paW++ = C_DATUM_NULL;                                                 \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// INEXPR

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// INEXPR

#define _INX_BCDBCD(eErrorCode)                                             \
  {                                                                         \
    bFilterResult = 0;                                                      \
    iArg1 = *(--paW);                                                       \
    if (eErrorCode != EC_NONE ||                                            \
        iArg1 == C_DATUM_NULL) {                                            \
      *(paW++) = C_DATUM_NULL;                                              \
    }                                                                       \
    else {                                                                  \
      iInCount = *(--paW);                                                  \
      while(iInCount--) {                                                   \
        *(paW++) = iArg1;                                                   \
        _EQ_BCDBCD(eErrorCode)                                              \
        bFilterResult = *(--paW);                                           \
        if (bFilterResult) {                                                \
          break;                                                            \
          }      \
      }                                                                      \
      while(iInCount-- > 0) {                                               \
        iArg2 = *(--paW);                                                   \
      }                                                                     \
      *(paW++) = bFilterResult;                                             \
    }                                                                       \
  }

#define _INX_FLTFLT(eErrorCode)                                             \
  {                                                                         \
    bFilterResult = 0;                                                      \
    iArg1 = *(--paW);                                                       \
    if (eErrorCode != EC_NONE ||                                            \
        iArg1 == C_DATUM_NULL) {                                            \
      *(paW++) = C_DATUM_NULL;                                              \
    }                                                                       \
    else {                                                                  \
      iInCount = *(--paW);                                                  \
      while(iInCount--) {                                                   \
        *(paW++) = iArg1;                                                   \
        _EQ_FLTFLT(eErrorCode)                                              \
        bFilterResult = *(--paW);                                           \
        if (bFilterResult) {                                                \
          break;                                                            \
          }      \
      }                                                                     \
      while(iInCount-- > 0) {                                               \
        iArg2 = *(--paW);                                                   \
      }                                                                     \
      *(paW++) = bFilterResult;                                             \
    }                                                                       \
  }

#define _INX_INTINT(eErrorCode)                                             \
  {                                                                         \
    bFilterResult = 0;                                                      \
    iArg1 = *(--paW);                                                       \
    if (eErrorCode != EC_NONE ||                                            \
        iArg1 == C_DATUM_NULL) {                                            \
      while(iInCount-- > 0) {                                               \
        iArg2 = *(--paW);                                                   \
      }                                                                     \
      *(paW++) = C_DATUM_NULL;                                              \
    }                                                                       \
    else {                                                                  \
      iInCount = *(--paW);                                                  \
      while(iInCount--) {                                                   \
        iArg2 = *(--paW);                                                   \
        if (iArg1 == iArg2) {                                               \
           bFilterResult = 1;                                               \
           break;                                                           \
        }                                                                   \
      }                                                                     \
      while(iInCount-- > 0) {                                               \
        iArg2 = *(--paW);                                                   \
      }                                                                     \
      *(paW++) = bFilterResult;                                             \
    }                                                                       \
  }

#define _INX_IVDIVD(eErrorCode)                                             \
  {                                                                         \
    bFilterResult = 0;                                                      \
    ivlArg1.m_iAsI8 = *(--paW);                                             \
    if (eErrorCode != EC_NONE ||                                            \
        ivlArg1.m_iAsI8 == C_DATUM_NULL) {                                    \
      *(paW++) = 0;                                                         \
    }                                                                       \
    else {                                                                  \
      iInCount = *(--paW);                                                  \
      while(iInCount--) {                                                   \
        ivlArg2.m_iAsI8 = *(--paW);                                         \
        if ( (ivlArg1.m_xIvl.m_bIsNeg == ivlArg2.m_xIvl.m_bIsNeg) &&        \
             (ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros == ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros) ) \
        {                                                                   \
           bFilterResult = 1;                                               \
           break;                                                           \
        }                                                                   \
      }                                                                     \
      while(iInCount-- > 0) {                                               \
        ivlArg2.m_iAsI8 = *(--paW);                                         \
      }                                                                     \
      *(paW++) = bFilterResult;                                             \
    }                                                                       \
  }


#define _INX_IVYIVY(eErrorCode)                                             \
  {                                                                         \
    bFilterResult = 0;                                                      \
    ivlArg1.m_iAsI8 = *(--paW);                                             \
    if (eErrorCode != EC_NONE ||                                            \
        ivlArg1.m_iAsI8 == C_DATUM_NULL) {                                  \
      *(paW++) = 0;                                                         \
    }                                                                       \
    else {                                                                  \
      iInCount = *(--paW);                                                  \
      while(iInCount--) {                                                   \
        ivlArg2.m_iAsI8 = *(--paW);                                         \
        if ( (ivlArg1.m_xIvl.m_bIsNeg == ivlArg2.m_xIvl.m_bIsNeg) &&        \
             (ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths == ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths) ) \
        {                                                                   \
           bFilterResult = 1;                                               \
           break;                                                           \
        }                                                                   \
      }                                                                     \
      while(iInCount-- > 0) {                                               \
        ivlArg2.m_iAsI8 = *(--paW);                                         \
      }                                                                     \
      *(paW++) = bFilterResult;                                             \
    }                                                                       \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// INEXPR

#define _INX_VCHVCH(eErrorCode)                                             \
  {                                                                         \
    bFilterResult = 0;                                                      \
    iArg1 = *(--paW);                                                       \
    if (eErrorCode != EC_NONE ||                                            \
        iArg1 == C_DATUM_NULL) {                                            \
      *(paW++) = C_DATUM_NULL;                                              \
    }                                                                       \
    else {                                                                  \
      iInCount = *(--paW);                                                  \
      while(iInCount--) {                                                   \
        *(paW++) = iArg1;                                                   \
        _EQ_VCHVCH(eErrorCode)                                              \
        bFilterResult = *(--paW);                                           \
        if (bFilterResult) {                                                \
          break;                                                            \
          }      \
      }                                                                     \
      while(iInCount-- > 0) {                                               \
        iArg2 = *(--paW);                                                   \
      }                                                                     \
      *(paW++) = bFilterResult;                                             \
    }                                                                       \
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

#define _DAY_DTE(eErrorCode)                                                  \
  {                                                                           \
    xDte1.m_iAsI8 = *(--paW);                                                 \
    if (eErrorCode != EC_NONE ||                                              \
        xDte1.m_iAsI8 == C_DATUM_NULL) {                                      \
        *(paW++) = C_DATUM_NULL;                                              \
    }                                                                         \
    else {                                                                    \
      *(paW++) = xDte1.m_uDay;                                                \
    }                                                                         \
  }

#define _DAY_TSP(eErrorCode)                                                  \
  {                                                                           \
    xTsp1.m_iAsI8 = *(--paW);                                                 \
    if (eErrorCode != EC_NONE ||                                              \
        xTsp1.m_iAsI8 == C_DATUM_NULL) {                                      \
        *(paW++) = C_DATUM_NULL;                                              \
    }                                                                         \
    else {                                                                    \
      *(paW++) = xTsp1.m_uDay;                                                \
    }                                                                         \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// EXTRACT HOUR

#define _HOUR_TME(eErrorCode)                                                 \
  {                                                                           \
    xTme1.m_iAsI8 = *(--paW);                                                 \
    if (eErrorCode != EC_NONE ||                                              \
        xTme1.m_iAsI8 == C_DATUM_NULL) {                                      \
        *(paW++) = C_DATUM_NULL;                                              \
    }                                                                         \
    else {                                                                    \
      *(paW++) = xTme1.m_uHour;                                               \
    }                                                                         \
  }

#define _HOUR_TSP(eErrorCode)                                                 \
  {                                                                           \
    xTsp1.m_iAsI8 = *(--paW);                                                 \
    if (eErrorCode != EC_NONE ||                                              \
        xTsp1.m_iAsI8 == C_DATUM_NULL) {                                      \
        *(paW++) = C_DATUM_NULL;                                              \
    }                                                                         \
    else {                                                                    \
      *(paW++) = xTsp1.m_uHour;                                               \
    }                                                                         \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// EXTRACT MINUTE

#define _MINUTE_TME(eErrorCode)                                                 \
  {                                                                           \
    xTme1.m_iAsI8 = *(--paW);                                                 \
    if (eErrorCode != EC_NONE ||                                              \
        xTme1.m_iAsI8 == C_DATUM_NULL) {                                      \
        *(paW++) = C_DATUM_NULL;                                              \
    }                                                                         \
    else {                                                                    \
      *(paW++) = xTme1.m_uMinute;                                             \
    }                                                                         \
  }

#define _MINUTE_TSP(eErrorCode)                                               \
  {                                                                           \
    xTsp1.m_iAsI8 = *(--paW);                                                 \
    if (eErrorCode != EC_NONE ||                                              \
        xTsp1.m_iAsI8 == C_DATUM_NULL) {                                      \
        *(paW++) = C_DATUM_NULL;                                              \
    }                                                                         \
    else {                                                                    \
      *(paW++) = xTsp1.m_uMinute;                                             \
    }                                                                         \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// EXTRACT SECOND

#define _SECOND_TME(eErrorCode)                                               \
  {                                                                           \
    xTme1.m_iAsI8 = *(--paW);                                                 \
    if (eErrorCode != EC_NONE ||                                              \
        xTme1.m_iAsI8 == C_DATUM_NULL) {                                      \
        *(paW++) = C_DATUM_NULL;                                              \
    }                                                                         \
    else {                                                                    \
      *(paW++) = xTme1.m_uSecond;                                             \
    }                                                                         \
  }

#define _SECOND_TSP(eErrorCode)                                               \
  {                                                                           \
    xTsp1.m_iAsI8 = *(--paW);                                                 \
    if (eErrorCode != EC_NONE ||                                              \
        xTsp1.m_iAsI8 == C_DATUM_NULL) {                                      \
        *(paW++) = C_DATUM_NULL;                                              \
    }                                                                         \
    else {                                                                    \
      *(paW++) = xTsp1.m_uSecond;                                             \
    }                                                                         \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// EXTRACT MONTH

#define _MONTH_DTE(eErrorCode)                                                \
  {                                                                           \
    xDte1.m_iAsI8 = *(--paW);                                                 \
    if (eErrorCode != EC_NONE ||                                              \
        xDte1.m_iAsI8 == C_DATUM_NULL) {                                      \
        *(paW++) = C_DATUM_NULL;                                              \
    }                                                                         \
    else {                                                                    \
      *(paW++) = xDte1.m_uMonth;                                              \
    }                                                                         \
  }

#define _MONTH_TSP(eErrorCode)                                                \
  {                                                                           \
    xTsp1.m_iAsI8 = *(--paW);                                                 \
    if (eErrorCode != EC_NONE ||                                              \
        xTsp1.m_iAsI8 == C_DATUM_NULL) {                                      \
        *(paW++) = C_DATUM_NULL;                                              \
    }                                                                         \
    else {                                                                    \
      *(paW++) = xTsp1.m_uMonth;                                              \
    }                                                                         \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// EXTRACT YEAR

#define _YEAR_DTE(eErrorCode)                                                 \
  {                                                                           \
    xDte1.m_iAsI8 = *(--paW);                                                 \
    if (eErrorCode != EC_NONE ||                                              \
        xDte1.m_iAsI8 == C_DATUM_NULL) {                                      \
        *(paW++) = C_DATUM_NULL;                                              \
    }                                                                         \
    else {                                                                    \
      *(paW++) = C_YEAR_BASE + xDte1.m_uYearDelta;                            \
    }                                                                         \
  }

#define _YEAR_TSP(eErrorCode)                                                 \
  {                                                                           \
    xTsp1.m_iAsI8 = *(--paW);                                                 \
    if (eErrorCode != EC_NONE ||                                              \
        xTsp1.m_iAsI8 == C_DATUM_NULL) {                                      \
        *(paW++) = C_DATUM_NULL;                                              \
    }                                                                         \
    else {                                                                    \
      *(paW++) = C_YEAR_BASE + xTsp1.m_uYearDelta;                            \
    }                                                                         \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// NULL TESTS
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// do we need separate macros for all datatypes??? Not sure

#define _ISNOTNULL(eErrorCode)                                               \
  {                                                                          \
  iArg1 = *(--paW);                                                          \
   if (eErrorCode != EC_NONE)                                                \
    (*paW++) = C_DATUM_NULL;                                                 \
  else                                                                       \
    (*paW++) = (iArg1 != C_DATUM_NULL ? 1 : 0);                              \
}

#define _ISNULL(eErrorCode)                                                  \
  iArg1 = *(--paW);                                                          \
  if (eErrorCode != EC_NONE)                                                 \
    (*paW++) = C_DATUM_NULL;                                                 \
  else    {                                                                  \
   (*paW++) = (iArg1 == C_DATUM_NULL ? 1 : 0);                               \
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// STRING FUNCTIONS
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _LIKE2(eErrorCode)                                                  \
  {                                                                         \
    szArg1 = (char*)*(--paW);                                               \
    szArg2 = (char*)*(--paW);                                               \
    if (eErrorCode != EC_NONE ||                                            \
        szArg1  == (char*)C_DATUM_NULL ||                                   \
        szArg2  == (char*)C_DATUM_NULL) {                                   \
      *(paW++) = C_DATUM_NULL;                                              \
    }                                                                       \
    else {                                                                  \
      *(paW++) = StrLib_iLike2(szArg2, szArg1);                             \
    }                                                                       \
  }

#define _LIKE3(eErrorCode)                                                  \
  {                                                                         \
    szArg1 = (char*)*(--paW);                                               \
    szArg2 = (char*)*(--paW);                                               \
    szArg3 = (char*)*(--paW);                                               \
    if (eErrorCode != EC_NONE ||                                            \
        szArg1  == (char*)C_DATUM_NULL ||                                   \
        szArg2  == (char*)C_DATUM_NULL) {                                   \
      *(paW++) = C_DATUM_NULL;                                              \
    }                                                                       \
    else {                                                                  \
      *(paW++) = StrLib_iLike3(szArg3, szArg2, *szArg1);                    \
    }                                                                       \
  }

#define _LIKE2_V2(eErrorCode)                                               \
  {                                                                         \
    szArg1 = (char*)*(--paW);                                               \
    szArg2 = (char*)*(--paW);                                               \
    if (eErrorCode != EC_NONE ||                                            \
        szArg1  == (char*)C_DATUM_NULL ||                                   \
        szArg2  == (char*)C_DATUM_NULL) {                                   \
      *(paW++) = C_DATUM_NULL;                                              \
    }                                                                       \
    else {                                                                  \
      *(paW++) = iExtern_StrLibLike2(szArg2, szArg1);                       \
    }                                                                       \
  }

#define _LIKE3_V2(eErrorCode)                                               \
  {                                                                         \
    szArg1 = (char*)*(--paW);                                               \
    szArg2 = (char*)*(--paW);                                               \
    szArg3 = (char*)*(--paW);                                               \
    if (eErrorCode != EC_NONE ||                                            \
        szArg1  == (char*)C_DATUM_NULL ||                                   \
        szArg2  == (char*)C_DATUM_NULL) {                                   \
      *(paW++) = C_DATUM_NULL;                                              \
    }                                                                       \
    else {                                                                  \
      *(paW++) = iExtern_StrLibLike3(szArg3, szArg2, *szArg1);              \
    }                                                                       \
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

#define _MATCH_LEADING(eErrorCode)                                          \
  {                                                                         \
    iArg1  = (i8)*(--paW) - 1;                                              \
    szArg1 = (char*)*(--paW);                                               \
    szArg2 = (char*)*(--paW);                                               \
    if (eErrorCode != EC_NONE ||                                            \
        szArg1  == (char*)C_DATUM_NULL ||                                   \
        szArg2  == (char*)C_DATUM_NULL) {                                   \
      *(paW++) = C_DATUM_NULL;                                              \
    }                                                                       \
    else {                                                                  \
      i8 bMatch = 1;                                                        \
      char* r = szArg1;                                                     \
      char* s = szArg2;                                                     \
                                                                            \
      while (iArg1--)                                                       \
      {                                                                     \
        if (*r++ != *s++) {                                                 \
          bMatch = 0;                                                       \
          break;                                                            \
        }                                                                   \
      }                                                                     \
      if (bMatch)                                                           \
        *(paW++) = 1;                                                       \
      else                                                                  \
        *(paW++) = 0;                                                       \
   }                                                                        \
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

#define _MATCH_MIDDLE(eErrorCode)                                           \
  {                                                                         \
    iArg1  = (i8)*(--paW);                                                  \
    szArg1 = (char*)*(--paW);                                               \
    szArg2 = (char*)*(--paW);                                               \
    if (eErrorCode != EC_NONE ||                                            \
        szArg1  == (char*)C_DATUM_NULL ||                                   \
        szArg2  == (char*)C_DATUM_NULL) {                                   \
      *(paW++) = C_DATUM_NULL;                                              \
    }                                                                       \
    else {                                                                  \
      i8 bMatch = 0;                                                        \
      i8 iLen = iArg1;                                                      \
      char* sub = szArg1 + 1;                                               \
      char* str = szArg2;                                                   \
      _MATCH_STRSTR(str, sub, iLen, bMatch)                                 \
      *(paW++) = bMatch;                                                    \
    }                                                                       \
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

#define _MATCH_MIDDLE1(eErrorCode)                                          \
  {                                                                         \
    iArg1  = (i8)*(--paW);                                                  \
    iArg2  = (i8)*(--paW);                                                  \
    szArg1 = (char*)*(--paW);                                               \
    szArg2 = (char*)*(--paW);                                               \
    if (eErrorCode != EC_NONE ||                                            \
        szArg1  == (char*)C_DATUM_NULL ||                                   \
        szArg2  == (char*)C_DATUM_NULL) {                                   \
      *(paW++) = C_DATUM_NULL;                                              \
    }                                                                       \
    else {                                                                  \
      i8 bMatch = 0;                                                        \
      i8 iLen = iArg2;                                                      \
      char* sub = szArg1 + 1;                                               \
      char* str = szArg2;                                                   \
      _MATCH_STRSTR(str, sub, iLen, bMatch)                                 \
      if (bMatch) {                                                         \
        bMatch = 0;                                                         \
        sub = szArg1 + iLen + 2;                                            \
        iLen = iArg1;                                                       \
        _MATCH_STRSTR(str, sub, iLen, bMatch)                               \
      }                                                                     \
      *(paW++) = bMatch;                                                    \
    }                                                                       \
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

#define _MATCH_TRAILING(eErrorCode)                                         \
  {                                                                         \
    iArg1  = (i8)*(--paW);                                                  \
    szArg1 = (char*)*(--paW);                                               \
    szArg2 = (char*)*(--paW);                                               \
    if (eErrorCode != EC_NONE ||                                            \
        szArg1  == (char*)C_DATUM_NULL ||                                   \
        szArg2  == (char*)C_DATUM_NULL) {                                   \
      *(paW++) = C_DATUM_NULL;                                              \
    }                                                                       \
    else {                                                                  \
      i8 bMatch = 1;                                                        \
      char* r = szArg1 + iArg1 - 1;                                         \
      char* s = szArg2;                                                     \
                                                                            \
      while (*s)                                                            \
        s++;                                                                \
      s--;                                                                  \
                                                                            \
      while (*r != '%')                                                     \
      {                                                                     \
        if (*r-- != *s--) {                                                 \
          bMatch = 0;                                                       \
          break;                                                            \
        }                                                                   \
      }                                                                     \
      if (bMatch)                                                           \
        *(paW++) = 1;                                                       \
      else                                                                  \
        *(paW++) = 0;                                                       \
    }                                                                       \
  }

#define _STR_SUBSTRING2(szDstArea, eErrorCode)                              \
  {                                                                         \
    i8 iStart = *(--paW);                                                   \
    szArg1 = (char*)*(--paW);                                               \
    if (eErrorCode != EC_NONE ||                                            \
        iStart  == C_DATUM_NULL ||                                          \
        szArg1  == (char*)C_DATUM_NULL) {                                   \
      *(paW++) = C_DATUM_NULL;                                              \
    }                                                                       \
    else {                                                                  \
      char* szDstStz;                                                       \
      if (iStart == 0) {                                                    \
        iStart = 1;                                                         \
      }                                                                     \
      nSrcSize = strlen(szArg1);                                            \
      nDstSize = 0;                                                         \
      szDstStz = szDstArea;                                                 \
      if (nSrcSize >= iStart) {                                             \
        szSrcStz = (char*)(szArg1 + iStart - 1);                            \
        while (*szSrcStz) {                                                 \
          *szDstStz++ = *szSrcStz++;                                        \
          nDstSize++;                                                       \
        }                                                                   \
      }                                                                     \
      *szDstStz = '\0';                                                     \
      *(paW++) = (i8)szDstArea;                                             \
      szDstArea += nDstSize + 1;                                            \
    }                                                                       \
  }

#define _STR_SUBSTRING3(szDstArea, eErrorCode)                              \
  {                                                                         \
    i8 iLength = *(--paW);                                                  \
    i8 iStart = *(--paW);                                                   \
    szArg1 = (char*)*(--paW);                                               \
    if (eErrorCode != EC_NONE ||                                            \
        iLength == C_DATUM_NULL ||                                          \
        iStart  == C_DATUM_NULL ||                                          \
        szArg1  == (char*)C_DATUM_NULL) {                                   \
      *(paW++) = C_DATUM_NULL;                                              \
    }                                                                       \
    else {                                                                  \
      char* szDstStz;                                                       \
      if (iStart == 0) {                                                    \
        iStart = 1;                                                         \
        iLength--;                                                          \
      }                                                                     \
      szSrcStz = (char*)(szArg1 + iStart - 1);                              \
      nSrcSize = strlen(szArg1);                                            \
      nDstSize = 0;                                                         \
      szDstStz = szDstArea;                                                 \
      if (nSrcSize >= iStart) {                                             \
         while (*szSrcStz && iLength-- && nSrcSize > nDstSize) {            \
           *szDstStz++ = *szSrcStz++;                                       \
           nDstSize++;                                                      \
        }                                                                   \
      }                                                                     \
      *szDstStz = '\0';                                                     \
      *(paW++) = (i8)szDstArea;                                             \
      szDstArea += (i8)(nDstSize) + 1;                                      \
    }                                                                       \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  NOT SUPPORTED
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define _OPR_NOTNOT(eErrorCode)                                            \
  {                                                                        \
    iArg2 = *(--paW);                                                      \
    iArg1 = *(--paW);                                                      \
    eErrorCode = EC_NOT_IMPLEMENTED;                                       \
    *paW++ = C_DATUM_NULL;                                                 \
  }

#define _OPR_NOT(eErrorCode)                                               \
  {                                                                        \
    iArg2 = *(--paW);                                                      \
    eErrorCode = EC_NOT_IMPLEMENTED;                                       \
    *paW++ = C_DATUM_NULL;                                                 \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  INVALID TYPES
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define _OPR_INVINV(eErrorCode)                                            \
  {                                                                        \
    iArg2 = *(--paW);                                                      \
    iArg1 = *(--paW);                                                      \
    eErrorCode = EC_INVALID_TYPES;                                         \
    *paW++ = C_DATUM_NULL;                                                 \
  }

#define _OPR_INV(eErrorCode)                                               \
  {                                                                        \
    iArg2 = *(--paW);                                                      \
    eErrorCode = EC_INVALID_TYPES;                                         \
    *paW++ = C_DATUM_NULL;                                                 \
  }

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
