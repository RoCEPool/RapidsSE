/*============================================================================*/
/* Steps_AccessPath.cpp - access path step group                              */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_AccessPath(void)
{
  vAddStep("accessPath.table@"_Stz,   &Step::vAccessPathTableFetch);
  vAddStep("accessPath.table!"_Stz,   &Step::vAccessPathTableStore);
}

/*----------------------------------------------------------------------------*/
/* accessPath.table! -- store table into access path                          */
/*----------------------------------------------------------------------------*/
void Step::vAccessPathTableStore(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vAccessPathTableStore"_Stz);
  
  Object* pPtrAccessPath;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrAccessPath) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  AccessPath* pAccessPath = _pAccessPath(pPtrAccessPath);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrTable) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Table* pTable = _pTable(pPtrTable);
  
  pAccessPath->vTable(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accessPath.table@ -- fetch table from access path                          */
/*----------------------------------------------------------------------------*/
void Step::vAccessPathTableFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vAccessPathTableFetch"_Stz);
  
  Object* pPtrAccessPath;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrAccessPath) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  AccessPath* pAccessPath = _pAccessPath(pPtrAccessPath);
  
  Table* pTable = pAccessPath->pTable();
  
  mpTask->m_pForth->m_pDataStack->vPushPointer(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
