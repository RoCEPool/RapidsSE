/*============================================================================*/
/* Steps_Int.cpp - int step group                                             */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Int(void)
{
  vAddStep("+"_Stz,               &Step::vIntAdd);
  vAddStep("/"_Stz,               &Step::vIntDivide);
  vAddStep("=="_Stz,              &Step::vIntEqual);
  vAddStep(">"_Stz,               &Step::vIntGreater);
  vAddStep(">="_Stz,              &Step::vIntGreaterEqual);
  vAddStep("hash"_Stz,            &Step::vIntHash);
  vAddStep("ho16"_Stz,            &Step::vIntHo16);
  vAddStep("<"_Stz,               &Step::vIntLess);
  vAddStep("<="_Stz,              &Step::vIntLessEqual);
  vAddStep("%"_Stz,               &Step::vIntModulo);
  vAddStep("*"_Stz,               &Step::vIntMultiply);
  vAddStep("neg"_Stz,             &Step::vIntNeg);
  vAddStep("!="_Stz,              &Step::vIntNotEqual);
  vAddStep("-"_Stz,               &Step::vIntSubtract);
  vAddStep("int.random"_Stz,      &Step::vIntRandom);
  vAddStep("uint.random"_Stz,     &Step::vUnsignedIntRandom);
}

/*----------------------------------------------------------------------------*/
/* + -- add top two integers on stack                                         */
/*----------------------------------------------------------------------------*/
void Step::vIntAdd(MP_Task mpTask)
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
  ForthValue* j = mpTask->m_pDataStack->aPeekValue();
  
  // discard the value
  mpTask->m_pDataStack->vDiscard();
  
  // get pointer to value on the top of the stack
  ForthValue* i = mpTask->m_pDataStack->aPeekValue();
  
  // add the two values and leave the result on the stack
  *_aIntVal(i) = _aIntVal(j)->iInt() + _aIntVal(i)->iInt();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* / -- divide top integer by second integer on stack                         */
