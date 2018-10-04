/*============================================================================*/
/* Steps_Astack.cpp - aux stack step group                                    */
/*============================================================================*/

#include "StorageEngine.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// astack: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Astack(void)
{
  vAddStep("astack.class"_Stz,    &Task::vAstackClass);
  vAddStep("adepth"_Stz,          &Task::vAstackDepth);
  vAddStep("adrop"_Stz,           &Task::vAstackDrop);
  vAddStep("adup"_Stz,            &Task::vAstackDup);
  vAddStep("a@"_Stz,              &Task::vAstackFetch);
  vAddStep("anip"_Stz,            &Task::vAstackNip);
  vAddStep("apick"_Stz,           &Task::vAstackPick);
  vAddStep("areverse"_Stz,        &Task::vAstackReverse);
  vAddStep("astk"_Stz,            &Task::vAstackStk);
  vAddStep(">a"_Stz,              &Task::vAstackToA);
  vAddStep("a>"_Stz,              &Task::vAstackToP);
  vAddStep(">auxStack"_Stz,       &Task::vAstackTransfer);
  vAddStep("aswap"_Stz,           &Task::vAstackSwap);
  vAddStep("avalset>"_Stz,        &Task::vAstackToValues);
  vAddStep(">avalset"_Stz,        &Task::vAstackToValSet);
  vAddStep("a0"_Stz,              &Task::vAstack0);
  vAddStep("a1"_Stz,              &Task::vAstack1);
  vAddStep("a2"_Stz,              &Task::vAstack2);
  vAddStep("a3"_Stz,              &Task::vAstack3);
  vAddStep("a4"_Stz,              &Task::vAstack4);
  vAddStep("a5"_Stz,              &Task::vAstack5);
  vAddStep("a6"_Stz,              &Task::vAstack6);
  vAddStep("a7"_Stz,              &Task::vAstack7);
  vAddStep("a8"_Stz,              &Task::vAstack8);
  vAddStep("a9"_Stz,              &Task::vAstack9);
}

