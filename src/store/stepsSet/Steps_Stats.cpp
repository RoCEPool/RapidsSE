/*============================================================================*/
/* Steps_Stats.cpp - stats step group                                         */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "LogicUnit.hpp"

#include "Stats.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Stats(void)
{
  vAddStep("stats[].delete"_Stz,         &Step::vStatsDelete);
  vAddStep("stats[].microsecs"_Stz,      &Step::vStatsMicrosecs);
  vAddStep("stats[].millisecs"_Stz,      &Step::vStatsMillisecs);
  vAddStep("stats[].nanosecs"_Stz,       &Step::vStatsNanosecs);
  vAddStep("stats[].new"_Stz,            &Step::vStatsNew);
  vAddStep("stats[].secs"_Stz,           &Step::vStatsSecs);
  vAddStep("stats[].startTimer"_Stz,     &Step::vStatsStartTimer);
  vAddStep("stats[].updateTimer"_Stz,    &Step::vStatsUpdateTimer);
}

/*----------------------------------------------------------------------------*/
/* stats[].delete -- delete stats                                             */
/*----------------------------------------------------------------------------*/
void Step::vStatsDelete(void)
{
  ENTRY();
  
  Stats* pStats = _pStats(*m_p_TaskLogicUnit->m_xpStats);
  ASSERT(CN_STATS == pStats->eGetClassNo());
  
  Object* pStatsObject = reinterpret_cast<Object*>(pStats);
  
  delete pStats;
  
  Heaps::vFree(pStatsObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* stats[].microsecs -- get elapsed duration in microseconds                  */
/*----------------------------------------------------------------------------*/
void Step::vStatsMicrosecs(void)
{
  ENTRY();
  
  Stats* pStats = _pStats(*m_p_TaskLogicUnit->m_xpStats);
  ASSERT(CN_STATS == pStats->eGetClassNo());
  
  i8 iMicroseconds = pStats->iMicroseconds();
  
  m_pDataStack->vPushInt(iMicroseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* stats[].millisecs -- get elapsed duration in milliseconds                  */
/*----------------------------------------------------------------------------*/
void Step::vStatsMillisecs(void)
{
  ENTRY();
  
  Stats* pStats = _pStats(*m_p_TaskLogicUnit->m_xpStats);
  ASSERT(CN_STATS == pStats->eGetClassNo());
  
  i8 iMilliseconds = pStats->iMilliseconds();
  
  m_pDataStack->vPushInt(iMilliseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* stats[].nanosecs -- get elapsed duration in nanoseconds                    */
/*----------------------------------------------------------------------------*/
void Step::vStatsNanosecs(void)
{
  ENTRY();
  
  Stats* pStats = _pStats(*m_p_TaskLogicUnit->m_xpStats);
  ASSERT(CN_STATS == pStats->eGetClassNo());
  
  i8 iNanoseconds = pStats->iNanoseconds();
  
  m_pDataStack->vPushInt(iNanoseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* stats[].new -- new stats                                                   */
/*----------------------------------------------------------------------------*/
void Step::vStatsNew(void)
{
  ENTRY();

  m_p_TaskLogicUnit->m_xpStats = new Stats;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* stats[].secs -- get elapsed duration in seconds                            */
/*----------------------------------------------------------------------------*/
void Step::vStatsSecs(void)
{
  ENTRY();
  
  Stats* pStats = _pStats(new Stats);
  ASSERT(CN_STATS == pStats->eGetClassNo());
  
  i8 iSeconds = pStats->iSeconds();
  
  m_pDataStack->vPushInt(iSeconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* stats[].startTimer -- start stats timer                                    */
/*----------------------------------------------------------------------------*/
void Step::vStatsStartTimer(void)
{
  ENTRY();
  
  Stats* pStats = _pStats(*m_p_TaskLogicUnit->m_xpStats);
  ASSERT(CN_STATS == pStats->eGetClassNo());
  
  pStats->vStartTimer();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* stats[].updateTimer -- update stats timer                                  */
/*----------------------------------------------------------------------------*/
void Step::vStatsUpdateTimer(void)
{
  ENTRY();
  
  Stats* pStats = _pStats(*m_p_TaskLogicUnit->m_xpStats);
  ASSERT(CN_STATS == pStats->eGetClassNo());
  
  pStats->vUpdateTimer();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
