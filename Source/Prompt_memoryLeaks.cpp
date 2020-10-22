/*============================================================================*/
/* Prompt_memoryLeaks.cpp -- memory leaks                                     */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_memoryLeaks (
  PF_MEMORYLEAKS,              // prompt function
  "Prompt_memoryLeaks"_Stz,    // name of the prompt function
  "memory leaks"_Stz,          // comment about the function
  "memoryLeaks"_Stz,           // the prompt command
  nullptr,                     // the second word
  bPrompt_memoryLeaks,         // prompt function
  true,                        // enabled

  true,                        // visible
  "memoryLeaks"_Stz,           // help
  ""_Stz                       // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_memoryLeaks(SePrompt* pPrompt)
{
  i8 iRowsProcessed = 0;
  
  if ( ! pPrompt->bTrackingMemory() ) {
    vTrace("error: Memory not being tracked; Run trackMemory;\n"_Stz);
    return true;
  }
  
  // Wait for all jobs in the system to complete
  // Note: the tracking of memory works with an assumption that
  //  whatever was started after trackingMemory has completed
  Api::vWaitForAllJobsToComplete();
  
  bool bFatal = false;
  
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  i8 iNumTokens = pInpTknVec->nItems();
  
  if (2 == iNumTokens && pInpTknVec->pGet(1)->bSame("fatal"_Stz))
    bFatal = true;
  
  if (AM::bMetadata()) {
    vTrace("metadata: (objectName VARCHAR, count INTEGER)\n"_Stz);
  }
  
  ObjectsInPool* pObjectsInPool = pPrompt->pObjectsInPool();
  ObjectsInPool* pSnapshotObjects = pPrompt->pSnapshotObjects();
  
  // NOTE: Need to wait for job to complete and cleanup
  // Loop C_ATTEMPTS times to wait for memory leaks to go away
  
  for (i8 iAttempt = 0; iAttempt < C_ATTEMPTS; iAttempt++) {
    
    PoolHelper::vObjectSnapshot(pSnapshotObjects);
    
    for (i8 i_Object = 0 ; i_Object < CN_ENUMS; i_Object++) {
      
      eCN eClass = (eCN)i_Object;
      
      if (eClass == CN_FREEBUFFER || eClass == CN_HEAPBUFFER)
        continue;
      
      i8 nCount = pSnapshotObjects->m_nObjects[eClass] -
        pObjectsInPool->m_nObjects[eClass];
      
      if ( nCount == 0 )
        continue;
      
      // we have a memory leak
      iRowsProcessed++;
      
      // lets retry
      break;
    }
    
    // no memory leaks ?
    if ( iRowsProcessed == 0 )
      goto L_REPORT;
    
    // we have memory leaks; sleep and try again!
    AM::vNapMsec(100,CN_SEPROMPT);
  }

  // attempted C_ATTEMPTS to see if memory leaks settle down
  // time to report
  iRowsProcessed = 0;
  PoolHelper::vObjectSnapshot(pSnapshotObjects);
  
  for (i8 i_Object = 0 ; i_Object < CN_ENUMS; i_Object++) {
    
    eCN eClass = (eCN)i_Object;
    
    if (eClass == CN_FREEBUFFER || eClass == CN_HEAPBUFFER)
      continue;
    
    i8 nCount = pSnapshotObjects->m_nObjects[eClass] -
    pObjectsInPool->m_nObjects[eClass];
    if ( nCount == 0 )
      continue;
    
    vTrace("row: ('%s',  %lld)\n"_Stz,
           aCN[eClass],
           nCount
           );
    iRowsProcessed++;
  }
  

L_REPORT:
  vTrace("success: %lld rows affected\n"_Stz, iRowsProcessed);
  
  if (iRowsProcessed && bFatal)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "fatal: memory leaks detected"_Stz);
  
  if (! iRowsProcessed)
    vTrace("No memory leaks detected\n"_Stz);
  
  pPrompt->vTrackingMemory(false);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
