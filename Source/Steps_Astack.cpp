/*============================================================================*/
/* Steps_Astack.cpp - aux stack step group                                    */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Astack(void)
{
  vAddStep("astack.class"_Stz,    &Step::vAstackClass);
  vAddStep("adepth"_Stz,          &Step::vAstackDepth);
  vAddStep("adrop"_Stz,           &Step::vAstackDrop);
  vAddStep("adup"_Stz,            &Step::vAstackDup);
  vAddStep("a@"_Stz,              &Step::vAstackFetch);
  vAddStep("anip"_Stz,            &Step::vAstackNip);
  vAddStep("apick"_Stz,           &Step::vAstackPick);
  vAddStep("areverse"_Stz,        &Step::vAstackReverse);
  vAddStep("astk"_Stz,            &Step::vAstackStk);
  vAddStep(">a"_Stz,              &Step::vAstackToA);
  vAddStep("a>"_Stz,              &Step::vAstackToD);
  vAddStep(">auxStack"_Stz,       &Step::vAstackTransfer);
  vAddStep("aswap"_Stz,           &Step::vAstackSwap);
  vAddStep("a0"_Stz,              &Step::vAstack0);
  vAddStep("a1"_Stz,              &Step::vAstack1);
  vAddStep("a2"_Stz,              &Step::vAstack2);
  vAddStep("a3"_Stz,              &Step::vAstack3);
  vAddStep("a4"_Stz,              &Step::vAstack4);
  vAddStep("a5"_Stz,              &Step::vAstack5);
  vAddStep("a6"_Stz,              &Step::vAstack6);
  vAddStep("a7"_Stz,              &Step::vAstack7);
  vAddStep("a8"_Stz,              &Step::vAstack8);
  vAddStep("a9"_Stz,              &Step::vAstack9);
}

/*----------------------------------------------------------------------------*/
/* astack.class -- get class of nth value on aux stack                        */
/*----------------------------------------------------------------------------*/
void Step::vAstackClass(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vAstackClass"_Stz);
  
  i8 nTh;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(nTh) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK);
  
  GenVar xVariable;
  
  mpTask->m_pForth->m_pAuxStack->vPeekValue(nTh,xVariable);
  
  eCN eClass = xVariable.eClass();
  
  mpTask->m_pForth->m_pAuxStack->vPushI8(eClass);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adepth -- put number of values in aux stack onto data stack                */
