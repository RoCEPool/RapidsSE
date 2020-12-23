/*============================================================================*/
/* Prompt_memoryLeaks.cpp -- memory leaks                                     */
/*============================================================================*/

#include "StorageEngine.h"

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
  i8 nLeaksDetected = 0;
  
  // force memory leaks test
//  Text* pText_leak = new Text;
//  vTrace("Text_leak : %p\n"_Stz,pText_leak);

  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  i8 iNumTokens = pInpTknVec->nItems();

  bool bFatal = false;
  if (2 == iNumTokens && pInpTknVec->pGet(1)->bSame("fatal"_Stz))
    bFatal = true;
  
  // are we tracking memory?
  if ( ! pPrompt->bTrackingMemory() ) {
    vTrace("error: Memory not being tracked; Run trackMemory;\n"_Stz);
    return true;
  }
  
  if (AM::bMetadata())
    vTrace("metadata: (objectName VARCHAR, count INTEGER)\n"_Stz);


  if (2 == iNumTokens && pInpTknVec->pGet(1)->bSame("dump"_Stz)) {
    bPrompt_memoryLeaks_dump(pPrompt);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // wait for all jobs in the system to complete
  
  // note: the tracking of memory works with an assumption that
  //       whatever was started after trackingMemory has completed
  Api::vSpinTillAllJobsCompleted();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  ObjectSnapshot* pPoolSnapshot_then = pPrompt->pSnapshotAtTrackingMemory();
  ObjectSnapshot* pPoolSnapshot_curr = pPrompt->pSnapshotAtMemoryLeaks();
  
  // NOTE: Need to wait for job to complete and cleanup
  // Loop C_ATTEMPTS times to wait for memory leaks to go away
  
  for (i8 iAttempt = 0; iAttempt < C_ATTEMPTS; iAttempt++) {

    nLeaksDetected = 0;
    pPoolSnapshot_curr->vTakeSnapshot();
    
    for (i8 i_Class = 0 ; i_Class < CN_ENUMS; i_Class++) {
      
      eCN eClass = (eCN)i_Class;
      
      if (eClass == CN_FREEBUFFER || eClass == CN_HEAPBUFFER)
        continue;
      
      i8 nDelta = pPoolSnapshot_curr->m_nObjects[eClass] -
                    pPoolSnapshot_then->m_nObjects[eClass];
      
//      vTrace("LEAKTEST: %s %lld %lld %lld\n"_Stz,aCN[eClass],
//             pPoolSnapshot_curr->m_nObjects[eClass],
//             pPoolSnapshot_then->m_nObjects[eClass],
//             nDelta);
      
      if (0 == nDelta)
        continue;
      
      // we have a memory leak
      nLeaksDetected++;
      
      // lets retry
      break;
    }
    
    // no memory leaks ?
    if (0 == nLeaksDetected)
      goto L_END;
    
    // we have memory leaks; sleep and try again!
    AM::vNapMsec(200,CN_SEPROMPT);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // attempted C_ATTEMPTS to see if memory leaks settle down, now time to report
  nLeaksDetected = 0;
  pPoolSnapshot_curr->vTakeSnapshot();

  for (i8 i_Class = 0 ; i_Class < CN_ENUMS; i_Class++) {
    
    eCN eClass = (eCN)i_Class;
    
    if (eClass == CN_FREEBUFFER || eClass == CN_HEAPBUFFER)
      continue;
    
    i8 nDelta = pPoolSnapshot_curr->m_nObjects[eClass] -
                  pPoolSnapshot_then->m_nObjects[eClass];
    
    if (0 == nDelta)
      continue;
    
    vTrace("row: ('%s',  %lld)\n"_Stz,aCN[eClass],nDelta);
    nLeaksDetected++;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

L_END:
  
  vTrace("\nsuccess: %lld rows affected\n"_Stz, nLeaksDetected);
  
  if (nLeaksDetected)
    bPrompt_memoryLeaks_dump(pPrompt);
  
  if (nLeaksDetected && bFatal)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "fatal: memory leaks detected"_Stz);
  
  if (! nLeaksDetected)
    vTrace("No memory leaks detected\n"_Stz);
  
  pPrompt->vTrackMemory(false);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_memoryLeaks_dump(SePrompt* pPrompt)
{
  PoolHelper::vDumpObjects(0);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
