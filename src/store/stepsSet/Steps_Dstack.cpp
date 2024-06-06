/*============================================================================*/
/* Steps_Dstack.cpp - data stack step group                                   */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Dstack(void)
{
  vAddStep("stack.class"_Stz,     &Step::vDstackClass);
  vAddStep("depth"_Stz,           &Step::vDstackDepth);
  vAddStep("discard"_Stz,         &Step::vDstackDiscard);
  vAddStep("2discard"_Stz,        &Step::vDstackDiscardTwo);
  vAddStep("dup"_Stz,             &Step::vDstackDup);
  vAddStep("?dup"_Stz,            &Step::vDstackDupNonZero);
  vAddStep("nip"_Stz,             &Step::vDstackNip);
  vAddStep("over"_Stz,            &Step::vDstackOver);
  vAddStep("pick"_Stz,            &Step::vDstackPick);
  vAddStep("reverse"_Stz,         &Step::vDstackReverse);
  vAddStep("roll"_Stz,            &Step::vDstackRoll);
  vAddStep("-roll"_Stz,           &Step::vDstackRollInv);
  vAddStep("rot"_Stz,             &Step::vDstackRot);
  vAddStep("-rot"_Stz,            &Step::vDstackRotInv);
  vAddStep("stk"_Stz,             &Step::vDstackStk);
  vAddStep("swap"_Stz,            &Step::vDstackSwap);
  vAddStep("tuck"_Stz,            &Step::vDstackTuck);
}


