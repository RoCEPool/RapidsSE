/*============================================================================*/
/* LockMetrics.cpp -- Lock Metrics                                            */
/*============================================================================*/

#include "Metrics.h"

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sample current lock metrics                                                */
/*----------------------------------------------------------------------------*/
InfoSet* LockMetrics::pSample(StreamDef* pStreamDef)
{
  _static_
  
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
  
  return pInfoSet;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
