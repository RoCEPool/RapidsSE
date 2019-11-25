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
void Step::vIntAdd(void)
{
  ENTRY(); // ENTRY("vIntAdd"_Stz);
  /*
  i8 r, j, i;
  
  if ( ! m_pDataStack->bPop(j,i) )
    THROW_DSTACK();
  
  r = i + j;
  
  m_pDataStack->vPush(r);
  */
  
  // get pointer to value on the top of the stack
  StkVal* j = m_pDataStack->aTopStkVal();
  
  // discard the value
  m_pDataStack->vDiscard();
  
  // get pointer to value on the top of the stack
  StkVal* i = m_pDataStack->aTopStkVal();
  
  // add the two values and leave the result on the stack
  *_aIntVal(i) = _aIntVal(j)->iInt() + _aIntVal(i)->iInt();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* / -- divide top integer by second integer on stack                         */
/*----------------------------------------------------------------------------*/
void Step::vIntDivide(void)
{
  ENTRY(); // ENTRY("vIntDivide"_Stz);
  
  i8 r, j, i;
  
  if ( ! m_pDataStack->bPopI8(j,i) )
    THROW_DSTACK();
  
  r = DIV(i,j);
  
  m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* == -- test top two integers on data stack for equality                     */
/*----------------------------------------------------------------------------*/
void Step::vIntEqual(void)
{
  ENTRY(); // ENTRY("vIntEqual"_Stz);
  
  i8 r, j, i;
  
  if ( ! m_pDataStack->bPopI8(j,i) )
    THROW_DSTACK();
  
  r = i == j;
  
  m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* > -- test top two integers on data stack for greater than                  */
/*----------------------------------------------------------------------------*/
void Step::vIntGreater(void)
{
  ENTRY(); // ENTRY("vIntGreater"_Stz);
  
  i8 r, j, i;
  
  if ( ! m_pDataStack->bPopI8(j,i) )
    THROW_DSTACK();
  
  r = i > j;
  
  m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >= -- test top two integers on data stack for greater than equal to        */
/*----------------------------------------------------------------------------*/
void Step::vIntGreaterEqual(void)
{
  ENTRY(); // ENTRY("vIntGreaterEqual"_Stz);
  
  i8 r, j, i;
  
  if ( ! m_pDataStack->bPopI8(j,i) )
    THROW_DSTACK();
  
  r = i >= j;
  
  m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* hash -- get hash of string                                                 */
/*----------------------------------------------------------------------------*/
void Step::vIntHash(void)
{
  ENTRY(); // ENTRY("vIntHash"_Stz);
  
  s48K sString;
  if ( ! m_pDataStack->bPopS48K(sString) )
    THROW_DSTACK();
  
  i8 nLength = sString.nLength();
  
  i8 iHash = HshLib::iHash(sString.ayByte(),nLength);
  
  m_pDataStack->vPushI8(iHash);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ho16 -- get high order 16 bits of int                                      */
/*----------------------------------------------------------------------------*/
void Step::vIntHo16(void)
{
  ENTRY(); // ENTRY("vIntHash"_Stz);
  
  i8 i;
  if ( ! m_pDataStack->bPopI8(i) )
    THROW_DSTACK();
  
  m_pDataStack->vPushI8(i >> 48);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* < -- test top two integers on data stack for less than                     */
/*----------------------------------------------------------------------------*/
void Step::vIntLess(void)
{
  ENTRY(); // ENTRY("vIntLess"_Stz);
  
  i8 r, j, i;
  
  if ( ! m_pDataStack->bPopI8(j,i) )
    THROW_DSTACK();
  
  r = i < j;
  
  m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* <= -- test top two integers on data stack for less than equal to           */
/*----------------------------------------------------------------------------*/
void Step::vIntLessEqual(void)
{
  ENTRY(); // ENTRY("vIntLessEqual"_Stz);
  
  i8 r, j, i;
  
  if ( ! m_pDataStack->bPopI8(j,i) )
    THROW_DSTACK();
  
  r = i <= j;
  
  m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* % -- modulo of top integer by second integer on stack                      */
/*----------------------------------------------------------------------------*/
void Step::vIntModulo(void)
{
  ENTRY(); // ENTRY("vIntModulo"_Stz);
  
  i8 r, j, i;
  
  if ( ! m_pDataStack->bPopI8(j,i) )
    THROW_DSTACK();
  
  r = MOD(i,j);
  
  m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* * multiply second integer by top integer on stack                          */
/*----------------------------------------------------------------------------*/
void Step::vIntMultiply(void)
{
  ENTRY(); // ENTRY("vIntMultiply"_Stz);
  
  i8 r, j, i;
  
  if ( ! m_pDataStack->bPopI8(j,i) )
    THROW_DSTACK();
  
  r = i * j;
  
  m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* neg -- gegate top integer on stack                                         */
/*----------------------------------------------------------------------------*/
void Step::vIntNeg(void)
{
  ENTRY(); // ENTRY("vIntNeg"_Stz);
  
  i8 r, i;
  
  if ( ! m_pDataStack->bPopI8(i) )
    THROW_DSTACK();
  
  r = -1 * i;
  
  m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* != -- test top two integers on data stack for not equal                    */
/*----------------------------------------------------------------------------*/
void Step::vIntNotEqual(void)
{
  ENTRY(); // ENTRY("vIntNotEqual"_Stz);
  
  i8 r, j, i;
  
  if ( ! m_pDataStack->bPopI8(j,i) )
    THROW_DSTACK();
  
  r = i != j;
  
  m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* int.random -- generate a random number in the given range                  */
/*----------------------------------------------------------------------------*/
void Step::vIntRandom(void)
{
  ENTRY(); // ENTRY("vIntRandom"_Stz);
  
  i8 iHigh;
  if ( ! m_pDataStack->bPopI8(iHigh) )
    THROW_DSTACK();
  
  i8 iLow;
  if ( ! m_pDataStack->bPopI8(iLow) )
    THROW_DSTACK();
  
  i8 iRandom = Random::iInteger(iLow,iHigh);
  
  m_pDataStack->vPushI8(iRandom);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* - -- subtract second integer from top integer on stack                     */
/*----------------------------------------------------------------------------*/
void Step::vIntSubtract(void)
{
  ENTRY(); // ENTRY("vIntSubtract"_Stz);
  
  i8 r, j, i;
  
  if ( ! m_pDataStack->bPopI8(j,i) )
    THROW_DSTACK();
  
  r = i - j;
  
  m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* uint.random -- generate a random number in the given range                  */
/*----------------------------------------------------------------------------*/
void Step::vUnsignedIntRandom(void)
{
  ENTRY(); // ENTRY("vUnsignedIntRandom"_Stz);
  
  i8 iHigh;
  if ( ! m_pDataStack->bPopI8(iHigh) )
    THROW_DSTACK();
  
  i8 iLow;
  if ( ! m_pDataStack->bPopI8(iLow) )
    THROW_DSTACK();
  
  u8 uRandom = Random::uInteger(iLow,iHigh);
  
  m_pDataStack->vPushI8(uRandom);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
