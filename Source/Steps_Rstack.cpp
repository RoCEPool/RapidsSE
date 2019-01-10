/*============================================================================*/
/* Steps_Rstack.cpp - return stack step group                                 */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

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
void Step::vRstackCsPick(MP_Task mpTask)
{
  ENTRY("vRstackCsPick"_Stz);
  
  // could do this directly on stack
  
  // pop which value
  i8 nThValue;
  if ( ! mpTask->m_pDataStack->bPopAsInt(nThValue) )
    THROW_INVALID_DSTACK("cs-pick"_Stz);
  
  // pop one return address
//  GenVar xRtnWsp;
//  bool b = mpTask->m_pReturnStack->bPop(&xRtnWsp);
//  ASSERT(b);
  
  // copy n-th value onto top
  GenVar xVariable;
  mpTask->m_pReturnStack->vPeekValWrk(nThValue,xVariable);
  mpTask->m_pReturnStack->vPushGenVar(xVariable);
  
  // push back the one return address
//  mpTask->m_pReturnStack->vPush(xRtnWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* cs-roll -- roll top control values at n ( Z Y ... B A n -- Y ... B A Z )   */
/*----------------------------------------------------------------------------*/
void Step::vRstackCsRoll(MP_Task mpTask)
{
  ENTRY("vRstackCsRoll"_Stz);
  
  // pop n values
  i8 nValues;
  if ( ! mpTask->m_pDataStack->bPop(nValues) )
    THROW_INVALID_DSTACK("cs-roll"_Stz);
  
  GenVar xValMid;
  for (i8 i_Count = 0; i_Count < nValues; i_Count++) {
    if ( ! mpTask->m_pDataStack->bPop(xValMid) )
      THROW_INVALID_DSTACK("cs-roll"_Stz);
    
    mpTask->m_pReturnStack->vPushGenVar(xValMid);
  }
  
  Cell* pCellZ;
  if ( ! mpTask->m_pDataStack->bPop(pCellZ) )
    THROW_INVALID_DSTACK("cs-roll"_Stz);
  
  for (i8 i_Count = 0; i_Count < nValues; i_Count++) {
    if ( ! mpTask->m_pReturnStack->bPop(xValMid) )
      THROW_INVALID_RSTACK("cs-roll"_Stz);
    
    mpTask->m_pDataStack->vPushGenVar(xValMid);
  }

  mpTask->m_pDataStack->vPushCell(pCellZ);
  
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rdepth -- put number of values in return stack onto data stack             */
/*----------------------------------------------------------------------------*/
void Step::vRstackDepth(MP_Task mpTask)
{
  ENTRY("vRstackDepth"_Stz);
  
  i8 iDepth = mpTask->m_pReturnStack->iValues();
  
  mpTask->m_pDataStack->vPushI8(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rdrop -- discard top value off rstack                                      */
/*----------------------------------------------------------------------------*/
void Step::vRstackDrop(MP_Task mpTask)
{
  ENTRY("vRstackDrop"_Stz);
  
  // should just discard on stack
  
  GenVar xValA;
  if ( ! mpTask->m_pReturnStack->bPop(xValA) )
    THROW_INVALID_RSTACK("rdrop"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r@ -- place copy of top value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstackFetch(MP_Task mpTask)
{
  ENTRY("vRstackFetch"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pReturnStack->vPeekValWrk(0,xVariable);
  
  mpTask->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 2r@ -- place copy of two top values in return stack onto data stack        */
/*----------------------------------------------------------------------------*/
void Step::vRstackFetchTwo(MP_Task mpTask)
{
  ENTRY("vRstackFetchTwo"_Stz);
  
  // could do this directly on stack
  
  GenVar xValX0;
  GenVar xValX1;
  
  mpTask->m_pReturnStack->vPeekValWrk(0,xValX0);
  mpTask->m_pReturnStack->vPeekValWrk(1,xValX1);
  
  mpTask->m_pDataStack->vPushGenVar(xValX1);
  mpTask->m_pDataStack->vPushGenVar(xValX0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rpick -- place copy of nth value in return stack onto data stack           */
/*----------------------------------------------------------------------------*/
void Step::vRstackPick(MP_Task mpTask)
{
  ENTRY("vRstackPick"_Stz);
  
  i8 iPick;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iPick) )
    THROW_INVALID_DSTACK("rpick"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pReturnStack->vPeekValWrk(iPick,xVariable);
  
  mpTask->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rreverse -- reverse rstack values ( ... D C B A n -- A B C D ... )         */
/*----------------------------------------------------------------------------*/
void Step::vRstackReverse(MP_Task mpTask)
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
  if ( ! mpTask->m_pDataStack->bPop(nValues) )
    THROW_INVALID_DSTACK("areverse"_Stz);
  
  for (i8 i = 0; i < nValues; i++) {
    if ( ! mpTask->m_pReturnStack->bPop(apValWrk[i]) )
      THROW_INVALID_DSTACK("areverse"_Stz);
  }
  
  for (i8 i = 0; i < nValues; i++) {
    mpTask->m_pReturnStack->vPushGenVar(apValWrk[i]);
  }
}

/*----------------------------------------------------------------------------*/
/* rroll -- roll top rstack values at n ( ... D C B A n -- A ... D C B )      */
/*----------------------------------------------------------------------------*/
void Step::vRstackRoll(MP_Task mpTask)
{
  ENTRY("vRstackRoll"_Stz);
  
  GenVar xValA;
  GenVar xValX;
  
  i8 nValues;
  if ( ! mpTask->m_pDataStack->bPop(nValues) )
    THROW_INVALID_DSTACK("rroll"_Stz);
  
  if ( ! mpTask->m_pReturnStack->bPop(xValA) )
    THROW_INVALID_RSTACK("rroll"_Stz);
  
  for (i8 i_Count = 0; i_Count < nValues; i_Count++) {
    if ( ! mpTask->m_pReturnStack->bPop(xValX) )
      THROW_INVALID_RSTACK("rroll"_Stz);
    
    mpTask->m_pDataStack->vPushGenVar(xValX);
  }
  
  mpTask->m_pReturnStack->vPushGenVar(xValA);
  
  for (i8 i_Count = 0; i_Count < nValues; i_Count++) {
    if ( ! mpTask->m_pDataStack->bPop(xValX) )
      THROW_INVALID_DSTACK("rroll"_Stz);
    
    mpTask->m_pReturnStack->vPushGenVar(xValX);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -rroll -- inverse roll top rstack values at n ( Z ... B A n -- ... B A Z ) */
/*----------------------------------------------------------------------------*/
void Step::vRstackRollInv(MP_Task mpTask)
{
  ENTRY("vRstackRollInv"_Stz);
  
  GenVar xValZ;
  GenVar xValX;
  
  i8 nValues;
  if ( ! mpTask->m_pDataStack->bPop(nValues) )
    THROW_INVALID_DSTACK("-rroll"_Stz);
  
  for (i8 i_Count = 0; i_Count < nValues; i_Count++) {
    if ( ! mpTask->m_pReturnStack->bPop(xValX) )
      THROW_INVALID_RSTACK("-rroll"_Stz);
    
    mpTask->m_pDataStack->vPushGenVar(xValX);
  }
  
  if ( ! mpTask->m_pReturnStack->bPop(xValZ) )
    THROW_INVALID_DSTACK("-rroll"_Stz);
  
  
  for (i8 i_Count = 0; i_Count < nValues; i_Count++) {
    if ( ! mpTask->m_pDataStack->bPop(xValX) )
      THROW_INVALID_DSTACK("-rroll"_Stz);
    
    mpTask->m_pReturnStack->vPushGenVar(xValX);
  }
  
  mpTask->m_pReturnStack->vPushGenVar(xValZ);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rstk -- dump the return stack                                              */
/*----------------------------------------------------------------------------*/
/*
void Step::vRstackStk(MP_Task mpTask)
{
  ENTRY("vRstackStk"_Stz);
  
  mpTask->m_pReturnStack->vDump(mpTask->m_sName.s(),"ReturnStack:");
  
  EXIT();
}
*/

/*----------------------------------------------------------------------------*/
/* rswap -- swap top two values on return stack                               */
/*----------------------------------------------------------------------------*/
void Step::vRstackSwap(MP_Task mpTask)
{
  ENTRY("vRstackSwap"_Stz);
  
  GenVar xValA;
  GenVar xValB;
  
  if ( ! mpTask->m_pReturnStack->bPop(xValA,xValB) )
    THROW_INVALID_DSTACK("swap"_Stz);
  
  mpTask->m_pReturnStack->vPushGenVar(xValA);
  mpTask->m_pReturnStack->vPushGenVar(xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r> -- move value from rstack to dstack                                     */
/*----------------------------------------------------------------------------*/
void Step::vRstackToP(MP_Task mpTask)
{
  ENTRY("vRstackToP"_Stz);
  
  GenVar xVal;
  if ( ! mpTask->m_pReturnStack->bPop(xVal) )
    THROW_INVALID_DSTACK("r>"_Stz);
  
  mpTask->m_pDataStack->vPushGenVar(xVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >r -- move value from dstack to rstack                                     */
/*----------------------------------------------------------------------------*/
void Step::vRstackToR(MP_Task mpTask)
{
  ENTRY("vRstackToR"_Stz);
  
  GenVar xValOpaque;
  if ( ! mpTask->m_pDataStack->bPop(xValOpaque) )
    THROW_INVALID_DSTACK(">r"_Stz);
  
  mpTask->m_pReturnStack->vPushGenVar(xValOpaque);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 2>r -- move two values from dstack to rstack preserving order              */
/*----------------------------------------------------------------------------*/
void Step::vRstackToRTwo(MP_Task mpTask)
{
  ENTRY("vRstackToRTwo"_Stz);
  
  GenVar xValX0;
  GenVar xValX1;
  if ( ! mpTask->m_pDataStack->bPop(xValX0,xValX1) )
    THROW_INVALID_DSTACK("2>r"_Stz);
  
  mpTask->m_pReturnStack->vPushGenVar(xValX1);
  mpTask->m_pReturnStack->vPushGenVar(xValX0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r0 -- place copy of 0th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack0(MP_Task mpTask)
{
  ENTRY("vRstack0"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pReturnStack->vPeekValWrk(0,xVariable);
  
  mpTask->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r1 -- place copy of 1st value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack1(MP_Task mpTask)
{
  ENTRY("vRstack1"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pReturnStack->vPeekValWrk(1,xVariable);
  
  mpTask->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r2 -- place copy of 2nd value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack2(MP_Task mpTask)
{
  ENTRY("vRstack2"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pReturnStack->vPeekValWrk(2,xVariable);
  
  mpTask->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r3 -- place copy of 3rd value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack3(MP_Task mpTask)
{
  ENTRY("vRstack3"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pReturnStack->vPeekValWrk(3,xVariable);
  
  mpTask->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r4 -- place copy of 4th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack4(MP_Task mpTask)
{
  ENTRY("vRstack4"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pReturnStack->vPeekValWrk(4,xVariable);
  
  mpTask->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r5 -- place copy of 0th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack5(MP_Task mpTask)
{
  ENTRY("vRstack5"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pReturnStack->vPeekValWrk(5,xVariable);
  
  mpTask->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r6 -- place copy of 6th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack6(MP_Task mpTask)
{
  ENTRY("vRstack6"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pReturnStack->vPeekValWrk(6,xVariable);
  
  mpTask->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r7 -- place copy of 7th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack7(MP_Task mpTask)
{
  ENTRY("vRstack7"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pReturnStack->vPeekValWrk(7,xVariable);
  
  mpTask->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r8 -- place copy of 8th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack8(MP_Task mpTask)
{
  ENTRY("vRstack8"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pReturnStack->vPeekValWrk(8,xVariable);
  
  mpTask->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r9 -- place copy of 9th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack9(MP_Task mpTask)
{
  ENTRY("vRstack9"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pReturnStack->vPeekValWrk(9,xVariable);
  
  mpTask->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
