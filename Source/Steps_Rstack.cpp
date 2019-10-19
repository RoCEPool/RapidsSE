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
  vAddStep("rdrop"_Stz,           &Step::vRstackDrop);
  vAddStep("r@"_Stz,              &Step::vRstackFetch);
  vAddStep("2r@"_Stz,             &Step::vRstackFetchTwo);
  vAddStep("rpick"_Stz,           &Step::vRstackPick);
  vAddStep("rreverse"_Stz,        &Step::vRstackReverse);
  vAddStep("rroll"_Stz,           &Step::vRstackRoll);
  vAddStep("-rroll"_Stz,          &Step::vRstackRollInv);
//  vAddStep("rstk"_Stz,            &Step::vRstackStk);
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
void Step::vRstackCsPick(Forth* pForth)
{
  ENTRY("vRstackCsPick"_Stz);
  
  // could do this directly on stack
  
  // pop which value
  i8 nThValue;
  if ( ! pForth->m_pDataStack->bPopAsInt(nThValue) )
    THROW_DSTACK();
  
  // pop one return address
//  GenVar xRtnWsp;
//  bool b = pForth->m_pReturnStack->bPop(&xRtnWsp);
//  ASSERT(b);
  
  // copy n-th value onto top
  GenVar xVariable;
  pForth->m_pReturnStack->vPeekValue(nThValue,xVariable);
  pForth->m_pReturnStack->vPushGenVar(xVariable);
  
  // push back the one return address
//  pForth->m_pReturnStack->vPush(xRtnWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* cs-roll -- roll top control values at n ( Z Y ... B A n -- Y ... B A Z )   */
/*----------------------------------------------------------------------------*/
void Step::vRstackCsRoll(Forth* pForth)
{
  ENTRY("vRstackCsRoll"_Stz);
  
  // pop n values
  i8 nValues;
  if ( ! pForth->m_pDataStack->bPop(nValues) )
    THROW_DSTACK();
  
  GenVar xValMid;
  for (i8 i_Count = 0; i_Count < nValues; i_Count++) {
    if ( ! pForth->m_pDataStack->bPop(xValMid) )
      THROW_DSTACK();
    
    pForth->m_pReturnStack->vPushGenVar(xValMid);
  }
  
  Cell* aCellZ;
  if ( ! pForth->m_pDataStack->bPop(aCellZ) )
    THROW_DSTACK();
  
  for (i8 i_Count = 0; i_Count < nValues; i_Count++) {
    if ( ! pForth->m_pReturnStack->bPop(xValMid) )
      THROW_RSTACK();
    
    pForth->m_pDataStack->vPushGenVar(xValMid);
  }

  pForth->m_pDataStack->vPushCell(aCellZ);
  
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rdepth -- put number of values in return stack onto data stack             */
/*----------------------------------------------------------------------------*/
void Step::vRstackDepth(Forth* pForth)
{
  ENTRY("vRstackDepth"_Stz);
  
  i8 iDepth = pForth->m_pReturnStack->nValues();
  
  pForth->m_pDataStack->vPushI8(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rdrop -- discard top value off rstack                                      */
/*----------------------------------------------------------------------------*/
void Step::vRstackDrop(Forth* pForth)
{
  ENTRY("vRstackDrop"_Stz);
  
  // should just discard on stack
  
  GenVar xValA;
  if ( ! pForth->m_pReturnStack->bPop(xValA) )
    THROW_RSTACK();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r@ -- place copy of top value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstackFetch(Forth* pForth)
{
  ENTRY("vRstackFetch"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  pForth->m_pReturnStack->vPeekValue(0,xVariable);
  
  pForth->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 2r@ -- place copy of two top values in return stack onto data stack        */
/*----------------------------------------------------------------------------*/
void Step::vRstackFetchTwo(Forth* pForth)
{
  ENTRY("vRstackFetchTwo"_Stz);
  
  // could do this directly on stack
  
  GenVar xValX0;
  GenVar xValX1;
  
  pForth->m_pReturnStack->vPeekValue(0,xValX0);
  pForth->m_pReturnStack->vPeekValue(1,xValX1);
  
  pForth->m_pDataStack->vPushGenVar(xValX1);
  pForth->m_pDataStack->vPushGenVar(xValX0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rpick -- place copy of nth value in return stack onto data stack           */
/*----------------------------------------------------------------------------*/
void Step::vRstackPick(Forth* pForth)
{
  ENTRY("vRstackPick"_Stz);
  
  i8 iPick;
  if ( ! pForth->m_pDataStack->bPopAsInt(iPick) )
    THROW_DSTACK();
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  pForth->m_pReturnStack->vPeekValue(iPick,xVariable);
  
  pForth->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rreverse -- reverse rstack values ( ... D C B A n -- A B C D ... )         */
/*----------------------------------------------------------------------------*/
void Step::vRstackReverse(Forth* pForth)
{
  ENTRY("vRstackReverse"_Stz);
  
  GenVar xVal0;
  GenVar xVal1;
  GenVar xVal2;
  GenVar xVal3;
  GenVar xVal4;
  GenVar xVal5;
  GenVar xVal6;
  GenVar xVal7;
  
  GenVar* apValWrk[8] = {
    &xVal0, &xVal1, &xVal2, &xVal3, &xVal4, &xVal5, &xVal6, &xVal7
  };
  
  i8 nValues;
  if ( ! pForth->m_pDataStack->bPop(nValues) )
    THROW_DSTACK();
  
  for (i8 i = 0; i < nValues; i++) {
    if ( ! pForth->m_pReturnStack->bPop(apValWrk[i]) )
      THROW_DSTACK();
  }
  
  for (i8 i = 0; i < nValues; i++) {
    pForth->m_pReturnStack->vPushGenVar(apValWrk[i]);
  }
}

/*----------------------------------------------------------------------------*/
/* rroll -- roll top rstack values at n ( ... D C B A n -- A ... D C B )      */
/*----------------------------------------------------------------------------*/
void Step::vRstackRoll(Forth* pForth)
{
  ENTRY("vRstackRoll"_Stz);
  
  GenVar xValA;
  GenVar xValX;
  
  i8 nValues;
  if ( ! pForth->m_pDataStack->bPop(nValues) )
    THROW_DSTACK();
  
  if ( ! pForth->m_pReturnStack->bPop(xValA) )
    THROW_RSTACK();
  
  for (i8 i_Count = 0; i_Count < nValues; i_Count++) {
    if ( ! pForth->m_pReturnStack->bPop(xValX) )
      THROW_RSTACK();
    
    pForth->m_pDataStack->vPushGenVar(xValX);
  }
  
  pForth->m_pReturnStack->vPushGenVar(xValA);
  
  for (i8 i_Count = 0; i_Count < nValues; i_Count++) {
    if ( ! pForth->m_pDataStack->bPop(xValX) )
      THROW_DSTACK();
    
    pForth->m_pReturnStack->vPushGenVar(xValX);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -rroll -- inverse roll top rstack values at n ( Z ... B A n -- ... B A Z ) */
/*----------------------------------------------------------------------------*/
void Step::vRstackRollInv(Forth* pForth)
{
  ENTRY("vRstackRollInv"_Stz);
  
  GenVar xValZ;
  GenVar xValX;
  
  i8 nValues;
  if ( ! pForth->m_pDataStack->bPop(nValues) )
    THROW_DSTACK();
  
  for (i8 i_Count = 0; i_Count < nValues; i_Count++) {
    if ( ! pForth->m_pReturnStack->bPop(xValX) )
      THROW_RSTACK();
    
    pForth->m_pDataStack->vPushGenVar(xValX);
  }
  
  if ( ! pForth->m_pReturnStack->bPop(xValZ) )
    THROW_DSTACK();
  
  
  for (i8 i_Count = 0; i_Count < nValues; i_Count++) {
    if ( ! pForth->m_pDataStack->bPop(xValX) )
      THROW_DSTACK();
    
    pForth->m_pReturnStack->vPushGenVar(xValX);
  }
  
  pForth->m_pReturnStack->vPushGenVar(xValZ);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rstk -- dump the return stack                                              */
/*----------------------------------------------------------------------------*/
/*
void Step::vRstackStk(Forth* pForth)
{
  ENTRY("vRstackStk"_Stz);
  
  pForth->m_pReturnStack->vDump(pForth->m_pTask->m_sName.s(),"ReturnStack:");
  
  EXIT();
}
*/

/*----------------------------------------------------------------------------*/
/* rswap -- swap top two values on return stack                               */
/*----------------------------------------------------------------------------*/
void Step::vRstackSwap(Forth* pForth)
{
  ENTRY("vRstackSwap"_Stz);
  
  GenVar xValA;
  GenVar xValB;
  
  if ( ! pForth->m_pReturnStack->bPop(xValA,xValB) )
    THROW_DSTACK();
  
  pForth->m_pReturnStack->vPushGenVar(xValA);
  pForth->m_pReturnStack->vPushGenVar(xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r> -- move value from rstack to dstack                                     */
/*----------------------------------------------------------------------------*/
void Step::vRstackToP(Forth* pForth)
{
  ENTRY("vRstackToP"_Stz);
  
  GenVar xVal;
  if ( ! pForth->m_pReturnStack->bPop(xVal) )
    THROW_DSTACK();
  
  pForth->m_pDataStack->vPushGenVar(xVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >r -- move value from dstack to rstack                                     */
/*----------------------------------------------------------------------------*/
void Step::vRstackToR(Forth* pForth)
{
  ENTRY("vRstackToR"_Stz);
  
  GenVar xValOpaque;
  if ( ! pForth->m_pDataStack->bPop(xValOpaque) )
    THROW_DSTACK();
  
  pForth->m_pReturnStack->vPushGenVar(xValOpaque);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 2>r -- move two values from dstack to rstack preserving order              */
/*----------------------------------------------------------------------------*/
void Step::vRstackToRTwo(Forth* pForth)
{
  ENTRY("vRstackToRTwo"_Stz);
  
  GenVar xValX0;
  GenVar xValX1;
  if ( ! pForth->m_pDataStack->bPop(xValX0,xValX1) )
    THROW_DSTACK();
  
  pForth->m_pReturnStack->vPushGenVar(xValX1);
  pForth->m_pReturnStack->vPushGenVar(xValX0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r0 -- place copy of 0th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack0(Forth* pForth)
{
  ENTRY("vRstack0"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  pForth->m_pReturnStack->vPeekValue(0,xVariable);
  
  pForth->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r1 -- place copy of 1st value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack1(Forth* pForth)
{
  ENTRY("vRstack1"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  pForth->m_pReturnStack->vPeekValue(1,xVariable);
  
  pForth->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r2 -- place copy of 2nd value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack2(Forth* pForth)
{
  ENTRY("vRstack2"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  pForth->m_pReturnStack->vPeekValue(2,xVariable);
  
  pForth->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r3 -- place copy of 3rd value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack3(Forth* pForth)
{
  ENTRY("vRstack3"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  pForth->m_pReturnStack->vPeekValue(3,xVariable);
  
  pForth->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r4 -- place copy of 4th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack4(Forth* pForth)
{
  ENTRY("vRstack4"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  pForth->m_pReturnStack->vPeekValue(4,xVariable);
  
  pForth->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r5 -- place copy of 0th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack5(Forth* pForth)
{
  ENTRY("vRstack5"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  pForth->m_pReturnStack->vPeekValue(5,xVariable);
  
  pForth->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r6 -- place copy of 6th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack6(Forth* pForth)
{
  ENTRY("vRstack6"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  pForth->m_pReturnStack->vPeekValue(6,xVariable);
  
  pForth->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r7 -- place copy of 7th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack7(Forth* pForth)
{
  ENTRY("vRstack7"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  pForth->m_pReturnStack->vPeekValue(7,xVariable);
  
  pForth->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r8 -- place copy of 8th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack8(Forth* pForth)
{
  ENTRY("vRstack8"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  pForth->m_pReturnStack->vPeekValue(8,xVariable);
  
  pForth->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r9 -- place copy of 9th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack9(Forth* pForth)
{
  ENTRY("vRstack9"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  pForth->m_pReturnStack->vPeekValue(9,xVariable);
  
  pForth->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
