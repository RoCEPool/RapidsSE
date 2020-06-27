/*============================================================================*/
/* PromptHelper.cpp -- a set of prompt helper functions                       */
/*============================================================================*/

#include "Prompt.h"

/*----------------------------------------------------------------------------*/
/* process command flags                                                      */
/*----------------------------------------------------------------------------*/
void PromptHelper::vEnumFlags(EnumFlags* pFlags,
                              Stz* szGroupNm,
                              Stz* szaEnumNm[],
                              i8   iEnumLimit,
                              TokenVec* pTknVec)
{
  Stz* aTkn1 = nullptr;
  Stz* aTkn2 = nullptr;
  
  i8 nTokens = pTknVec->nTokens();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark<Group>
  
  if (nTokens == 1) {
    
    for (i8 i_Enum = 1; i_Enum < iEnumLimit; i_Enum++)
      if (pFlags->bEnum((eCN)i_Enum))
        vTrace("%s : %s\n"_Stz,
               szaEnumNm[i_Enum],
               "on"_Stz);
    
    vTrace("Mark%sAll is currently : %s\n"_Stz,
           szGroupNm,
           pFlags->bAll() ? "on"_Stz : "off"_Stz);
    
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark<Group> ?
  // mark<Group> all
  // mark<Group> <name>

  if (nTokens == 2) {
    aTkn1 = pTknVec->pGet(1)->szChars();
    
    if (aTkn1->bSame("?"_Stz)) {
      for (i8 i_Enum = 1; i_Enum < iEnumLimit; i_Enum++)
          vTrace("  %s\n"_Stz,szaEnumNm[i_Enum]);
      return;
    }
    
    if (aTkn1->bSame("all"_Stz)) {
      vTrace("Mark%sAll is currently : %s\n"_Stz,
             szGroupNm,
             pFlags->bAll() ? "on"_Stz : "off"_Stz);
      return;
    }
    
    for (i8 i_Enum = 1; i_Enum < iEnumLimit; i_Enum++)
      if (aTkn1->bSame(szaEnumNm[i_Enum]))
        vTrace("%s : %s\n"_Stz,
               szaEnumNm[i_Enum],
               pFlags->bEnum(i_Enum) ? "on"_Stz : "off"_Stz);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark<Group> all on
  // mark<Group> all off
  // mark<Group> <name> on
  // mark<Group> <name> off

  if (nTokens == 3) {
    aTkn1 = pTknVec->pGet(1)->szChars();
    aTkn2 = pTknVec->pGet(2)->szChars();
    
    if (aTkn1->bSame("all"_Stz)) {
      if (aTkn2->bIsSomeTrue())
        pFlags->vAll(true);
      else
        if (aTkn2->bIsSomeFalse())
          pFlags->vAll(false);
      return;
    }
    
    if (aTkn2->bIsSomeTrue()) {
      for (i8 i_Enum = 1; i_Enum < iEnumLimit; i_Enum++)
        if (aTkn1->bSame(szaEnumNm[i_Enum]))
          pFlags->vEnum(i_Enum,true);
      return;
    }
    
    if (aTkn2->bIsSomeFalse()) {
      for (i8 i_Enum = 1; i_Enum < iEnumLimit; i_Enum++)
        if (aTkn1->bSame(szaEnumNm[i_Enum]))
          pFlags->vEnum(i_Enum,false);
      return;
    }
    
    return;
  }

}

/*----------------------------------------------------------------------------*/
/* trace memory statistics                                                    */
/*----------------------------------------------------------------------------*/
void PromptHelper::vMemoryStatistics(void)
{
  s32 sByteMag;
  
  Statistics* pStats = AM::pStatistics();
  
  MagLib::vByteMag(pStats->Memory.m_iCopyTotal, sByteMag);
  vTrace("MemLib::vCopy() total %s\n"_Stz,sByteMag.szStz());
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (pStats->Memory.m_iaCopy[i_Class]) {
      MagLib::vByteMag(pStats->Memory.m_iaCopy[i_Class], sByteMag);
      vTrace("  %s %s\n"_Stz,aCN[i_Class],sByteMag.szStz());
    }
  }
  
  MagLib::vByteMag(pStats->Memory.m_iCompareTotal, sByteMag);
  vTrace("MemLib::vCompare() total %s\n"_Stz,sByteMag.szStz());
  
  MagLib::vByteMag(pStats->Memory.m_iMoveTotal, sByteMag);
  vTrace("MemLib::vMove() total %s\n"_Stz,sByteMag.szStz());
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (pStats->Memory.m_iaMove[i_Class]) {
      MagLib::vByteMag(pStats->Memory.m_iaMove[i_Class], sByteMag);
      vTrace("  %s %s\n"_Stz,aCN[i_Class],sByteMag.szStz());
    }
  }
  
  MagLib::vByteMag(pStats->Memory.m_iScribbleTotal, sByteMag);
  vTrace("MemLib::vScribble() total %s\n"_Stz,sByteMag.szStz());
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (pStats->Memory.m_iaScribble[i_Class]) {
      MagLib::vByteMag(pStats->Memory.m_iaScribble[i_Class], sByteMag);
      vTrace("  %s %s\n"_Stz,aCN[i_Class],sByteMag.szStz());
    }
  }
  
  MagLib::vByteMag(pStats->Memory.m_iSetTotal, sByteMag);
  vTrace("MemLib::vSet() total %s\n"_Stz,sByteMag.szStz());
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (pStats->Memory.m_iaSet[i_Class]) {
      MagLib::vByteMag(pStats->Memory.m_iaSet[i_Class], sByteMag);
      vTrace("  %s %s\n"_Stz,aCN[i_Class],sByteMag.szStz());
    }
  }
  
  MagLib::vByteMag(pStats->Memory.m_iZeroTotal, sByteMag);
  vTrace("MemLib::vZero() total %s\n"_Stz,sByteMag.szStz());
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (pStats->Memory.m_iaZero[i_Class]) {
      MagLib::vByteMag(pStats->Memory.m_iaZero[i_Class], sByteMag);
      vTrace("  %s %s\n"_Stz,aCN[i_Class],sByteMag.szStz());
    }
  }
  
  i8 iGrandTotal = pStats->Memory.m_iCopyTotal
  + pStats->Memory.m_iMoveTotal
  + pStats->Memory.m_iScribbleTotal
  + pStats->Memory.m_iSetTotal
  + pStats->Memory.m_iZeroTotal
  ;
  MagLib::vByteMag(iGrandTotal, sByteMag);
  vTrace("Grand Total %s\n"_Stz,sByteMag.szStz());
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
