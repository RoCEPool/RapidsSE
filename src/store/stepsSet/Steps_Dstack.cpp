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
  
  i8 i_Val = m_pDataStack->iPopInt(__func__);
  
  SenCell xVar;
  
  m_pDataStack->vPeekTypeValueArea(i_Val,xVar);
  
  eCN eValClass = xVar.eType();
  
  m_pDataStack->vPushInt(eValClass);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* depth -- push depth of parameters stack onto stack                         */
/*----------------------------------------------------------------------------*/
void Step::vDstackDepth(void)
{
  ENTRY();
  
  i8 iDepth = m_pDataStack->nValues();
  
  m_pDataStack->vPushInt(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* drop -- discard top value off stack                                        */
/*----------------------------------------------------------------------------*/
void Step::vDstackDiscard(void)
{
  ENTRY();
  
  m_pDataStack->vDiscardTop();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 2drop -- discard top two values off stack                                  */
/*----------------------------------------------------------------------------*/
void Step::vDstackDiscardTwo(void)
{
  ENTRY();
  
  //! should just discard on stack
  
  m_pDataStack->vDiscardTop();

  m_pDataStack->vDiscardTop();

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
  m_pDataStack->vPopSenCell(xVarA,__func__);
  
  m_pDataStack->vPushSenCell(xVarA);
  m_pDataStack->vPushSenCell(xVarA);
  
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
  m_pDataStack->vPopSenCell(xVarA,__func__);
  
  if (xVarA.iGetColVal()) {
    m_pDataStack->vPushSenCell(xVarA);
    m_pDataStack->vPushSenCell(xVarA);
  }
  else
    m_pDataStack->vPushInt(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nip -- drop the 2nd value from the stack ( B A -- A )                      */
/*----------------------------------------------------------------------------*/
void Step::vDstackNip(void)
{
  ENTRY();
  
  SenCell xVarA;
  m_pDataStack->vPopSenCell(xVarA,__func__);
  
  SenCell xVarB;
  m_pDataStack->vPopSenCell(xVarB,__func__);
  
  m_pDataStack->vPushSenCell(xVarA);
  
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
  
  m_pDataStack->vPeekTypeValueArea(1,xVariable);
  
  m_pDataStack->vPushSenCell(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pick -- put copy of nth value onto stack ( ... D C B A n -- D C B A N ... )*/
/*----------------------------------------------------------------------------*/
void Step::vDstackPick(void)
{
  ENTRY();
  
  i8 iPick = m_pDataStack->iPopAsInt(__func__);
  
  // could do this directly on stack
  
  SenCell xVariable;
  
  m_pDataStack->vPeekTypeValueArea(iPick,xVariable);
  
  m_pDataStack->vPushSenCell(xVariable);
  
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

  i8 nValues = m_pDataStack->iPopInt(__func__);

  // Note: the [[gnu::fallthrough]]; statement needs to occur before the next case
  // label. This will suppress the -Wimplicit-fallthrough warning.
  // The semicolon at the end is significant.
  // If/when we adopt C++17 then we can use [[fallthrough]]; instead.
  
  switch (nValues) {
    case 8: m_pDataStack->vPopSenCell(xVal8,__func__);  [[gnu::fallthrough]];
    case 7: m_pDataStack->vPopSenCell(xVal7,__func__);  [[gnu::fallthrough]];
    case 6: m_pDataStack->vPopSenCell(xVal6,__func__);  [[gnu::fallthrough]];
    case 5: m_pDataStack->vPopSenCell(xVal5,__func__);  [[gnu::fallthrough]];
    case 4: m_pDataStack->vPopSenCell(xVal4,__func__);  [[gnu::fallthrough]];
    case 3: m_pDataStack->vPopSenCell(xVal3,__func__);  [[gnu::fallthrough]];
    case 2: m_pDataStack->vPopSenCell(xVal2,__func__);  [[gnu::fallthrough]];
    case 1: m_pDataStack->vPopSenCell(xVal1,__func__);
  }

  switch (nValues) {
    case 8: m_pDataStack->vPushSenCell(xVal8);  [[gnu::fallthrough]];
    case 7: m_pDataStack->vPushSenCell(xVal7);  [[gnu::fallthrough]];
    case 6: m_pDataStack->vPushSenCell(xVal6);  [[gnu::fallthrough]];
    case 5: m_pDataStack->vPushSenCell(xVal5);  [[gnu::fallthrough]];
    case 4: m_pDataStack->vPushSenCell(xVal4);  [[gnu::fallthrough]];
    case 3: m_pDataStack->vPushSenCell(xVal3);  [[gnu::fallthrough]];
    case 2: m_pDataStack->vPushSenCell(xVal2);  [[gnu::fallthrough]];
    case 1: m_pDataStack->vPushSenCell(xVal1);
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
  
  i8 nValues = m_pDataStack->iPopInt(__func__);
  
  m_pDataStack->vPopSenCell(xVarA,__func__);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    m_pDataStack->vPopSenCell(xVarX,__func__);
    
    m_pReturnStack->vPushSenCell(xVarX);
  }
  
  m_pDataStack->vPushSenCell(xVarA);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    m_pReturnStack->vPopSenCell(xVarX,__func__);
    
    m_pDataStack->vPushSenCell(xVarX);
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
  
  i8 nValues = m_pDataStack->iPopInt(__func__);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    m_pDataStack->vPopSenCell(xVarX,__func__);
    
    m_pReturnStack->vPushSenCell(xVarX);
  }
  
  m_pDataStack->vPopSenCell(xVarZ,__func__);
  
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    m_pReturnStack->vPopSenCell(xVarX,__func__);
    
    m_pDataStack->vPushSenCell(xVarX);
  }
  
  m_pDataStack->vPushSenCell(xVarZ);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rot -- rotate top 3 values clockwise (rotate C to top) ( C B A -- B A C )  */
/*----------------------------------------------------------------------------*/
void Step::vDstackRot(void)
{
  ENTRY();
  
  SenCell xVarA;
  m_pDataStack->vPopSenCell(xVarA,__func__);
  SenCell xVarB;
  m_pDataStack->vPopSenCell(xVarB,__func__);
  SenCell xVarC;
  m_pDataStack->vPopSenCell(xVarC,__func__);

  
  m_pDataStack->vPushSenCell(xVarB);
  m_pDataStack->vPushSenCell(xVarA);
  m_pDataStack->vPushSenCell(xVarC);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -rot -- rotate top 3 values anticlockwise (A to bottom) ( C B A -- A C B ) */
/*----------------------------------------------------------------------------*/
void Step::vDstackRotInv(void)
{
  ENTRY();
  
  SenCell xVarA;
  m_pDataStack->vPopSenCell(xVarA,__func__);
  SenCell xVarB;
  m_pDataStack->vPopSenCell(xVarB,__func__);
  SenCell xVarC;
  m_pDataStack->vPopSenCell(xVarC,__func__);

  m_pDataStack->vPushSenCell(xVarA);
  m_pDataStack->vPushSenCell(xVarC);
  m_pDataStack->vPushSenCell(xVarB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* stk -- dump the data stack                                                 */
/*----------------------------------------------------------------------------*/
void Step::vDstackStk(void)
{
  ENTRY();
  
  m_pDataStack->vDump(&m_oDicStack,"DataStack:"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* swap -- swap top two values on data stack                                  */
/*----------------------------------------------------------------------------*/
void Step::vDstackSwap(void)
{
  ENTRY();
  
  SenCell xVarA;
  m_pDataStack->vPopSenCell(xVarA,__func__);
  
  SenCell xVarB;
  m_pDataStack->vPopSenCell(xVarB,__func__);
  
  m_pDataStack->vPushSenCell(xVarA);
  m_pDataStack->vPushSenCell(xVarB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tuck -- copy the first stack value below second ( B A -- A B A )           */
/*----------------------------------------------------------------------------*/
void Step::vDstackTuck(void)
{
  ENTRY();
  
  SenCell xVarA;
  m_pDataStack->vPopSenCell(xVarA,__func__);
  
  SenCell xVarB;
  m_pDataStack->vPopSenCell(xVarB,__func__);

  m_pDataStack->vPushSenCell(xVarA);
  m_pDataStack->vPushSenCell(xVarB);
  m_pDataStack->vPushSenCell(xVarA);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
