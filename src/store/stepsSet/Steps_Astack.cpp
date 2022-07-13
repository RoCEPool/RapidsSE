/*============================================================================*/
/* Steps_Astack.cpp - aux stack step group                                    */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


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
  vAddStep("arot"_Stz,            &Step::vDstackRot);
  vAddStep("-arot"_Stz,           &Step::vDstackRotInv);
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
  
  SenCell xVariable;
  
   m_pAuxStack->vPeekItem(nTh,xVariable);
  
  eCN eClass = xVariable.eGetClassNo();
  
   m_pAuxStack->vPushInt(eClass);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adepth -- put number of values in aux stack onto data stack                */
/*----------------------------------------------------------------------------*/
void Step::vAstackDepth(void)
{
  ENTRY();
  
  i8 iDepth =  m_pAuxStack->nValues();
  
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
  
  SenCell xValA;
   m_pAuxStack->vPopTypeValue(xValA,__func__);
  
   m_pAuxStack->vPushTypeValue(xValA);
   m_pAuxStack->vPushTypeValue(xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a@ -- place copy of top value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstackFetch(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  SenCell xVariable;
  
   m_pAuxStack->vPeekItem(0,xVariable);
  
  m_pDataStack->vPushTypeValue(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nip -- drop the 2nd value from the aux stack ( B A -- A )                  */
/*----------------------------------------------------------------------------*/
void Step::vAstackNip(void)
{
  ENTRY();
  
  SenCell xValA;
   m_pAuxStack->vPopTypeValue(xValA,__func__);
  
  SenCell xValB;
   m_pAuxStack->vPopTypeValue(xValB,__func__);

   m_pAuxStack->vPushTypeValue(xValA);
  
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
  
  SenCell xVariable;
  
   m_pAuxStack->vPeekItem(iPick,xVariable);
  
  m_pDataStack->vPushTypeValue(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* areverse -- reverse astack values ( ... D C B A n -- A B C D ... )         */
/*----------------------------------------------------------------------------*/
void Step::vAstackReverse(void)
{
  ENTRY();
  
  SenCell xVal1;
  SenCell xVal2;
  SenCell xVal3;
  SenCell xVal4;
  SenCell xVal5;
  SenCell xVal6;
  SenCell xVal7;
  SenCell xVal8;
  
  i8 nValues = m_pDataStack->iPopInt(__func__);
  
  
  switch (nValues) {
    case 8:  m_pAuxStack->vPopTypeValue(xVal8,__func__);
    case 7:  m_pAuxStack->vPopTypeValue(xVal7,__func__);
    case 6:  m_pAuxStack->vPopTypeValue(xVal6,__func__);
    case 5:  m_pAuxStack->vPopTypeValue(xVal5,__func__);
    case 4:  m_pAuxStack->vPopTypeValue(xVal4,__func__);
    case 3:  m_pAuxStack->vPopTypeValue(xVal3,__func__);
    case 2:  m_pAuxStack->vPopTypeValue(xVal2,__func__);
    case 1:  m_pAuxStack->vPopTypeValue(xVal1,__func__);
  }
  
  switch (nValues) {
    case 8:  m_pAuxStack->vPushTypeValue(xVal8);
    case 7:  m_pAuxStack->vPushTypeValue(xVal7);
    case 6:  m_pAuxStack->vPushTypeValue(xVal6);
    case 5:  m_pAuxStack->vPushTypeValue(xVal5);
    case 4:  m_pAuxStack->vPushTypeValue(xVal4);
    case 3:  m_pAuxStack->vPushTypeValue(xVal3);
    case 2:  m_pAuxStack->vPushTypeValue(xVal2);
    case 1:  m_pAuxStack->vPushTypeValue(xVal1);
  }
}

/*----------------------------------------------------------------------------*/
/* arot -- rotate top 3 values clockwise (rotate C to top) ( C B A -- B A C ) */
/*----------------------------------------------------------------------------*/
void Step::vAstackRot(void)
{
  ENTRY();
  
  SenCell xVarA;
  m_pAuxStack->vPopTypeValue(xVarA,__func__);
  SenCell xVarB;
  m_pAuxStack->vPopTypeValue(xVarB,__func__);
  SenCell xVarC;
  m_pAuxStack->vPopTypeValue(xVarC,__func__);

  
  m_pAuxStack->vPushTypeValue(xVarB);
  m_pAuxStack->vPushTypeValue(xVarA);
  m_pAuxStack->vPushTypeValue(xVarC);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -arot -- rotate top 3 values anticlockwise (A to bottom) ( C B A -- A C B )*/
/*----------------------------------------------------------------------------*/
void Step::vAstackRotInv(void)
{
  ENTRY();
  
  SenCell xVarA;
  m_pAuxStack->vPopTypeValue(xVarA,__func__);
  SenCell xVarB;
  m_pAuxStack->vPopTypeValue(xVarB,__func__);
  SenCell xVarC;
  m_pAuxStack->vPopTypeValue(xVarC,__func__);

  m_pAuxStack->vPushTypeValue(xVarA);
  m_pAuxStack->vPushTypeValue(xVarC);
  m_pAuxStack->vPushTypeValue(xVarB);
  
  EXIT();
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
  
  SenCell xValA;
   m_pAuxStack->vPopTypeValue(xValA,__func__);
  
  SenCell xValB;
   m_pAuxStack->vPopTypeValue(xValB,__func__);

   m_pAuxStack->vPushTypeValue(xValA);
   m_pAuxStack->vPushTypeValue(xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >a -- move value from dstack to astack                                     */
/*----------------------------------------------------------------------------*/
void Step::vAstackToA(void)
{
  ENTRY();
  
  SenCell xValOpaque;
  m_pDataStack->vPopTypeValue(xValOpaque,__func__);
  
   m_pAuxStack->vPushTypeValue(xValOpaque);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a> -- move value from astack to dstack                                     */
/*----------------------------------------------------------------------------*/
void Step::vAstackToD(void)
{
  ENTRY();
  
  SenCell xVal;
   m_pAuxStack->vPopTypeValue(xVal,__func__);
  
  m_pDataStack->vPushTypeValue(xVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a0 -- place copy of 0th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack0(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  SenCell xVariable;
  
   m_pAuxStack->vPeekItem(0,xVariable);
  
  m_pDataStack->vPushTypeValue(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a1 -- place copy of 1st value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack1(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  SenCell xVariable;
  
   m_pAuxStack->vPeekItem(1,xVariable);
  
  m_pDataStack->vPushTypeValue(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r2 -- place copy of 2nd value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack2(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  SenCell xVariable;
  
   m_pAuxStack->vPeekItem(2,xVariable);
  
  m_pDataStack->vPushTypeValue(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r3 -- place copy of 3rd value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack3(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  SenCell xVariable;
  
   m_pAuxStack->vPeekItem(3,xVariable);
  
  m_pDataStack->vPushTypeValue(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r4 -- place copy of 4th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack4(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  SenCell xVariable;
  
   m_pAuxStack->vPeekItem(4,xVariable);
  
  m_pDataStack->vPushTypeValue(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r5 -- place copy of 0th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack5(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  SenCell xVariable;
  
   m_pAuxStack->vPeekItem(5,xVariable);
  
  m_pDataStack->vPushTypeValue(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r6 -- place copy of 6th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack6(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  SenCell xVariable;
  
   m_pAuxStack->vPeekItem(6,xVariable);
  
  m_pDataStack->vPushTypeValue(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r7 -- place copy of 7th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack7(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  SenCell xVariable;
  
   m_pAuxStack->vPeekItem(7,xVariable);
  
  m_pDataStack->vPushTypeValue(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r8 -- place copy of 8th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack8(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  SenCell xVariable;
  
   m_pAuxStack->vPeekItem(8,xVariable);
  
  m_pDataStack->vPushTypeValue(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r9 -- place copy of 9th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack9(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  SenCell xVariable;
  
   m_pAuxStack->vPeekItem(9,xVariable);
  
  m_pDataStack->vPushTypeValue(xVariable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
