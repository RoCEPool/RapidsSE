/*============================================================================*/
/* Steps_Locals.cpp - locals step group                                       */
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
void Task::vLocalFetch(MP_Task mpTask)
{
  ENTRY("vLocalFetch"_Str);
  
  if ( ! mpTask->m_pLocalsFrame )
    THROW_INVALID_DSTACK("local.fetch"_Str);
  
  i8 iLocal;
  if ( ! mpTask->m_pDataStack->bPop(iLocal) )
    THROW_INVALID_DSTACK("local.fetch"_Str);
  
  ValReg* pValReg = mpTask->m_pLocalsFrame->pValReg(iLocal);
  
  mpTask->m_pDataStack->vPushValue(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* localStore -- pop dstack into locals[n]                                    */
/*----------------------------------------------------------------------------*/
void Task::vLocalStore(MP_Task mpTask)
{
  ENTRY("vLocalStore"_Str);
  
  if ( ! mpTask->m_pLocalsFrame )
    THROW_INVALID_DSTACK("localStore"_Str);
  
  i8 iLocal;
  if ( ! mpTask->m_pDataStack->bPop(iLocal) )
    THROW_INVALID_DSTACK("localStore"_Str);
  
  ValReg* pValReg = mpTask->m_pLocalsFrame->pValReg(iLocal);
  
  mpTask->m_pDataStack->bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locals| -- declare set of locals                                immediate  */
/*----------------------------------------------------------------------------*/
void Task::vLocalsBar(MP_Task mpTask)
{
ENTRY("vLocalsBar"_Str);
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  Dic* pDic = mpTask->m_xPathStack.pPick(0);

  ValWrk xValWrk;
  
  for (;;) {
    
    // get name of local
    mpTask->m_pTokeniser->vNextToken();
    
    // end of locals declaration?
    if (mpTask->m_pTokeniser->sToken().bEqual("|"_Str))
      break;

    // test for first use of locals|
    if ( ! mpTask->m_nVariables ) {
      
      // create a locals dictionary
      pDic = new Dic;
      pDic->vName("<localsDic>"_Str);
      mpTask->m_xPathStack.vPush(pDic);
      
      // enocde locals frame push
      pCurrentWord->vEncode("locals.push"_Str);
    }
    
    // create new word for local in current dictionary
    Word* pWordLocal = new(pDic) Word(mpTask->m_pTokeniser->sToken());
    // ** do not set as current word **
    
    // set word as local variable
    pWordLocal->vWordType(WT_VARIABLE);
    pWordLocal->vVariable(mpTask->m_nVariables);
    
    // encode the word
    xValWrk = mpTask->m_nVariables;
    pWordLocal->vEncode("pushValue"_Str,&xValWrk);
    pWordLocal->vEncode("local.fetch"_Str);
    pWordLocal->vEncode("exit"_Str);

    // initialise the local from dstack
    pCurrentWord->vEncode("pushValue"_Str,&xValWrk);
    pCurrentWord->vEncode("local.store"_Str);
    
    mpTask->m_nVariables++;
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locals.pop -- pop locals frame                                             */
/*----------------------------------------------------------------------------*/
void Task::vLocalsPop(MP_Task mpTask)
{
ENTRY("vLocalsPop"_Str);
  
  RegFrame* pLocalsFrame = mpTask->m_pLocalsFrame;
  
  if ( ! pLocalsFrame )
    THROW_INVALID_DSTACK("locals.pop"_Str);
  
  mpTask->m_pLocalsFrame = mpTask->m_pLocalsFrame->pNext();
  
  delete pLocalsFrame;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locals.push -- push locals frame                                           */
/*----------------------------------------------------------------------------*/
void Task::vLocalsPush(MP_Task mpTask)
{
ENTRY("vLocalsPush"_Str);
  
  RegFrame* pLocalsFrame = new RegFrame;
  
  pLocalsFrame->vStackPtr(mpTask->m_pReturnStack->pTopValue());
  
  pLocalsFrame->vNext(mpTask->m_pLocalsFrame);
  
  mpTask->m_pLocalsFrame = pLocalsFrame;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* to -- compile store value into local variable                    immediate */
/*----------------------------------------------------------------------------*/
void Task::vLocalsTo(MP_Task mpTask)
{
ENTRY("vLocalsTo"_Str);
  
  Word* pCurrentWord = mpTask->m_pCurrentWord;
  
  // get name of local
  mpTask->m_pTokeniser->vNextToken();

  Word* pWordVariable = mpTask->m_xPathStack.pFindWord(mpTask->m_pTokeniser->sToken());
  ASSERT(pWordVariable);
 
  ValWrk xValWrk;
  xValWrk = pWordVariable->iVariable();
  
  pCurrentWord->vEncode("pushValue"_Str,&xValWrk);
  pCurrentWord->vEncode("local.store"_Str);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
