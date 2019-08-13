/*============================================================================*/
/* Portrait.cpp -- Portrait                                                   */
/*============================================================================*/

#include "StorageEngine.h"
#include "AM.hpp"

/*----------------------------------------------------------------------------*/
/* memory monitor profile                                                     */
/*----------------------------------------------------------------------------*/
void Portrait::vMemoryPortrait(s48K& sPortrait)
{
  Statistics* pStats = AM::pStatistics();

  s32 sCopyMag; MagLib::vByteMag(pStats->Memory.m_iCopyTotal,     sCopyMag);
  s32 sMoveMag; MagLib::vByteMag(pStats->Memory.m_iMoveTotal,     sMoveMag);
  s32 sSribMag; MagLib::vByteMag(pStats->Memory.m_iScribbleTotal, sSribMag);
  s32 sSetMag;  MagLib::vByteMag(pStats->Memory.m_iSetTotal,      sSetMag);
  s32 sZeroMag; MagLib::vByteMag(pStats->Memory.m_iZeroTotal,     sZeroMag);

  sPortrait.vSprint("Memory: copy %s move %s scribble %s set %s zero %s\n"_Stz,
                    sCopyMag.aStz(),
                    sMoveMag.aStz(),
                    sSribMag.aStz(),
                    sSetMag.aStz(),
                    sZeroMag.aStz()
                    );
}

/*----------------------------------------------------------------------------*/
/* trace memory statistics                                                    */
/*----------------------------------------------------------------------------*/
void Portrait::vMemoryStatistics(void)
{
  s32 sByteMag;
  
  Statistics* pStats = AM::pStatistics();
  
  MagLib::vByteMag(pStats->Memory.m_iCopyTotal, sByteMag);
  vTrace("MemLib::vCopy() total %s\n"_Stz,sByteMag.aStz());
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (pStats->Memory.m_iaCopy[i_Class]) {
      MagLib::vByteMag(pStats->Memory.m_iaCopy[i_Class], sByteMag);
      vTrace("  %s %s\n"_Stz,aCN[i_Class],sByteMag.aStz());
    }
  }
  
  MagLib::vByteMag(pStats->Memory.m_iCompareTotal, sByteMag);
  vTrace("MemLib::vCompare() total %s\n"_Stz,sByteMag.aStz());
  
  MagLib::vByteMag(pStats->Memory.m_iMoveTotal, sByteMag);
  vTrace("MemLib::vMove() total %s\n"_Stz,sByteMag.aStz());
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (pStats->Memory.m_iaMove[i_Class]) {
      MagLib::vByteMag(pStats->Memory.m_iaMove[i_Class], sByteMag);
      vTrace("  %s %s\n"_Stz,aCN[i_Class],sByteMag.aStz());
    }
  }
  
  MagLib::vByteMag(pStats->Memory.m_iScribbleTotal, sByteMag);
  vTrace("MemLib::vScribble() total %s\n"_Stz,sByteMag.aStz());
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (pStats->Memory.m_iaScribble[i_Class]) {
      MagLib::vByteMag(pStats->Memory.m_iaScribble[i_Class], sByteMag);
      vTrace("  %s %s\n"_Stz,aCN[i_Class],sByteMag.aStz());
    }
  }
  
  MagLib::vByteMag(pStats->Memory.m_iSetTotal, sByteMag);
  vTrace("MemLib::vSet() total %s\n"_Stz,sByteMag.aStz());
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (pStats->Memory.m_iaSet[i_Class]) {
      MagLib::vByteMag(pStats->Memory.m_iaSet[i_Class], sByteMag);
      vTrace("  %s %s\n"_Stz,aCN[i_Class],sByteMag.aStz());
    }
  }
  
  MagLib::vByteMag(pStats->Memory.m_iZeroTotal, sByteMag);
  vTrace("MemLib::vZero() total %s\n"_Stz,sByteMag.aStz());
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (pStats->Memory.m_iaZero[i_Class]) {
      MagLib::vByteMag(pStats->Memory.m_iaZero[i_Class], sByteMag);
      vTrace("  %s %s\n"_Stz,aCN[i_Class],sByteMag.aStz());
    }
  }
  
  i8 iGrandTotal = pStats->Memory.m_iCopyTotal
  + pStats->Memory.m_iMoveTotal
  + pStats->Memory.m_iScribbleTotal
  + pStats->Memory.m_iSetTotal
  + pStats->Memory.m_iZeroTotal
  ;
  MagLib::vByteMag(iGrandTotal, sByteMag);
  vTrace("Grand Total %s\n"_Stz,sByteMag.aStz());
}



/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
