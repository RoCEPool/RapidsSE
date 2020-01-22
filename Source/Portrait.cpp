/*============================================================================*/
/* Portrait.cpp -- Portrait                                                   */
/*============================================================================*/

#include "StorageEngine.h"


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
                    sCopyMag.szStz(),
                    sMoveMag.szStz(),
                    sSribMag.szStz(),
                    sSetMag.szStz(),
                    sZeroMag.szStz()
                    );
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
