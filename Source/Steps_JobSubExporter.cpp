/*============================================================================*/
/* Steps_JobSubExporter.cpp - jobSubExporter step group                       */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobSubExporter(void)
{
  vAddStep("Job_subExporter::spawn"_Stz,     &Step::vJob_SubExporterSpawn);

  vAddStep("job_subExporter.exportJob@"_Stz, &Step::vJob_SubExporterExportJobFetch);
  
  vAddStep("job_subExporter.begin"_Stz,      &Step::vJob_SubExporterBegin);
  vAddStep("job_subExporter.chunk"_Stz,      &Step::vJob_SubExporterChunk);
  vAddStep("job_subExporter.end"_Stz,        &Step::vJob_SubExporterEnd);
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
  JobClass* pJobClass_SubExporter = JobClass::pFind(CN_JOB_SUBEXPORTER);
  
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


/*----------------------------------------------------------------------------*/
/* job_subExporter.begin -- begin sub-exporter                                */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubExporterBegin(MP_Task mpTask)
{
  ENTRY("vJob_SubExporterBegin"_Stz);
  
  Object* pPtrJobSubExporter;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobSubExporter) )
    THROW_INVALID_DSTACK("job_subExporter.begin"_Stz);
  Job_SubExporter* pJob_SubExporter = _pJob_SubExporter(pPtrJobSubExporter);
  
  bool bBegun = pJob_SubExporter->bBegin();
  
  mpTask->m_pDataStack->vPushBool(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subExporter.chunk -- chunk sub-exporter                                */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubExporterChunk(MP_Task mpTask)
{
  ENTRY("vJob_SubExporterChunk"_Stz);
  
  Object* pPtrJobSubExporter;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobSubExporter) )
    THROW_INVALID_DSTACK("job_subExporter.chunk"_Stz);
  Job_SubExporter* pJob_SubExporter = _pJob_SubExporter(pPtrJobSubExporter);
  
  bool bCallAgain = pJob_SubExporter->bChunk();
  
  mpTask->m_pDataStack->vPushBool(bCallAgain);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subExporter.end -- end sub-exporter                                    */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubExporterEnd(MP_Task mpTask)
{
  ENTRY("vJob_SubExporterEnd"_Stz);
  
  Object* pPtrJobSubExporter;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobSubExporter) )
    THROW_INVALID_DSTACK("job_subexporter.end"_Stz);
  Job_SubExporter* pJob_SubExporter = _pJob_SubExporter(pPtrJobSubExporter);
  
  pJob_SubExporter->vEnd();
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
