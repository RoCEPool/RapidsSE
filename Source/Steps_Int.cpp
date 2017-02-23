/*============================================================================*/
/* Steps_Int.cpp - int step group                                             */
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

// int: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Int(void)
{
  vAddPrimitive("+",               &vIntAdd,           false);
  vAddPrimitive("/",               &vIntDivide,        false);
  vAddPrimitive("==",              &vIntEqual,         false);
  vAddPrimitive(">",               &vIntGreater,       false);
  vAddPrimitive(">=",              &vIntGreaterEqual,  false);
  vAddPrimitive("hash",            &vIntHash,          false);
  vAddPrimitive("<",               &vIntLess,          false);
  vAddPrimitive("<=",              &vIntLessEqual,     false);
  vAddPrimitive("%",               &vIntModulo,        false);
  vAddPrimitive("*",               &vIntMultiply,      false);
  vAddPrimitive("neg",             &vIntNeg,           false);
  vAddPrimitive("!=",              &vIntNotEqual,      false);
  vAddPrimitive("-",               &vIntSubtract,      false);
}

/*----------------------------------------------------------------------------*/
/* + -- add top two integers on stack                                         */
/*----------------------------------------------------------------------------*/
void Task::vIntAdd(NNP_Task nnpTask)
{
  ENTRY("vIntAdd");
  /*
  i8 r, j, i;
  
  if ( ! nnpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("+");
  
  r = i + j;
  
  nnpTask->m_pDataStack->vPush(r);
  */
  
  // get pointer to value on the top of the stack
  Value* j = nnpTask->m_pDataStack->pTopValue();
  
  // discard the value
  nnpTask->m_pDataStack->vDiscard();
  
  // get pointer to value on the top of the stack
  Value* i = nnpTask->m_pDataStack->pTopValue();
  
  // add the two values and leave the result on the stack
  *_pIntVal(i) = _pIntVal(j)->iInt() + _pIntVal(i)->iInt();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* / -- divide top integer by second integer on stack                         */
/*----------------------------------------------------------------------------*/
void Task::vIntDivide(NNP_Task nnpTask)
{
  ENTRY("vIntDivide");
  
  i8 r, j, i;
  
  if ( ! nnpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("/");
  
  r = DIV(i,j);
  
  nnpTask->m_pDataStack->vPush(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* == -- test top two integers on data stack for equality                     */
/*----------------------------------------------------------------------------*/
void Task::vIntEqual(NNP_Task nnpTask)
{
  ENTRY("vIntEqual");
  
  i8 r, j, i;
  
  if ( ! nnpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("==");
  
  r = i == j;
  
  nnpTask->m_pDataStack->vPush(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* > -- test top two integers on data stack for greater than                  */
/*----------------------------------------------------------------------------*/
void Task::vIntGreater(NNP_Task nnpTask)
{
  ENTRY("vIntGreater");
  
  i8 r, j, i;
  
  if ( ! nnpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK(">");
  
  r = i > j;
  
  nnpTask->m_pDataStack->vPush(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >= -- test top two integers on data stack for greater than equal to        */
/*----------------------------------------------------------------------------*/
void Task::vIntGreaterEqual(NNP_Task nnpTask)
{
  ENTRY("vIntGreaterEqual");
  
  i8 r, j, i;
  
  if ( ! nnpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK(">=");
  
  r = i >= j;
  
  nnpTask->m_pDataStack->vPush(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* hash -- get hash of string                                                 */
/*----------------------------------------------------------------------------*/
void Task::vIntHash(NNP_Task nnpTask)
{
  ENTRY("vIntHash");
  
  s48K sString;
  if ( ! nnpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("hash");
  
  i8 iLength = String::iLength(sString.a());
  
  i8 iHash = XXH32(sString.a(),iLength,0);
  
  nnpTask->m_pDataStack->vPush(iHash);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* < -- test top two integers on data stack for less than                     */
/*----------------------------------------------------------------------------*/
void Task::vIntLess(NNP_Task nnpTask)
{
  ENTRY("vIntLess");
  
  i8 r, j, i;
  
  if ( ! nnpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("<");
  
  r = i < j;
  
  nnpTask->m_pDataStack->vPush(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* <= -- test top two integers on data stack for less than equal to           */
/*----------------------------------------------------------------------------*/
void Task::vIntLessEqual(NNP_Task nnpTask)
{
  ENTRY("vIntLessEqual");
  
  i8 r, j, i;
  
  if ( ! nnpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("<=");
  
  r = i <= j;
  
  nnpTask->m_pDataStack->vPush(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* % -- modulo of top integer by second integer on stack                      */
/*----------------------------------------------------------------------------*/
void Task::vIntModulo(NNP_Task nnpTask)
{
  ENTRY("vIntModulo");
  
  i8 r, j, i;
  
  if ( ! nnpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("%");
  
  r = MOD(i,j);
  
  nnpTask->m_pDataStack->vPush(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* * multiply second integer by top integer on stack                          */
/*----------------------------------------------------------------------------*/
void Task::vIntMultiply(NNP_Task nnpTask)
{
  ENTRY("vIntMultiply");
  
  i8 r, j, i;
  
  if ( ! nnpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("*");
  
  r = i * j;
  
  nnpTask->m_pDataStack->vPush(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* neg -- gegate top integer on stack                                         */
/*----------------------------------------------------------------------------*/
void Task::vIntNeg(NNP_Task nnpTask)
{
  ENTRY("vIntNeg");
  
  i8 r, i;
  
  if ( ! nnpTask->m_pDataStack->bPop(i) )
    THROW_INVALID_DSTACK("neg");
  
  r = -1 * i;
  
  nnpTask->m_pDataStack->vPush(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* != -- test top two integers on data stack for not equal                    */
/*----------------------------------------------------------------------------*/
void Task::vIntNotEqual(NNP_Task nnpTask)
{
  ENTRY("vIntNotEqual");
  
  i8 r, j, i;
  
  if ( ! nnpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("!=");
  
  r = i != j;
  
  nnpTask->m_pDataStack->vPush(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* - -- subtract second integer from top integer on stack                     */
/*----------------------------------------------------------------------------*/
void Task::vIntSubtract(NNP_Task nnpTask)
{
  ENTRY("vIntSubtract");
  
  i8 r, j, i;
  
  if ( ! nnpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("-");
  
  r = i - j;
  
  nnpTask->m_pDataStack->vPush(r);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
