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
  ENTRY(); // ENTRY("vAccessPathTableStore"_Stz);
  
  Object* pPtrAccessPath; m_pDataStack->vPopPtr(pPtrAccessPath,__func__);
  AccessPath* pAccessPath = _pAccessPath(pPtrAccessPath);
  
  Object* pPtrTable; m_pDataStack->vPopPtr(pPtrTable,__func__);
  Table* pTable = _pTable(pPtrTable);
  
  pAccessPath->vTable(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accessPath.table@ -- fetch table from access path                          */
/*----------------------------------------------------------------------------*/
void Step::vAccessPathTableFetch(void)
{
  ENTRY(); // ENTRY("vAccessPathTableFetch"_Stz);
  
  Object* pPtrAccessPath; m_pDataStack->vPopPtr(pPtrAccessPath,__func__);
  AccessPath* pAccessPath = _pAccessPath(pPtrAccessPath);
  
  Table* pTable = pAccessPath->pTable();
  
  m_pDataStack->vPushPtr(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
