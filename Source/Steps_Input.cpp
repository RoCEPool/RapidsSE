/*============================================================================*/
/* Steps_Input.cpp - input step group                                         */
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

// input: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Input(void)
{
  vAddPrimitive("accept"_Str,          &vInputAccept,      false);
  vAddPrimitive("input.close"_Str,     &vInputClose,       false);
  vAddPrimitive("input.depth"_Str,     &vInputDepth,       false);
  vAddPrimitive("input.next"_Str,      &vInputNext,        false);
  vAddPrimitive("input.open"_Str,      &vInputOpen,        false);
  vAddPrimitive("input.pop"_Str,       &vInputPop,         false);
  vAddPrimitive("input.push"_Str,      &vInputPush,        false);
  vAddPrimitive("input.stdin"_Str,     &vInputStdin,       false);
  vAddPrimitive("input.text"_Str,      &vInputText,        false);
  vAddPrimitive("input.token"_Str,     &vInputToken,       false);
  vAddPrimitive("input.type"_Str,      &vInputType,        false);
  vAddPrimitive("token:"_Str,          &vTokenColon,       false);
}

/*----------------------------------------------------------------------------*/
/* accept -- accept input string and push on stack                            */
/*----------------------------------------------------------------------------*/
void Task::vInputAccept(NNP_Task nnpTask)
{
  ENTRY("vInputAccept");
  
  ValWsp xWsp;
  xWsp.vClass(CN_STRVAL);
  
  char* y = _pStrVal(&xWsp)->aStr();
  
  while ( '\n' != (*y++ = getc(stdin)) ) ;
  
  *--y = '\0';
  
  _pStrVal(&xWsp)->vAreaSize();
  
  nnpTask->m_pDataStack->vPush(xWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.close -- close current input stream                                  */
/*----------------------------------------------------------------------------*/
void Task::vInputClose(NNP_Task nnpTask)
{
ENTRY("vInputClose");
  
  nnpTask->m_pTokeniser->vUseEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.depth -- push current depth of input stream onto data stack         */
/*----------------------------------------------------------------------------*/
void Task::vInputDepth(NNP_Task nnpTask)
{
ENTRY("vInputDepth");
  
  i8 iDepth = nnpTask->m_pTokeniser->iDepth();
  
  nnpTask->m_pDataStack->vPush(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.next -- get next input token from current input stream               */
/*----------------------------------------------------------------------------*/
void Task::vInputNext(NNP_Task nnpTask)
{
ENTRY("vInputNext");
  
  if (nnpTask->m_iTracing) {
    ASSERT(nnpTask->m_iTracing);
  }
  
  char* aToken = nnpTask->m_pTokeniser->sToken().aStr();
  
  eTK eToken = nnpTask->m_pTokeniser->eNextToken(aToken);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.open -- open new input using filepath on data stack                  */
/*----------------------------------------------------------------------------*/
void Task::vInputOpen(NNP_Task nnpTask)
{
ENTRY("vInputOpen");
  
  s256 sFilePath;
  
  if ( ! nnpTask->m_pDataStack->bPop(sFilePath) )
    THROW_INVALID_DSTACK("input.open");
  
  bool bOpened = nnpTask->m_pTokeniser->bUseFile(sFilePath);
  
  if ( ! bOpened )
    THROW("No such file: %s",sFilePath.aStr());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.pop -- pop to previous input stream                                  */
/*----------------------------------------------------------------------------*/
void Task::vInputPop(NNP_Task nnpTask)
{
ENTRY("vInputPop");
  
  bool bPopped = nnpTask->m_pTokeniser->bPop();
  ASSERT(bPopped);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.push -- push input stream, allowing for new input stream             */
/*----------------------------------------------------------------------------*/
void Task::vInputPush(NNP_Task nnpTask)
{
ENTRY("vInputPush");
  
  nnpTask->m_pTokeniser->vPush();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.stdin -- use stdin as new input stream                               */
/*----------------------------------------------------------------------------*/
void Task::vInputStdin(NNP_Task nnpTask)
{
ENTRY("vInputStdin");
  
  nnpTask->m_pTokeniser->vUseStdin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.text -- use string as new input stream                               */
/*----------------------------------------------------------------------------*/
void Task::vInputText(NNP_Task nnpTask)
{
ENTRY("vInputText");
  
  if (nnpTask->m_iTracing) {
    ASSERT(nnpTask->m_iTracing);
  }
  
  s48K sInput;                                             //! NEED STACK CHECKS
  
  if ( ! nnpTask->m_pDataStack->bPop(sInput) )
    THROW_INVALID_DSTACK("input.text");
    
  nnpTask->m_pTokeniser->vUseString(sInput);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.token -- push current token of input stream onto data stack          */
/*----------------------------------------------------------------------------*/
void Task::vInputToken(NNP_Task nnpTask)
{
ENTRY("vInputToken");
  
  nnpTask->m_pDataStack->vPush(nnpTask->m_pTokeniser->sToken());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.type -- push current token type onto data stack                      */
/*----------------------------------------------------------------------------*/
void Task::vInputType(NNP_Task nnpTask)
{
ENTRY("vInputType");
  
  eTK eTokenType = nnpTask->m_pTokeniser->eToken();
  
  nnpTask->m_pDataStack->vPush(eTokenType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* token: -- token colon                                                      */
/*----------------------------------------------------------------------------*/
void Task::vTokenColon(NNP_Task nnpTask)
{
  ENTRY("vTokenColon");
  
  nnpTask->m_pTokeniser->vNextToken();
  
  nnpTask->m_pDataStack->vPush(nnpTask->m_pTokeniser->sToken());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
