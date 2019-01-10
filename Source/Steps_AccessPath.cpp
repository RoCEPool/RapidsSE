/*============================================================================*/
/* Steps_AccessPath.cpp - access path step group                              */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

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
void Step::vAccessPathTableStore(MP_Task mpTask)
{
  ENTRY("vAccessPathTableStore"_Stz);
  
  Object* pPtrAccessPath;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccessPath) )
    THROW_INVALID_DSTACK("accessPath.table!"_Stz);
  AccessPath* pAccessPath = _pAccessPath(pPtrAccessPath);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("accessPath.table!"_Stz);
  Table* pTable = _pTable(pPtrTable);
  
  pAccessPath->vTable(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accessPath.table@ -- fetch table from access path                          */
/*----------------------------------------------------------------------------*/
void Step::vAccessPathTableFetch(MP_Task mpTask)
{
  ENTRY("vAccessPathTableFetch"_Stz);
  
  Object* pPtrAccessPath;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccessPath) )
    THROW_INVALID_DSTACK("accessPath.table@"_Stz);
  AccessPath* pAccessPath = _pAccessPath(pPtrAccessPath);
  
  Table* pTable = pAccessPath->pTable();
  
  mpTask->m_pDataStack->vPushPointer(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
