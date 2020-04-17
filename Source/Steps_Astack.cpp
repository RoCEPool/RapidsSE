/*============================================================================*/
/* Steps_Astack.cpp - aux stack step group                                    */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Astack(void)
{
  vAddStep("astack.class"_Stz,    &Step::vAstackClass);
  vAddStep("adepth"_Stz,          &Step::vAstackDepth);
  vAddStep("adiscard"_Stz,        &Step::vAstackDiscard);
  vAddStep("adup"_Stz,            &Step::vAstackDup);
  vAddStep("a@"_Stz,              &Step::vAstackFetch);
  vAddStep("anip"_Stz,            &Step::vAstackNip);
  vAddStep("apick"_Stz,           &Step::vAstackPick);
  vAddStep("areverse"_Stz,        &Step::vAstackReverse);
  vAddStep("astk"_Stz,            &Step::vAstackStk);
  vAddStep(">a"_Stz,              &Step::vAstackToA);
  vAddStep("a>"_Stz,              &Step::vAstackToD);
  vAddStep("aswap"_Stz,           &Step::vAstackSwap);
  vAddStep("a0"_Stz,              &Step::vAstack0);
  vAddStep("a1"_Stz,              &Step::vAstack1);
  vAddStep("a2"_Stz,              &Step::vAstack2);
  vAddStep("a3"_Stz,              &Step::vAstack3);
  vAddStep("a4"_Stz,              &Step::vAstack4);
  vAddStep("a5"_Stz,              &Step::vAstack5);
  vAddStep("a6"_Stz,              &Step::vAstack6);
  vAddStep("a7"_Stz,              &Step::vAstack7);
  vAddStep("a8"_Stz,              &Step::vAstack8);
  vAddStep("a9"_Stz,              &Step::vAstack9);
}

