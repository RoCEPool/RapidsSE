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
  vAddPrimitive("accessPath.genPaths"_Str, &vAccessPathGenPaths,   false);
  vAddPrimitive("accessPath::new"_Str,     &vAccessPathNew,        false);
  vAddPrimitive("accessPath.parse"_Str,    &vAccessPathParse,      false);
  vAddPrimitive("accessPath.set"_Str,      &vAccessPathSet,        false);
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
/* accessPath.genPaths -- generate all access paths for table                 */
/*----------------------------------------------------------------------------*/
void Task::vAccessPathGenPaths(MP_Task mpTask)
{
  ENTRY("vAccessPathDelete"_Str);
  
  Object* pPtrAccessPath;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccessPath) )
    THROW_INVALID_DSTACK("accessPath.delete"_Str);
  
  AccessPath* pAccessPath = _pAccessPath(pPtrAccessPath);
  
  pAccessPath->vGenPaths();
  
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
/* accessPath.parse -- parse where string (where accessPath -- technique)     */
/*----------------------------------------------------------------------------*/
void Task::vAccessPathParse(MP_Task mpTask)
{
  ENTRY("vAccessPathParse"_Str);
  
  Object* pPtrAccessPath;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccessPath) )
    THROW_INVALID_DSTACK("accessPath.parse"_Str);
  AccessPath* pAccessPath = _pAccessPath(pPtrAccessPath);
  
  s48K sWhere;
  if ( ! mpTask->m_pDataStack->bPop(sWhere) )
    THROW_INVALID_DSTACK("accessPath.parse"_Str);
  
  pAccessPath->vWhereString(sWhere);
  pAccessPath->vParse();
  
  s32 sValue_low = "1"_Str;
  s32 sValue_high  = "1"_Str;
  s32 sIndexNm = "jCdt_idx1"_Str;
  
  //mpTask->m_pDataStack->vPushS32(sValue);
  //  mpTask->m_pDataStack->vPushInt(FT_LOWER);
  
  eFT eFetchTechnique = FT_NONE;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  TokenSet xTokenSet;
  Tokeniser* pTokeniser = new Tokeniser;
  pTokeniser->vPush();
  pTokeniser->vUseString(sWhere);
  
  // column name
  pTokeniser->vNextToken();
  
  // operator
  pTokeniser->vNextToken();
  if (pTokeniser->sToken().bSame(">"_Str) ||
      pTokeniser->sToken().bSame(">="_Str))
    eFetchTechnique = FT_LOWER;
  else
  if (pTokeniser->sToken().bSame("="_Str) ||
      pTokeniser->sToken().bSame("=="_Str))
    eFetchTechnique = FT_EQUAL;
  else
  if (pTokeniser->sToken().bSame("<"_Str) ||
      pTokeniser->sToken().bSame("<="_Str))
    eFetchTechnique = FT_UPPER;

  // value
  pTokeniser->vNextToken();
  sValue_low = pTokeniser->sToken();
  
  // and?
  pTokeniser->vNextToken();
  if (pTokeniser->sToken().bSame("and"_Str)) {
    eFetchTechnique = FT_RANGE;
    // column
    pTokeniser->vNextToken();
    // operator
    pTokeniser->vNextToken();
    // value
    pTokeniser->vNextToken();
    sValue_high = pTokeniser->sToken();
    mpTask->m_pDataStack->vPushS32(sValue_high);
    mpTask->m_pDataStack->vPushS32(sValue_low);
    mpTask->m_pDataStack->vPushS32(sIndexNm);
    mpTask->m_pDataStack->vPushInt(eFetchTechnique);
  }
  else {
    mpTask->m_pDataStack->vPushS32(sValue_low);
    mpTask->m_pDataStack->vPushS32(sIndexNm);
    mpTask->m_pDataStack->vPushInt(eFetchTechnique);
  }
  delete pTokeniser;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accessPath.set -- set access path ( seitf accessPath -- )                  */
/*----------------------------------------------------------------------------*/
void Task::vAccessPathSet(MP_Task mpTask)
{
  ENTRY("vAccessPathSet"_Str);
  
  Object* pPtrAccessPath;
  if ( ! mpTask->m_pDataStack->bPop(pPtrAccessPath) )
    THROW_INVALID_DSTACK("accessPath.set"_Str);
  AccessPath* pAccessPath = _pAccessPath(pPtrAccessPath);
  
  Table* pTable = pAccessPath->pTable();
  mpTask->m_pDataStack->vPushObject(pTable);
  
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
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("accessPath.table@"_Str);
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  
  pAccessPath->vInit(pSEitf);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
