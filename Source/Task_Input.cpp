/*============================================================================*/
/* Task_Input.cpp - task input group                                          */
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
  vAddPrimitive("accept",          &vInputAccept,      false);
  vAddPrimitive("input.close",     &vInputClose,       false);
  vAddPrimitive("input.depth",     &vInputDepth,       false);
  vAddPrimitive("input.next",      &vInputNext,        false);
  vAddPrimitive("input.open",      &vInputOpen,        false);
  vAddPrimitive("input.pop",       &vInputPop,         false);
  vAddPrimitive("input.push",      &vInputPush,        false);
  vAddPrimitive("input.stdin",     &vInputStdin,       false);
  vAddPrimitive("input.text",      &vInputText,        false);
  vAddPrimitive("input.token",     &vInputToken,       false);
  vAddPrimitive("input.type",      &vInputType,        false);
  vAddPrimitive("token:",          &vTokenColon,       false);
}

/*----------------------------------------------------------------------------*/
/* accept -- accept input string and push on stack                            */
/*----------------------------------------------------------------------------*/
void Task::vInputAccept(NNP_Task nnpTask)
{
  ENTRY("vInputAccept");
  
  ValWsp xWsp;
  xWsp.vClass(CN_STRVAL);
  
  char* y = xWsp.sStr();
  
  y = xWsp.sStr();
  while ( '\n' != (*y++ = getc(stdin)) ) ;
  
  *--y = '\0';
  
  xWsp.pAsStrVal()->vSize();
  
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
/* input.depth -- push current depth of input stream onto param stack        */
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
  
  char* aToken = nnpTask->m_pTokeniser->sToken().a();
  
  eTK eToken = nnpTask->m_pTokeniser->eNextToken(aToken);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.open -- open new input using filepath on param stack                 */
/*----------------------------------------------------------------------------*/
void Task::vInputOpen(NNP_Task nnpTask)
{
ENTRY("vInputOpen");
  
  s256 sFilePath;
  
  if ( ! nnpTask->m_pDataStack->bPop(sFilePath) )
    THROW_INVALID_DSTACK("input.open");
  
  bool bOpened = nnpTask->m_pTokeniser->bUseFile(sFilePath);
  
  if ( ! bOpened )
    THROW("No such file: %s",sFilePath.a());
  
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
  
  s256 sInput;
  
  if ( ! nnpTask->m_pDataStack->bPop(sInput) )
    THROW_INVALID_DSTACK("input.text");
    
  nnpTask->m_pTokeniser->vUseString(sInput);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.token -- push current token of input stream onto param stack         */
/*----------------------------------------------------------------------------*/
void Task::vInputToken(NNP_Task nnpTask)
{
ENTRY("vInputToken");
  
  nnpTask->m_pDataStack->vPush(nnpTask->m_pTokeniser->sToken());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.type -- push current token type onto param stack                     */
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
  
  nnpTask->vTokenNext();
  
  nnpTask->m_pDataStack->vPush(nnpTask->m_pTokeniser->sToken());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
