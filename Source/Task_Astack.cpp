/*============================================================================*/
/* Task_Astack.cpp - task aux stack group                                     */
/*============================================================================*/

#include "StorageEngine.h"
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

// astack: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Astack(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  vAddPrimitive("astack.class",    &vAstackClass,      false);
  vAddPrimitive("adepth",          &vAstackDepth,      false);
  vAddPrimitive("adrop",           &vAstackDrop,       false);
  vAddPrimitive("adup",            &vAstackDup,        false);
  vAddPrimitive("a@",              &vAstackFetch,      false);
  vAddPrimitive("anip",            &vAstackNip,        false);
  vAddPrimitive("apick",           &vAstackPick,       false);
  vAddPrimitive("areverse",        &vAstackReverse,    false);
  vAddPrimitive(">a",              &vAstackToA,        false);
  vAddPrimitive("a>",              &vAstackToP,        false);
  vAddPrimitive(">auxStack",       &vAstackTransfer,   false);
  vAddPrimitive("aswap",           &vAstackSwap,       false);
  vAddPrimitive("avalset>",        &vAstackToValues,   false);
  vAddPrimitive(">avalset",        &vAstackToValSet,   false);
  vAddPrimitive("a0",              &vAstack0,          false);
  vAddPrimitive("a1",              &vAstack1,          false);
  vAddPrimitive("a2",              &vAstack2,          false);
  vAddPrimitive("a3",              &vAstack3,          false);
  vAddPrimitive("a4",              &vAstack4,          false);
  vAddPrimitive("a5",              &vAstack5,          false);
  vAddPrimitive("a6",              &vAstack6,          false);
  vAddPrimitive("a7",              &vAstack7,          false);
  vAddPrimitive("a8",              &vAstack8,          false);
  vAddPrimitive("a9",              &vAstack9,          false);
}

