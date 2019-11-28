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
  ENTRY(); // ENTRY("vRstackCsPick"_Stz);
  
  // could do this directly on stack
  
  // pop which value
  i8 nThValue;
  if ( ! m_pDataStack->bPopAsInt(nThValue) )
    THROW_DSTACK();
  
  // pop one return address
//  GenVar xRtnWsp;
//  bool b = m_pReturnStack->bPop(&xRtnWsp);
//  ASSERT(b);
  
  // copy n-th value onto top
  GenVar xVariable;
  m_pReturnStack->vPeekVal(nThValue,xVariable);
  m_pReturnStack->vPushVal(xVariable);
  
  // push back the one return address
//  m_pReturnStack->vPush(xRtnWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* cs-roll -- roll top control values at n ( Z Y ... B A n -- Y ... B A Z )   */
/*----------------------------------------------------------------------------*/
void Step::vRstackCsRoll(void)
{
  ENTRY(); // ENTRY("vRstackCsRoll"_Stz);
  
  // pop n values
  i8 nValues;
  if ( ! m_pDataStack->bPopInt(nValues) )
    THROW_DSTACK();
  
  GenVar xValMid;
  for (i8 i_Count = 0; i_Count < nValues; i_Count++) {
    if ( ! m_pDataStack->bPopVal(xValMid) )
      THROW_DSTACK();
    
    m_pReturnStack->vPushVal(xValMid);
  }
  
  Cell* aFipZ;
  if ( ! m_pDataStack->bPopFip(aFipZ) )
    THROW_DSTACK();
  
  for (i8 i_Count = 0; i_Count < nValues; i_Count++) {
    if ( ! m_pReturnStack->bPopVal(xValMid) )
      THROW_RSTACK();
    
    m_pDataStack->vPushVal(xValMid);
  }

  m_pDataStack->vPushFip(aFipZ);
  
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rdepth -- put number of values in return stack onto data stack             */
/*----------------------------------------------------------------------------*/
void Step::vRstackDepth(void)
{
  ENTRY(); // ENTRY("vRstackDepth"_Stz);
  
  i8 iDepth = m_pReturnStack->nValues();
  
  m_pDataStack->vPushInt(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rdrop -- discard top value off rstack                                      */
/*----------------------------------------------------------------------------*/
void Step::vRstackDrop(void)
{
  ENTRY(); // ENTRY("vRstackDrop"_Stz);
  
  // should just discard on stack
  
  GenVar xValA;
  if ( ! m_pReturnStack->bPopVal(xValA) )
    THROW_RSTACK();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r@ -- place copy of top value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstackFetch(void)
{
  ENTRY(); // ENTRY("vRstackFetch"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  m_pReturnStack->vPeekVal(0,xVariable);
  
  m_pDataStack->vPushVal(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 2r@ -- place copy of two top values in return stack onto data stack        */
/*----------------------------------------------------------------------------*/
void Step::vRstackFetchTwo(void)
{
  ENTRY(); // ENTRY("vRstackFetchTwo"_Stz);
  
  // could do this directly on stack
  
  GenVar xValX0;
  GenVar xValX1;
  
  m_pReturnStack->vPeekVal(0,xValX0);
  m_pReturnStack->vPeekVal(1,xValX1);
  
  m_pDataStack->vPushVal(xValX1);
  m_pDataStack->vPushVal(xValX0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rpick -- place copy of nth value in return stack onto data stack           */
/*----------------------------------------------------------------------------*/
void Step::vRstackPick(void)
{
  ENTRY(); // ENTRY("vRstackPick"_Stz);
  
  i8 iPick;
  if ( ! m_pDataStack->bPopAsInt(iPick) )
    THROW_DSTACK();
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  m_pReturnStack->vPeekVal(iPick,xVariable);
  
  m_pDataStack->vPushVal(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rreverse -- reverse rstack values ( ... D C B A n -- A B C D ... )         */
/*----------------------------------------------------------------------------*/
void Step::vRstackReverse(void)
{
  ENTRY(); // ENTRY("vRstackReverse"_Stz);
  
  GenVar xVal0;
  GenVar xVal1;
  GenVar xVal2;
  GenVar xVal3;
  GenVar xVal4;
  GenVar xVal5;
  GenVar xVal6;
  GenVar xVal7;
  
  GenVar* paVal[8] = {
    &xVal0, &xVal1, &xVal2, &xVal3, &xVal4, &xVal5, &xVal6, &xVal7
  };
  
  i8 nValues;
  if ( ! m_pDataStack->bPopInt(nValues) )
    THROW_DSTACK();
  
  for (i8 i = 0; i < nValues; i++) {
    if ( ! m_pReturnStack->bPopVal(paVal[i]) )
      THROW_DSTACK();
  }
  
  for (i8 i = 0; i < nValues; i++) {
    m_pReturnStack->vPushVal(paVal[i]);
  }
}

/*----------------------------------------------------------------------------*/
/* rroll -- roll top rstack values at n ( ... D C B A n -- A ... D C B )      */
/*----------------------------------------------------------------------------*/
void Step::vRstackRoll(void)
{
  ENTRY(); // ENTRY("vRstackRoll"_Stz);
  
  GenVar xValA;
  GenVar xValX;
  
  i8 nValues;
  if ( ! m_pDataStack->bPopInt(nValues) )
    THROW_DSTACK();
  
  if ( ! m_pReturnStack->bPopVal(xValA) )
    THROW_RSTACK();
  
  for (i8 i_Count = 0; i_Count < nValues; i_Count++) {
    if ( ! m_pReturnStack->bPopVal(xValX) )
      THROW_RSTACK();
    
    m_pDataStack->vPushVal(xValX);
  }
  
  m_pReturnStack->vPushVal(xValA);
  
  for (i8 i_Count = 0; i_Count < nValues; i_Count++) {
    if ( ! m_pDataStack->bPopVal(xValX) )
      THROW_DSTACK();
    
    m_pReturnStack->vPushVal(xValX);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -rroll -- inverse roll top rstack values at n ( Z ... B A n -- ... B A Z ) */
/*----------------------------------------------------------------------------*/
void Step::vRstackRollInv(void)
{
  ENTRY(); // ENTRY("vRstackRollInv"_Stz);
  
  GenVar xValZ;
  GenVar xValX;
  
  i8 nValues;
  if ( ! m_pDataStack->bPopInt(nValues) )
    THROW_DSTACK();
  
  for (i8 i_Count = 0; i_Count < nValues; i_Count++) {
    if ( ! m_pReturnStack->bPopVal(xValX) )
      THROW_RSTACK();
    
    m_pDataStack->vPushVal(xValX);
  }
  
  if ( ! m_pReturnStack->bPopVal(xValZ) )
    THROW_DSTACK();
  
  
  for (i8 i_Count = 0; i_Count < nValues; i_Count++) {
    if ( ! m_pDataStack->bPopVal(xValX) )
      THROW_DSTACK();
    
    m_pReturnStack->vPushVal(xValX);
  }
  
  m_pReturnStack->vPushVal(xValZ);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rstk -- dump the return stack                                              */
/*----------------------------------------------------------------------------*/
void Step::vRstackStk(void)
{
  ENTRY(); // ENTRY("vRstackStk"_Stz);
  
  m_pReturnStack->vContentDump(&m_oDicStack,"ReturnStack:"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rswap -- swap top two values on return stack                               */
/*----------------------------------------------------------------------------*/
void Step::vRstackSwap(void)
{
  ENTRY(); // ENTRY("vRstackSwap"_Stz);
  
  GenVar xValA;
  if ( ! m_pReturnStack->bPopVal(xValA) )
    THROW_DSTACK();
  
  GenVar xValB;
  if ( ! m_pReturnStack->bPopVal(xValB) )
    THROW_DSTACK();

  m_pReturnStack->vPushVal(xValA);
  m_pReturnStack->vPushVal(xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r> -- move value from rstack to dstack                                     */
/*----------------------------------------------------------------------------*/
void Step::vRstackToP(void)
{
  ENTRY(); // ENTRY("vRstackToP"_Stz);
  
  GenVar xVal;
  if ( ! m_pReturnStack->bPopVal(xVal) )
    THROW_DSTACK();
  
  m_pDataStack->vPushVal(xVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >r -- move value from dstack to rstack                                     */
/*----------------------------------------------------------------------------*/
void Step::vRstackToR(void)
{
  ENTRY(); // ENTRY("vRstackToR"_Stz);
  
  GenVar xValOpaque;
  if ( ! m_pDataStack->bPopVal(xValOpaque) )
    THROW_DSTACK();
  
  m_pReturnStack->vPushVal(xValOpaque);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 2>r -- move two values from dstack to rstack preserving order              */
/*----------------------------------------------------------------------------*/
void Step::vRstackToRTwo(void)
{
  ENTRY(); // ENTRY("vRstackToRTwo"_Stz);
  
  GenVar xValX0;
  if ( ! m_pDataStack->bPopVal(xValX0) )
    THROW_DSTACK();

  GenVar xValX1;
  if ( ! m_pDataStack->bPopVal(xValX1) )
    THROW_DSTACK();
  
  m_pReturnStack->vPushVal(xValX1);
  m_pReturnStack->vPushVal(xValX0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r0 -- place copy of 0th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack0(void)
{
  ENTRY(); // ENTRY("vRstack0"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  m_pReturnStack->vPeekVal(0,xVariable);
  
  m_pDataStack->vPushVal(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r1 -- place copy of 1st value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack1(void)
{
  ENTRY(); // ENTRY("vRstack1"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  m_pReturnStack->vPeekVal(1,xVariable);
  
  m_pDataStack->vPushVal(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r2 -- place copy of 2nd value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack2(void)
{
  ENTRY(); // ENTRY("vRstack2"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  m_pReturnStack->vPeekVal(2,xVariable);
  
  m_pDataStack->vPushVal(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r3 -- place copy of 3rd value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack3(void)
{
  ENTRY(); // ENTRY("vRstack3"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  m_pReturnStack->vPeekVal(3,xVariable);
  
  m_pDataStack->vPushVal(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r4 -- place copy of 4th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack4(void)
{
  ENTRY(); // ENTRY("vRstack4"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  m_pReturnStack->vPeekVal(4,xVariable);
  
  m_pDataStack->vPushVal(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r5 -- place copy of 0th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack5(void)
{
  ENTRY(); // ENTRY("vRstack5"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  m_pReturnStack->vPeekVal(5,xVariable);
  
  m_pDataStack->vPushVal(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r6 -- place copy of 6th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack6(void)
{
  ENTRY(); // ENTRY("vRstack6"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  m_pReturnStack->vPeekVal(6,xVariable);
  
  m_pDataStack->vPushVal(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r7 -- place copy of 7th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack7(void)
{
  ENTRY(); // ENTRY("vRstack7"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  m_pReturnStack->vPeekVal(7,xVariable);
  
  m_pDataStack->vPushVal(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r8 -- place copy of 8th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack8(void)
{
  ENTRY(); // ENTRY("vRstack8"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  m_pReturnStack->vPeekVal(8,xVariable);
  
  m_pDataStack->vPushVal(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r9 -- place copy of 9th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vRstack9(void)
{
  ENTRY(); // ENTRY("vRstack9"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  m_pReturnStack->vPeekVal(9,xVariable);
  
  m_pDataStack->vPushVal(xVariable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
