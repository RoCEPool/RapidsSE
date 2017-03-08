/*============================================================================*/
/* Steps_Astack.cpp - aux stack step group                                    */
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
  
  vAddPrimitive("astack.class"_Str,    &vAstackClass,      false);
  vAddPrimitive("adepth"_Str,          &vAstackDepth,      false);
  vAddPrimitive("adrop"_Str,           &vAstackDrop,       false);
  vAddPrimitive("adup"_Str,            &vAstackDup,        false);
  vAddPrimitive("a@"_Str,              &vAstackFetch,      false);
  vAddPrimitive("anip"_Str,            &vAstackNip,        false);
  vAddPrimitive("apick"_Str,           &vAstackPick,       false);
  vAddPrimitive("areverse"_Str,        &vAstackReverse,    false);
  vAddPrimitive("astk"_Str,            &vAstackStk,        false);
  vAddPrimitive(">a"_Str,              &vAstackToA,        false);
  vAddPrimitive("a>"_Str,              &vAstackToP,        false);
  vAddPrimitive(">auxStack"_Str,       &vAstackTransfer,   false);
  vAddPrimitive("aswap"_Str,           &vAstackSwap,       false);
  vAddPrimitive("avalset>"_Str,        &vAstackToValues,   false);
  vAddPrimitive(">avalset"_Str,        &vAstackToValSet,   false);
  vAddPrimitive("a0"_Str,              &vAstack0,          false);
  vAddPrimitive("a1"_Str,              &vAstack1,          false);
  vAddPrimitive("a2"_Str,              &vAstack2,          false);
  vAddPrimitive("a3"_Str,              &vAstack3,          false);
  vAddPrimitive("a4"_Str,              &vAstack4,          false);
  vAddPrimitive("a5"_Str,              &vAstack5,          false);
  vAddPrimitive("a6"_Str,              &vAstack6,          false);
  vAddPrimitive("a7"_Str,              &vAstack7,          false);
  vAddPrimitive("a8"_Str,              &vAstack8,          false);
  vAddPrimitive("a9"_Str,              &vAstack9,          false);
}