/*----------------------------------------------------------------------------*/
/* astack.class -- get class of nth value on aux stack                        */
/*----------------------------------------------------------------------------*/
void Task::vAstackClass(NNP_Task nnpTask)
{
  ENTRY("vAstackClass");
  
  i8 nTh;
  if ( ! nnpTask->m_xDataStack.bPop(nTh) )
    THROW_INVALID_DSTACK("astack.class");
  
  ValWsp xValWsp;
  
  nnpTask->m_xAuxStack.vPeek(nTh,&xValWsp);
  
  eCN eClass = xValWsp.eClass();
  
  nnpTask->m_xAuxStack.vPush(eClass);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adepth -- put number of values in aux stack onto param stack               */
/*----------------------------------------------------------------------------*/
void Task::vAstackDepth(NNP_Task nnpTask)
{
  ENTRY("vAstackDepth");
  
  i8 iDepth = nnpTask->m_xAuxStack.iValues();
  
  nnpTask->m_xDataStack.vPush(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adrop -- discard top value off astack                                      */
/*----------------------------------------------------------------------------*/
void Task::vAstackDrop(NNP_Task nnpTask)
{
  ENTRY("vAstackDrop");
  
  // should just discard on stack
  
  ValWsp xValA;
  if ( ! nnpTask->m_xAuxStack.bPop(xValA) )
    THROW_INVALID_ASTACK("adrop");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adup -- dup top of aux stack                                               */
/*----------------------------------------------------------------------------*/
void Task::vAstackDup(NNP_Task nnpTask)
{
  ENTRY("vAstackDup");
  
  // could just copy from stack back into stack
  
  ValWsp xValA;
  if ( ! nnpTask->m_xAuxStack.bPop(xValA) )
  THROW_INVALID_DSTACK("adup");
  
  nnpTask->m_xAuxStack.vPush(xValA);
  nnpTask->m_xAuxStack.vPush(xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a@ -- place copy of top value in aux stack onto param stack                */
/*----------------------------------------------------------------------------*/
void Task::vAstackFetch(NNP_Task nnpTask)
{
  ENTRY("vAstackFetch");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xAuxStack.vPeek(0,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nip -- drop the 2nd value from the aux stack ( B A -- A )                  */
/*----------------------------------------------------------------------------*/
void Task::vAstackNip(NNP_Task nnpTask)
{
  ENTRY("vAstackNip");
  
  ValWsp xValA;
  ValWsp xValB;
  
  if ( ! nnpTask->m_xAuxStack.bPop(xValA,xValB) )
  THROW_INVALID_ASTACK("anip");
  
  nnpTask->m_xAuxStack.vPush(&xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* apick -- place copy of nth value in aux stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vAstackPick(NNP_Task nnpTask)
{
  ENTRY("vAstackPick");
  
  i8 iPick;
  if ( ! nnpTask->m_xDataStack.bPopParamInt(iPick) )
    THROW_INVALID_DSTACK("apick");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xAuxStack.vPeek(iPick,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* areverse -- reverse astack values ( ... D C B A n -- A B C D ... )         */
/*----------------------------------------------------------------------------*/
void Task::vAstackReverse(NNP_Task nnpTask)
{
  ENTRY("vAstackReverse");
  
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
    if ( ! nnpTask->m_xAuxStack.bPop(apValWsp[i]) )
      THROW_INVALID_ASTACK("areverse");
  }
  
  for (i8 i = 0; i < nValues; i++) {
    nnpTask->m_xAuxStack.vPush(apValWsp[i]);
  }
}

/*----------------------------------------------------------------------------*/
/* swap -- swap top two values on aux stack                                   */
/*----------------------------------------------------------------------------*/
void Task::vAstackSwap(NNP_Task nnpTask)
{
  ENTRY("vAstackSwap");
  
  ValWsp xValA;
  ValWsp xValB;
  
  if ( ! nnpTask->m_xAuxStack.bPop(xValA,xValB) )
  THROW_INVALID_ASTACK("aswap");
  
  nnpTask->m_xAuxStack.vPush(&xValA);
  nnpTask->m_xAuxStack.vPush(&xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >avalset -- collapse values on aux stack to a valset                       */
/*----------------------------------------------------------------------------*/
void Task::vAstackToValSet(NNP_Task nnpTask)
{
  ENTRY("vAstackToValSet");
  
  i8 nVal;
  
  if ( ! nnpTask->m_xAuxStack.bPop(nVal) )
  THROW_INVALID_ASTACK(">avalset");
  
  if ( ! nnpTask->m_xAuxStack.bGuard(nVal) )
  THROW_INVALID_ASTACK(">avalset");
  
  char* pCursor = nullptr;
  ValSet xValSet;
  
  ValWsp xVal;
  for (i8 iVal = 0; iVal < nVal; iVal++) {
    
    if ( ! nnpTask->m_xAuxStack.bPop(xVal) )
    THROW_INVALID_ASTACK(">avalset");
    
    xValSet.vAppend(pCursor,xVal);
  }
  
  //  vTrace("ValSet %lld bytes\n",xValSet.iSize());
  
  nnpTask->m_xAuxStack.vPush(xValSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >avalues -- expand valset to values on aux stack                           */
/*----------------------------------------------------------------------------*/
void Task::vAstackToValues(NNP_Task nnpTask)
{
  ENTRY("vDstackToValues");
  
  ValSet xValSet;
  
  if ( ! nnpTask->m_xAuxStack.bPop(xValSet) )
    THROW_INVALID_ASTACK(">avalues");
  
  i8 nVal = 0;
  ValWsp* pValWsp[128];
  
  char* pCursor = nullptr;
  while (xValSet.bExtract(pCursor,pValWsp[nVal]))
    nVal++;
  
  for (i8 iVal = nVal - 1; iVal >= 0; iVal--)
  nnpTask->m_xAuxStack.vPush(pValWsp[iVal]);
  
  nnpTask->m_xAuxStack.vPush(nVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >a -- move value from dstack to astack                                     */
/*----------------------------------------------------------------------------*/
void Task::vAstackToA(NNP_Task nnpTask)
{
  ENTRY("vAstackToA");
  
  ValWsp xValOpaque;
  if ( ! nnpTask->m_xDataStack.bPop(xValOpaque) )
    THROW_INVALID_DSTACK(">a");
  
  nnpTask->m_xAuxStack.vPush(xValOpaque);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a> -- move value from astack to dstack                                     */
/*----------------------------------------------------------------------------*/
void Task::vAstackToP(NNP_Task nnpTask)
{
  ENTRY("vAstackToP");
  
  ValWsp xVal;
  if ( ! nnpTask->m_xAuxStack.bPop(xVal) )
    THROW_INVALID_ASTACK("a>");
  
  nnpTask->m_xDataStack.vPush(xVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >auxStack -- pop stack of this task and push onto aux stack of that task   */
/*----------------------------------------------------------------------------*/
void Task::vAstackTransfer(NNP_Task nnpTask)
{
  ENTRY("vAstackTransfer");
  
  Object* pPtrTask;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTask) )
    THROW_INVALID_DSTACK(">auxStack");
  Task* pThatTask = _pTask(pPtrTask);
  
  ValWsp xValWsp;
  if ( ! nnpTask->m_xDataStack.bPop(xValWsp) )
    THROW_INVALID_DSTACK(">auxStack");
  
  pThatTask->m_xLock.vGrabWrite();
  pThatTask->m_xAuxStack.vPush(xValWsp);
  pThatTask->m_xLock.vDropWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a0 -- place copy of 0th value in aux stack onto param stack                */
/*----------------------------------------------------------------------------*/
void Task::vAstack0(NNP_Task nnpTask)
{
  ENTRY("vAstack0");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xAuxStack.vPeek(0,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a1 -- place copy of 1st value in aux stack onto param stack                */
/*----------------------------------------------------------------------------*/
void Task::vAstack1(NNP_Task nnpTask)
{
  ENTRY("vAstack1");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xAuxStack.vPeek(1,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r2 -- place copy of 2nd value in aux stack onto param stack                */
/*----------------------------------------------------------------------------*/
void Task::vAstack2(NNP_Task nnpTask)
{
  ENTRY("vAstack2");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xAuxStack.vPeek(2,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r3 -- place copy of 3rd value in aux stack onto param stack                */
/*----------------------------------------------------------------------------*/
void Task::vAstack3(NNP_Task nnpTask)
{
  ENTRY("vAstack3");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xAuxStack.vPeek(3,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r4 -- place copy of 4th value in aux stack onto param stack                */
/*----------------------------------------------------------------------------*/
void Task::vAstack4(NNP_Task nnpTask)
{
  ENTRY("vAstack4");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xAuxStack.vPeek(4,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r5 -- place copy of 0th value in aux stack onto param stack                */
/*----------------------------------------------------------------------------*/
void Task::vAstack5(NNP_Task nnpTask)
{
  ENTRY("vAstack5");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xAuxStack.vPeek(5,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r6 -- place copy of 6th value in aux stack onto param stack                */
/*----------------------------------------------------------------------------*/
void Task::vAstack6(NNP_Task nnpTask)
{
  ENTRY("vAstack6");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xAuxStack.vPeek(6,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r7 -- place copy of 7th value in aux stack onto param stack                */
/*----------------------------------------------------------------------------*/
void Task::vAstack7(NNP_Task nnpTask)
{
  ENTRY("vAstack7");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xAuxStack.vPeek(7,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r8 -- place copy of 8th value in aux stack onto param stack                */
/*----------------------------------------------------------------------------*/
void Task::vAstack8(NNP_Task nnpTask)
{
  ENTRY("vAstack8");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xAuxStack.vPeek(8,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r9 -- place copy of 9th value in aux stack onto param stack                */
/*----------------------------------------------------------------------------*/
void Task::vAstack9(NNP_Task nnpTask)
{
  ENTRY("vAstack9");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xAuxStack.vPeek(9,&xValWsp);
  
  nnpTask->m_xDataStack.vPush(xValWsp);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
