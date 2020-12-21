/*============================================================================*/
/* Steps_Rstack.cpp - return stack step group                                 */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Rstack(void)
{
  vAddStep("cs-pick"_Stz,         &Step::vRstackCsPick);
  vAddStep("cs-roll"_Stz,         &Step::vRstackCsRoll);
  vAddStep("rdepth"_Stz,          &Step::vRstackDepth);
  vAddStep("rdiscard"_Stz,        &Step::vRstackDiscard);
  vAddStep("r@"_Stz,              &Step::vRstackFetch);
  vAddStep("2r@"_Stz,             &Step::vRstackFetchTwo);
  vAddStep("rpick"_Stz,           &Step::vRstackPick);
  vAddStep("rreverse"_Stz,        &Step::vRstackReverse);
  vAddStep("rroll"_Stz,           &Step::vRstackRoll);
  vAddStep("-rroll"_Stz,          &Step::vRstackRollInv);
  vAddStep("rstk"_Stz,            &Step::vRstackStk);
  vAddStep("rswap"_Stz,           &Step::vRstackSwap);
  vAddStep("r>"_Stz,              &Step::vRstackToP);
  vAddStep(">r"_Stz,              &Step::vRstackToR);
  vAddStep("2>r"_Stz,             &Step::vRstackToRTwo);
  vAddStep("r0"_Stz,              &Step::vRstack0);
  vAddStep("r1"_Stz,              &Step::vRstack1);
  vAddStep("r2"_Stz,              &Step::vRstack2);
  vAddStep("r3"_Stz,              &Step::vRstack3);
  vAddStep("r4"_Stz,              &Step::vRstack4);
  vAddStep("r5"_Stz,              &Step::vRstack5);
  vAddStep("r6"_Stz,              &Step::vRstack6);
  vAddStep("r7"_Stz,              &Step::vRstack7);
  vAddStep("r8"_Stz,              &Step::vRstack8);
  vAddStep("r9"_Stz,              &Step::vRstack9);
}

