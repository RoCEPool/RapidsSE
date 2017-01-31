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
  vAddPrimitive("common/",         &vCommonBar,        true);
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
  
  if ( ! nnpTask->m_pLocalsFrame )
    THROW_INVALID_DSTACK("common.fetch");
  
  i8 iLocal;
  if ( ! nnpTask->m_pDataStack->bPop(iLocal) )
    THROW_INVALID_DSTACK("common.fetch");
  
  ValReg* pValReg = nnpTask->m_pLocalsFrame->pValReg(iLocal);
  
  nnpTask->m_pDataStack->vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* common.store -- pop dstack into common[n]                                  */
/*----------------------------------------------------------------------------*/
void Task::vCommonStore(NNP_Task nnpTask)
{
  ENTRY("vCommonStore");
  
  if ( ! nnpTask->m_pLocalsFrame )
    THROW_INVALID_DSTACK("commonStore");
  
  i8 iLocal;
  if ( ! nnpTask->m_pDataStack->bPop(iLocal) )
    THROW_INVALID_DSTACK("commonStore");
  
  ValReg* pValReg = nnpTask->m_pLocalsFrame->pValReg(iLocal);
  
  nnpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* common/ -- declare set of common variables                      immediate  */
/*----------------------------------------------------------------------------*/
void Task::vCommonBar(NNP_Task nnpTask)
{
ENTRY("vCommonBar");
  
  Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
  Dic* pDic = nnpTask->m_xPathStack.pPick(0);

  ValWsp xValWsp;
  
  for (;;) {
    
    // get name of local
    nnpTask->m_pTokeniser->vNextToken();
    
    // end of locals declaration?
    if (String::bEqualTo("/",nnpTask->m_pTokeniser->sToken().a()))
      break;

    // test for first use of locals|
    if ( ! nnpTask->m_nLocals ) {
      
      // create a locals dictionary
      pDic = new Dic;
      pDic->vName("<localsDic>");
      nnpTask->m_xPathStack.vPush(pDic);
      
      // enocde locals frame push
      pCurrentWord->vEncode("locals.push");
    }
    
    // create new word for local in current dictionary
    Word* pWordLocal = new(pDic) Word(nnpTask->m_pTokeniser->sToken().a());
    // ** do not set as current word **
    
    // set word as local variable
    pWordLocal->vWordType(WT_LOCAL);
    pWordLocal->vLocal(nnpTask->m_nLocals);
    
    // encode the word
    xValWsp = nnpTask->m_nLocals;
    pWordLocal->vEncode("pushValue",&xValWsp);
    pWordLocal->vEncode("local.fetch");
    pWordLocal->vEncode("exit");

    // initialise the local from dstack
    pCurrentWord->vEncode("pushValue",&xValWsp);
    pCurrentWord->vEncode("local.store");
    
    nnpTask->m_nLocals++;
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* common.pop -- pop common frame                                             */
/*----------------------------------------------------------------------------*/
void Task::vCommonPop(NNP_Task nnpTask)
{
ENTRY("vCommonPop");
  
  RegFrame* pLocalsFrame = nnpTask->m_pLocalsFrame;
  
  if ( ! pLocalsFrame )
    THROW_INVALID_DSTACK("common.pop");
  
  nnpTask->m_pLocalsFrame = nnpTask->m_pLocalsFrame->pNext();
  
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
  
  pLocalsFrame->vNext(nnpTask->m_pLocalsFrame);
  
  nnpTask->m_pLocalsFrame = pLocalsFrame;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toCommon -- compile store value into common variable             immediate */
/*----------------------------------------------------------------------------*/
void Task::vCommonToCommon(NNP_Task nnpTask)
{
ENTRY("vCommonToCommon");
  
  Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
  // get name of local
  nnpTask->m_pTokeniser->vNextToken();

  Word* pWordLocal = nnpTask->m_xPathStack.pFindWord(nnpTask->m_pTokeniser->sToken().a());
  ASSERT(pWordLocal);
 
  ValWsp xValWsp;
  xValWsp = pWordLocal->iLocal();
  
  pCurrentWord->vEncode("pushValue",&xValWsp);
  pCurrentWord->vEncode("local.store");

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
