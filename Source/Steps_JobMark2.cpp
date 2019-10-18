/*============================================================================*/
/* Steps_JobMark2.cpp - job mark II step group                                */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobMark2(void)
{
  vAddStep("jobMark2.index!"_Stz,       &Step::vJobMark2IndexStore);
  vAddStep("jobMark2.tableName@"_Stz,   &Step::vJobMark2TableNameFetch);
  vAddStep("jobMark2.table!"_Stz,       &Step::vJobMark2TableStore);
}

/*----------------------------------------------------------------------------*/
/* jobMark2.index! -- store index pointer                                     */
/*----------------------------------------------------------------------------*/
void Step::vJobMark2IndexStore(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJobMark2IndexStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobMark2indextable!"_Stz);
  JobMark2* pJobMark2 = _pJobMark2(pJobPtr);
  
  Object* pIndexPtr;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pIndexPtr) )
    THROW_INVALID_DSTACK("jobMark2indextable!"_Stz);
  Index* pIndex = _pIndex(pIndexPtr);
  
  pJobMark2->m_pIndex = pIndex;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobMark2.tableName@ -- fetch table name                                    */
/*----------------------------------------------------------------------------*/
void Step::vJobMark2TableNameFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJobMark2TableNameFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobMark2.tableName@"_Stz);
  JobMark2* pJobMark2 = _pJobMark2(pJobPtr);
  
  s32 sTableNm;
  sTableNm = pJobMark2->szObjectName();
  
  mpTask->m_pForth->m_pDataStack->vPushS32(sTableNm);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobMark2.table! -- store table pointer                                     */
/*----------------------------------------------------------------------------*/
void Step::vJobMark2TableStore(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJobMark2TableStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobMark2.table!"_Stz);
  JobMark2* pJobMark2 = _pJobMark2(pJobPtr);

  Object* pTablePtr;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pTablePtr) )
    THROW_INVALID_DSTACK("jobMark2.table!"_Stz);
  Table* pTable = _pTable(pTablePtr);
  
  pJobMark2->m_pTable = pTable;

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
