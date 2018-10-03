/*============================================================================*/
/* Steps_SubLoader.cpp - sub-loader step group                                */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*  gpAttachMap;

// loaderSub: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_SubLoader(void)
{
  vAddStep("subLoader.danglingBuffer"_Stz,   &Task::vSubLoaderDanglingBuffer,    false);
  vAddStep("subLoader.delete"_Stz,           &Task::vSubLoaderDelete,            false);
  vAddStep("subLoader.processRecords"_Stz,   &Task::vSubLoaderProcessRecords,    false);
}

/*----------------------------------------------------------------------------*/
/* subLoader.dangingBuffer -- ensure there's a dangling buffer for partition  */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderDanglingBuffer(MP_Task mpTask)
{
  ENTRY("vSubLoaderDanglingBuffer"_Stz);
  
  Object* pPtrSubLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSubLoader) )
    THROW_INVALID_DSTACK("subLoader.delete"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrSubLoader);
  
  //if ( ! pSubLoader->pWorkingBuffer() )
  //  pSubLoader->vNewWorkingBuffer();
  
  Job_SubLoader* pJob_SubLoader = pSubLoader->pJob_SubLoader();
  
  if ( ! pJob_SubLoader->m_pWorkingBuffer[pJob_SubLoader->m_iPartition] )
    pJob_SubLoader->vNewWorkingBuffer();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* subLoader.delete -- delete a table loader                                  */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderDelete(MP_Task mpTask)
{
  ENTRY("vSubLoaderDelete"_Stz);
  
  Object* pPtrSubLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSubLoader) )
    THROW_INVALID_DSTACK("subLoader.delete"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrSubLoader);
  
  ASSERT(CN_SUBLOADER == pSubLoader->eClass());
  
  delete pSubLoader;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* subLoader.processRecords -- read & process records till buffer full or eof */
/*----------------------------------------------------------------------------*/
void Task::vSubLoaderProcessRecords(MP_Task mpTask)
{
  ENTRY("vSubLoaderProcessRecords"_Stz);
  
  Object* pPtrSubLoaderSub;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSubLoaderSub) )
    THROW_INVALID_DSTACK("subLoader.processRecords"_Stz);
  SubLoader* pSubLoader = _pLoaderSub(pPtrSubLoaderSub);
  
  Job_SubLoader* pJob_SubLoader = pSubLoader->pJob_SubLoader();
  bool bTargetBufferFull = pJob_SubLoader->bProcessRecords();
  
  mpTask->m_pDataStack->vPushInt(bTargetBufferFull);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
