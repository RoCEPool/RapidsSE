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
  vAddStep("subLoader.delete"_Stz,           &Task::vSubLoaderDelete,            false);
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