/*----------------------------------------------------------------------------*/
/* astack.class -- get class of nth value on aux stack                        */
/*----------------------------------------------------------------------------*/
void Task::vAstackClass(NNP_Task nnpTask)
{
  ENTRY("vAstackClass"_Str);
  
  i8 nTh;
  if ( ! nnpTask->m_pDataStack->bPop(nTh) )
    THROW_INVALID_DSTACK("astack.class"_Str);
  
  ValWsp xValWsp;
  
  nnpTask->m_pAuxStack->vPeek(nTh,&xValWsp);
  
  eCN eClass = xValWsp.eClass();
  
  nnpTask->m_pAuxStack->vPush(eClass);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adepth -- put number of values in aux stack onto data stack                */
/*----------------------------------------------------------------------------*/
void Task::vAstackDepth(NNP_Task nnpTask)
{
  ENTRY("vAstackDepth"_Str);
  
  i8 iDepth = nnpTask->m_pAuxStack->iValues();
  
  nnpTask->m_pDataStack->vPush(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adrop -- discard top value off astack                                      */
/*----------------------------------------------------------------------------*/
void Task::vAstackDrop(NNP_Task nnpTask)
{
  ENTRY("vAstackDrop"_Str);
  
  // should just discard on stack
  
  ValWsp xValA;
  if ( ! nnpTask->m_pAuxStack->bPop(xValA) )
    THROW_INVALID_ASTACK("adrop"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adup -- dup top of aux stack                                               */
/*----------------------------------------------------------------------------*/
void Task::vAstackDup(NNP_Task nnpTask)
{
  ENTRY("vAstackDup"_Str);
  
  // could just copy from stack back into stack
  
  ValWsp xValA;
  if ( ! nnpTask->m_pAuxStack->bPop(xValA) )
  THROW_INVALID_DSTACK("adup"_Str);
  
  nnpTask->m_pAuxStack->vPush(xValA);
  nnpTask->m_pAuxStack->vPush(xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a@ -- place copy of top value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Task::vAstackFetch(NNP_Task nnpTask)
{
  ENTRY("vAstackFetch"_Str);
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_pAuxStack->vPeek(0,&xValWsp);
  
  nnpTask->m_pDataStack->vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nip -- drop the 2nd value from the aux stack ( B A -- A )                  */
/*----------------------------------------------------------------------------*/
void Task::vAstackNip(NNP_Task nnpTask)
{
  ENTRY("vAstackNip"_Str);
  
  ValWsp xValA;
  ValWsp xValB;
  
  if ( ! nnpTask->m_pAuxStack->bPop(xValA,xValB) )
  THROW_INVALID_ASTACK("anip"_Str);
  
  nnpTask->m_pAuxStack->vPush(&xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* apick -- place copy of nth value in aux stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Task::vAstackPick(NNP_Task nnpTask)
{
  ENTRY("vAstackPick"_Str);
  
  i8 iPick;
  if ( ! nnpTask->m_pDataStack->bPopParamInt(iPick) )
    THROW_INVALID_DSTACK("apick"_Str);
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_pAuxStack->vPeek(iPick,&xValWsp);
  
  nnpTask->m_pDataStack->vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* areverse -- reverse astack values ( ... D C B A n -- A B C D ... )         */
/*----------------------------------------------------------------------------*/
void Task::vAstackReverse(NNP_Task nnpTask)
{
  ENTRY("vAstackReverse"_Str);
  
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
    if ( ! nnpTask->m_pAuxStack->bPop(apValWsp[i]) )
      THROW_INVALID_ASTACK("areverse"_Str);
  }
  
  for (i8 i = 0; i < nValues; i++) {
    nnpTask->m_pAuxStack->vPush(apValWsp[i]);
  }
}

/*----------------------------------------------------------------------------*/
/* astk -- dump the aux stack                                                 */
/*----------------------------------------------------------------------------*/
void Task::vAstackStk(NNP_Task nnpTask)
{
  ENTRY("vAstackStk"_Str);
  
  nnpTask->m_pAuxStack->vContentDump(nnpTask->m_sName,"AuxStack:"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* swap -- swap top two values on aux stack                                   */
/*----------------------------------------------------------------------------*/
void Task::vAstackSwap(NNP_Task nnpTask)
{
  ENTRY("vAstackSwap"_Str);
  
  ValWsp xValA;
  ValWsp xValB;
  
  if ( ! nnpTask->m_pAuxStack->bPop(xValA,xValB) )
  THROW_INVALID_ASTACK("aswap"_Str);
  
  nnpTask->m_pAuxStack->vPush(&xValA);
  nnpTask->m_pAuxStack->vPush(&xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >avalset -- collapse values on aux stack to a valset                       */
/*----------------------------------------------------------------------------*/
void Task::vAstackToValSet(NNP_Task nnpTask)
{
  ENTRY("vAstackToValSet"_Str);
  
  i8 nVal;
  
  if ( ! nnpTask->m_pAuxStack->bPop(nVal) )
  THROW_INVALID_ASTACK(">avalset"_Str);
  
  if ( ! nnpTask->m_pAuxStack->bGuard(nVal) )
  THROW_INVALID_ASTACK(">avalset"_Str);
  
  char* pCursor = nullptr;
  ValSet xValSet;
  
  ValWsp xVal;
  for (i8 iVal = 0; iVal < nVal; iVal++) {
    
    if ( ! nnpTask->m_pAuxStack->bPop(xVal) )
    THROW_INVALID_ASTACK(">avalset"_Str);
    
    xValSet.vAppend(pCursor,xVal);
  }
  
  //  vTrace("ValSet %lld bytes\n"_Str,xValSet.nSize());
  
  nnpTask->m_pAuxStack->vPush(xValSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >avalues -- expand valset to values on aux stack                           */
/*----------------------------------------------------------------------------*/
void Task::vAstackToValues(NNP_Task nnpTask)
{
  ENTRY("vDstackToValues"_Str);
  
  ValSet xValSet;
  
  if ( ! nnpTask->m_pAuxStack->bPop(xValSet) )
    THROW_INVALID_ASTACK(">avalues"_Str);
  
  i8 nVal = 0;
  ValWsp* pValWsp[128];
  
  char* pCursor = nullptr;
  while (xValSet.bExtract(pCursor,pValWsp[nVal]))
    nVal++;
  
  for (i8 iVal = nVal - 1; iVal >= 0; iVal--)
  nnpTask->m_pAuxStack->vPush(pValWsp[iVal]);
  
  nnpTask->m_pAuxStack->vPush(nVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >a -- move value from dstack to astack                                     */
/*----------------------------------------------------------------------------*/
void Task::vAstackToA(NNP_Task nnpTask)
{
  ENTRY("vAstackToA"_Str);
  
  ValWsp xValOpaque;
  if ( ! nnpTask->m_pDataStack->bPop(xValOpaque) )
    THROW_INVALID_DSTACK(">a"_Str);
  
  nnpTask->m_pAuxStack->vPush(xValOpaque);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a> -- move value from astack to dstack                                     */
/*----------------------------------------------------------------------------*/
void Task::vAstackToP(NNP_Task nnpTask)
{
  ENTRY("vAstackToP"_Str);
  
  ValWsp xVal;
  if ( ! nnpTask->m_pAuxStack->bPop(xVal) )
    THROW_INVALID_ASTACK("a>"_Str);
  
  nnpTask->m_pDataStack->vPush(xVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >auxStack -- pop stack of this task and push onto aux stack of that task   */
/*----------------------------------------------------------------------------*/
void Task::vAstackTransfer(NNP_Task nnpTask)
{
  ENTRY("vAstackTransfer"_Str);
  
  Object* pPtrTask;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK(">auxStack"_Str);
  Task* pThatTask = _pTask(pPtrTask);
  
  ValWsp xValWsp;
  if ( ! nnpTask->m_pDataStack->bPop(xValWsp) )
    THROW_INVALID_DSTACK(">auxStack"_Str);
  
  pThatTask->m_xLock.vGrabWrite();
  pThatTask->m_pAuxStack->vPush(xValWsp);
  pThatTask->m_xLock.vDropWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a0 -- place copy of 0th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Task::vAstack0(NNP_Task nnpTask)
{
  ENTRY("vAstack0"_Str);
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_pAuxStack->vPeek(0,&xValWsp);
  
  nnpTask->m_pDataStack->vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a1 -- place copy of 1st value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Task::vAstack1(NNP_Task nnpTask)
{
  ENTRY("vAstack1"_Str);
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_pAuxStack->vPeek(1,&xValWsp);
  
  nnpTask->m_pDataStack->vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r2 -- place copy of 2nd value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Task::vAstack2(NNP_Task nnpTask)
{
  ENTRY("vAstack2"_Str);
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_pAuxStack->vPeek(2,&xValWsp);
  
  nnpTask->m_pDataStack->vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r3 -- place copy of 3rd value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Task::vAstack3(NNP_Task nnpTask)
{
  ENTRY("vAstack3"_Str);
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_pAuxStack->vPeek(3,&xValWsp);
  
  nnpTask->m_pDataStack->vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r4 -- place copy of 4th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Task::vAstack4(NNP_Task nnpTask)
{
  ENTRY("vAstack4"_Str);
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_pAuxStack->vPeek(4,&xValWsp);
  
  nnpTask->m_pDataStack->vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r5 -- place copy of 0th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Task::vAstack5(NNP_Task nnpTask)
{
  ENTRY("vAstack5"_Str);
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_pAuxStack->vPeek(5,&xValWsp);
  
  nnpTask->m_pDataStack->vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r6 -- place copy of 6th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Task::vAstack6(NNP_Task nnpTask)
{
  ENTRY("vAstack6"_Str);
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_pAuxStack->vPeek(6,&xValWsp);
  
  nnpTask->m_pDataStack->vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r7 -- place copy of 7th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Task::vAstack7(NNP_Task nnpTask)
{
  ENTRY("vAstack7"_Str);
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_pAuxStack->vPeek(7,&xValWsp);
  
  nnpTask->m_pDataStack->vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r8 -- place copy of 8th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Task::vAstack8(NNP_Task nnpTask)
{
  ENTRY("vAstack8"_Str);
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_pAuxStack->vPeek(8,&xValWsp);
  
  nnpTask->m_pDataStack->vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r9 -- place copy of 9th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Task::vAstack9(NNP_Task nnpTask)
{
  ENTRY("vAstack9"_Str);
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_pAuxStack->vPeek(9,&xValWsp);
  
  nnpTask->m_pDataStack->vPush(xValWsp);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
