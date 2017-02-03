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
  vAddPrimitive("toCommon",        &vCommonToCommon,   true);

  vAddPrimitive("opFrame.drop",    &vOpFrameDrop,      false);
  vAddPrimitive("opFrame.push",    &vOpFramePush,      false);
}

/*----------------------------------------------------------------------------*/
/* common.fetch -- push commons[n] onto dstack                                */
/*----------------------------------------------------------------------------*/
void Task::vCommonFetch(NNP_Task nnpTask)
{
  ENTRY("vCommonFetch");
  
  if ( ! nnpTask->m_pOpFrame )
    THROW_INVALID_DSTACK("common.fetch");
  
  i8 iCommon;
  if ( ! nnpTask->m_pDataStack->bPop(iCommon) )
    THROW_INVALID_DSTACK("common.fetch");
  
  ValReg* pValReg = nnpTask->m_pOpFrame->pValReg(iCommon);
  
  nnpTask->m_pDataStack->vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* common.store -- pop dstack into common[n]                                  */
/*----------------------------------------------------------------------------*/
void Task::vCommonStore(NNP_Task nnpTask)
{
  ENTRY("vCommonStore");
  
  if ( ! nnpTask->m_pOpFrame )
    THROW_INVALID_DSTACK("commonStore");
  
  i8 iCommon;
  if ( ! nnpTask->m_pDataStack->bPop(iCommon) )
    THROW_INVALID_DSTACK("commonStore");
  
  ValReg* pValReg = nnpTask->m_pOpFrame->pValReg(iCommon);
  
  nnpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* common/ -- declare set of common variables                      immediate  */
/*----------------------------------------------------------------------------*/
void Task::vCommonSlash(NNP_Task nnpTask)
{
ENTRY("vCommonSlash");

  Dic* pSubDic;
  
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
      pSubDic = _pDic(pWordDic->pDataArea());
    }
    else {
      // create a common dictionary      
      pWordDic = new(gpMainDic) Word(sDicName.a());
      pWordDic->vWordType(WT_OBJECT);
      pWordDic->pAllot(sizeof(Dic));
      
      pSubDic = new((Dic*)pWordDic->pDataArea()) Dic;
    }
    
    // get name of common variable
    nnpTask->m_pTokeniser->vNextToken();
    sVarName = nnpTask->m_pTokeniser->sToken();
    
    // create new word for common in current dictionary
    Word* pWordCommon = new(pSubDic) Word(sVarName.a());
    // ** do not set as current word **
    
    // set word as variable
    pWordCommon->vWordType(WT_VARIABLE);
    pWordCommon->vVariable(nnpTask->m_nVariables);
    
    // encode the word
    xValWsp = nnpTask->m_nVariables;
    pWordCommon->vEncode("pushValue",&xValWsp);
    pWordCommon->vEncode("common.fetch");
    pWordCommon->vEncode("exit");
    
    // accept /
    nnpTask->m_pTokeniser->vNextToken();
//  }
  
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
  xValWsp = pWordCommon->iVariable();
  
  pCurrentWord->vEncode("pushValue",&xValWsp);
  pCurrentWord->vEncode("common.store");

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* opFrame.drop -- pop op frame                                               */
/*----------------------------------------------------------------------------*/
void Task::vOpFrameDrop(NNP_Task nnpTask)
{
  ENTRY("vOpFrameDrop");
  
  RegFrame* pOpFrame = nnpTask->m_pOpFrame;
  
  if ( ! pOpFrame )
  THROW_INVALID_DSTACK("opFrame.drop");
  
  nnpTask->m_pOpFrame = nnpTask->m_pOpFrame->pNext();
  
  //delete pCommonFrame;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* opFrame.push -- push op frame                                              */
/*----------------------------------------------------------------------------*/
void Task::vOpFramePush(NNP_Task nnpTask)
{
  ENTRY("vOpFramePush");
  
  Object* pObjectRegFrame;
  if ( ! nnpTask->m_pDataStack->bPop(pObjectRegFrame) )
  THROW_INVALID_DSTACK("opFrame.push");
  
  RegFrame* pCommonFrame = _pRegFrame(pObjectRegFrame);
  
  pCommonFrame->vNext(nnpTask->m_pOpFrame);
  
  nnpTask->m_pOpFrame = pCommonFrame;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
