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
  
  i8 nTh = m_oDataStack.iPopInt(__func__);
  
  SenCell xVariable;
  
   m_oAuxStack.vPeekTypeValueArea(nTh, xVariable);
  
  eCN eClass = xVariable.eGetClassNo();
  
   m_oAuxStack.vPushInt(eClass);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adepth -- put number of values in aux stack onto data stack                */
/*----------------------------------------------------------------------------*/
void Step::vAstackDepth(void)
{
  ENTRY();
  
  i8 iDepth =  m_oAuxStack.nValues();
  
  m_oDataStack.vPushInt(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adrop -- discard top value off astack                                      */
/*----------------------------------------------------------------------------*/
void Step::vAstackDiscard(void)
{
  ENTRY();
  
   m_oAuxStack.vDiscardTop();
  
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
   m_oAuxStack.vPopSenCell(xValA, __func__);
  
   m_oAuxStack.vPushSenCell(xValA);
   m_oAuxStack.vPushSenCell(xValA);
  
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
  
   m_oAuxStack.vPeekTypeValueArea(0, xVariable);
  
  m_oDataStack.vPushSenCell(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nip -- drop the 2nd value from the aux stack ( B A -- A )                  */
/*----------------------------------------------------------------------------*/
void Step::vAstackNip(void)
{
  ENTRY();
  
  SenCell xValA;
   m_oAuxStack.vPopSenCell(xValA, __func__);
  
  SenCell xValB;
   m_oAuxStack.vPopSenCell(xValB, __func__);

   m_oAuxStack.vPushSenCell(xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* apick -- place copy of nth value in aux stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vAstackPick(void)
{
  ENTRY();
  
  i8 iPick = m_oDataStack.iPopAsInt(__func__);
  
  // could do this directly on stack
  
  SenCell xVariable;
  
   m_oAuxStack.vPeekTypeValueArea(iPick, xVariable);
  
  m_oDataStack.vPushSenCell(xVariable);
  
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
  
  i8 nValues = m_oDataStack.iPopInt(__func__);

  // Note: the [[gnu::fallthrough]]; statement needs to occur before the next case
  // label. This will suppress the -Wimplicit-fallthrough warning.
  // The semicolon at the end is significant.
  // If/when we adopt C++17 then we can use [[fallthrough]]; instead.
  
  switch (nValues) {
    case 8:  m_oAuxStack.vPopSenCell(xVal8, __func__);  [[gnu::fallthrough]];
    case 7:  m_oAuxStack.vPopSenCell(xVal7, __func__);  [[gnu::fallthrough]];
    case 6:  m_oAuxStack.vPopSenCell(xVal6, __func__);  [[gnu::fallthrough]];
    case 5:  m_oAuxStack.vPopSenCell(xVal5, __func__);  [[gnu::fallthrough]];
    case 4:  m_oAuxStack.vPopSenCell(xVal4, __func__);  [[gnu::fallthrough]];
    case 3:  m_oAuxStack.vPopSenCell(xVal3, __func__);  [[gnu::fallthrough]];
    case 2:  m_oAuxStack.vPopSenCell(xVal2, __func__);  [[gnu::fallthrough]];
    case 1:  m_oAuxStack.vPopSenCell(xVal1, __func__);
  }
  
  switch (nValues) {
    case 8:  m_oAuxStack.vPushSenCell(xVal8);  [[gnu::fallthrough]];
    case 7:  m_oAuxStack.vPushSenCell(xVal7);  [[gnu::fallthrough]];
    case 6:  m_oAuxStack.vPushSenCell(xVal6);  [[gnu::fallthrough]];
    case 5:  m_oAuxStack.vPushSenCell(xVal5);  [[gnu::fallthrough]];
    case 4:  m_oAuxStack.vPushSenCell(xVal4);  [[gnu::fallthrough]];
    case 3:  m_oAuxStack.vPushSenCell(xVal3);  [[gnu::fallthrough]];
    case 2:  m_oAuxStack.vPushSenCell(xVal2);  [[gnu::fallthrough]];
    case 1:  m_oAuxStack.vPushSenCell(xVal1);
  }
}

/*----------------------------------------------------------------------------*/
/* arot -- rotate top 3 values clockwise (rotate C to top) ( C B A -- B A C ) */
/*----------------------------------------------------------------------------*/
void Step::vAstackRot(void)
{
  ENTRY();
  
  SenCell xVarA;
  m_oAuxStack.vPopSenCell(xVarA, __func__);
  SenCell xVarB;
  m_oAuxStack.vPopSenCell(xVarB, __func__);
  SenCell xVarC;
  m_oAuxStack.vPopSenCell(xVarC, __func__);

  
  m_oAuxStack.vPushSenCell(xVarB);
  m_oAuxStack.vPushSenCell(xVarA);
  m_oAuxStack.vPushSenCell(xVarC);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -arot -- rotate top 3 values anticlockwise (A to bottom) ( C B A -- A C B )*/
/*----------------------------------------------------------------------------*/
void Step::vAstackRotInv(void)
{
  ENTRY();
  
  SenCell xVarA;
  m_oAuxStack.vPopSenCell(xVarA, __func__);
  SenCell xVarB;
  m_oAuxStack.vPopSenCell(xVarB, __func__);
  SenCell xVarC;
  m_oAuxStack.vPopSenCell(xVarC, __func__);

  m_oAuxStack.vPushSenCell(xVarA);
  m_oAuxStack.vPushSenCell(xVarC);
  m_oAuxStack.vPushSenCell(xVarB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* astk -- dump the aux stack                                                 */
/*----------------------------------------------------------------------------*/
void Step::vAstackStk(void)
{
  ENTRY();
  
   m_oAuxStack.vDump(&m_oDicStack, "AuxStack:"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* swap -- swap top two values on aux stack                                   */
/*----------------------------------------------------------------------------*/
void Step::vAstackSwap(void)
{
  ENTRY();
  
  SenCell xValA;
   m_oAuxStack.vPopSenCell(xValA, __func__);
  
  SenCell xValB;
   m_oAuxStack.vPopSenCell(xValB, __func__);

   m_oAuxStack.vPushSenCell(xValA);
   m_oAuxStack.vPushSenCell(xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >a -- move value from dstack to astack                                     */
/*----------------------------------------------------------------------------*/
void Step::vAstackToA(void)
{
  ENTRY();
  
  SenCell xValOpaque;
  m_oDataStack.vPopSenCell(xValOpaque, __func__);
  
   m_oAuxStack.vPushSenCell(xValOpaque);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a> -- move value from astack to dstack                                     */
/*----------------------------------------------------------------------------*/
void Step::vAstackToD(void)
{
  ENTRY();
  
  SenCell xVal;
   m_oAuxStack.vPopSenCell(xVal, __func__);
  
  m_oDataStack.vPushSenCell(xVal);
  
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
  
   m_oAuxStack.vPeekTypeValueArea(0, xVariable);
  
  m_oDataStack.vPushSenCell(xVariable);
  
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
  
   m_oAuxStack.vPeekTypeValueArea(1, xVariable);
  
  m_oDataStack.vPushSenCell(xVariable);
  
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
  
   m_oAuxStack.vPeekTypeValueArea(2, xVariable);
  
  m_oDataStack.vPushSenCell(xVariable);
  
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
  
   m_oAuxStack.vPeekTypeValueArea(3, xVariable);
  
  m_oDataStack.vPushSenCell(xVariable);
  
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
  
   m_oAuxStack.vPeekTypeValueArea(4, xVariable);
  
  m_oDataStack.vPushSenCell(xVariable);
  
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
  
   m_oAuxStack.vPeekTypeValueArea(5, xVariable);
  
  m_oDataStack.vPushSenCell(xVariable);
  
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
  
   m_oAuxStack.vPeekTypeValueArea(6, xVariable);
  
  m_oDataStack.vPushSenCell(xVariable);
  
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
  
   m_oAuxStack.vPeekTypeValueArea(7, xVariable);
  
  m_oDataStack.vPushSenCell(xVariable);
  
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
  
   m_oAuxStack.vPeekTypeValueArea(8, xVariable);
  
  m_oDataStack.vPushSenCell(xVariable);
  
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
  
   m_oAuxStack.vPeekTypeValueArea(9, xVariable);
  
  m_oDataStack.vPushSenCell(xVariable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
