/*============================================================================*/
/* Steps_JobSubExport.cpp - jobSubExport step group                           */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobSubExport(void)
{
  vAddStep("Job_subExporter::spawn"_Stz,     &Step::vJob_SubExporterSpawn);

  vAddStep("job_subExporter.exportJob@"_Stz, &Step::vJob_SubExporterExportJobFetch);
}

/*----------------------------------------------------------------------------*/
/* job_subExporter::spawn -- spawn a new subexporter                          */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubExporterSpawn(MP_Task mpTask)
{
  ENTRY("vJob_SubExporterSpawn"_Stz);
  
  Object* pPtrJob_Export;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_Export) )
    THROW_INVALID_DSTACK("Job_subExporter::spawn"_Stz);
  Job_Export* pJob_Export = _pJob_Export(pPtrJob_Export);
  
  i8 i_Slot;
  if ( ! mpTask->m_pDataStack->bPop(i_Slot) )
    THROW_INVALID_DSTACK("Job_subExporter::spawn"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Session* pSession = pJob_Export->m_pSession;
  
  //! this needs to be optimised
  JobClass* pJobClass_SubExporter = JobClass::pFind(CN_JOB_SUBEXPORT);
  
  Job_SubExporter* pJob_SubExporter =
    static_cast<Job_SubExporter*>(pJobClass_SubExporter->pPreJob(pSession));
  
  pJob_SubExporter->m_pJobExport = pJob_Export;
  pJob_SubExporter->m_iSlot = i_Slot;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pJob_SubExporter->vSeed(SS_SEED_RUN);

  IGNORE_RETURN(Api::bSpawnJob(pSession,pJob_SubExporter,SS_SEED_RUN));
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* job_subExporter.exportJob@ -- get export job                               */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubExporterExportJobFetch(MP_Task mpTask)
{
  ENTRY("vJob_SubExporterSpawn"_Stz);
  
  Object* pPtrJob_SubExporter;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_SubExporter) )
    THROW_INVALID_DSTACK("Job_subExporter.exportJob@"_Stz);
  Job_SubExporter* pJob_SubExporter = _pJob_SubExporter(pPtrJob_SubExporter);
  
  mpTask->m_pDataStack->vPushPointer(pJob_SubExporter->m_pJobExport);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
