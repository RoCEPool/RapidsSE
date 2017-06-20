/*============================================================================*/
/* Steps_Int.cpp - int step group                                             */
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
  vAddPrimitive("+"_Str,               &vIntAdd,           false);
  vAddPrimitive("/"_Str,               &vIntDivide,        false);
  vAddPrimitive("=="_Str,              &vIntEqual,         false);
  vAddPrimitive(">"_Str,               &vIntGreater,       false);
  vAddPrimitive(">="_Str,              &vIntGreaterEqual,  false);
  vAddPrimitive("hash"_Str,            &vIntHash,          false);
  vAddPrimitive("<"_Str,               &vIntLess,          false);
  vAddPrimitive("<="_Str,              &vIntLessEqual,     false);
  vAddPrimitive("%"_Str,               &vIntModulo,        false);
  vAddPrimitive("*"_Str,               &vIntMultiply,      false);
  vAddPrimitive("neg"_Str,             &vIntNeg,           false);
  vAddPrimitive("!="_Str,              &vIntNotEqual,      false);
  vAddPrimitive("-"_Str,               &vIntSubtract,      false);
}

/*----------------------------------------------------------------------------*/
/* + -- add top two integers on stack                                         */
/*----------------------------------------------------------------------------*/
void Task::vIntAdd(MP_Task mpTask)
{
  ENTRY("vIntAdd"_Str);
  /*
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("+"_Str);
  
  r = i + j;
  
  mpTask->m_pDataStack->vPush(r);
  */
  
  // get pointer to value on the top of the stack
  Value* j = mpTask->m_pDataStack->pTopValue();
  
  // discard the value
  mpTask->m_pDataStack->vDiscard();
  
  // get pointer to value on the top of the stack
  Value* i = mpTask->m_pDataStack->pTopValue();
  
  // add the two values and leave the result on the stack
  *_pIntVal(i) = _pIntVal(j)->iInt() + _pIntVal(i)->iInt();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* / -- divide top integer by second integer on stack                         */
/*----------------------------------------------------------------------------*/
void Task::vIntDivide(MP_Task mpTask)
{
  ENTRY("vIntDivide"_Str);
  
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("/"_Str);
  
  r = DIV(i,j);
  
  mpTask->m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* == -- test top two integers on data stack for equality                     */
/*----------------------------------------------------------------------------*/
void Task::vIntEqual(MP_Task mpTask)
{
  ENTRY("vIntEqual"_Str);
  
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("=="_Str);
  
  r = i == j;
  
  mpTask->m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* > -- test top two integers on data stack for greater than                  */
/*----------------------------------------------------------------------------*/
void Task::vIntGreater(MP_Task mpTask)
{
  ENTRY("vIntGreater"_Str);
  
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK(">"_Str);
  
  r = i > j;
  
  mpTask->m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >= -- test top two integers on data stack for greater than equal to        */
/*----------------------------------------------------------------------------*/
void Task::vIntGreaterEqual(MP_Task mpTask)
{
  ENTRY("vIntGreaterEqual"_Str);
  
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK(">="_Str);
  
  r = i >= j;
  
  mpTask->m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* hash -- get hash of string                                                 */
/*----------------------------------------------------------------------------*/
void Task::vIntHash(MP_Task mpTask)
{
  ENTRY("vIntHash"_Str);
  
  s48K sString;
  if ( ! mpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("hash"_Str);
  
  i8 iLength = sString.iLength();
  
  i8 iHash = XXH32(sString.s(),iLength,0);
  
  mpTask->m_pDataStack->vPushInt(iHash);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* < -- test top two integers on data stack for less than                     */
/*----------------------------------------------------------------------------*/
void Task::vIntLess(MP_Task mpTask)
{
  ENTRY("vIntLess"_Str);
  
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("<"_Str);
  
  r = i < j;
  
  mpTask->m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* <= -- test top two integers on data stack for less than equal to           */
/*----------------------------------------------------------------------------*/
void Task::vIntLessEqual(MP_Task mpTask)
{
  ENTRY("vIntLessEqual"_Str);
  
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("<="_Str);
  
  r = i <= j;
  
  mpTask->m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* % -- modulo of top integer by second integer on stack                      */
/*----------------------------------------------------------------------------*/
void Task::vIntModulo(MP_Task mpTask)
{
  ENTRY("vIntModulo"_Str);
  
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("%"_Str);
  
  r = MOD(i,j);
  
  mpTask->m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* * multiply second integer by top integer on stack                          */
/*----------------------------------------------------------------------------*/
void Task::vIntMultiply(MP_Task mpTask)
{
  ENTRY("vIntMultiply"_Str);
  
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("*"_Str);
  
  r = i * j;
  
  mpTask->m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* neg -- gegate top integer on stack                                         */
/*----------------------------------------------------------------------------*/
void Task::vIntNeg(MP_Task mpTask)
{
  ENTRY("vIntNeg"_Str);
  
  i8 r, i;
  
  if ( ! mpTask->m_pDataStack->bPop(i) )
    THROW_INVALID_DSTACK("neg"_Str);
  
  r = -1 * i;
  
  mpTask->m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* != -- test top two integers on data stack for not equal                    */
/*----------------------------------------------------------------------------*/
void Task::vIntNotEqual(MP_Task mpTask)
{
  ENTRY("vIntNotEqual"_Str);
  
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("!="_Str);
  
  r = i != j;
  
  mpTask->m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* - -- subtract second integer from top integer on stack                     */
/*----------------------------------------------------------------------------*/
void Task::vIntSubtract(MP_Task mpTask)
{
  ENTRY("vIntSubtract"_Str);
  
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("-"_Str);
  
  r = i - j;
  
  mpTask->m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