/*----------------------------------------------------------------------------*/
void Step::vIntDivide(MP_Task mpTask)
{
  ENTRY("vIntDivide"_Stz);
  
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("/"_Stz);
  
  r = DIV(i,j);
  
  mpTask->m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* == -- test top two integers on data stack for equality                     */
/*----------------------------------------------------------------------------*/
void Step::vIntEqual(MP_Task mpTask)
{
  ENTRY("vIntEqual"_Stz);
  
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("=="_Stz);
  
  r = i == j;
  
  mpTask->m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* > -- test top two integers on data stack for greater than                  */
/*----------------------------------------------------------------------------*/
void Step::vIntGreater(MP_Task mpTask)
{
  ENTRY("vIntGreater"_Stz);
  
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK(">"_Stz);
  
  r = i > j;
  
  mpTask->m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >= -- test top two integers on data stack for greater than equal to        */
/*----------------------------------------------------------------------------*/
void Step::vIntGreaterEqual(MP_Task mpTask)
{
  ENTRY("vIntGreaterEqual"_Stz);
  
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK(">="_Stz);
  
  r = i >= j;
  
  mpTask->m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* hash -- get hash of string                                                 */
/*----------------------------------------------------------------------------*/
void Step::vIntHash(MP_Task mpTask)
{
  ENTRY("vIntHash"_Stz);
  
  s48K sString;
  if ( ! mpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("hash"_Stz);
  
  i8 iLength = sString.iLength();
  
  i8 iHash = HshLib::iHash((byte*)sString.aChar(),iLength);
  
  mpTask->m_pDataStack->vPushI8(iHash);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ho16 -- get high order 16 bits of int                                      */
/*----------------------------------------------------------------------------*/
void Step::vIntHo16(MP_Task mpTask)
{
  ENTRY("vIntHash"_Stz);
  
  i8 i;
  if ( ! mpTask->m_pDataStack->bPop(i) )
    THROW_INVALID_DSTACK(">="_Stz);
  
  mpTask->m_pDataStack->vPushI8(i >> 48);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* < -- test top two integers on data stack for less than                     */
/*----------------------------------------------------------------------------*/
void Step::vIntLess(MP_Task mpTask)
{
  ENTRY("vIntLess"_Stz);
  
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("<"_Stz);
  
  r = i < j;
  
  mpTask->m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* <= -- test top two integers on data stack for less than equal to           */
/*----------------------------------------------------------------------------*/
void Step::vIntLessEqual(MP_Task mpTask)
{
  ENTRY("vIntLessEqual"_Stz);
  
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("<="_Stz);
  
  r = i <= j;
  
  mpTask->m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* % -- modulo of top integer by second integer on stack                      */
/*----------------------------------------------------------------------------*/
void Step::vIntModulo(MP_Task mpTask)
{
  ENTRY("vIntModulo"_Stz);
  
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("%"_Stz);
  
  r = MOD(i,j);
  
  mpTask->m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* * multiply second integer by top integer on stack                          */
/*----------------------------------------------------------------------------*/
void Step::vIntMultiply(MP_Task mpTask)
{
  ENTRY("vIntMultiply"_Stz);
  
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("*"_Stz);
  
  r = i * j;
  
  mpTask->m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* neg -- gegate top integer on stack                                         */
/*----------------------------------------------------------------------------*/
void Step::vIntNeg(MP_Task mpTask)
{
  ENTRY("vIntNeg"_Stz);
  
  i8 r, i;
  
  if ( ! mpTask->m_pDataStack->bPop(i) )
    THROW_INVALID_DSTACK("neg"_Stz);
  
  r = -1 * i;
  
  mpTask->m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* != -- test top two integers on data stack for not equal                    */
/*----------------------------------------------------------------------------*/
void Step::vIntNotEqual(MP_Task mpTask)
{
  ENTRY("vIntNotEqual"_Stz);
  
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("!="_Stz);
  
  r = i != j;
  
  mpTask->m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* int.random -- generate a random number in the given range                  */
/*----------------------------------------------------------------------------*/
void Step::vIntRandom(MP_Task mpTask)
{
  ENTRY("vIntRandom"_Stz);
  
  i8 iHigh;
  if ( ! mpTask->m_pDataStack->bPop(iHigh) )
    THROW_INVALID_DSTACK("int.random"_Stz);
  
  i8 iLow;
  if ( ! mpTask->m_pDataStack->bPop(iLow) )
    THROW_INVALID_DSTACK("int.random"_Stz);
  
  i8 iRandom = Random::iInteger(iLow,iHigh);
  
  mpTask->m_pDataStack->vPushI8(iRandom);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* - -- subtract second integer from top integer on stack                     */
/*----------------------------------------------------------------------------*/
void Step::vIntSubtract(MP_Task mpTask)
{
  ENTRY("vIntSubtract"_Stz);
  
  i8 r, j, i;
  
  if ( ! mpTask->m_pDataStack->bPop(j,i) )
    THROW_INVALID_DSTACK("-"_Stz);
  
  r = i - j;
  
  mpTask->m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* uint.random -- generate a random number in the given range                  */
/*----------------------------------------------------------------------------*/
void Step::vUnsignedIntRandom(MP_Task mpTask)
{
  ENTRY("vUnsignedIntRandom"_Stz);
  
  i8 iHigh;
  if ( ! mpTask->m_pDataStack->bPop(iHigh) )
    THROW_INVALID_DSTACK("uint.random"_Stz);
  
  i8 iLow;
  if ( ! mpTask->m_pDataStack->bPop(iLow) )
    THROW_INVALID_DSTACK("uint.random"_Stz);
  
  u8 uRandom = Random::uInteger(iLow,iHigh);
  
  mpTask->m_pDataStack->vPushI8(uRandom);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