/*----------------------------------------------------------------------------*/
/* cs-pick -- place copy of nth value in control stack onto control stack     */
/*----------------------------------------------------------------------------*/
void Step::vRstackCsPick(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  // pop which value
  i8 nThValue = m_pDataStack->iPopAsInt(__func__);
  
  // pop one return address
//  CellSingle xRtnWsp;
//  bool b = m_pReturnStack->bPop(&xRtnWsp);
//  ASSERT(b);
  
  // copy n-th value onto top
  CellSingle xVariable;
  m_pReturnStack->vPeekItem(nThValue,xVariable);
  m_pReturnStack->vPushItem(xVariable);
  
  // push back the one return address
//  m_pReturnStack->vPush(xRtnWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* cs-roll -- roll top control values at n ( Z Y ... B A n -- Y ... B A Z )   */
/*----------------------------------------------------------------------------*/
void Step::vRstackCsRoll(void)
{
  ENTRY();
  
  // pop n values
  i8 nValues = m_pDataStack->iPopInt(__func__);
  
  CellSingle xValMid;
  for (i8 i_Count = 0; i_Count < nValues; i_Count++) {
    m_pDataStack->vPopItem(xValMid,__func__);
    
    m_pReturnStack->vPushItem(xValMid);
  }
  
  ForthCell* aFipZ = m_pDataStack->aPopFip(__func__);
  
  for (i8 i_Count = 0; i_Count < nValues; i_Count++) {
    m_pReturnStack->vPopItem(xValMid,__func__);
    
    m_pDataStack->vPushItem(xValMid);
  }

  m_pDataStack->vPushFip(aFipZ);
  
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rdepth -- put number of values in return stack onto data stack             */
/*----------------------------------------------------------------------------*/
void Step::vRstackDepth(void)
{
  ENTRY();
  
  i8 iDepth = m_pReturnStack->nValues();
  
  m_pDataStack->vPushInt(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rdiscard -- discard top value off rstack                                   */
/*----------------------------------------------------------------------------*/
void Step::vRstackDiscard(void)
{
  ENTRY();
  
  m_pReturnStack->vDiscardTop();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r@ -- place copy of top value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstackFetch(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  CellSingle xVariable;
  
  m_pReturnStack->vPeekItem(0,xVariable);
  
  m_pDataStack->vPushItem(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 2r@ -- place copy of two top values in return stack onto data stack        */
/*----------------------------------------------------------------------------*/
void Step::vRstackFetchTwo(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  CellSingle xValX0;
  CellSingle xValX1;
  
  m_pReturnStack->vPeekItem(0,xValX0);
  m_pReturnStack->vPeekItem(1,xValX1);
  
  m_pDataStack->vPushItem(xValX1);
  m_pDataStack->vPushItem(xValX0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rpick -- place copy of nth value in return stack onto data stack           */
/*----------------------------------------------------------------------------*/
void Step::vRstackPick(void)
{
  ENTRY();
  
  i8 iPick = m_pDataStack->iPopAsInt(__func__);
  
  // could do this directly on stack
  
  CellSingle xVariable;
  
  m_pReturnStack->vPeekItem(iPick,xVariable);
  
  m_pDataStack->vPushItem(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rreverse -- reverse rstack values ( ... D C B A n -- A B C D ... )         */
/*----------------------------------------------------------------------------*/
void Step::vRstackReverse(void)
{
  ENTRY();
  
  CellSingle xVal1;
  CellSingle xVal2;
  CellSingle xVal3;
  CellSingle xVal4;
  CellSingle xVal5;
  CellSingle xVal6;
  CellSingle xVal7;
  CellSingle xVal8;
  
  i8 nValues = m_pDataStack->iPopInt(__func__);
  
  switch (nValues) {
    case 8: m_pReturnStack->vPopItem(xVal8,__func__);
    case 7: m_pReturnStack->vPopItem(xVal7,__func__);
    case 6: m_pReturnStack->vPopItem(xVal6,__func__);
    case 5: m_pReturnStack->vPopItem(xVal5,__func__);
    case 4: m_pReturnStack->vPopItem(xVal4,__func__);
    case 3: m_pReturnStack->vPopItem(xVal3,__func__);
    case 2: m_pReturnStack->vPopItem(xVal2,__func__);
    case 1: m_pReturnStack->vPopItem(xVal1,__func__);
  }
 
  switch (nValues) {
    case 8: m_pReturnStack->vPushItem(xVal8);
    case 7: m_pReturnStack->vPushItem(xVal7);
    case 6: m_pReturnStack->vPushItem(xVal6);
    case 5: m_pReturnStack->vPushItem(xVal5);
    case 4: m_pReturnStack->vPushItem(xVal4);
    case 3: m_pReturnStack->vPushItem(xVal3);
    case 2: m_pReturnStack->vPushItem(xVal2);
    case 1: m_pReturnStack->vPushItem(xVal1);
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rroll -- roll top rstack values at n ( ... D C B A n -- A ... D C B )      */
/*----------------------------------------------------------------------------*/
void Step::vRstackRoll(void)
{
  ENTRY();
  
  CellSingle xValA;
  CellSingle xValX;
  
  i8 nValues = m_pDataStack->iPopInt(__func__);
  
  m_pReturnStack->vPopItem(xValA,__func__);
  
  for (i8 i_Count = 0; i_Count < nValues; i_Count++) {
    m_pReturnStack->vPopItem(xValX,__func__);
    
    m_pDataStack->vPushItem(xValX);
  }
  
  m_pReturnStack->vPushItem(xValA);
  
  for (i8 i_Count = 0; i_Count < nValues; i_Count++) {
    m_pDataStack->vPopItem(xValX,__func__);
    
    m_pReturnStack->vPushItem(xValX);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -rroll -- inverse roll top rstack values at n ( Z ... B A n -- ... B A Z ) */
/*----------------------------------------------------------------------------*/
void Step::vRstackRollInv(void)
{
  ENTRY();
  
  CellSingle xValZ;
  CellSingle xValX;
  
  i8 nValues = m_pDataStack->iPopInt(__func__);
  
  for (i8 i_Count = 0; i_Count < nValues; i_Count++) {
    m_pReturnStack->vPopItem(xValX,__func__);
    
    m_pDataStack->vPushItem(xValX);
  }
  
  m_pReturnStack->vPopItem(xValZ,__func__);
  
  
  for (i8 i_Count = 0; i_Count < nValues; i_Count++) {
    m_pDataStack->vPopItem(xValX,__func__);
    
    m_pReturnStack->vPushItem(xValX);
  }
  
  m_pReturnStack->vPushItem(xValZ);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rstk -- dump the return stack                                              */
/*----------------------------------------------------------------------------*/
void Step::vRstackStk(void)
{
  ENTRY();
  
  m_pReturnStack->vDump(&m_oDicStack,"ReturnStack:"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rswap -- swap top two values on return stack                               */
/*----------------------------------------------------------------------------*/
void Step::vRstackSwap(void)
{
  ENTRY();
  
  CellSingle xValA;
  m_pReturnStack->vPopItem(xValA,__func__);
  
  CellSingle xValB;
  m_pReturnStack->vPopItem(xValB,__func__);

  m_pReturnStack->vPushItem(xValA);
  m_pReturnStack->vPushItem(xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r> -- move value from rstack to dstack                                     */
/*----------------------------------------------------------------------------*/
void Step::vRstackToP(void)
{
  ENTRY();
  
  CellSingle xVal;
  m_pReturnStack->vPopItem(xVal,__func__);
  
  m_pDataStack->vPushItem(xVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >r -- move value from dstack to rstack                                     */
/*----------------------------------------------------------------------------*/
void Step::vRstackToR(void)
{
  ENTRY();
  
  CellSingle xValOpaque;
  m_pDataStack->vPopItem(xValOpaque,__func__);
  
  m_pReturnStack->vPushItem(xValOpaque);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 2>r -- move two values from dstack to rstack preserving order              */
/*----------------------------------------------------------------------------*/
void Step::vRstackToRTwo(void)
{
  ENTRY();
  
  CellSingle xValX0;
  m_pDataStack->vPopItem(xValX0,__func__);

  CellSingle xValX1;
  m_pDataStack->vPopItem(xValX1,__func__);
  
  m_pReturnStack->vPushItem(xValX1);
  m_pReturnStack->vPushItem(xValX0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r0 -- place copy of 0th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack0(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  CellSingle xVariable;
  
  m_pReturnStack->vPeekItem(0,xVariable);
  
  m_pDataStack->vPushItem(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r1 -- place copy of 1st value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack1(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  CellSingle xVariable;
  
  m_pReturnStack->vPeekItem(1,xVariable);
  
  m_pDataStack->vPushItem(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r2 -- place copy of 2nd value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack2(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  CellSingle xVariable;
  
  m_pReturnStack->vPeekItem(2,xVariable);
  
  m_pDataStack->vPushItem(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r3 -- place copy of 3rd value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack3(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  CellSingle xVariable;
  
  m_pReturnStack->vPeekItem(3,xVariable);
  
  m_pDataStack->vPushItem(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r4 -- place copy of 4th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack4(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  CellSingle xVariable;
  
  m_pReturnStack->vPeekItem(4,xVariable);
  
  m_pDataStack->vPushItem(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r5 -- place copy of 0th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack5(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  CellSingle xVariable;
  
  m_pReturnStack->vPeekItem(5,xVariable);
  
  m_pDataStack->vPushItem(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r6 -- place copy of 6th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack6(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  CellSingle xVariable;
  
  m_pReturnStack->vPeekItem(6,xVariable);
  
  m_pDataStack->vPushItem(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r7 -- place copy of 7th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack7(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  CellSingle xVariable;
  
  m_pReturnStack->vPeekItem(7,xVariable);
  
  m_pDataStack->vPushItem(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r8 -- place copy of 8th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack8(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  CellSingle xVariable;
  
  m_pReturnStack->vPeekItem(8,xVariable);
  
  m_pDataStack->vPushItem(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r9 -- place copy of 9th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack9(void)
{
  ENTRY();
  
  // could do this directly on stack
  
  CellSingle xVariable;
  
  m_pReturnStack->vPeekItem(9,xVariable);
  
  m_pDataStack->vPushItem(xVariable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
