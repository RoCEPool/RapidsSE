/*============================================================================*/
/* LIB_interface.cpp -- RLU Lib Interface                                     */
/*============================================================================*/

#include "Expressions.h"

#include "LIB_interface.h"


/*----------------------------------------------------------------------------*/
/* breakpoint                                                                 */
/*----------------------------------------------------------------------------*/
void vLib_ASSERT(void* pLU, void* aArg)
{
  _static_
  
  RASSERT(aArg);
}

/*----------------------------------------------------------------------------*/
/* breakpoint                                                                 */
/*----------------------------------------------------------------------------*/
void vLib_BREAKPOINT(void* pLU)
{
  _static_
  
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* push current date                                                          */
/*----------------------------------------------------------------------------*/
i8 iLib_DTE_NOW(void* pLU)
{
  _static_
  
  Now oNow;
  Dtm wDte;

  wDte.m_wDte.m_uDay = oNow.m_wTsp.m_uDay;
  wDte.m_wDte.m_uMonth = oNow.m_wTsp.m_uMonth;
  wDte.m_wDte.m_uYearDelta = oNow.m_wTsp.m_uYearDelta;
  wDte.m_wDte.m_uSpare = 0;
  
  i8 iRes = wDte.iAsI8();

  return iRes;
}

/*----------------------------------------------------------------------------*/
/* format given date                                                          */
/*----------------------------------------------------------------------------*/
Stz* szLib_DTE_PRINT(void* pLU, i8 iTopA)
{
  _static_
  
  SP_Dte spDate;

  LogicUnit* pLogicUnit = _pLogicUnit(pLU);
  
  struct RluCommon* aRluCommon = & pLogicUnit->m_xRluCommon;

  StrLib::vCopy(aRluCommon->m_caStrWrk,(char*)spDate.sz(iTopA));
  
  return (Stz*)aRluCommon->m_caStrWrk;
}

/*----------------------------------------------------------------------------*/
/* return year of date                                                        */
/*----------------------------------------------------------------------------*/
i8 iLib_DTE_YEAR(void* pLU, i8 iArg)
{
  _static_
  
  Dtm wDte;

  wDte = iArg;
 
  i8 iRes = wDte.m_wDte.m_uYearDelta + C_YEAR_BASE;

  return iRes;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* // error marker                                                            */
/*----------------------------------------------------------------------------*/
void vLib_ERROR(void* pLU, Stz* szErr)
{
  _static_
  
  vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,szErr);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add hash of top int to running hash                                        */
/*----------------------------------------------------------------------------*/
void vLib_HSH_ADD_HASHI(void* pLU, i8 iTopA)
{
  _static_

  LogicUnit* pLogicUnit = _pLogicUnit(pLU);
  struct RluCommon* aRluCommon = & pLogicUnit->m_xRluCommon;

  i8 iRes = HshLib::iHash((byte*)&iTopA,sizeof(w8));

  aRluCommon->m_iHash += iRes;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* calculate m_i_Ptn using running m_iHash                                    */
/*----------------------------------------------------------------------------*/
void vLib_PTN_CALC(void* pLU)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(pLU);
  struct RluCommon* aRluCommon = & pLogicUnit->m_xRluCommon;

  aRluCommon->m_i_Ptn =
    PtnMap::tHashToPtnNo(aRluCommon->m_iHash,aRluCommon->m_nPtns);
}

/*----------------------------------------------------------------------------*/
/* mix top into running m_iHash                                               */
/*----------------------------------------------------------------------------*/
void vLib_PTN_RUNNING(void* pLU, i8 iTopA)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(pLU);
  struct RluCommon* aRluCommon = & pLogicUnit->m_xRluCommon;

  PtnMap::vRunningHash(&aRluCommon->m_iHash,iTopA);
}

/*----------------------------------------------------------------------------*/
/* mix top into running m_iHash                                               */
/*----------------------------------------------------------------------------*/
void vLib_SET_LOCAL(void* pLU, i8 iArg, i8 iTopA)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(pLU);
  struct RluCommon* aRluCommon = & pLogicUnit->m_xRluCommon;
  aRluCommon->m_iaLocals[iArg] = iTopA;
}

