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
  vAddPrimitive("accessPath.delete"_Str, &vAccessPathDelete, false);
  vAddPrimitive("accessPath::new"_Str,   &vAccessPathNew,    false);
  vAddPrimitive("accessPath.parse"_Str,  &vAccessPathParse,  false);
  vAddPrimitive("accessPath.set"_Str,    &vAccessPathSet,    false);
}

/*----------------------------------------------------------------------------*/
/* accessPath.delete -- delete an access path ( accessPath -- )               */
/*----------------------------------------------------------------------------*/
void Task::vAccessPathDelete(NNP_Task nnpTask)
{
ENTRY("vAccessPathDelete"_Str);
  
  Object* pPtrAccessPath;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrAccessPath) )
    THROW_INVALID_DSTACK("accessPath.delete"_Str);
  
  AccessPath* pAccessPath = _pAccessPath(pPtrAccessPath);

  delete pAccessPath;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accessPath::New -- new a file ( -- file )                                  */
/*----------------------------------------------------------------------------*/
void Task::vAccessPathNew(NNP_Task nnpTask)
{
ENTRY("vAccessPathNew"_Str);
  
  AccessPath* pAccessPath = new AccessPath;
  
  Object* pPtrAccessPath;
  pPtrAccessPath = (Object*)pAccessPath;
  
  nnpTask->m_pDataStack->vPushObject(pPtrAccessPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accessPath.set -- set access path ( seitf accessPath -- )                  */
/*----------------------------------------------------------------------------*/
void Task::vAccessPathSet(NNP_Task nnpTask)
{
  ENTRY("vAccessPathSet"_Str);
  
  Object* pPtrAccessPath;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrAccessPath) )
    THROW_INVALID_DSTACK("accessPath.set"_Str);
  AccessPath* pAccessPath = _pAccessPath(pPtrAccessPath);
  
  Object* pPtrSEitf;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("accessPath.set"_Str);
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  
  pAccessPath->vInit(pSEitf);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accessPath.parse -- parse where string (where accessPath -- technique)     */
/*----------------------------------------------------------------------------*/
void Task::vAccessPathParse(NNP_Task nnpTask)
{
  ENTRY("vAccessPathParse"_Str);
  
  Object* pPtrAccessPath;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrAccessPath) )
    THROW_INVALID_DSTACK("accessPath.parse"_Str);
  AccessPath* pAccessPath = _pAccessPath(pPtrAccessPath);
  
  s48K sWhere;
  if ( ! nnpTask->m_pDataStack->bPop(sWhere) )
    THROW_INVALID_DSTACK("accessPath.parse"_Str);
  
  pAccessPath->vWhereString(sWhere);
  pAccessPath->vParse();
  
  s32 sValue_low = "1"_Str;
  s32 sValue_high  = "1"_Str;
  s32 sIndexNm = "jCdt_idx1"_Str;
  
  //nnpTask->m_pDataStack->vPushS32(sValue);
  //  nnpTask->m_pDataStack->vPushInt(FT_LOWER);
  
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
    nnpTask->m_pDataStack->vPushS32(sValue_high);
    nnpTask->m_pDataStack->vPushS32(sValue_low);
    nnpTask->m_pDataStack->vPushS32(sIndexNm);
    nnpTask->m_pDataStack->vPushInt(eFetchTechnique);
  }
  else {
    nnpTask->m_pDataStack->vPushS32(sValue_low);
    nnpTask->m_pDataStack->vPushS32(sIndexNm);
    nnpTask->m_pDataStack->vPushInt(eFetchTechnique);
  }
  delete pTokeniser;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
