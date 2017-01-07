/*============================================================================*/
/* Task_groups.cpp - task groups                                              */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Arguments       gxArguments;

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
  vAddPrimitive("cs-pick",         &vRstackCsPick,     false);
  vAddPrimitive("cs-roll",         &vRstackCsRoll,     false);
  vAddPrimitive("rdepth",          &vRstackDepth,      false);
  vAddPrimitive("rdrop",           &vRstackDrop,       false);
  vAddPrimitive("r@",              &vRstackFetch,      false);
  vAddPrimitive("2r@",             &vRstackFetchTwo,   false);
  vAddPrimitive("rpick",           &vRstackPick,       false);
  vAddPrimitive("rreverse",        &vRstackReverse,    false);
  vAddPrimitive("rroll",           &vRstackRoll,       false);
  vAddPrimitive("-rroll",          &vRstackRollInv,    false);
  vAddPrimitive("rswap",           &vRstackSwap,       false);
  vAddPrimitive("r>",              &vRstackToP,        false);
  vAddPrimitive(">r",              &vRstackToR,        false);
  vAddPrimitive("2>r",             &vRstackToRTwo,     false);
  vAddPrimitive("r0",              &vRstack0,          false);
  vAddPrimitive("r1",              &vRstack1,          false);
  vAddPrimitive("r2",              &vRstack2,          false);
  vAddPrimitive("r3",              &vRstack3,          false);
  vAddPrimitive("r4",              &vRstack4,          false);
  vAddPrimitive("r5",              &vRstack5,          false);
  vAddPrimitive("r6",              &vRstack6,          false);
  vAddPrimitive("r7",              &vRstack7,          false);
  vAddPrimitive("r8",              &vRstack8,          false);
  vAddPrimitive("r9",              &vRstack9,          false);
}