/*----------------------------------------------------------------------------*/
/* get local                                                                  */
/*----------------------------------------------------------------------------*/
i8 iLib_GET_LOCAL(void* pLU, i8 iArg)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(pLU);
  struct RluCommon* aRluCommon = & pLogicUnit->m_xRluCommon;
  
  i8 iLcl = aRluCommon->m_iaLocals[iArg];
  
  return iLcl;
}

/*----------------------------------------------------------------------------*/
/* get local string                                                           */
/*----------------------------------------------------------------------------*/
char* sLib_GET_LOCALS(void* pLU, i8 iArg)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(pLU);
  struct RluCommon* aRluCommon = & pLogicUnit->m_xRluCommon;
  //return (char*)&aRluCommon->m_iaLocals[iArg];
  
  char* azLcl = (char*)aRluCommon->m_iaLocals[iArg];
  
  return azLcl;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* push random number 0..arg-1                                                */
/*----------------------------------------------------------------------------*/
i8 iLib_RANDOM(void* pLU, i8 iArg)
{
  _static_
  
  i8 iRes = RndLib::iInteger(iArg);

  return iRes;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sleep for top seconds                                                      */
/*----------------------------------------------------------------------------*/
void vLib_SLEEP(void* pLU, i8 iTopA)
{
  _static_
  
  sleep((u4)iTopA);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compare strings                                                            */
/*----------------------------------------------------------------------------*/
i8 iLib_STR_COMPARE(void* pLU, Stz* szTopA, Stz* szTopB)
{
  _static_
  
  i8 iRes = StrLib::iCompare((char*)szTopA,(char*)szTopB);
  
  return iRes;
}

/*----------------------------------------------------------------------------*/
/* formatted print                                                            */
/*----------------------------------------------------------------------------*/
void vLib_PRINTF(void* pLU, Stz* szFmt, i8** aaiDstk)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(pLU);
  
  RluCommon* aRluCommon = &pLogicUnit->m_xRluCommon;
  
  i8* aiDstk = *aaiDstk;
  
  char caFormatted[C_1KB];
  
  char* acFmt = szFmt->azChar();
  char* acDst = caFormatted;

  while (*acFmt) {
  
    // test for ordinary character
    if ('%' != *acFmt) {
      *acDst++ = *acFmt++;
      continue;
    }
        
    // we have a '%'
    acFmt++;
    
    // test for the %% case
    if ('%' == *acFmt) {
      *acDst++ = *acFmt++;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // we have a %[width]v field
    
    i8 nWidth = 0;

    // record where output for this value began
    char* acFieldBegan = acDst;
    
    // do we have a %v of the form: %widthV ?
    if (StrLib::bIsDigit( * acFmt )) {
      i8 nDigits = 0;
      StrLib::bExtractInteger(&acFmt,&nWidth,nDigits);
      ASSERT(nDigits);
    }
        
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // should be %d or %s
    
    ASSERT('d' == *acFmt || 'i' == *acFmt || 's' == *acFmt);
    
    if ('i' == *acFmt) {
      
      i8 iInt = *aiDstk--;
      
      char caValue[32];
      StrLib::wToString(iInt,caValue);
      
      for (char* pv = caValue; *pv; )
        *acDst++ = *pv++;
      
      acFmt++;
    }
    
    else
    if ('d' == *acFmt) {
      
      SP_Dte spDate;

      i8 iDte = *aiDstk--;
      
      StrLib::vAppendString(&acDst,(char*)spDate.sz(iDte));
      
      acFmt++;
    }
    
    else
    if ('s' == *acFmt) {
     
      i8 nSize = *aiDstk--;
      Stz* szStr = (Stz*) (aiDstk-(nSize/8 + 1));

      StrLib::vAppendString(&acDst,(char*)szStr);

      aiDstk -= (nSize/8) + 2;

      acFmt++;
    }
        
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // handle optional field width case
    
    // test for field width
    if (! nWidth)
      continue;
    
    i8 nDiff = nDifference(acDst,acFieldBegan);
    
    if (nDiff >= nWidth)
      continue;
    
    i8 nPad = nWidth - nDiff;
    StrLib::vPad(acDst,nPad,' ');
    acDst += nPad;
  }
  
  // final termination
  *acDst = '\0';
  
  vTrace("[Q%lld.%lld] %s"_Stz,
         aRluCommon->m_iSql,
         aRluCommon->m_i_TablePtn,
         caFormatted);
  
  // publish
  *aaiDstk = aiDstk;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
