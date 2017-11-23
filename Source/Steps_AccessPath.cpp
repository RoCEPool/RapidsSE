/*============================================================================*/
/* Steps_AccessPath.cpp - access path step group                              */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Dic*            gpMainDic;

extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// accessPath: /////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_AccessPath(void)
{
  vAddPrimitive("accessPath.table@"_Stz,   &vAccessPathTableFetch, false);
  vAddPrimitive("accessPath.table!"_Stz,   &vAccessPathTableStore, false);
}

/*----------------------------------------------------------------------------*/
/* accessPath.table! -- store table into access path                          */
/*----------------------------------------------------------------------------*/
void Task::vAccessPathTableStore(MP_Task mpTask)
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
void Task::vAccessPathTableFetch(MP_Task mpTask)
{
  ENTRY("vAccessPathTableFetch"_Stz);
  
  Object* pPtrAccessPath;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccessPath) )
    THROW_INVALID_DSTACK("accessPath.table@"_Stz);
  AccessPath* pAccessPath = _pAccessPath(pPtrAccessPath);
  
  Table* pTable = pAccessPath->pTable();
  
  mpTask->m_pDataStack->vPushObject(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
