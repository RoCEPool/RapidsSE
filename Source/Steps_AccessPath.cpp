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
void Step::vAccessPathTableStore(void)
{
  ENTRY("vAccessPathTableStore"_Stz);
  
  Object* pPtrAccessPath;
  if ( ! m_pDataStack->bPopObject(pPtrAccessPath) )
    THROW_DSTACK();
  AccessPath* pAccessPath = _pAccessPath(pPtrAccessPath);
  
  Object* pPtrTable;
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
    THROW_DSTACK();
  Table* pTable = _pTable(pPtrTable);
  
  pAccessPath->vTable(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accessPath.table@ -- fetch table from access path                          */
/*----------------------------------------------------------------------------*/
void Step::vAccessPathTableFetch(void)
{
  ENTRY("vAccessPathTableFetch"_Stz);
  
  Object* pPtrAccessPath;
  if ( ! m_pDataStack->bPopObject(pPtrAccessPath) )
    THROW_DSTACK();
  AccessPath* pAccessPath = _pAccessPath(pPtrAccessPath);
  
  Table* pTable = pAccessPath->pTable();
  
  m_pDataStack->vPushPointer(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
