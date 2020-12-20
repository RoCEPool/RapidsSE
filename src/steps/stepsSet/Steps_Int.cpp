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
  vAddStep("int.percent"_Stz,     &Step::vIntPercent);
  vAddStep("int.random"_Stz,      &Step::vIntRandom);
  vAddStep("uint.random"_Stz,     &Step::vUnsignedIntRandom);
}

/*----------------------------------------------------------------------------*/
/* + -- add top two integers on stack                                         */
/*----------------------------------------------------------------------------*/
void Step::vIntAdd(void)
{
  ENTRY();
  /*
  i8 r, j, i;
  
  if ( ! m_pDataStack->bPop(j,i) ) THROW_DSTACK();
  
  r = i + j;
  
  m_pDataStack->vPush(r);
  */
  
  // get pointer to value on the top of the stack
  TypedCell* j = (TypedCell*)m_pDataStack->aTopVal();
  
  // discard the value
  m_pDataStack->vDiscardTop();
  
  // get pointer to value on the top of the stack
  TypedCell* i = (TypedCell*)m_pDataStack->aTopVal();
  
  // add the two values and leave the result on the stack
  i->vInt(j->iInt() + i->iInt());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* / -- divide top integer by second integer on stack                         */
/*----------------------------------------------------------------------------*/
void Step::vIntDivide(void)
{
  ENTRY();
  
  i8 j = m_pDataStack->iPopInt(__func__);
  i8 i = m_pDataStack->iPopInt(__func__);

  i8 r = DIV(i,j);
  
  m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* == -- test top two integers on data stack for equality                     */
/*----------------------------------------------------------------------------*/
void Step::vIntEqual(void)
{
  ENTRY();
  
  i8 j = m_pDataStack->iPopInt(__func__);
  i8 i = m_pDataStack->iPopInt(__func__);

  i8 r = i == j;
  
  m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* > -- test top two integers on data stack for greater than                  */
/*----------------------------------------------------------------------------*/
void Step::vIntGreater(void)
{
  ENTRY();
  
  i8 j = m_pDataStack->iPopInt(__func__);
  i8 i = m_pDataStack->iPopInt(__func__);

  i8 r = i > j;
  
  m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >= -- test top two integers on data stack for greater than equal to        */
/*----------------------------------------------------------------------------*/
void Step::vIntGreaterEqual(void)
{
  ENTRY();
  
  i8 j = m_pDataStack->iPopInt(__func__);
  i8 i = m_pDataStack->iPopInt(__func__);

  i8 r = i >= j;
  
  m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* hash -- get hash of string                                                 */
/*----------------------------------------------------------------------------*/
void Step::vIntHash(void)
{
  ENTRY();
  
  s48K sString;
  m_pDataStack->vPopStr(sString,__func__);
  
  i8 nLength = sString.nLength();
  
  i8 iHash = HshLib::iHash(sString.ayByte(),nLength);
  
  m_pDataStack->vPushInt(iHash);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ho16 -- get high order 16 bits of int                                      */
/*----------------------------------------------------------------------------*/
void Step::vIntHo16(void)
{
  ENTRY();
  
  i8 i = m_pDataStack->iPopInt(__func__);
  
  m_pDataStack->vPushInt(i >> 48);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* < -- test top two integers on data stack for less than                     */
/*----------------------------------------------------------------------------*/
void Step::vIntLess(void)
{
  ENTRY();
  
  i8 j = m_pDataStack->iPopInt(__func__);
  i8 i = m_pDataStack->iPopInt(__func__);

  i8 r = i < j;
  
  m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* <= -- test top two integers on data stack for less than equal to           */
/*----------------------------------------------------------------------------*/
void Step::vIntLessEqual(void)
{
  ENTRY();
  
  i8 j = m_pDataStack->iPopInt(__func__);
  i8 i = m_pDataStack->iPopInt(__func__);

  i8 r = i <= j;
  
  m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* % -- modulo of top integer by second integer on stack                      */
/*----------------------------------------------------------------------------*/
void Step::vIntModulo(void)
{
  ENTRY();
  
  i8 j = m_pDataStack->iPopInt(__func__);
  i8 i = m_pDataStack->iPopInt(__func__);

  i8 r = MOD(i,j);
  
  m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* * multiply second integer by top integer on stack                          */
/*----------------------------------------------------------------------------*/
void Step::vIntMultiply(void)
{
  ENTRY();
  
  i8 j = m_pDataStack->iPopInt(__func__);
  i8 i = m_pDataStack->iPopInt(__func__);

  i8 r = i * j;
  
  m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* neg -- gegate top integer on stack                                         */
/*----------------------------------------------------------------------------*/
void Step::vIntNeg(void)
{
  ENTRY();
  
  i8 i = m_pDataStack->iPopInt(__func__);
  
 i8 r = -1 * i;
  
  m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* != -- test top two integers on data stack for not equal                    */
/*----------------------------------------------------------------------------*/
void Step::vIntNotEqual(void)
{
  ENTRY();
  
  i8 j = m_pDataStack->iPopInt(__func__);
  i8 i = m_pDataStack->iPopInt(__func__);

  i8 r = i != j;
  
  m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* int.percent -- canculate the percentage given two numbers                  */
/*----------------------------------------------------------------------------*/
void Step::vIntPercent(void)
{
  ENTRY();
  
  i8 j = m_pDataStack->iPopInt(__func__);
  
  i8 i = m_pDataStack->iPopInt(__func__);
  
  i8 iPercent = IntLib::iPercent(i,j);
  
  m_pDataStack->vPushInt(iPercent);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* int.random -- generate a random number in the given range                  */
/*----------------------------------------------------------------------------*/
void Step::vIntRandom(void)
{
  ENTRY();
  
  i8 iHigh = m_pDataStack->iPopInt(__func__);
  
  i8 iLow = m_pDataStack->iPopInt(__func__);
  
  i8 iRandom = RndLib::iInteger(iLow,iHigh);
  
  m_pDataStack->vPushInt(iRandom);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* - -- subtract second integer from top integer on stack                     */
/*----------------------------------------------------------------------------*/
void Step::vIntSubtract(void)
{
  ENTRY();
  
  i8 j = m_pDataStack->iPopInt(__func__);
  i8 i = m_pDataStack->iPopInt(__func__);

  i8 r = i - j;
  
  m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* uint.random -- generate a random number in the given range                  */
/*----------------------------------------------------------------------------*/
void Step::vUnsignedIntRandom(void)
{
  ENTRY();
  
  i8 iHigh = m_pDataStack->iPopInt(__func__);
  
  i8 iLow = m_pDataStack->iPopInt(__func__);
  
  u8 uRandom = RndLib::uInteger(iLow,iHigh);
  
  m_pDataStack->vPushInt(uRandom);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
