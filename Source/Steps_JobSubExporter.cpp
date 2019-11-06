/*============================================================================*/
/* Steps_JobSubExporter.cpp - jobSubExporter step group                       */
/*============================================================================*/

#include "Steps.h"

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
void Step::vJob_SubExporterSpawn(void)
{
  ENTRY("vJob_SubExporterSpawn"_Stz);
  
  Object* pPtrJob_Export;
  if ( ! m_pDataStack->bPopObject(pPtrJob_Export) )
    THROW_DSTACK();
  Job_Export* pJob_Export = _pJob_Export(pPtrJob_Export);
  
  i8 i_Slot;
  if ( ! m_pDataStack->bPopI8(i_Slot) )
    THROW_DSTACK();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Session* pSession = pJob_Export->pSession();
  ReqTuple* aReqTuple = &pJob_Export->m_xReqTuple;

  //! this needs to be optimised
  JobClass* pJobClass_SubExporter = JobClass::pFind(CN_JOB_SUBEXPORTER);
  
  Job_SubExporter* pJob_SubExporter =
    static_cast<Job_SubExporter*>
      (pJobClass_SubExporter->pPreJob(aReqTuple,pSession));
  
  pJob_SubExporter->m_iParentId    = pJob_Export->m_iFocalId;
  pJob_SubExporter->m_pJobExport   = pJob_Export;
  pJob_SubExporter->vSlot(i_Slot);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pJob_SubExporter->vSeed(SS_SEED_RUN);

  IGNORE_RETURN(Api::bSpawnJob(pJob_SubExporter));
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* job_subExporter.exportJob@ -- get export job                               */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubExporterExportJobFetch(void)
{
  ENTRY("vJob_SubExporterSpawn"_Stz);
  
  Object* pPtrJob_SubExporter;
  if ( ! m_pDataStack->bPopObject(pPtrJob_SubExporter) )
    THROW_DSTACK();
  Job_SubExporter* pJob_SubExporter = _pJob_SubExporter(pPtrJob_SubExporter);
  
  m_pDataStack->vPushPointer(pJob_SubExporter->m_pJobExport);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* job_subExporter.begin -- begin sub-exporter                                */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubExporterBegin(void)
{
  ENTRY("vJob_SubExporterBegin"_Stz);
  
  Object* pPtrJobSubExporter;
  if ( ! m_pDataStack->bPopObject(pPtrJobSubExporter) )
    THROW_DSTACK();
  Job_SubExporter* pJob_SubExporter = _pJob_SubExporter(pPtrJobSubExporter);
  
  bool bBegun = pJob_SubExporter->bBegin();
  
  m_pDataStack->vPushBool(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subExporter.chunk -- chunk sub-exporter                                */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubExporterChunk(void)
{
  ENTRY("vJob_SubExporterChunk"_Stz);
  
  Object* pPtrJobSubExporter;
  if ( ! m_pDataStack->bPopObject(pPtrJobSubExporter) )
    THROW_DSTACK();
  Job_SubExporter* pJob_SubExporter = _pJob_SubExporter(pPtrJobSubExporter);
  
  bool bCallAgain = pJob_SubExporter->bChunk();
  
  m_pDataStack->vPushBool(bCallAgain);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subExporter.end -- end sub-exporter                                    */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubExporterEnd(void)
{
  ENTRY("vJob_SubExporterEnd"_Stz);
  
  Object* pPtrJobSubExporter;
  if ( ! m_pDataStack->bPopObject(pPtrJobSubExporter) )
    THROW_DSTACK();
  Job_SubExporter* pJob_SubExporter = _pJob_SubExporter(pPtrJobSubExporter);
  
  pJob_SubExporter->vEnd();
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
