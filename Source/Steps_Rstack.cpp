/*============================================================================*/
/* Steps_Rstack.cpp - return stack step group                                 */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern Heap*           gpMediumHeap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;
extern TxCollection*   gpTxCollection;

extern NP_Task         gnpRootTask;
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// rstack: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Rstack(void)
{
  vAddPrimitive("cs-pick"_Str,         &vRstackCsPick,     false);
  vAddPrimitive("cs-roll"_Str,         &vRstackCsRoll,     false);
  vAddPrimitive("rdepth"_Str,          &vRstackDepth,      false);
  vAddPrimitive("rdrop"_Str,           &vRstackDrop,       false);
  vAddPrimitive("r@"_Str,              &vRstackFetch,      false);
  vAddPrimitive("2r@"_Str,             &vRstackFetchTwo,   false);
  vAddPrimitive("rpick"_Str,           &vRstackPick,       false);
  vAddPrimitive("rreverse"_Str,        &vRstackReverse,    false);
  vAddPrimitive("rroll"_Str,           &vRstackRoll,       false);
  vAddPrimitive("-rroll"_Str,          &vRstackRollInv,    false);
//  vAddPrimitive("rstk"_Str,            &vRstackStk,       false);
  vAddPrimitive("rswap"_Str,           &vRstackSwap,       false);
  vAddPrimitive("r>"_Str,              &vRstackToP,        false);
  vAddPrimitive(">r"_Str,              &vRstackToR,        false);
  vAddPrimitive("2>r"_Str,             &vRstackToRTwo,     false);
  vAddPrimitive("r0"_Str,              &vRstack0,          false);
  vAddPrimitive("r1"_Str,              &vRstack1,          false);
  vAddPrimitive("r2"_Str,              &vRstack2,          false);
  vAddPrimitive("r3"_Str,              &vRstack3,          false);
  vAddPrimitive("r4"_Str,              &vRstack4,          false);
  vAddPrimitive("r5"_Str,              &vRstack5,          false);
  vAddPrimitive("r6"_Str,              &vRstack6,          false);
  vAddPrimitive("r7"_Str,              &vRstack7,          false);
  vAddPrimitive("r8"_Str,              &vRstack8,          false);
  vAddPrimitive("r9"_Str,              &vRstack9,          false);
}