/*----------------------------------------------------------------------------*/
/* cs-pick -- place copy of nth value in control stack onto control stack     */
/*----------------------------------------------------------------------------*/
void Task::vRstackCsPick(NNP_Task nnpTask)
{
  ENTRY("vRstackCsPick");
  
  // could do this directly on stack
  
  // pop which value
  i8 nThValue;
  if ( ! nnpTask->m_xDataStack.bPopParamInt(nThValue) )
    THROW_INVALID_DSTACK("cs-pick");
  
  // pop one return address
//  ValWsp xRtnWsp;
//  bool b = nnpTask->m_xReturnStack.bPop(&xRtnWsp);
//  ASSERT(b);
  
  // copy n-th value onto top
  ValWsp xValWsp;
  nnpTask->m_xReturnStack.vPeek(nThValue,&xValWsp);
  nnpTask->m_xReturnStack.vPush(xValWsp);
  
  // push back the one return address
//  nnpTask->m_xReturnStack.vPush(xRtnWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* cs-roll -- roll top control values at n ( Z Y ... B A n -- Y ... B A Z )   */
/*----------------------------------------------------------------------------*/
void Task::vRstackCsRoll(NNP_Task nnpTask)
{
  ENTRY("vRstackCsRoll");
  
  // pop n values
  i8 nValues;
  if ( ! nnpTask->m_xDataStack.bPop(nValues) )
    THROW_INVALID_DSTACK("cs-roll");
  
  ValWsp xValMid;
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_xDataStack.bPop(xValMid) )
      THROW_INVALID_DSTACK("cs-roll");
    
    nnpTask->m_xReturnStack.vPush(&xValMid);
  }
  
  Cell* pCellZ;
  if ( ! nnpTask->m_xDataStack.bPop(pCellZ) )
    THROW_INVALID_DSTACK("cs-roll");
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_xReturnStack.bPop(xValMid) )
      THROW_INVALID_RSTACK("cs-roll");
    
    nnpTask->m_xDataStack.vPush(&xValMid);
  }

  nnpTask->m_xDataStack.vPush(pCellZ);
  
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rdepth -- put number of values in return stack onto param stack            */
/*----------------------------------------------------------------------------*/
void Task::vRstackDepth(NNP_Task nnpTask)
{
  ENTRY("vRstackDepth");
  
  i8 iDepth = nnpTask->m_xReturnStack.iValues();
  
  nnpTask->m_xDataStack.vPush(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rdrop -- discard top value off rstack                                      */
/*----------------------------------------------------------------------------*/
void Task::vRstackDrop(NNP_Task nnpTask)
{
  ENTRY("vRstackDrop");
  
  // should just discard on stack
  
  ValWsp xValA;
  if ( ! nnpTask->m_xReturnStack.bPop(xValA) )
    THROW_INVALID_RSTACK("rdrop");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r@ -- place copy of top value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstackFetch(NNP_Task nnpTask)
{
  ENTRY("vRstackFetch");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(0,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 2r@ -- place copy of two top values in return stack onto param stack       */
/*----------------------------------------------------------------------------*/
void Task::vRstackFetchTwo(NNP_Task nnpTask)
{
  ENTRY("vRstackFetchTwo");
  
  // could do this directly on stack
  
  ValWsp xValX0;
  ValWsp xValX1;
  
  nnpTask->m_xReturnStack.vPeek(0,&xValX0);
  nnpTask->m_xReturnStack.vPeek(1,&xValX1);
  
  nnpTask->m_xDataStack.vPush(xValX1);
  nnpTask->m_xDataStack.vPush(xValX0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rpick -- place copy of nth value in return stack onto param stack          */
/*----------------------------------------------------------------------------*/
void Task::vRstackPick(NNP_Task nnpTask)
{
  ENTRY("vRstackPick");
  
  i8 iPick;
  if ( ! nnpTask->m_xDataStack.bPopParamInt(iPick) )
    THROW_INVALID_DSTACK("rpick");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(iPick,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rreverse -- reverse rstack values ( ... D C B A n -- A B C D ... )         */
/*----------------------------------------------------------------------------*/
void Task::vRstackReverse(NNP_Task nnpTask)
{
  ENTRY("vRstackReverse");
  
  ValWsp xVal0;
  ValWsp xVal1;
  ValWsp xVal2;
  ValWsp xVal3;
  ValWsp xVal4;
  ValWsp xVal5;
  ValWsp xVal6;
  ValWsp xVal7;
  
  ValWsp* apValWsp[8] = {
    &xVal0, &xVal1, &xVal2, &xVal3, &xVal4, &xVal5, &xVal6, &xVal7
  };
  
  i8 nValues;
  if ( ! nnpTask->m_xDataStack.bPop(nValues) )
    THROW_INVALID_DSTACK("areverse");
  
  for (i8 i = 0; i < nValues; i++) {
    if ( ! nnpTask->m_xReturnStack.bPop(apValWsp[i]) )
      THROW_INVALID_DSTACK("areverse");
  }
  
  for (i8 i = 0; i < nValues; i++) {
    nnpTask->m_xReturnStack.vPush(apValWsp[i]);
  }
}

/*----------------------------------------------------------------------------*/
/* rroll -- roll top rstack values at n ( ... D C B A n -- A ... D C B )      */
/*----------------------------------------------------------------------------*/
void Task::vRstackRoll(NNP_Task nnpTask)
{
  ENTRY("vRstackRoll");
  
  ValWsp xValA;
  ValWsp xValX;
  
  i8 nValues;
  if ( ! nnpTask->m_xDataStack.bPop(nValues) )
    THROW_INVALID_DSTACK("rroll");
  
  if ( ! nnpTask->m_xReturnStack.bPop(xValA) )
    THROW_INVALID_RSTACK("rroll");
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_xReturnStack.bPop(xValX) )
      THROW_INVALID_RSTACK("rroll");
    
    nnpTask->m_xDataStack.vPush(&xValX);
  }
  
  nnpTask->m_xReturnStack.vPush(&xValA);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_xDataStack.bPop(xValX) )
      THROW_INVALID_DSTACK("rroll");
    
    nnpTask->m_xReturnStack.vPush(&xValX);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -rroll -- inverse roll top rstack values at n ( Z ... B A n -- ... B A Z ) */
/*----------------------------------------------------------------------------*/
void Task::vRstackRollInv(NNP_Task nnpTask)
{
  ENTRY("vRstackRollInv");
  
  ValWsp xValZ;
  ValWsp xValX;
  
  i8 nValues;
  if ( ! nnpTask->m_xDataStack.bPop(nValues) )
    THROW_INVALID_DSTACK("-rroll");
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_xReturnStack.bPop(xValX) )
      THROW_INVALID_RSTACK("-rroll");
    
    nnpTask->m_xDataStack.vPush(&xValX);
  }
  
  if ( ! nnpTask->m_xReturnStack.bPop(xValZ) )
    THROW_INVALID_DSTACK("-rroll");
  
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_xDataStack.bPop(xValX) )
      THROW_INVALID_DSTACK("-rroll");
    
    nnpTask->m_xReturnStack.vPush(&xValX);
  }
  
  nnpTask->m_xReturnStack.vPush(&xValZ);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rswap -- swap top two values on return stack                               */
/*----------------------------------------------------------------------------*/
void Task::vRstackSwap(NNP_Task nnpTask)
{
  ENTRY("vRstackSwap");
  
  ValWsp xValA;
  ValWsp xValB;
  
  if ( ! nnpTask->m_xReturnStack.bPop(xValA,xValB) )
    THROW_INVALID_DSTACK("swap");
  
  nnpTask->m_xReturnStack.vPush(&xValA);
  nnpTask->m_xReturnStack.vPush(&xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r> -- move value from rstack to dstack                                     */
/*----------------------------------------------------------------------------*/
void Task::vRstackToP(NNP_Task nnpTask)
{
  ENTRY("vRstackToP");
  
  ValWsp xVal;
  if ( ! nnpTask->m_xReturnStack.bPop(xVal) )
    THROW_INVALID_DSTACK("r>");
  
  nnpTask->m_xDataStack.vPush(xVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >r -- move value from dstack to rstack                                     */
/*----------------------------------------------------------------------------*/
void Task::vRstackToR(NNP_Task nnpTask)
{
  ENTRY("vRstackToR");
  
  ValWsp xValOpaque;
  if ( ! nnpTask->m_xDataStack.bPop(xValOpaque) )
    THROW_INVALID_DSTACK(">r");
  
  nnpTask->m_xReturnStack.vPush(xValOpaque);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 2>r -- move two values from dstack to rstack preserving order              */
/*----------------------------------------------------------------------------*/
void Task::vRstackToRTwo(NNP_Task nnpTask)
{
  ENTRY("vRstackToRTwo");
  
  ValWsp xValX0;
  ValWsp xValX1;
  if ( ! nnpTask->m_xDataStack.bPop(xValX0,xValX1) )
    THROW_INVALID_DSTACK("2>r");
  
  nnpTask->m_xReturnStack.vPush(xValX1);
  nnpTask->m_xReturnStack.vPush(xValX0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r0 -- place copy of 0th value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstack0(NNP_Task nnpTask)
{
  ENTRY("vRstack0");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(0,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r1 -- place copy of 1st value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstack1(NNP_Task nnpTask)
{
  ENTRY("vRstack1");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(1,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r2 -- place copy of 2nd value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstack2(NNP_Task nnpTask)
{
  ENTRY("vRstack2");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(2,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r3 -- place copy of 3rd value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstack3(NNP_Task nnpTask)
{
  ENTRY("vRstack3");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(3,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r4 -- place copy of 4th value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstack4(NNP_Task nnpTask)
{
  ENTRY("vRstack4");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(4,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r5 -- place copy of 0th value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstack5(NNP_Task nnpTask)
{
  ENTRY("vRstack5");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(5,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r6 -- place copy of 6th value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstack6(NNP_Task nnpTask)
{
  ENTRY("vRstack6");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(6,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r7 -- place copy of 7th value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstack7(NNP_Task nnpTask)
{
  ENTRY("vRstack7");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(7,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r8 -- place copy of 8th value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstack8(NNP_Task nnpTask)
{
  ENTRY("vRstack8");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(8,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r9 -- place copy of 9th value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstack9(NNP_Task nnpTask)
{
  ENTRY("vRstack9");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(9,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
