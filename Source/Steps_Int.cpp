/*============================================================================*/
/* Steps_Int.cpp - int step group                                             */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// int: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Int(void)
{
  vAddStep("+"_Stz,               &Task::vIntAdd,           false);
  vAddStep("/"_Stz,               &Task::vIntDivide,        false);
  vAddStep("=="_Stz,              &Task::vIntEqual,         false);
  vAddStep(">"_Stz,               &Task::vIntGreater,       false);
  vAddStep(">="_Stz,              &Task::vIntGreaterEqual,  false);
  vAddStep("hash"_Stz,            &Task::vIntHash,          false);
  vAddStep("ho16"_Stz,            &Task::vIntHo16,          false);
  vAddStep("<"_Stz,               &Task::vIntLess,          false);
  vAddStep("<="_Stz,              &Task::vIntLessEqual,     false);
  vAddStep("%"_Stz,               &Task::vIntModulo,        false);
  vAddStep("*"_Stz,               &Task::vIntMultiply,      false);
  vAddStep("neg"_Stz,             &Task::vIntNeg,           false);
  vAddStep("!="_Stz,              &Task::vIntNotEqual,      false);
  vAddStep("-"_Stz,               &Task::vIntSubtract,      false);
}

/*----------------------------------------------------------------------------*/
/* + -- add top two integers on stack                                         */
/*----------------------------------------------------------------------------*/
void Task::vIntAdd(MP_Task mpTask)
{
  ENTRY("vIntAdd"_Stz);
  /*
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("+"_Stz);
  
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
  ENTRY("vIntDivide"_Stz);
  
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("/"_Stz);
  
  r = DIV(i,j);
  
  mpTask->m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* == -- test top two integers on data stack for equality                     */
/*----------------------------------------------------------------------------*/
void Task::vIntEqual(MP_Task mpTask)
{
  ENTRY("vIntEqual"_Stz);
  
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("=="_Stz);
  
  r = i == j;
  
  mpTask->m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* > -- test top two integers on data stack for greater than                  */
/*----------------------------------------------------------------------------*/
void Task::vIntGreater(MP_Task mpTask)
{
  ENTRY("vIntGreater"_Stz);
  
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK(">"_Stz);
  
  r = i > j;
  
  mpTask->m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >= -- test top two integers on data stack for greater than equal to        */
/*----------------------------------------------------------------------------*/
void Task::vIntGreaterEqual(MP_Task mpTask)
{
  ENTRY("vIntGreaterEqual"_Stz);
  
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK(">="_Stz);
  
  r = i >= j;
  
  mpTask->m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* hash -- get hash of string                                                 */
/*----------------------------------------------------------------------------*/
void Task::vIntHash(MP_Task mpTask)
{
  ENTRY("vIntHash"_Stz);
  
  s48K sString;
  if ( ! mpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("hash"_Stz);
  
  i8 iLength = sString.iLength();
  
  i8 iHash = XXH32(sString.aChar(),iLength,0);
  
  mpTask->m_pDataStack->vPushInt(iHash);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ho16 -- get high order 16 bits of int                                      */
/*----------------------------------------------------------------------------*/
void Task::vIntHo16(MP_Task mpTask)
{
  ENTRY("vIntHash"_Stz);
  
  i8 i;
  if ( ! mpTask->m_pDataStack->bPop(i) )
    THROW_INVALID_DSTACK(">="_Stz);
  
  mpTask->m_pDataStack->vPushInt(i >> 48);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* < -- test top two integers on data stack for less than                     */
/*----------------------------------------------------------------------------*/
void Task::vIntLess(MP_Task mpTask)
{
  ENTRY("vIntLess"_Stz);
  
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("<"_Stz);
  
  r = i < j;
  
  mpTask->m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* <= -- test top two integers on data stack for less than equal to           */
/*----------------------------------------------------------------------------*/
void Task::vIntLessEqual(MP_Task mpTask)
{
  ENTRY("vIntLessEqual"_Stz);
  
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("<="_Stz);
  
  r = i <= j;
  
  mpTask->m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* % -- modulo of top integer by second integer on stack                      */
/*----------------------------------------------------------------------------*/
void Task::vIntModulo(MP_Task mpTask)
{
  ENTRY("vIntModulo"_Stz);
  
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("%"_Stz);
  
  r = MOD(i,j);
  
  mpTask->m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* * multiply second integer by top integer on stack                          */
/*----------------------------------------------------------------------------*/
void Task::vIntMultiply(MP_Task mpTask)
{
  ENTRY("vIntMultiply"_Stz);
  
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("*"_Stz);
  
  r = i * j;
  
  mpTask->m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* neg -- gegate top integer on stack                                         */
/*----------------------------------------------------------------------------*/
void Task::vIntNeg(MP_Task mpTask)
{
  ENTRY("vIntNeg"_Stz);
  
  i8 r, i;
  
  if ( ! mpTask->m_pDataStack->bPop(i) )
    THROW_INVALID_DSTACK("neg"_Stz);
  
  r = -1 * i;
  
  mpTask->m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* != -- test top two integers on data stack for not equal                    */
/*----------------------------------------------------------------------------*/
void Task::vIntNotEqual(MP_Task mpTask)
{
  ENTRY("vIntNotEqual"_Stz);
  
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("!="_Stz);
  
  r = i != j;
  
  mpTask->m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* - -- subtract second integer from top integer on stack                     */
/*----------------------------------------------------------------------------*/
void Task::vIntSubtract(MP_Task mpTask)
{
  ENTRY("vIntSubtract"_Stz);
  
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("-"_Stz);
  
  r = i - j;
  
  mpTask->m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
