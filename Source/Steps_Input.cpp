/*============================================================================*/
/* Steps_Input.cpp - input step group                                         */
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

// input: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Input(void)
{
  vAddPrimitive("accept"_Stz,          &vInputAccept,      false);
  vAddPrimitive("input.close"_Stz,     &vInputClose,       false);
  vAddPrimitive("input.depth"_Stz,     &vInputDepth,       false);
  vAddPrimitive("input.next"_Stz,      &vInputNext,        false);
  vAddPrimitive("input.open"_Stz,      &vInputOpen,        false);
  vAddPrimitive("input.pop"_Stz,       &vInputPop,         false);
  vAddPrimitive("input.push"_Stz,      &vInputPush,        false);
  vAddPrimitive("input.stdin"_Stz,     &vInputStdin,       false);
  vAddPrimitive("input.text"_Stz,      &vInputText,        false);
  vAddPrimitive("input.token"_Stz,     &vInputToken,       false);
  vAddPrimitive("input.type"_Stz,      &vInputType,        false);
  vAddPrimitive("token:"_Stz,          &vTokenColon,       false);
}

/*----------------------------------------------------------------------------*/
/* accept -- accept input string and push on stack                            */
/*----------------------------------------------------------------------------*/
void Task::vInputAccept(MP_Task mpTask)
{
  ENTRY("vInputAccept"_Stz);
  
  ValWrk xWsp;
  xWsp.vSetClass(CN_STRVAL);
  
  char* y = _pStrVal(&xWsp)->aChar();
  
  while ( '\n' != (*y++ = getc(stdin)) ) ;
  
  *--y = '\0';
  
  _pStrVal(&xWsp)->vAreaSize();
  
  mpTask->m_pDataStack->vPushValWrk(xWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.close -- close current input stream                                  */
/*----------------------------------------------------------------------------*/
void Task::vInputClose(MP_Task mpTask)
{
ENTRY("vInputClose"_Stz);
  
  mpTask->m_pTokeniser->vUseEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.depth -- push current depth of input stream onto data stack         */
/*----------------------------------------------------------------------------*/
void Task::vInputDepth(MP_Task mpTask)
{
ENTRY("vInputDepth"_Stz);
  
  i8 iDepth = mpTask->m_pTokeniser->iDepth();
  
  mpTask->m_pDataStack->vPushInt(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.next -- get next input token from current input stream               */
/*----------------------------------------------------------------------------*/
void Task::vInputNext(MP_Task mpTask)
{
ENTRY("vInputNext"_Stz);
  
  if (mpTask->m_iTracing) {
    ASSERT(mpTask->m_iTracing);
  }
  
  char* aToken = mpTask->m_pTokeniser->sToken().aChar();
  
  eTN eToken = mpTask->m_pTokeniser->eNextToken(aToken);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.open -- open new input using filepath on data stack                  */
/*----------------------------------------------------------------------------*/
void Task::vInputOpen(MP_Task mpTask)
{
ENTRY("vInputOpen"_Stz);
  
  s256 sFilePath;
  
  if ( ! mpTask->m_pDataStack->bPop(sFilePath) )
    THROW_INVALID_DSTACK("input.open"_Stz);
  
  bool bOpened = mpTask->m_pTokeniser->bUseFile(sFilePath);
  
  if ( ! bOpened )
    THROW("No such file: %s"_Stz,sFilePath.aChar());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.pop -- pop to previous input stream                                  */
/*----------------------------------------------------------------------------*/
void Task::vInputPop(MP_Task mpTask)
{
ENTRY("vInputPop"_Stz);
  
  bool bPopped = mpTask->m_pTokeniser->bPop();
  ASSERT(bPopped);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.push -- push input stream, allowing for new input stream             */
/*----------------------------------------------------------------------------*/
void Task::vInputPush(MP_Task mpTask)
{
ENTRY("vInputPush"_Stz);
  
  mpTask->m_pTokeniser->vPush();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.stdin -- use stdin as new input stream                               */
/*----------------------------------------------------------------------------*/
void Task::vInputStdin(MP_Task mpTask)
{
ENTRY("vInputStdin"_Stz);
  
  mpTask->m_pTokeniser->vUseStdin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.text -- use string as new input stream                               */
/*----------------------------------------------------------------------------*/
void Task::vInputText(MP_Task mpTask)
{
ENTRY("vInputText"_Stz);
  
  if (mpTask->m_iTracing) {
    ASSERT(mpTask->m_iTracing);
  }
  
  s48K sInput;                                             //! NEED STACK CHECKS
  
  if ( ! mpTask->m_pDataStack->bPop(sInput) )
    THROW_INVALID_DSTACK("input.text"_Stz);
    
  mpTask->m_pTokeniser->vUseString(sInput);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.token -- push current token of input stream onto data stack          */
/*----------------------------------------------------------------------------*/
void Task::vInputToken(MP_Task mpTask)
{
ENTRY("vInputToken"_Stz);
  
  mpTask->m_pDataStack->vPushS48K(mpTask->m_pTokeniser->sToken());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.type -- push current token type onto data stack                      */
/*----------------------------------------------------------------------------*/
void Task::vInputType(MP_Task mpTask)
{
ENTRY("vInputType"_Stz);
  
  eTN eTokenType = mpTask->m_pTokeniser->eToken();
  
  mpTask->m_pDataStack->vPushInt(eTokenType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* token: -- token colon                                                      */
/*----------------------------------------------------------------------------*/
void Task::vTokenColon(MP_Task mpTask)
{
  ENTRY("vTokenColon"_Stz);
  
  mpTask->m_pTokeniser->vNextToken();
  
  mpTask->m_pDataStack->vPushS48K(mpTask->m_pTokeniser->sToken());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
