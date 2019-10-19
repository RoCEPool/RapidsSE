/*============================================================================*/
/* Steps_Int.cpp - int step group                                             */
/*============================================================================*/

#include "Steps.h"

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
void Step::vIntAdd(Forth* pForth)
{
  ENTRY("vIntAdd"_Stz);
  /*
  i8 r, j, i;
  
  if ( ! pForth->m_pDataStack->bPop(j,i) )
    THROW_DSTACK();
  
  r = i + j;
  
  pForth->m_pDataStack->vPush(r);
  */
  
  // get pointer to value on the top of the stack
  ForthValue* j = pForth->m_pDataStack->aPeekValue();
  
  // discard the value
  pForth->m_pDataStack->vDiscard();
  
  // get pointer to value on the top of the stack
  ForthValue* i = pForth->m_pDataStack->aPeekValue();
  
  // add the two values and leave the result on the stack
  *_aIntVal(i) = _aIntVal(j)->iInt() + _aIntVal(i)->iInt();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* / -- divide top integer by second integer on stack                         */
/*----------------------------------------------------------------------------*/
void Step::vIntDivide(Forth* pForth)
{
  ENTRY("vIntDivide"_Stz);
  
  i8 r, j, i;
  
  if ( ! pForth->m_pDataStack->bPop(j,i) )
    THROW_DSTACK();
  
  r = DIV(i,j);
  
  pForth->m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* == -- test top two integers on data stack for equality                     */
/*----------------------------------------------------------------------------*/
void Step::vIntEqual(Forth* pForth)
{
  ENTRY("vIntEqual"_Stz);
  
  i8 r, j, i;
  
  if ( ! pForth->m_pDataStack->bPop(j,i) )
    THROW_DSTACK();
  
  r = i == j;
  
  pForth->m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* > -- test top two integers on data stack for greater than                  */
/*----------------------------------------------------------------------------*/
void Step::vIntGreater(Forth* pForth)
{
  ENTRY("vIntGreater"_Stz);
  
  i8 r, j, i;
  
  if ( ! pForth->m_pDataStack->bPop(j,i) )
    THROW_DSTACK();
  
  r = i > j;
  
  pForth->m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >= -- test top two integers on data stack for greater than equal to        */
/*----------------------------------------------------------------------------*/
void Step::vIntGreaterEqual(Forth* pForth)
{
  ENTRY("vIntGreaterEqual"_Stz);
  
  i8 r, j, i;
  
  if ( ! pForth->m_pDataStack->bPop(j,i) )
    THROW_DSTACK();
  
  r = i >= j;
  
  pForth->m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* hash -- get hash of string                                                 */
/*----------------------------------------------------------------------------*/
void Step::vIntHash(Forth* pForth)
{
  ENTRY("vIntHash"_Stz);
  
  s48K sString;
  if ( ! pForth->m_pDataStack->bPop(sString) )
    THROW_DSTACK();
  
  i8 nLength = sString.nLength();
  
  i8 iHash = HshLib::iHash(sString.ayByte(),nLength);
  
  pForth->m_pDataStack->vPushI8(iHash);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ho16 -- get high order 16 bits of int                                      */
/*----------------------------------------------------------------------------*/
void Step::vIntHo16(Forth* pForth)
{
  ENTRY("vIntHash"_Stz);
  
  i8 i;
  if ( ! pForth->m_pDataStack->bPop(i) )
    THROW_DSTACK();
  
  pForth->m_pDataStack->vPushI8(i >> 48);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* < -- test top two integers on data stack for less than                     */
/*----------------------------------------------------------------------------*/
void Step::vIntLess(Forth* pForth)
{
  ENTRY("vIntLess"_Stz);
  
  i8 r, j, i;
  
  if ( ! pForth->m_pDataStack->bPop(j,i) )
    THROW_DSTACK();
  
  r = i < j;
  
  pForth->m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* <= -- test top two integers on data stack for less than equal to           */
/*----------------------------------------------------------------------------*/
void Step::vIntLessEqual(Forth* pForth)
{
  ENTRY("vIntLessEqual"_Stz);
  
  i8 r, j, i;
  
  if ( ! pForth->m_pDataStack->bPop(j,i) )
    THROW_DSTACK();
  
  r = i <= j;
  
  pForth->m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* % -- modulo of top integer by second integer on stack                      */
/*----------------------------------------------------------------------------*/
void Step::vIntModulo(Forth* pForth)
{
  ENTRY("vIntModulo"_Stz);
  
  i8 r, j, i;
  
  if ( ! pForth->m_pDataStack->bPop(j,i) )
    THROW_DSTACK();
  
  r = MOD(i,j);
  
  pForth->m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* * multiply second integer by top integer on stack                          */
/*----------------------------------------------------------------------------*/
void Step::vIntMultiply(Forth* pForth)
{
  ENTRY("vIntMultiply"_Stz);
  
  i8 r, j, i;
  
  if ( ! pForth->m_pDataStack->bPop(j,i) )
    THROW_DSTACK();
  
  r = i * j;
  
  pForth->m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* neg -- gegate top integer on stack                                         */
/*----------------------------------------------------------------------------*/
void Step::vIntNeg(Forth* pForth)
{
  ENTRY("vIntNeg"_Stz);
  
  i8 r, i;
  
  if ( ! pForth->m_pDataStack->bPop(i) )
    THROW_DSTACK();
  
  r = -1 * i;
  
  pForth->m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* != -- test top two integers on data stack for not equal                    */
/*----------------------------------------------------------------------------*/
void Step::vIntNotEqual(Forth* pForth)
{
  ENTRY("vIntNotEqual"_Stz);
  
  i8 r, j, i;
  
  if ( ! pForth->m_pDataStack->bPop(j,i) )
    THROW_DSTACK();
  
  r = i != j;
  
  pForth->m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* int.random -- generate a random number in the given range                  */
/*----------------------------------------------------------------------------*/
void Step::vIntRandom(Forth* pForth)
{
  ENTRY("vIntRandom"_Stz);
  
  i8 iHigh;
  if ( ! pForth->m_pDataStack->bPop(iHigh) )
    THROW_DSTACK();
  
  i8 iLow;
  if ( ! pForth->m_pDataStack->bPop(iLow) )
    THROW_DSTACK();
  
  i8 iRandom = Random::iInteger(iLow,iHigh);
  
  pForth->m_pDataStack->vPushI8(iRandom);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* - -- subtract second integer from top integer on stack                     */
/*----------------------------------------------------------------------------*/
void Step::vIntSubtract(Forth* pForth)
{
  ENTRY("vIntSubtract"_Stz);
  
  i8 r, j, i;
  
  if ( ! pForth->m_pDataStack->bPop(j,i) )
    THROW_DSTACK();
  
  r = i - j;
  
  pForth->m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* uint.random -- generate a random number in the given range                  */
/*----------------------------------------------------------------------------*/
void Step::vUnsignedIntRandom(Forth* pForth)
{
  ENTRY("vUnsignedIntRandom"_Stz);
  
  i8 iHigh;
  if ( ! pForth->m_pDataStack->bPop(iHigh) )
    THROW_DSTACK();
  
  i8 iLow;
  if ( ! pForth->m_pDataStack->bPop(iLow) )
    THROW_DSTACK();
  
  u8 uRandom = Random::uInteger(iLow,iHigh);
  
  pForth->m_pDataStack->vPushI8(uRandom);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