/*----------------------------------------------------------------------------*/
/* astack.class -- get class of nth value on aux stack                        */
/*----------------------------------------------------------------------------*/
void Task::vAstackClass(MP_Task mpTask)
{
  ENTRY("vAstackClass"_Stz);
  
  i8 nTh;
  if ( ! mpTask->m_pDataStack->bPop(nTh) )
    THROW_INVALID_DSTACK("astack.class"_Stz);
  
  ValWrk xValWrk;
  
  mpTask->m_pAuxStack->vPeekValWrk(nTh,xValWrk);
  
  eCN eClass = xValWrk.eClass();
  
  mpTask->m_pAuxStack->vPushInt(eClass);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adepth -- put number of values in aux stack onto data stack                */
/*----------------------------------------------------------------------------*/
void Task::vAstackDepth(MP_Task mpTask)
{
  ENTRY("vAstackDepth"_Stz);
  
  i8 iDepth = mpTask->m_pAuxStack->iValues();
  
  mpTask->m_pDataStack->vPushInt(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adrop -- discard top value off astack                                      */
/*----------------------------------------------------------------------------*/
void Task::vAstackDrop(MP_Task mpTask)
{
  ENTRY("vAstackDrop"_Stz);
  
  // should just discard on stack
  
  ValWrk xValA;
  if ( ! mpTask->m_pAuxStack->bPop(xValA) )
    THROW_INVALID_ASTACK("adrop"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adup -- dup top of aux stack                                               */
/*----------------------------------------------------------------------------*/
void Task::vAstackDup(MP_Task mpTask)
{
  ENTRY("vAstackDup"_Stz);
  
  // could just copy from stack back into stack
  
  ValWrk xValA;
  if ( ! mpTask->m_pAuxStack->bPop(xValA) )
  THROW_INVALID_DSTACK("adup"_Stz);
  
  mpTask->m_pAuxStack->vPushValWrk(xValA);
  mpTask->m_pAuxStack->vPushValWrk(xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a@ -- place copy of top value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Task::vAstackFetch(MP_Task mpTask)
{
  ENTRY("vAstackFetch"_Stz);
  
  // could do this directly on stack
  
  ValWrk xValWrk;
  
  mpTask->m_pAuxStack->vPeekValWrk(0,xValWrk);
  
  mpTask->m_pDataStack->vPushValWrk(xValWrk);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nip -- drop the 2nd value from the aux stack ( B A -- A )                  */
/*----------------------------------------------------------------------------*/
void Task::vAstackNip(MP_Task mpTask)
{
  ENTRY("vAstackNip"_Stz);
  
  ValWrk xValA;
  ValWrk xValB;
  
  if ( ! mpTask->m_pAuxStack->bPop(xValA,xValB) )
  THROW_INVALID_ASTACK("anip"_Stz);
  
  mpTask->m_pAuxStack->vPushValWrk(xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* apick -- place copy of nth value in aux stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Task::vAstackPick(MP_Task mpTask)
{
  ENTRY("vAstackPick"_Stz);
  
  i8 iPick;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iPick) )
    THROW_INVALID_DSTACK("apick"_Stz);
  
  // could do this directly on stack
  
  ValWrk xValWrk;
  
  mpTask->m_pAuxStack->vPeekValWrk(iPick,xValWrk);
  
  mpTask->m_pDataStack->vPushValWrk(xValWrk);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* areverse -- reverse astack values ( ... D C B A n -- A B C D ... )         */
/*----------------------------------------------------------------------------*/
void Task::vAstackReverse(MP_Task mpTask)
{
  ENTRY("vAstackReverse"_Stz);
  
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
    THROW_INVALID_DSTACK("areverse"_Stz);
  
  for (i8 i = 0; i < nValues; i++) {
    if ( ! mpTask->m_pAuxStack->bPop(apValWrk[i]) )
      THROW_INVALID_ASTACK("areverse"_Stz);
  }
  
  for (i8 i = 0; i < nValues; i++) {
    mpTask->m_pAuxStack->vPushValWrk(apValWrk[i]);
  }
}

/*----------------------------------------------------------------------------*/
/* astk -- dump the aux stack                                                 */
/*----------------------------------------------------------------------------*/
void Task::vAstackStk(MP_Task mpTask)
{
  ENTRY("vAstackStk"_Stz);
  
  mpTask->m_pAuxStack->vContentDump(
    (Stz*)mpTask->aJobName(),
    "AuxStack:"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* swap -- swap top two values on aux stack                                   */
/*----------------------------------------------------------------------------*/
void Task::vAstackSwap(MP_Task mpTask)
{
  ENTRY("vAstackSwap"_Stz);
  
  ValWrk xValA;
  ValWrk xValB;
  
  if ( ! mpTask->m_pAuxStack->bPop(xValA,xValB) )
  THROW_INVALID_ASTACK("aswap"_Stz);
  
  mpTask->m_pAuxStack->vPushValWrk(xValA);
  mpTask->m_pAuxStack->vPushValWrk(xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >avalset -- collapse values on aux stack to a valset                       */
/*----------------------------------------------------------------------------*/
void Task::vAstackToValSet(MP_Task mpTask)
{
  ENTRY("vAstackToValSet"_Stz);
  
  i8 nVal;
  
  if ( ! mpTask->m_pAuxStack->bPop(nVal) )
  THROW_INVALID_ASTACK(">avalset"_Stz);
  
  if ( ! mpTask->m_pAuxStack->bGuard(nVal) )
  THROW_INVALID_ASTACK(">avalset"_Stz);
  
  Stz* pCursor = nullptr;
  ValSet xValSet;
  
  ValWrk xVal;
  for (i8 iVal = 0; iVal < nVal; iVal++) {
    
    if ( ! mpTask->m_pAuxStack->bPop(xVal) )
    THROW_INVALID_ASTACK(">avalset"_Stz);
    
    xValSet.vAppend(pCursor,xVal.pNestedValueSelf());
  }
  
  //  vTrace("ValSet %lld bytes\n"_Stz,xValSet.nSize());
  
  mpTask->m_pAuxStack->vPushValSet(xValSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >avalues -- expand valset to values on aux stack                           */
/*----------------------------------------------------------------------------*/
void Task::vAstackToValues(MP_Task mpTask)
{
  ENTRY("vDstackToValues"_Stz);
  
  ValSet xValSet;
  
  if ( ! mpTask->m_pAuxStack->bPop(xValSet) )
    THROW_INVALID_ASTACK(">avalues"_Stz);
  
  i8 nVal = 0;
  Value* pValue[128];
  
  Stz* pCursor = nullptr;
  while (xValSet.bExtractValue(pCursor,pValue[nVal]))
    nVal++;
  
  for (i8 iVal = nVal - 1; iVal >= 0; iVal--)
    mpTask->m_pAuxStack->vPushValue(pValue[iVal]);
  
  mpTask->m_pAuxStack->vPushInt(nVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >a -- move value from dstack to astack                                     */
/*----------------------------------------------------------------------------*/
void Task::vAstackToA(MP_Task mpTask)
{
  ENTRY("vAstackToA"_Stz);
  
  ValWrk xValOpaque;
  if ( ! mpTask->m_pDataStack->bPop(xValOpaque) )
    THROW_INVALID_DSTACK(">a"_Stz);
  
  mpTask->m_pAuxStack->vPushValWrk(xValOpaque);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a> -- move value from astack to dstack                                     */
/*----------------------------------------------------------------------------*/
void Task::vAstackToP(MP_Task mpTask)
{
  ENTRY("vAstackToP"_Stz);
  
  ValWrk xVal;
  if ( ! mpTask->m_pAuxStack->bPop(xVal) )
    THROW_INVALID_ASTACK("a>"_Stz);
  
  mpTask->m_pDataStack->vPushValWrk(xVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >auxStack -- pop stack of this task and push onto aux stack of that task   */
/*----------------------------------------------------------------------------*/
void Task::vAstackTransfer(MP_Task mpTask)
{
  ENTRY("vAstackTransfer"_Stz);
  
  Object* pPtrTask;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK(">auxStack"_Stz);
  Task* pThatTask = _pTask(pPtrTask);
  
  ValWrk xValWrk;
  if ( ! mpTask->m_pDataStack->bPop(xValWrk) )
    THROW_INVALID_DSTACK(">auxStack"_Stz);
  
  pThatTask->m_xLock.vGrabWrite();
  pThatTask->m_pAuxStack->vPushValWrk(xValWrk);
  pThatTask->m_xLock.vDropWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a0 -- place copy of 0th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Task::vAstack0(MP_Task mpTask)
{
  ENTRY("vAstack0"_Stz);
  
  // could do this directly on stack
  
  ValWrk xValWrk;
  
  mpTask->m_pAuxStack->vPeekValWrk(0,xValWrk);
  
  mpTask->m_pDataStack->vPushValWrk(xValWrk);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a1 -- place copy of 1st value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Task::vAstack1(MP_Task mpTask)
{
  ENTRY("vAstack1"_Stz);
  
  // could do this directly on stack
  
  ValWrk xValWrk;
  
  mpTask->m_pAuxStack->vPeekValWrk(1,xValWrk);
  
  mpTask->m_pDataStack->vPushValWrk(xValWrk);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r2 -- place copy of 2nd value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Task::vAstack2(MP_Task mpTask)
{
  ENTRY("vAstack2"_Stz);
  
  // could do this directly on stack
  
  ValWrk xValWrk;
  
  mpTask->m_pAuxStack->vPeekValWrk(2,xValWrk);
  
  mpTask->m_pDataStack->vPushValWrk(xValWrk);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r3 -- place copy of 3rd value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Task::vAstack3(MP_Task mpTask)
{
  ENTRY("vAstack3"_Stz);
  
  // could do this directly on stack
  
  ValWrk xValWrk;
  
  mpTask->m_pAuxStack->vPeekValWrk(3,xValWrk);
  
  mpTask->m_pDataStack->vPushValWrk(xValWrk);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r4 -- place copy of 4th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Task::vAstack4(MP_Task mpTask)
{
  ENTRY("vAstack4"_Stz);
  
  // could do this directly on stack
  
  ValWrk xValWrk;
  
  mpTask->m_pAuxStack->vPeekValWrk(4,xValWrk);
  
  mpTask->m_pDataStack->vPushValWrk(xValWrk);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r5 -- place copy of 0th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Task::vAstack5(MP_Task mpTask)
{
  ENTRY("vAstack5"_Stz);
  
  // could do this directly on stack
  
  ValWrk xValWrk;
  
  mpTask->m_pAuxStack->vPeekValWrk(5,xValWrk);
  
  mpTask->m_pDataStack->vPushValWrk(xValWrk);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r6 -- place copy of 6th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Task::vAstack6(MP_Task mpTask)
{
  ENTRY("vAstack6"_Stz);
  
  // could do this directly on stack
  
  ValWrk xValWrk;
  
  mpTask->m_pAuxStack->vPeekValWrk(6,xValWrk);
  
  mpTask->m_pDataStack->vPushValWrk(xValWrk);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r7 -- place copy of 7th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Task::vAstack7(MP_Task mpTask)
{
  ENTRY("vAstack7"_Stz);
  
  // could do this directly on stack
  
  ValWrk xValWrk;
  
  mpTask->m_pAuxStack->vPeekValWrk(7,xValWrk);
  
  mpTask->m_pDataStack->vPushValWrk(xValWrk);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r8 -- place copy of 8th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Task::vAstack8(MP_Task mpTask)
{
  ENTRY("vAstack8"_Stz);
  
  // could do this directly on stack
  
  ValWrk xValWrk;
  
  mpTask->m_pAuxStack->vPeekValWrk(8,xValWrk);
  
  mpTask->m_pDataStack->vPushValWrk(xValWrk);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r9 -- place copy of 9th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Task::vAstack9(MP_Task mpTask)
{
  ENTRY("vAstack9"_Stz);
  
  // could do this directly on stack
  
  ValWrk xValWrk;
  
  mpTask->m_pAuxStack->vPeekValWrk(9,xValWrk);
  
  mpTask->m_pDataStack->vPushValWrk(xValWrk);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
