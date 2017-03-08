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
  vAddPrimitive("local.fetch"_Str,     &vLocalFetch,       false);
  vAddPrimitive("local.store"_Str,     &vLocalStore,       false);
  vAddPrimitive("locals|"_Str,         &vLocalsBar,        true);
  vAddPrimitive("locals.pop"_Str,      &vLocalsPop,        false);
  vAddPrimitive("locals.push"_Str,     &vLocalsPush,       false);
  vAddPrimitive("to"_Str,              &vLocalsTo,         true);
}

/*----------------------------------------------------------------------------*/
/* local.fetch -- push locals[n] onto dstack                                  */
/*----------------------------------------------------------------------------*/
void Task::vLocalFetch(NNP_Task nnpTask)
{
  ENTRY("vLocalFetch"_Str);
  
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
  ENTRY("vLocalStore"_Str);
  
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
ENTRY("vLocalsBar"_Str);
  
  Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
  Dic* pDic = nnpTask->m_xPathStack.pPick(0);

  ValWsp xValWsp;
  
  for (;;) {
    
    // get name of local
    nnpTask->m_pTokeniser->vNextToken();
    
    // end of locals declaration?
    if (StrLib::bEqualTo("|",nnpTask->m_pTokeniser->sToken().aStr()))
      break;

    // test for first use of locals|
    if ( ! nnpTask->m_nVariables ) {
      
      // create a locals dictionary
      pDic = new Dic;
      pDic->vName("<localsDic>"_Str);
      nnpTask->m_xPathStack.vPush(pDic);
      
      // enocde locals frame push
      pCurrentWord->vEncode("locals.push"_Str);
    }
    
    // create new word for local in current dictionary
    Word* pWordLocal = new(pDic) Word(nnpTask->m_pTokeniser->sToken());
    // ** do not set as current word **
    
    // set word as local variable
    pWordLocal->vWordType(WT_VARIABLE);
    pWordLocal->vVariable(nnpTask->m_nVariables);
    
    // encode the word
    xValWsp = nnpTask->m_nVariables;
    pWordLocal->vEncode("pushValue"_Str,&xValWsp);
    pWordLocal->vEncode("local.fetch"_Str);
    pWordLocal->vEncode("exit"_Str);

    // initialise the local from dstack
    pCurrentWord->vEncode("pushValue"_Str,&xValWsp);
    pCurrentWord->vEncode("local.store"_Str);
    
    nnpTask->m_nVariables++;
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locals.pop -- pop locals frame                                             */
/*----------------------------------------------------------------------------*/
void Task::vLocalsPop(NNP_Task nnpTask)
{
ENTRY("vLocalsPop"_Str);
  
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
ENTRY("vLocalsPush"_Str);
  
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
ENTRY("vLocalsTo"_Str);
  
  Word* pCurrentWord = nnpTask->m_pCurrentWord;
  
  // get name of local
  nnpTask->m_pTokeniser->vNextToken();

  Word* pWordVariable = nnpTask->m_xPathStack.pFindWord(nnpTask->m_pTokeniser->sToken().aStr());
  ASSERT(pWordVariable);
 
  ValWsp xValWsp;
  xValWsp = pWordVariable->iVariable();
  
  pCurrentWord->vEncode("pushValue"_Str,&xValWsp);
  pCurrentWord->vEncode("local.store"_Str);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
