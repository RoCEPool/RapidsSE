/*============================================================================*/
/* Steps_Stats.cpp - Stats step group                                         */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Stats(void)
{
  vAddStep("stats::lockStats"_Stz,   &Step::vStatsLockStats);
}

/*----------------------------------------------------------------------------*/
/* stats::lockStats -- sample the lock stats                                  */
/*----------------------------------------------------------------------------*/
void Step::vStatsLockStats(void)
{
  ENTRY(); // ENTRY("vStatsLockStats"_Stz);

  StreamDef* pStreamDef = *m_pRegSet->m_xpStreamDef;
  
  LockTracker* pLockTracker = AM::pLockTracker();
  
  pLockTracker->vReport();
  
  HV_Row hvRowSingle(pStreamDef,C_ALL);
    
  hvRowSingle->vSet(C_ARGS_BEGIN,
                    
                    0,       // node no
                    0,       // take no
                    
                    0,       // lock no
                    ""_Stz,  // lock name
                    
                    0,       // read grab
                    0,       // read collision
                    0,       // read percent
                    
                    0,       // write grab
                    0,       // write collision
                    0,       // write percent
                    
                    C_ARGS_END);
   
  InfoSet* pInfoSet = new InfoSet;
  Row* aLockStatRow = pInfoSet->aAllotRow();
  
  delete pInfoSet;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
