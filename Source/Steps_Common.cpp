/*============================================================================*/
/* Steps_Common.cpp - common step group                                       */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Arguments       gxArguments;

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

// common: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Common(void)
{
  vAddPrimitive("common.fetch",    &vCommonFetch,      false);
  vAddPrimitive("common.store",    &vCommonStore,      false);
  vAddPrimitive("common/",         &vCommonSlash,      true);
  vAddPrimitive("common.pop",      &vCommonPop,        false);
  vAddPrimitive("common.push",     &vCommonPush,       false);
  vAddPrimitive("to",              &vCommonToCommon,   true);
}

/*----------------------------------------------------------------------------*/
/* common.fetch -- push commons[n] onto dstack                                */
/*----------------------------------------------------------------------------*/
void Task::vCommonFetch(NNP_Task nnpTask)
{
  ENTRY("vCommonFetch");
  
  if ( ! nnpTask->m_pCommonFrame )
    THROW_INVALID_DSTACK("common.fetch");
  
  i8 iCommon;
  if ( ! nnpTask->m_pDataStack->bPop(iCommon) )
    THROW_INVALID_DSTACK("common.fetch");
  
  ValReg* pValReg = nnpTask->m_pCommonFrame->pValReg(iCommon);
  
  nnpTask->m_pDataStack->vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* common.store -- pop dstack into common[n]                                  */
/*----------------------------------------------------------------------------*/
void Task::vCommonStore(NNP_Task nnpTask)
{
  ENTRY("vCommonStore");
  
  if ( ! nnpTask->m_pCommonFrame )
    THROW_INVALID_DSTACK("commonStore");
  
  i8 iCommon;
  if ( ! nnpTask->m_pDataStack->bPop(iCommon) )
    THROW_INVALID_DSTACK("commonStore");
  
  ValReg* pValReg = nnpTask->m_pCommonFrame->pValReg(iCommon);
  
  nnpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* common/ -- declare set of common variables                      immediate  */
/*----------------------------------------------------------------------------*/
void Task::vCommonSlash(NNP_Task nnpTask)
{
ENTRY("vCommonSlash");

  Dic* pDic;
  
  s32 sDicName;
  s32 sVarName;
  
  //Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
  ValWsp xValWsp;
  
//  for (;;) {
    
    // get name of common dictionary
    nnpTask->m_pTokeniser->vNextToken();
    sDicName = nnpTask->m_pTokeniser->sToken();
    
    // test for that dictionary
    Word* pWordDic = nnpTask->m_xPathStack.pFindWord(sDicName);
    if (pWordDic) {
      // use that dictionary
      pDic = _pDic(pWordDic->pDataArea());
    }
    else {
      // create a common dictionary      
      pWordDic = new(gpMainDic) Word(sDicName.a());
      pWordDic->vWordType(WT_OBJECT);
      pWordDic->pAllot(sizeof(Dic));
      
      pDic = new((Dic*)pWordDic->pDataArea()) Dic;
    }
    
    // get name of common variable
    nnpTask->m_pTokeniser->vNextToken();
    sVarName = nnpTask->m_pTokeniser->sToken();
    
    // create new word for common in current dictionary
    Word* pWordCommon = new(pDic) Word(sVarName.a());
    // ** do not set as current word **
    
    // set word as variable
    pWordCommon->vWordType(WT_LOCAL);
    pWordCommon->vLocal(nnpTask->m_nLocals);
    
    // encode the word
    xValWsp = nnpTask->m_nLocals;
    pWordCommon->vEncode("pushValue",&xValWsp);
    pWordCommon->vEncode("common.fetch");
    pWordCommon->vEncode("exit");
    
    // accept /
    nnpTask->m_pTokeniser->vNextToken();
//  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* common.pop -- pop common frame                                             */
/*----------------------------------------------------------------------------*/
void Task::vCommonPop(NNP_Task nnpTask)
{
ENTRY("vCommonPop");
  
  RegFrame* pLocalsFrame = nnpTask->m_pCommonFrame;
  
  if ( ! pLocalsFrame )
    THROW_INVALID_DSTACK("common.pop");
  
  nnpTask->m_pCommonFrame = nnpTask->m_pCommonFrame->pNext();
  
  delete pLocalsFrame;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* common.push -- push common frame                                           */
/*----------------------------------------------------------------------------*/
void Task::vCommonPush(NNP_Task nnpTask)
{
ENTRY("vCommonPush");
  
  RegFrame* pLocalsFrame = new RegFrame;
  
  pLocalsFrame->vStackPtr(nnpTask->m_pReturnStack->pTopValue());
  
  pLocalsFrame->vNext(nnpTask->m_pCommonFrame);
  
  nnpTask->m_pCommonFrame = pLocalsFrame;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toCommon -- compile store value into common variable             immediate */
/*----------------------------------------------------------------------------*/
void Task::vCommonToCommon(NNP_Task nnpTask)
{
ENTRY("vCommonToCommon");
  
  Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
  // get name of common
  nnpTask->m_pTokeniser->vNextToken();

  Word* pWordCommon = nnpTask->m_xPathStack.pFindWord(nnpTask->m_pTokeniser->sToken().a());
  ASSERT(pWordCommon);
 
  ValWsp xValWsp;
  xValWsp = pWordCommon->iLocal();
  
  pCurrentWord->vEncode("pushValue",&xValWsp);
  pCurrentWord->vEncode("common.store");

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