/*----------------------------------------------------------------------------*/
/* cs-pick -- place copy of nth value in control stack onto control stack     */
/*----------------------------------------------------------------------------*/
void Task::vRstackCsPick(MP_Task mpTask)
{
  ENTRY("vRstackCsPick"_Str);
  
  // could do this directly on stack
  
  // pop which value
  i8 nThValue;
  if ( ! mpTask->m_pDataStack->bPopAsInt(nThValue) )
    THROW_INVALID_DSTACK("cs-pick"_Str);
  
  // pop one return address
//  ValWrk xRtnWsp;
//  bool b = mpTask->m_pReturnStack->bPop(&xRtnWsp);
//  ASSERT(b);
  
  // copy n-th value onto top
  ValWrk xValWrk;
  mpTask->m_pReturnStack->vPeekValWrk(nThValue,xValWrk);
  mpTask->m_pReturnStack->vPushValWrk(xValWrk);
  
  // push back the one return address
//  mpTask->m_pReturnStack->vPush(xRtnWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* cs-roll -- roll top control values at n ( Z Y ... B A n -- Y ... B A Z )   */
/*----------------------------------------------------------------------------*/
void Task::vRstackCsRoll(MP_Task mpTask)
{
  ENTRY("vRstackCsRoll"_Str);
  
  // pop n values
  i8 nValues;
  if ( ! mpTask->m_pDataStack->bPop(nValues) )
    THROW_INVALID_DSTACK("cs-roll"_Str);
  
  ValWrk xValMid;
  for (i8 i_Count = 0; i_Count < nValues; i_Count++) {
    if ( ! mpTask->m_pDataStack->bPop(xValMid) )
      THROW_INVALID_DSTACK("cs-roll"_Str);
    
    mpTask->m_pReturnStack->vPushValWrk(xValMid);
  }
  
  Cell* pCellZ;
  if ( ! mpTask->m_pDataStack->bPop(pCellZ) )
    THROW_INVALID_DSTACK("cs-roll"_Str);
  
  for (i8 i_Count = 0; i_Count < nValues; i_Count++) {
    if ( ! mpTask->m_pReturnStack->bPop(xValMid) )
      THROW_INVALID_RSTACK("cs-roll"_Str);
    
    mpTask->m_pDataStack->vPushValWrk(xValMid);
  }

  mpTask->m_pDataStack->vPushCell(pCellZ);
  
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rdepth -- put number of values in return stack onto data stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstackDepth(MP_Task mpTask)
{
  ENTRY("vRstackDepth"_Str);
  
  i8 iDepth = mpTask->m_pReturnStack->iValues();
  
  mpTask->m_pDataStack->vPushInt(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rdrop -- discard top value off rstack                                      */
/*----------------------------------------------------------------------------*/
void Task::vRstackDrop(MP_Task mpTask)
{
  ENTRY("vRstackDrop"_Str);
  
  // should just discard on stack
  
  ValWrk xValA;
  if ( ! mpTask->m_pReturnStack->bPop(xValA) )
    THROW_INVALID_RSTACK("rdrop"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r@ -- place copy of top value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Task::vRstackFetch(MP_Task mpTask)
{
  ENTRY("vRstackFetch"_Str);
  
  // could do this directly on stack
  
  ValWrk xValWrk;
  
  mpTask->m_pReturnStack->vPeekValWrk(0,xValWrk);
  
  mpTask->m_pDataStack->vPushValWrk(xValWrk);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 2r@ -- place copy of two top values in return stack onto data stack        */
/*----------------------------------------------------------------------------*/
void Task::vRstackFetchTwo(MP_Task mpTask)
{
  ENTRY("vRstackFetchTwo"_Str);
  
  // could do this directly on stack
  
  ValWrk xValX0;
  ValWrk xValX1;
  
  mpTask->m_pReturnStack->vPeekValWrk(0,xValX0);
  mpTask->m_pReturnStack->vPeekValWrk(1,xValX1);
  
  mpTask->m_pDataStack->vPushValWrk(xValX1);
  mpTask->m_pDataStack->vPushValWrk(xValX0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rpick -- place copy of nth value in return stack onto data stack           */
/*----------------------------------------------------------------------------*/
void Task::vRstackPick(MP_Task mpTask)
{
  ENTRY("vRstackPick"_Str);
  
  i8 iPick;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iPick) )
    THROW_INVALID_DSTACK("rpick"_Str);
  
  // could do this directly on stack
  
  ValWrk xValWrk;
  
  mpTask->m_pReturnStack->vPeekValWrk(iPick,xValWrk);
  
  mpTask->m_pDataStack->vPushValWrk(xValWrk);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rreverse -- reverse rstack values ( ... D C B A n -- A B C D ... )         */
/*----------------------------------------------------------------------------*/
void Task::vRstackReverse(MP_Task mpTask)
{
  ENTRY("vRstackReverse"_Str);
  
  ValWrk xVal0;
  ValWrk xVal1;
  ValWrk xVal2;
  ValWrk xVal3;
  ValWrk xVal4;
  ValWrk xVal5;
  ValWrk xVal6;
  ValWrk xVal7;
  
  ValWrk* apValWrk[8] = {
    &xVal0, &xVal1, &xVal2, &xVal3, &xVal4, &xVal5, &xVal6, &xVal7
  };
  
  i8 nValues;
  if ( ! mpTask->m_pDataStack->bPop(nValues) )
    THROW_INVALID_DSTACK("areverse"_Str);
  
  for (i8 i = 0; i < nValues; i++) {
    if ( ! mpTask->m_pReturnStack->bPop(apValWrk[i]) )
      THROW_INVALID_DSTACK("areverse"_Str);
  }
  
  for (i8 i = 0; i < nValues; i++) {
    mpTask->m_pReturnStack->vPushValWrk(apValWrk[i]);
  }
}

/*----------------------------------------------------------------------------*/
/* rroll -- roll top rstack values at n ( ... D C B A n -- A ... D C B )      */
/*----------------------------------------------------------------------------*/
void Task::vRstackRoll(MP_Task mpTask)
{
  ENTRY("vRstackRoll"_Str);
  
  ValWrk xValA;
  ValWrk xValX;
  
  i8 nValues;
  if ( ! mpTask->m_pDataStack->bPop(nValues) )
    THROW_INVALID_DSTACK("rroll"_Str);
  
  if ( ! mpTask->m_pReturnStack->bPop(xValA) )
    THROW_INVALID_RSTACK("rroll"_Str);
  
  for (i8 i_Count = 0; i_Count < nValues; i_Count++) {
    if ( ! mpTask->m_pReturnStack->bPop(xValX) )
      THROW_INVALID_RSTACK("rroll"_Str);
    
    mpTask->m_pDataStack->vPushValWrk(xValX);
  }
  
  mpTask->m_pReturnStack->vPushValWrk(xValA);
  
  for (i8 i_Count = 0; i_Count < nValues; i_Count++) {
    if ( ! mpTask->m_pDataStack->bPop(xValX) )
      THROW_INVALID_DSTACK("rroll"_Str);
    
    mpTask->m_pReturnStack->vPushValWrk(xValX);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -rroll -- inverse roll top rstack values at n ( Z ... B A n -- ... B A Z ) */
/*----------------------------------------------------------------------------*/
void Task::vRstackRollInv(MP_Task mpTask)
{
  ENTRY("vRstackRollInv"_Str);
  
  ValWrk xValZ;
  ValWrk xValX;
  
  i8 nValues;
  if ( ! mpTask->m_pDataStack->bPop(nValues) )
    THROW_INVALID_DSTACK("-rroll"_Str);
  
  for (i8 i_Count = 0; i_Count < nValues; i_Count++) {
    if ( ! mpTask->m_pReturnStack->bPop(xValX) )
      THROW_INVALID_RSTACK("-rroll"_Str);
    
    mpTask->m_pDataStack->vPushValWrk(xValX);
  }
  
  if ( ! mpTask->m_pReturnStack->bPop(xValZ) )
    THROW_INVALID_DSTACK("-rroll"_Str);
  
  
  for (i8 i_Count = 0; i_Count < nValues; i_Count++) {
    if ( ! mpTask->m_pDataStack->bPop(xValX) )
      THROW_INVALID_DSTACK("-rroll"_Str);
    
    mpTask->m_pReturnStack->vPushValWrk(xValX);
  }
  
  mpTask->m_pReturnStack->vPushValWrk(xValZ);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rstk -- dump the return stack                                              */
/*----------------------------------------------------------------------------*/
/*
void Task::vRstackStk(MP_Task mpTask)
{
  ENTRY("vRstackStk"_Str);
  
  mpTask->m_pReturnStack->vDump(mpTask->m_sName.s(),"ReturnStack:");
  
  EXIT();
}
*/

/*----------------------------------------------------------------------------*/
/* rswap -- swap top two values on return stack                               */
/*----------------------------------------------------------------------------*/
void Task::vRstackSwap(MP_Task mpTask)
{
  ENTRY("vRstackSwap"_Str);
  
  ValWrk xValA;
  ValWrk xValB;
  
  if ( ! mpTask->m_pReturnStack->bPop(xValA,xValB) )
    THROW_INVALID_DSTACK("swap"_Str);
  
  mpTask->m_pReturnStack->vPushValWrk(xValA);
  mpTask->m_pReturnStack->vPushValWrk(xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r> -- move value from rstack to dstack                                     */
/*----------------------------------------------------------------------------*/
void Task::vRstackToP(MP_Task mpTask)
{
  ENTRY("vRstackToP"_Str);
  
  ValWrk xVal;
  if ( ! mpTask->m_pReturnStack->bPop(xVal) )
    THROW_INVALID_DSTACK("r>"_Str);
  
  mpTask->m_pDataStack->vPushValWrk(xVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >r -- move value from dstack to rstack                                     */
/*----------------------------------------------------------------------------*/
void Task::vRstackToR(MP_Task mpTask)
{
  ENTRY("vRstackToR"_Str);
  
  ValWrk xValOpaque;
  if ( ! mpTask->m_pDataStack->bPop(xValOpaque) )
    THROW_INVALID_DSTACK(">r"_Str);
  
  mpTask->m_pReturnStack->vPushValWrk(xValOpaque);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 2>r -- move two values from dstack to rstack preserving order              */
/*----------------------------------------------------------------------------*/
void Task::vRstackToRTwo(MP_Task mpTask)
{
  ENTRY("vRstackToRTwo"_Str);
  
  ValWrk xValX0;
  ValWrk xValX1;
  if ( ! mpTask->m_pDataStack->bPop(xValX0,xValX1) )
    THROW_INVALID_DSTACK("2>r"_Str);
  
  mpTask->m_pReturnStack->vPushValWrk(xValX1);
  mpTask->m_pReturnStack->vPushValWrk(xValX0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r0 -- place copy of 0th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Task::vRstack0(MP_Task mpTask)
{
  ENTRY("vRstack0"_Str);
  
  // could do this directly on stack
  
  ValWrk xValWrk;
  
  mpTask->m_pReturnStack->vPeekValWrk(0,xValWrk);
  
  mpTask->m_pDataStack->vPushValWrk(xValWrk);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r1 -- place copy of 1st value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Task::vRstack1(MP_Task mpTask)
{
  ENTRY("vRstack1"_Str);
  
  // could do this directly on stack
  
  ValWrk xValWrk;
  
  mpTask->m_pReturnStack->vPeekValWrk(1,xValWrk);
  
  mpTask->m_pDataStack->vPushValWrk(xValWrk);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r2 -- place copy of 2nd value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Task::vRstack2(MP_Task mpTask)
{
  ENTRY("vRstack2"_Str);
  
  // could do this directly on stack
  
  ValWrk xValWrk;
  
  mpTask->m_pReturnStack->vPeekValWrk(2,xValWrk);
  
  mpTask->m_pDataStack->vPushValWrk(xValWrk);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r3 -- place copy of 3rd value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Task::vRstack3(MP_Task mpTask)
{
  ENTRY("vRstack3"_Str);
  
  // could do this directly on stack
  
  ValWrk xValWrk;
  
  mpTask->m_pReturnStack->vPeekValWrk(3,xValWrk);
  
  mpTask->m_pDataStack->vPushValWrk(xValWrk);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r4 -- place copy of 4th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Task::vRstack4(MP_Task mpTask)
{
  ENTRY("vRstack4"_Str);
  
  // could do this directly on stack
  
  ValWrk xValWrk;
  
  mpTask->m_pReturnStack->vPeekValWrk(4,xValWrk);
  
  mpTask->m_pDataStack->vPushValWrk(xValWrk);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r5 -- place copy of 0th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Task::vRstack5(MP_Task mpTask)
{
  ENTRY("vRstack5"_Str);
  
  // could do this directly on stack
  
  ValWrk xValWrk;
  
  mpTask->m_pReturnStack->vPeekValWrk(5,xValWrk);
  
  mpTask->m_pDataStack->vPushValWrk(xValWrk);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r6 -- place copy of 6th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Task::vRstack6(MP_Task mpTask)
{
  ENTRY("vRstack6"_Str);
  
  // could do this directly on stack
  
  ValWrk xValWrk;
  
  mpTask->m_pReturnStack->vPeekValWrk(6,xValWrk);
  
  mpTask->m_pDataStack->vPushValWrk(xValWrk);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r7 -- place copy of 7th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Task::vRstack7(MP_Task mpTask)
{
  ENTRY("vRstack7"_Str);
  
  // could do this directly on stack
  
  ValWrk xValWrk;
  
  mpTask->m_pReturnStack->vPeekValWrk(7,xValWrk);
  
  mpTask->m_pDataStack->vPushValWrk(xValWrk);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r8 -- place copy of 8th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Task::vRstack8(MP_Task mpTask)
{
  ENTRY("vRstack8"_Str);
  
  // could do this directly on stack
  
  ValWrk xValWrk;
  
  mpTask->m_pReturnStack->vPeekValWrk(8,xValWrk);
  
  mpTask->m_pDataStack->vPushValWrk(xValWrk);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r9 -- place copy of 9th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Task::vRstack9(MP_Task mpTask)
{
  ENTRY("vRstack9"_Str);
  
  // could do this directly on stack
  
  ValWrk xValWrk;
  
  mpTask->m_pReturnStack->vPeekValWrk(9,xValWrk);
  
  mpTask->m_pDataStack->vPushValWrk(xValWrk);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
