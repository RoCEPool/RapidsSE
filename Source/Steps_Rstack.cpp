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
void Task::vRstackCsPick(NNP_Task nnpTask)
{
  ENTRY("vRstackCsPick"_Str);
  
  // could do this directly on stack
  
  // pop which value
  i8 nThValue;
  if ( ! nnpTask->m_pDataStack->bPopParamInt(nThValue) )
    THROW_INVALID_DSTACK("cs-pick"_Str);
  
  // pop one return address
//  ValWsp xRtnWsp;
//  bool b = nnpTask->m_pReturnStack->bPop(&xRtnWsp);
//  ASSERT(b);
  
  // copy n-th value onto top
  ValWsp xValWsp;
  nnpTask->m_pReturnStack->vPeek(nThValue,&xValWsp);
  nnpTask->m_pReturnStack->vPushValWsp(xValWsp);
  
  // push back the one return address
//  nnpTask->m_pReturnStack->vPush(xRtnWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* cs-roll -- roll top control values at n ( Z Y ... B A n -- Y ... B A Z )   */
/*----------------------------------------------------------------------------*/
void Task::vRstackCsRoll(NNP_Task nnpTask)
{
  ENTRY("vRstackCsRoll"_Str);
  
  // pop n values
  i8 nValues;
  if ( ! nnpTask->m_pDataStack->bPop(nValues) )
    THROW_INVALID_DSTACK("cs-roll"_Str);
  
  ValWsp xValMid;
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_pDataStack->bPop(xValMid) )
      THROW_INVALID_DSTACK("cs-roll"_Str);
    
    nnpTask->m_pReturnStack->vPushValWsp(xValMid);
  }
  
  Cell* pCellZ;
  if ( ! nnpTask->m_pDataStack->bPop(pCellZ) )
    THROW_INVALID_DSTACK("cs-roll"_Str);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_pReturnStack->bPop(xValMid) )
      THROW_INVALID_RSTACK("cs-roll"_Str);
    
    nnpTask->m_pDataStack->vPushValWsp(xValMid);
  }

  nnpTask->m_pDataStack->vPushCell(pCellZ);
  
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rdepth -- put number of values in return stack onto data stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstackDepth(NNP_Task nnpTask)
{
  ENTRY("vRstackDepth"_Str);
  
  i8 iDepth = nnpTask->m_pReturnStack->iValues();
  
  nnpTask->m_pDataStack->vPushInt(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rdrop -- discard top value off rstack                                      */
/*----------------------------------------------------------------------------*/
void Task::vRstackDrop(NNP_Task nnpTask)
{
  ENTRY("vRstackDrop"_Str);
  
  // should just discard on stack
  
  ValWsp xValA;
  if ( ! nnpTask->m_pReturnStack->bPop(xValA) )
    THROW_INVALID_RSTACK("rdrop"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r@ -- place copy of top value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Task::vRstackFetch(NNP_Task nnpTask)
{
  ENTRY("vRstackFetch"_Str);
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_pReturnStack->vPeek(0,&xValWsp);
  
  nnpTask->m_pDataStack->vPushValWsp(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 2r@ -- place copy of two top values in return stack onto data stack        */
/*----------------------------------------------------------------------------*/
void Task::vRstackFetchTwo(NNP_Task nnpTask)
{
  ENTRY("vRstackFetchTwo"_Str);
  
  // could do this directly on stack
  
  ValWsp xValX0;
  ValWsp xValX1;
  
  nnpTask->m_pReturnStack->vPeek(0,&xValX0);
  nnpTask->m_pReturnStack->vPeek(1,&xValX1);
  
  nnpTask->m_pDataStack->vPushValWsp(xValX1);
  nnpTask->m_pDataStack->vPushValWsp(xValX0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rpick -- place copy of nth value in return stack onto data stack           */
/*----------------------------------------------------------------------------*/
void Task::vRstackPick(NNP_Task nnpTask)
{
  ENTRY("vRstackPick"_Str);
  
  i8 iPick;
  if ( ! nnpTask->m_pDataStack->bPopParamInt(iPick) )
    THROW_INVALID_DSTACK("rpick"_Str);
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_pReturnStack->vPeek(iPick,&xValWsp);
  
  nnpTask->m_pDataStack->vPushValWsp(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rreverse -- reverse rstack values ( ... D C B A n -- A B C D ... )         */
/*----------------------------------------------------------------------------*/
void Task::vRstackReverse(NNP_Task nnpTask)
{
  ENTRY("vRstackReverse"_Str);
  
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
  if ( ! nnpTask->m_pDataStack->bPop(nValues) )
    THROW_INVALID_DSTACK("areverse"_Str);
  
  for (i8 i = 0; i < nValues; i++) {
    if ( ! nnpTask->m_pReturnStack->bPop(apValWsp[i]) )
      THROW_INVALID_DSTACK("areverse"_Str);
  }
  
  for (i8 i = 0; i < nValues; i++) {
    nnpTask->m_pReturnStack->vPushValue(apValWsp[i]);
  }
}

/*----------------------------------------------------------------------------*/
/* rroll -- roll top rstack values at n ( ... D C B A n -- A ... D C B )      */
/*----------------------------------------------------------------------------*/
void Task::vRstackRoll(NNP_Task nnpTask)
{
  ENTRY("vRstackRoll"_Str);
  
  ValWsp xValA;
  ValWsp xValX;
  
  i8 nValues;
  if ( ! nnpTask->m_pDataStack->bPop(nValues) )
    THROW_INVALID_DSTACK("rroll"_Str);
  
  if ( ! nnpTask->m_pReturnStack->bPop(xValA) )
    THROW_INVALID_RSTACK("rroll"_Str);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_pReturnStack->bPop(xValX) )
      THROW_INVALID_RSTACK("rroll"_Str);
    
    nnpTask->m_pDataStack->vPushValWsp(xValX);
  }
  
  nnpTask->m_pReturnStack->vPushValWsp(xValA);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_pDataStack->bPop(xValX) )
      THROW_INVALID_DSTACK("rroll"_Str);
    
    nnpTask->m_pReturnStack->vPushValWsp(xValX);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -rroll -- inverse roll top rstack values at n ( Z ... B A n -- ... B A Z ) */
/*----------------------------------------------------------------------------*/
void Task::vRstackRollInv(NNP_Task nnpTask)
{
  ENTRY("vRstackRollInv"_Str);
  
  ValWsp xValZ;
  ValWsp xValX;
  
  i8 nValues;
  if ( ! nnpTask->m_pDataStack->bPop(nValues) )
    THROW_INVALID_DSTACK("-rroll"_Str);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_pReturnStack->bPop(xValX) )
      THROW_INVALID_RSTACK("-rroll"_Str);
    
    nnpTask->m_pDataStack->vPushValWsp(xValX);
  }
  
  if ( ! nnpTask->m_pReturnStack->bPop(xValZ) )
    THROW_INVALID_DSTACK("-rroll"_Str);
  
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_pDataStack->bPop(xValX) )
      THROW_INVALID_DSTACK("-rroll"_Str);
    
    nnpTask->m_pReturnStack->vPushValWsp(xValX);
  }
  
  nnpTask->m_pReturnStack->vPushValWsp(xValZ);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rstk -- dump the return stack                                              */
/*----------------------------------------------------------------------------*/
/*
void Task::vRstackStk(NNP_Task nnpTask)
{
  ENTRY("vRstackStk"_Str);
  
  nnpTask->m_pReturnStack->vDump(nnpTask->m_sName.s(),"ReturnStack:");
  
  EXIT();
}
*/

/*----------------------------------------------------------------------------*/
/* rswap -- swap top two values on return stack                               */
/*----------------------------------------------------------------------------*/
void Task::vRstackSwap(NNP_Task nnpTask)
{
  ENTRY("vRstackSwap"_Str);
  
  ValWsp xValA;
  ValWsp xValB;
  
  if ( ! nnpTask->m_pReturnStack->bPop(xValA,xValB) )
    THROW_INVALID_DSTACK("swap"_Str);
  
  nnpTask->m_pReturnStack->vPushValWsp(xValA);
  nnpTask->m_pReturnStack->vPushValWsp(xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r> -- move value from rstack to dstack                                     */
/*----------------------------------------------------------------------------*/
void Task::vRstackToP(NNP_Task nnpTask)
{
  ENTRY("vRstackToP"_Str);
  
  ValWsp xVal;
  if ( ! nnpTask->m_pReturnStack->bPop(xVal) )
    THROW_INVALID_DSTACK("r>"_Str);
  
  nnpTask->m_pDataStack->vPushValWsp(xVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >r -- move value from dstack to rstack                                     */
/*----------------------------------------------------------------------------*/
void Task::vRstackToR(NNP_Task nnpTask)
{
  ENTRY("vRstackToR"_Str);
  
  ValWsp xValOpaque;
  if ( ! nnpTask->m_pDataStack->bPop(xValOpaque) )
    THROW_INVALID_DSTACK(">r"_Str);
  
  nnpTask->m_pReturnStack->vPushValWsp(xValOpaque);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 2>r -- move two values from dstack to rstack preserving order              */
/*----------------------------------------------------------------------------*/
void Task::vRstackToRTwo(NNP_Task nnpTask)
{
  ENTRY("vRstackToRTwo"_Str);
  
  ValWsp xValX0;
  ValWsp xValX1;
  if ( ! nnpTask->m_pDataStack->bPop(xValX0,xValX1) )
    THROW_INVALID_DSTACK("2>r"_Str);
  
  nnpTask->m_pReturnStack->vPushValWsp(xValX1);
  nnpTask->m_pReturnStack->vPushValWsp(xValX0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r0 -- place copy of 0th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Task::vRstack0(NNP_Task nnpTask)
{
  ENTRY("vRstack0"_Str);
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_pReturnStack->vPeek(0,&xValWsp);
  
  nnpTask->m_pDataStack->vPushValWsp(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r1 -- place copy of 1st value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Task::vRstack1(NNP_Task nnpTask)
{
  ENTRY("vRstack1"_Str);
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_pReturnStack->vPeek(1,&xValWsp);
  
  nnpTask->m_pDataStack->vPushValWsp(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r2 -- place copy of 2nd value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Task::vRstack2(NNP_Task nnpTask)
{
  ENTRY("vRstack2"_Str);
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_pReturnStack->vPeek(2,&xValWsp);
  
  nnpTask->m_pDataStack->vPushValWsp(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r3 -- place copy of 3rd value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Task::vRstack3(NNP_Task nnpTask)
{
  ENTRY("vRstack3"_Str);
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_pReturnStack->vPeek(3,&xValWsp);
  
  nnpTask->m_pDataStack->vPushValWsp(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r4 -- place copy of 4th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Task::vRstack4(NNP_Task nnpTask)
{
  ENTRY("vRstack4"_Str);
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_pReturnStack->vPeek(4,&xValWsp);
  
  nnpTask->m_pDataStack->vPushValWsp(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r5 -- place copy of 0th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Task::vRstack5(NNP_Task nnpTask)
{
  ENTRY("vRstack5"_Str);
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_pReturnStack->vPeek(5,&xValWsp);
  
  nnpTask->m_pDataStack->vPushValWsp(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r6 -- place copy of 6th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Task::vRstack6(NNP_Task nnpTask)
{
  ENTRY("vRstack6"_Str);
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_pReturnStack->vPeek(6,&xValWsp);
  
  nnpTask->m_pDataStack->vPushValWsp(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r7 -- place copy of 7th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Task::vRstack7(NNP_Task nnpTask)
{
  ENTRY("vRstack7"_Str);
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_pReturnStack->vPeek(7,&xValWsp);
  
  nnpTask->m_pDataStack->vPushValWsp(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r8 -- place copy of 8th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Task::vRstack8(NNP_Task nnpTask)
{
  ENTRY("vRstack8"_Str);
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_pReturnStack->vPeek(8,&xValWsp);
  
  nnpTask->m_pDataStack->vPushValWsp(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r9 -- place copy of 9th value in return stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Task::vRstack9(NNP_Task nnpTask)
{
  ENTRY("vRstack9"_Str);
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_pReturnStack->vPeek(9,&xValWsp);
  
  nnpTask->m_pDataStack->vPushValWsp(xValWsp);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
