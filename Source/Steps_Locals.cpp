/*============================================================================*/
/* Steps_Locals.cpp - locals step group                                       */
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

// local: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Locals(void)
{
  vAddPrimitive("local.fetch",     &vLocalFetch,       false);
  vAddPrimitive("local.store",     &vLocalStore,       false);
  vAddPrimitive("locals|",         &vLocalsBar,        true);
  vAddPrimitive("locals.pop",      &vLocalsPop,        false);
  vAddPrimitive("locals.push",     &vLocalsPush,       false);
  vAddPrimitive("to",              &vLocalsTo,         true);
}

/*----------------------------------------------------------------------------*/
/* local.fetch -- push locals[n] onto dstack                                  */
/*----------------------------------------------------------------------------*/
void Task::vLocalFetch(NNP_Task nnpTask)
{
  ENTRY("vLocalFetch");
  
  if ( ! nnpTask->m_pLocalsFrame )
    THROW_INVALID_DSTACK("local.fetch");
  
  i8 iLocal;
  if ( ! nnpTask->m_pDataStack->bPop(iLocal) )
    THROW_INVALID_DSTACK("local.fetch");
  
  ValReg* pValReg = nnpTask->m_pLocalsFrame->pValReg(iLocal);
  
  nnpTask->m_pDataStack->vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* localStore -- pop dstack into locals[n]                                    */
/*----------------------------------------------------------------------------*/
void Task::vLocalStore(NNP_Task nnpTask)
{
  ENTRY("vLocalStore");
  
  if ( ! nnpTask->m_pLocalsFrame )
    THROW_INVALID_DSTACK("localStore");
  
  i8 iLocal;
  if ( ! nnpTask->m_pDataStack->bPop(iLocal) )
    THROW_INVALID_DSTACK("localStore");
  
  ValReg* pValReg = nnpTask->m_pLocalsFrame->pValReg(iLocal);
  
  nnpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locals| -- declare set of locals                                immediate  */
/*----------------------------------------------------------------------------*/
void Task::vLocalsBar(NNP_Task nnpTask)
{
ENTRY("vLocalsBar");
  
  Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
  Dic* pDic = nnpTask->m_xPathStack.pPick(0);

  ValWsp xValWsp;
  
  for (;;) {
    
    // get name of local
    nnpTask->m_pTokeniser->vNextToken();
    
    // end of locals declaration?
    if (String::bEqualTo("|",nnpTask->m_pTokeniser->sToken().a()))
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
    pWordLocal->vWordType(WT_VARIABLE);
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
/* locals.pop -- pop locals frame                                             */
/*----------------------------------------------------------------------------*/
void Task::vLocalsPop(NNP_Task nnpTask)
{
ENTRY("vLocalsPop");
  
  RegFrame* pLocalsFrame = nnpTask->m_pLocalsFrame;
  
  if ( ! pLocalsFrame )
    THROW_INVALID_DSTACK("locals.pop");
  
  nnpTask->m_pLocalsFrame = nnpTask->m_pLocalsFrame->pNext();
  
  delete pLocalsFrame;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locals.push -- push locals frame                                           */
/*----------------------------------------------------------------------------*/
void Task::vLocalsPush(NNP_Task nnpTask)
{
ENTRY("vLocalsPush");
  
  RegFrame* pLocalsFrame = new RegFrame;
  
  pLocalsFrame->vStackPtr(nnpTask->m_pReturnStack->pTopValue());
  
  pLocalsFrame->vNext(nnpTask->m_pLocalsFrame);
  
  nnpTask->m_pLocalsFrame = pLocalsFrame;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* to -- compile store value into local variable                    immediate */
/*----------------------------------------------------------------------------*/
void Task::vLocalsTo(NNP_Task nnpTask)
{
ENTRY("vLocalsTo");
  
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
