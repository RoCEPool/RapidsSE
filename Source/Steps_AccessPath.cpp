/*============================================================================*/
/* Steps_AccessPath.cpp - access path step group                              */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern Heap*           gpMediumHeap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;
extern TxCollection*   gpTxCollection;

extern NP_Task         gnpRootTask;
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// accessPath: /////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_AccessPath(void)
{
  vAddPrimitive("accessPath.delete"_Str,   &vAccessPathDelete,     false);
  vAddPrimitive("accessPath::new"_Str,     &vAccessPathNew,        false);
  vAddPrimitive("accessPath.table@"_Str,   &vAccessPathTableFetch, false);
  vAddPrimitive("accessPath.table!"_Str,   &vAccessPathTableStore, false);
}

/*----------------------------------------------------------------------------*/
/* accessPath.delete -- delete an access path ( accessPath -- )               */
/*----------------------------------------------------------------------------*/
void Task::vAccessPathDelete(MP_Task mpTask)
{
ENTRY("vAccessPathDelete"_Str);
  
  Object* pPtrAccessPath;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccessPath) )
    THROW_INVALID_DSTACK("accessPath.delete"_Str);
  
  AccessPath* pAccessPath = _pAccessPath(pPtrAccessPath);

  delete pAccessPath;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accessPath::New -- new a file ( -- file )                                  */
/*----------------------------------------------------------------------------*/
void Task::vAccessPathNew(MP_Task mpTask)
{
ENTRY("vAccessPathNew"_Str);
  
  AccessPath* pAccessPath = new AccessPath;
  
  Object* pPtrAccessPath;
  pPtrAccessPath = (Object*)pAccessPath;
  
  mpTask->m_pDataStack->vPushObject(pPtrAccessPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accessPath.table! -- store table into access path                          */
/*----------------------------------------------------------------------------*/
void Task::vAccessPathTableStore(MP_Task mpTask)
{
  ENTRY("vAccessPathTableStore"_Str);
  
  Object* pPtrAccessPath;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccessPath) )
    THROW_INVALID_DSTACK("accessPath.table!"_Str);
  AccessPath* pAccessPath = _pAccessPath(pPtrAccessPath);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("accessPath.table!"_Str);
  Table* pTable = _pTable(pPtrTable);
  
  pAccessPath->vTable(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accessPath.table@ -- fetch table from access path                          */
/*----------------------------------------------------------------------------*/
void Task::vAccessPathTableFetch(MP_Task mpTask)
{
  ENTRY("vAccessPathTableFetch"_Str);
  
  Object* pPtrAccessPath;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccessPath) )
    THROW_INVALID_DSTACK("accessPath.table@"_Str);
  AccessPath* pAccessPath = _pAccessPath(pPtrAccessPath);
  
  Table* pTable;
  mpTask->m_pDataStack->vPushObject(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