/*----------------------------------------------------------------------------*/
void Step::vAstackDepth(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vAstackDepth"_Stz);
  
  i8 iDepth = mpTask->m_pForth->m_pAuxStack->nValues();
  
  mpTask->m_pForth->m_pDataStack->vPushI8(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adrop -- discard top value off astack                                      */
/*----------------------------------------------------------------------------*/
void Step::vAstackDrop(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vAstackDrop"_Stz);
  
  // should just discard on stack
  
  GenVar xValA;
  if ( ! mpTask->m_pForth->m_pAuxStack->bPop(xValA) )
    vThrow(__func__,__FILE__,__LINE__,FK_ASTACK);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adup -- dup top of aux stack                                               */
/*----------------------------------------------------------------------------*/
void Step::vAstackDup(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vAstackDup"_Stz);
  
  // could just copy from stack back into stack
  
  GenVar xValA;
  if ( ! mpTask->m_pForth->m_pAuxStack->bPop(xValA) )
  vThrow(__func__,__FILE__,__LINE__,FK_DSTACK);
  
  mpTask->m_pForth->m_pAuxStack->vPushGenVar(xValA);
  mpTask->m_pForth->m_pAuxStack->vPushGenVar(xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a@ -- place copy of top value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstackFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vAstackFetch"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pForth->m_pAuxStack->vPeekValue(0,xVariable);
  
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nip -- drop the 2nd value from the aux stack ( B A -- A )                  */
/*----------------------------------------------------------------------------*/
void Step::vAstackNip(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vAstackNip"_Stz);
  
  GenVar xValA;
  GenVar xValB;
  
  if ( ! mpTask->m_pForth->m_pAuxStack->bPop(xValA,xValB) )
  vThrow(__func__,__FILE__,__LINE__,FK_ASTACK);
  
  mpTask->m_pForth->m_pAuxStack->vPushGenVar(xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* apick -- place copy of nth value in aux stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vAstackPick(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vAstackPick"_Stz);
  
  i8 iPick;
  if ( ! mpTask->m_pForth->m_pDataStack->bPopAsInt(iPick) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pForth->m_pAuxStack->vPeekValue(iPick,xVariable);
  
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* areverse -- reverse astack values ( ... D C B A n -- A B C D ... )         */
/*----------------------------------------------------------------------------*/
void Step::vAstackReverse(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vAstackReverse"_Stz);
  
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
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(nValues) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK);
  
  for (i8 i = 0; i < nValues; i++) {
    if ( ! mpTask->m_pForth->m_pAuxStack->bPop(apValWrk[i]) )
      vThrow(__func__,__FILE__,__LINE__,FK_ASTACK);
  }
  
  for (i8 i = 0; i < nValues; i++) {
    mpTask->m_pForth->m_pAuxStack->vPushGenVar(apValWrk[i]);
  }
}

/*----------------------------------------------------------------------------*/
/* astk -- dump the aux stack                                                 */
/*----------------------------------------------------------------------------*/
void Step::vAstackStk(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vAstackStk"_Stz);
  
  mpTask->m_pForth->m_pAuxStack->vContentDump(
    (Stz*)mpTask->szJobName(),
    "AuxStack:"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* swap -- swap top two values on aux stack                                   */
/*----------------------------------------------------------------------------*/
void Step::vAstackSwap(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vAstackSwap"_Stz);
  
  GenVar xValA;
  GenVar xValB;
  
  if ( ! mpTask->m_pForth->m_pAuxStack->bPop(xValA,xValB) )
  vThrow(__func__,__FILE__,__LINE__,FK_ASTACK);
  
  mpTask->m_pForth->m_pAuxStack->vPushGenVar(xValA);
  mpTask->m_pForth->m_pAuxStack->vPushGenVar(xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >a -- move value from dstack to astack                                     */
/*----------------------------------------------------------------------------*/
void Step::vAstackToA(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vAstackToA"_Stz);
  
  GenVar xValOpaque;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(xValOpaque) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK);
  
  mpTask->m_pForth->m_pAuxStack->vPushGenVar(xValOpaque);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a> -- move value from astack to dstack                                     */
/*----------------------------------------------------------------------------*/
void Step::vAstackToD(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vAstackToD"_Stz);
  
  GenVar xVal;
  if ( ! mpTask->m_pForth->m_pAuxStack->bPop(xVal) )
    vThrow(__func__,__FILE__,__LINE__,FK_ASTACK);
  
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >auxStack -- pop stack of this task and push onto aux stack of that task   */
/*----------------------------------------------------------------------------*/
void Step::vAstackTransfer(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vAstackTransfer"_Stz);
  
  Object* pPtrTask;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrTask) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK);
  Task* pThatTask = _pTask(pPtrTask);
  
  GenVar xVariable;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(xVariable) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK);
  
  pThatTask->m_okLock.vGrabWrite();
  pThatTask->m_pForth->m_pAuxStack->vPushGenVar(xVariable);
  pThatTask->m_okLock.vDropWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a0 -- place copy of 0th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack0(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vAstack0"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pForth->m_pAuxStack->vPeekValue(0,xVariable);
  
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a1 -- place copy of 1st value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack1(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vAstack1"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pForth->m_pAuxStack->vPeekValue(1,xVariable);
  
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r2 -- place copy of 2nd value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack2(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vAstack2"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pForth->m_pAuxStack->vPeekValue(2,xVariable);
  
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r3 -- place copy of 3rd value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack3(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vAstack3"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pForth->m_pAuxStack->vPeekValue(3,xVariable);
  
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r4 -- place copy of 4th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack4(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vAstack4"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pForth->m_pAuxStack->vPeekValue(4,xVariable);
  
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r5 -- place copy of 0th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack5(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vAstack5"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pForth->m_pAuxStack->vPeekValue(5,xVariable);
  
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r6 -- place copy of 6th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack6(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vAstack6"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pForth->m_pAuxStack->vPeekValue(6,xVariable);
  
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r7 -- place copy of 7th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack7(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vAstack7"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pForth->m_pAuxStack->vPeekValue(7,xVariable);
  
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r8 -- place copy of 8th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack8(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vAstack8"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pForth->m_pAuxStack->vPeekValue(8,xVariable);
  
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r9 -- place copy of 9th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack9(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vAstack9"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pForth->m_pAuxStack->vPeekValue(9,xVariable);
  
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