/*----------------------------------------------------------------------------*/
/* astack.class -- get class of nth value on aux stack                        */
/*----------------------------------------------------------------------------*/
void Step::vAstackClass(void)
{
  ENTRY();
  
  i8 nTh = m_pDataStack->iPopInt(__func__);
  
  Datum xVariable;
  
  m_pAuxStack->vPeekVal(nTh,xVariable);
  
  eCN eClass = xVariable.eClass();
  
  m_pAuxStack->vPushInt(eClass);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adepth -- put number of values in aux stack onto data stack                */
/*----------------------------------------------------------------------------*/
void Step::vAstackDepth(void)
{
  ENTRY();
  
  i8 iDepth = m_pAuxStack->nValues();
  
  m_pDataStack->vPushInt(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adrop -- discard top value off astack                                      */
/*----------------------------------------------------------------------------*/
void Step::vAstackDiscard(void)
{
  ENTRY();
  
  m_pAuxStack->vDiscardTop();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adup -- dup top of aux stack                                               */
/*----------------------------------------------------------------------------*/
void Step::vAstackDup(void)
{
  ENTRY();
  
  // could just copy from stack back into stack
  
  Datum xValA;
  m_pAuxStack->vPopVal(xValA,__func__);
  
  m_pAuxStack->vPushVal(xValA);
  m_pAuxStack->vPushVal(xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a@ -- place copy of top value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstackFetch(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  Datum xVariable;
  
  m_pAuxStack->vPeekVal(0,xVariable);
  
  m_pDataStack->vPushVal(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nip -- drop the 2nd value from the aux stack ( B A -- A )                  */
/*----------------------------------------------------------------------------*/
void Step::vAstackNip(void)
{
  ENTRY();
  
  Datum xValA;
  m_pAuxStack->vPopVal(xValA,__func__);
  
  Datum xValB;
  m_pAuxStack->vPopVal(xValB,__func__);

  m_pAuxStack->vPushVal(xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* apick -- place copy of nth value in aux stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vAstackPick(void)
{
  ENTRY();
  
  i8 iPick = m_pDataStack->iPopAsInt(__func__);
  
  // could do this directly on stack
  
  Datum xVariable;
  
  m_pAuxStack->vPeekVal(iPick,xVariable);
  
  m_pDataStack->vPushVal(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* areverse -- reverse astack values ( ... D C B A n -- A B C D ... )         */
/*----------------------------------------------------------------------------*/
void Step::vAstackReverse(void)
{
  ENTRY();
  
  Datum xVal1;
  Datum xVal2;
  Datum xVal3;
  Datum xVal4;
  Datum xVal5;
  Datum xVal6;
  Datum xVal7;
  Datum xVal8;
  
  i8 nValues = m_pDataStack->iPopInt(__func__);
  
  
  switch (nValues) {
    case 8: m_pAuxStack->vPopVal(xVal8,__func__);
    case 7: m_pAuxStack->vPopVal(xVal7,__func__);
    case 6: m_pAuxStack->vPopVal(xVal6,__func__);
    case 5: m_pAuxStack->vPopVal(xVal5,__func__);
    case 4: m_pAuxStack->vPopVal(xVal4,__func__);
    case 3: m_pAuxStack->vPopVal(xVal3,__func__);
    case 2: m_pAuxStack->vPopVal(xVal2,__func__);
    case 1: m_pAuxStack->vPopVal(xVal1,__func__);
  }
  
  switch (nValues) {
    case 8: m_pAuxStack->vPushVal(xVal8);
    case 7: m_pAuxStack->vPushVal(xVal7);
    case 6: m_pAuxStack->vPushVal(xVal6);
    case 5: m_pAuxStack->vPushVal(xVal5);
    case 4: m_pAuxStack->vPushVal(xVal4);
    case 3: m_pAuxStack->vPushVal(xVal3);
    case 2: m_pAuxStack->vPushVal(xVal2);
    case 1: m_pAuxStack->vPushVal(xVal1);
  }
}

/*----------------------------------------------------------------------------*/
/* astk -- dump the aux stack                                                 */
/*----------------------------------------------------------------------------*/
void Step::vAstackStk(void)
{
  ENTRY();
  
  m_pAuxStack->vDump(&m_oDicStack,"AuxStack:"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* swap -- swap top two values on aux stack                                   */
/*----------------------------------------------------------------------------*/
void Step::vAstackSwap(void)
{
  ENTRY();
  
  Datum xValA;
  m_pAuxStack->vPopVal(xValA,__func__);
  
  Datum xValB;
  m_pAuxStack->vPopVal(xValB,__func__);

  m_pAuxStack->vPushVal(xValA);
  m_pAuxStack->vPushVal(xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >a -- move value from dstack to astack                                     */
/*----------------------------------------------------------------------------*/
void Step::vAstackToA(void)
{
  ENTRY();
  
  Datum xValOpaque;
  m_pDataStack->vPopVal(xValOpaque,__func__);
  
  m_pAuxStack->vPushVal(xValOpaque);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a> -- move value from astack to dstack                                     */
/*----------------------------------------------------------------------------*/
void Step::vAstackToD(void)
{
  ENTRY();
  
  Datum xVal;
  m_pAuxStack->vPopVal(xVal,__func__);
  
  m_pDataStack->vPushVal(xVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a0 -- place copy of 0th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack0(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  Datum xVariable;
  
  m_pAuxStack->vPeekVal(0,xVariable);
  
  m_pDataStack->vPushVal(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a1 -- place copy of 1st value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack1(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  Datum xVariable;
  
  m_pAuxStack->vPeekVal(1,xVariable);
  
  m_pDataStack->vPushVal(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r2 -- place copy of 2nd value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack2(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  Datum xVariable;
  
  m_pAuxStack->vPeekVal(2,xVariable);
  
  m_pDataStack->vPushVal(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r3 -- place copy of 3rd value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack3(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  Datum xVariable;
  
  m_pAuxStack->vPeekVal(3,xVariable);
  
  m_pDataStack->vPushVal(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r4 -- place copy of 4th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack4(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  Datum xVariable;
  
  m_pAuxStack->vPeekVal(4,xVariable);
  
  m_pDataStack->vPushVal(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r5 -- place copy of 0th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack5(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  Datum xVariable;
  
  m_pAuxStack->vPeekVal(5,xVariable);
  
  m_pDataStack->vPushVal(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r6 -- place copy of 6th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack6(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  Datum xVariable;
  
  m_pAuxStack->vPeekVal(6,xVariable);
  
  m_pDataStack->vPushVal(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r7 -- place copy of 7th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack7(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  Datum xVariable;
  
  m_pAuxStack->vPeekVal(7,xVariable);
  
  m_pDataStack->vPushVal(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r8 -- place copy of 8th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack8(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  Datum xVariable;
  
  m_pAuxStack->vPeekVal(8,xVariable);
  
  m_pDataStack->vPushVal(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r9 -- place copy of 9th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack9(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  Datum xVariable;
  
  m_pAuxStack->vPeekVal(9,xVariable);
  
  m_pDataStack->vPushVal(xVariable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