/*----------------------------------------------------------------------------*/
/* stack.class -- get class of nth value on stack                             */
/*----------------------------------------------------------------------------*/
void Step::vDstackClass(void)
{
  ENTRY();
  
  i8 i_Val = m_oDataStack.iPopInt(__func__);
  
  SenCell xVar;
  
  m_oDataStack.vPeekTypeValueArea(i_Val, xVar);
  
  eCN eValClass = xVar.eType();
  
  m_oDataStack.vPushInt(eValClass);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* depth -- push depth of parameters stack onto stack                         */
/*----------------------------------------------------------------------------*/
void Step::vDstackDepth(void)
{
  ENTRY();
  
  i8 iDepth = m_oDataStack.nValues();
  
  m_oDataStack.vPushInt(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* drop -- discard top value off stack                                        */
/*----------------------------------------------------------------------------*/
void Step::vDstackDiscard(void)
{
  ENTRY();
  
  m_oDataStack.vDiscardTop();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 2drop -- discard top two values off stack                                  */
/*----------------------------------------------------------------------------*/
void Step::vDstackDiscardTwo(void)
{
  ENTRY();
  
  //! should just discard on stack
  
  m_oDataStack.vDiscardTop();

  m_oDataStack.vDiscardTop();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dup -- dup top of data stack                                               */
/*----------------------------------------------------------------------------*/
void Step::vDstackDup(void)
{
  ENTRY();
  
  if (m_iTraceMask) {
    ASSERT(m_iTraceMask);
  }

  // could just copy from stack back into stack
  
  //CellSingle xVarA;
  
  SenCell xVarA;
  m_oDataStack.vPopSenCell(xVarA, __func__);
  
  m_oDataStack.vPushSenCell(xVarA);
  m_oDataStack.vPushSenCell(xVarA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ?dup -- dup top of data stack  if non zero                                 */
/*----------------------------------------------------------------------------*/
void Step::vDstackDupNonZero(void)
{
  ENTRY();
  
  // could just copy from stack back into stack
  
  SenCell xVarA;
  m_oDataStack.vPopSenCell(xVarA, __func__);
  
  if (xVarA.iGetColVal()) {
    m_oDataStack.vPushSenCell(xVarA);
    m_oDataStack.vPushSenCell(xVarA);
  }
  else
    m_oDataStack.vPushInt(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nip -- drop the 2nd value from the stack ( B A -- A )                      */
/*----------------------------------------------------------------------------*/
void Step::vDstackNip(void)
{
  ENTRY();
  
  SenCell xVarA;
  m_oDataStack.vPopSenCell(xVarA, __func__);
  
  SenCell xVarB;
  m_oDataStack.vPopSenCell(xVarB, __func__);
  
  m_oDataStack.vPushSenCell(xVarA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* over -- place copy of 2nd value on top stack  ( B A -- B A B )             */
/*----------------------------------------------------------------------------*/
void Step::vDstackOver(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  SenCell xVariable;
  
  m_oDataStack.vPeekTypeValueArea(1, xVariable);
  
  m_oDataStack.vPushSenCell(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pick -- put copy of nth value onto stack ( ... D C B A n -- D C B A N ... )*/
/*----------------------------------------------------------------------------*/
void Step::vDstackPick(void)
{
  ENTRY();
  
  i8 iPick = m_oDataStack.iPopAsInt(__func__);
  
  // could do this directly on stack
  
  SenCell xVariable;
  
  m_oDataStack.vPeekTypeValueArea(iPick, xVariable);
  
  m_oDataStack.vPushSenCell(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reverse -- reverse stack values ( ... D C B A n -- A B C D ... )           */
/*----------------------------------------------------------------------------*/
void Step::vDstackReverse(void)
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
    case 8: m_oDataStack.vPopSenCell(xVal8, __func__);  [[gnu::fallthrough]];
    case 7: m_oDataStack.vPopSenCell(xVal7, __func__);  [[gnu::fallthrough]];
    case 6: m_oDataStack.vPopSenCell(xVal6, __func__);  [[gnu::fallthrough]];
    case 5: m_oDataStack.vPopSenCell(xVal5, __func__);  [[gnu::fallthrough]];
    case 4: m_oDataStack.vPopSenCell(xVal4, __func__);  [[gnu::fallthrough]];
    case 3: m_oDataStack.vPopSenCell(xVal3, __func__);  [[gnu::fallthrough]];
    case 2: m_oDataStack.vPopSenCell(xVal2, __func__);  [[gnu::fallthrough]];
    case 1: m_oDataStack.vPopSenCell(xVal1, __func__);
  }

  switch (nValues) {
    case 8: m_oDataStack.vPushSenCell(xVal8);  [[gnu::fallthrough]];
    case 7: m_oDataStack.vPushSenCell(xVal7);  [[gnu::fallthrough]];
    case 6: m_oDataStack.vPushSenCell(xVal6);  [[gnu::fallthrough]];
    case 5: m_oDataStack.vPushSenCell(xVal5);  [[gnu::fallthrough]];
    case 4: m_oDataStack.vPushSenCell(xVal4);  [[gnu::fallthrough]];
    case 3: m_oDataStack.vPushSenCell(xVal3);  [[gnu::fallthrough]];
    case 2: m_oDataStack.vPushSenCell(xVal2);  [[gnu::fallthrough]];
    case 1: m_oDataStack.vPushSenCell(xVal1);
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* roll -- roll top stack values at n ( ... D C B A n -- A ... D C B )        */
/*----------------------------------------------------------------------------*/
void Step::vDstackRoll(void)
{
  ENTRY();
  
  SenCell xVarA;
  SenCell xVarX;
  
  i8 nValues = m_oDataStack.iPopInt(__func__);
  
  m_oDataStack.vPopSenCell(xVarA, __func__);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    m_oDataStack.vPopSenCell(xVarX, __func__);
    
    m_oReturnStack.vPushSenCell(xVarX);
  }
  
  m_oDataStack.vPushSenCell(xVarA);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    m_oReturnStack.vPopSenCell(xVarX, __func__);
    
    m_oDataStack.vPushSenCell(xVarX);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -roll -- inverse roll top stack values at n ( Z ... B A n -- ... B A Z )   */
/*----------------------------------------------------------------------------*/
void Step::vDstackRollInv(void)
{
  ENTRY();
  
  SenCell xVarZ;
  SenCell xVarX;
  
  i8 nValues = m_oDataStack.iPopInt(__func__);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    m_oDataStack.vPopSenCell(xVarX, __func__);
    
    m_oReturnStack.vPushSenCell(xVarX);
  }
  
  m_oDataStack.vPopSenCell(xVarZ, __func__);
  
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    m_oReturnStack.vPopSenCell(xVarX, __func__);
    
    m_oDataStack.vPushSenCell(xVarX);
  }
  
  m_oDataStack.vPushSenCell(xVarZ);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rot -- rotate top 3 values clockwise (rotate C to top) ( C B A -- B A C )  */
/*----------------------------------------------------------------------------*/
void Step::vDstackRot(void)
{
  ENTRY();
  
  SenCell xVarA;
  m_oDataStack.vPopSenCell(xVarA, __func__);
  SenCell xVarB;
  m_oDataStack.vPopSenCell(xVarB, __func__);
  SenCell xVarC;
  m_oDataStack.vPopSenCell(xVarC, __func__);

  
  m_oDataStack.vPushSenCell(xVarB);
  m_oDataStack.vPushSenCell(xVarA);
  m_oDataStack.vPushSenCell(xVarC);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -rot -- rotate top 3 values anticlockwise (A to bottom) ( C B A -- A C B ) */
/*----------------------------------------------------------------------------*/
void Step::vDstackRotInv(void)
{
  ENTRY();
  
  SenCell xVarA;
  m_oDataStack.vPopSenCell(xVarA, __func__);
  SenCell xVarB;
  m_oDataStack.vPopSenCell(xVarB, __func__);
  SenCell xVarC;
  m_oDataStack.vPopSenCell(xVarC, __func__);

  m_oDataStack.vPushSenCell(xVarA);
  m_oDataStack.vPushSenCell(xVarC);
  m_oDataStack.vPushSenCell(xVarB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* stk -- dump the data stack                                                 */
/*----------------------------------------------------------------------------*/
void Step::vDstackStk(void)
{
  ENTRY();
  
  m_oDataStack.vDump(&m_oDicStack, "DataStack:"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* swap -- swap top two values on data stack                                  */
/*----------------------------------------------------------------------------*/
void Step::vDstackSwap(void)
{
  ENTRY();
  
  SenCell xVarA;
  m_oDataStack.vPopSenCell(xVarA, __func__);
  
  SenCell xVarB;
  m_oDataStack.vPopSenCell(xVarB, __func__);
  
  m_oDataStack.vPushSenCell(xVarA);
  m_oDataStack.vPushSenCell(xVarB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tuck -- copy the first stack value below second ( B A -- A B A )           */
/*----------------------------------------------------------------------------*/
void Step::vDstackTuck(void)
{
  ENTRY();
  
  SenCell xVarA;
  m_oDataStack.vPopSenCell(xVarA, __func__);
  
  SenCell xVarB;
  m_oDataStack.vPopSenCell(xVarB, __func__);

  m_oDataStack.vPushSenCell(xVarA);
  m_oDataStack.vPushSenCell(xVarB);
  m_oDataStack.vPushSenCell(xVarA);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
