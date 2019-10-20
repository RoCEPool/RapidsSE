/*============================================================================*/
/* Steps_JobExport.cpp - jobExport step group                                 */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobExport(void)
{
  vAddStep("Job_export.auxAckDecided@"_Stz, &Step::vJob_ExportAuxAckDecidedFetch);
  vAddStep("Job_export.auxAckDecided!"_Stz, &Step::vJob_ExportAuxAckDecidedStore);
  vAddStep("job_export.begin"_Stz,          &Step::vJob_ExportBegin);
  vAddStep("job_export.end"_Stz,            &Step::vJob_ExportEnd);
  vAddStep("Job_export.table!"_Stz,         &Step::vJob_ExportTableStore);
}

/*----------------------------------------------------------------------------*/
/* Job_export.auxAckDecided@ -- fetch if aux ack decided                      */
/*----------------------------------------------------------------------------*/
void Step::vJob_ExportAuxAckDecidedFetch(void)
{
  ENTRY("vJob_ExportAuxAckDecidedFetch"_Stz);
  
  Object* pPtrJobExport;
  if ( ! m_pDataStack->bPopObject(pPtrJobExport) )
    THROW_DSTACK();
  Job_Export* pJob_Export = _pJob_Export(pPtrJobExport);
  
  m_pDataStack->vPushI8(pJob_Export->m_bAuxAckDecided);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_export.auxAckDecided! -- store if aux ack decided                      */
/*----------------------------------------------------------------------------*/
void Step::vJob_ExportAuxAckDecidedStore(void)
{
  ENTRY("vJob_ExportAuxAckDecidedStore"_Stz);
  
  Object* pPtrJobExport;
  if ( ! m_pDataStack->bPopObject(pPtrJobExport) )
    THROW_DSTACK();
  Job_Export* pJob_Export = _pJob_Export(pPtrJobExport);
  
  i8 iAuxAckDecided;
  if ( ! m_pDataStack->bPopI8(iAuxAckDecided) )
    THROW_DSTACK();
  
  pJob_Export->m_bAuxAckDecided = (bool)iAuxAckDecided;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_export.begin -- begin export                                           */
/*----------------------------------------------------------------------------*/
void Step::vJob_ExportBegin(void)
{
  ENTRY("vJob_ExportBegin"_Stz);
  
  Object* pPtrJobExport;
  if ( ! m_pDataStack->bPopObject(pPtrJobExport) )
    THROW_DSTACK();
  Job_Export* pJob_Export = _pJob_Export(pPtrJobExport);
  
  bool bBegun = pJob_Export->bBegin();
  
  m_pDataStack->vPushBool(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_export.end -- end export                                               */
/*----------------------------------------------------------------------------*/
void Step::vJob_ExportEnd(void)
{
  ENTRY("vJob_ExportEnd"_Stz);
  
  Object* pPtrJobExport;
  if ( ! m_pDataStack->bPopObject(pPtrJobExport) )
    THROW_DSTACK();
  Job_Export* pJob_Export = _pJob_Export(pPtrJobExport);
  
  pJob_Export->vEnd();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_export.table! -- store table in the job                                */
/*----------------------------------------------------------------------------*/
void Step::vJob_ExportTableStore(void)
{
  ENTRY("vJob_ExportTableStore"_Stz);
  
  Object* pPtrJobExport;
  if ( ! m_pDataStack->bPopObject(pPtrJobExport) )
    THROW_DSTACK();
  Job_Export* pJob_Export = _pJob_Export(pPtrJobExport);
  
  Object* pTablePtr;
  if ( ! m_pDataStack->bPopObject(pTablePtr) )
    THROW_DSTACK();
  Table* pTable = _pTable(pTablePtr);
  
  pJob_Export->m_pTable = pTable;
    
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
