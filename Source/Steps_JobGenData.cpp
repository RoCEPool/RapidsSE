/*============================================================================*/
/* Steps_JobGenData.cpp - job_genData step group                              */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobGenData(void)
{
  vAddStep("job_genData.generate"_Stz,     &Step::vJob_GenDataGenerate);
}

/*----------------------------------------------------------------------------*/
/* job_nodeInfo.nodeName@ -- fetch node name                                  */
/*----------------------------------------------------------------------------*/
void Step::vJob_GenDataGenerate(MP_Task mpTask)
{
  ENTRY("vJob_GenDataGenerate"_Stz);
  
  Object* pPtrJob_GenData;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_GenData) )
    THROW_INVALID_DSTACK("job_nethog.hogNo@"_Stz);
  Job_GenData* pJob_GenData = _pJob_GenData(pPtrJob_GenData);
    
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
