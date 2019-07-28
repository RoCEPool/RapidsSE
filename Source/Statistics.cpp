/*============================================================================*/
/* Statistics.cpp -- Statistics                                               */
/*============================================================================*/

#include "Correctness.h"
#include "AM.hpp"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Statistics* Correctness::_pStatistics(void* aAddr)
{
  Statistics* pStatistics = reinterpret_cast<Statistics*>(aAddr);
  
  RASSERT_TYPE(pStatistics,CN_STATISTICS);
  
  return pStatistics;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Statistics::Statistics(void)
: Object(CN_STATISTICS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STATISTICS,_FOOTER_);

  _checkInitialised_(CN_STATISTICS);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_STATISTICS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Statistics::~Statistics(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STATISTICS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* memory monitor profile                                                     */
/*----------------------------------------------------------------------------*/
void Statistics::vMemoryPortrait(s48K& sPortrait)
{
  Statistics* pStats = StorageEngine::AM::pStatistics();

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
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Statistics::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STATISTICS);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
