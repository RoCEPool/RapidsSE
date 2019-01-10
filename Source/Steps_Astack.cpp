/*============================================================================*/
/* Steps_Astack.cpp - aux stack step group                                    */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

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
  vAddStep("a>"_Stz,              &Step::vAstackToP);
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
void Step::vAstackClass(MP_Task mpTask)
{
  ENTRY("vAstackClass"_Stz);
  
  i8 nTh;
  if ( ! mpTask->m_pDataStack->bPop(nTh) )
    THROW_INVALID_DSTACK("astack.class"_Stz);
  
  GenVar xVariable;
  
  mpTask->m_pAuxStack->vPeekValWrk(nTh,xVariable);
  
  eCN eClass = xVariable.eClass();
  
  mpTask->m_pAuxStack->vPushI8(eClass);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adepth -- put number of values in aux stack onto data stack                */
/*----------------------------------------------------------------------------*/
void Step::vAstackDepth(MP_Task mpTask)
{
  ENTRY("vAstackDepth"_Stz);
  
  i8 iDepth = mpTask->m_pAuxStack->iValues();
  
  mpTask->m_pDataStack->vPushI8(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adrop -- discard top value off astack                                      */
/*----------------------------------------------------------------------------*/
void Step::vAstackDrop(MP_Task mpTask)
{
  ENTRY("vAstackDrop"_Stz);
  
  // should just discard on stack
  
  GenVar xValA;
  if ( ! mpTask->m_pAuxStack->bPop(xValA) )
    THROW_INVALID_ASTACK("adrop"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adup -- dup top of aux stack                                               */
/*----------------------------------------------------------------------------*/
void Step::vAstackDup(MP_Task mpTask)
{
  ENTRY("vAstackDup"_Stz);
  
  // could just copy from stack back into stack
  
  GenVar xValA;
  if ( ! mpTask->m_pAuxStack->bPop(xValA) )
  THROW_INVALID_DSTACK("adup"_Stz);
  
  mpTask->m_pAuxStack->vPushGenVar(xValA);
  mpTask->m_pAuxStack->vPushGenVar(xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a@ -- place copy of top value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstackFetch(MP_Task mpTask)
{
  ENTRY("vAstackFetch"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pAuxStack->vPeekValWrk(0,xVariable);
  
  mpTask->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nip -- drop the 2nd value from the aux stack ( B A -- A )                  */
/*----------------------------------------------------------------------------*/
void Step::vAstackNip(MP_Task mpTask)
{
  ENTRY("vAstackNip"_Stz);
  
  GenVar xValA;
  GenVar xValB;
  
  if ( ! mpTask->m_pAuxStack->bPop(xValA,xValB) )
  THROW_INVALID_ASTACK("anip"_Stz);
  
  mpTask->m_pAuxStack->vPushGenVar(xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* apick -- place copy of nth value in aux stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vAstackPick(MP_Task mpTask)
{
  ENTRY("vAstackPick"_Stz);
  
  i8 iPick;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iPick) )
    THROW_INVALID_DSTACK("apick"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pAuxStack->vPeekValWrk(iPick,xVariable);
  
  mpTask->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* areverse -- reverse astack values ( ... D C B A n -- A B C D ... )         */
/*----------------------------------------------------------------------------*/
void Step::vAstackReverse(MP_Task mpTask)
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
  if ( ! mpTask->m_pDataStack->bPop(nValues) )
    THROW_INVALID_DSTACK("areverse"_Stz);
  
  for (i8 i = 0; i < nValues; i++) {
    if ( ! mpTask->m_pAuxStack->bPop(apValWrk[i]) )
      THROW_INVALID_ASTACK("areverse"_Stz);
  }
  
  for (i8 i = 0; i < nValues; i++) {
    mpTask->m_pAuxStack->vPushGenVar(apValWrk[i]);
  }
}

/*----------------------------------------------------------------------------*/
/* astk -- dump the aux stack                                                 */
/*----------------------------------------------------------------------------*/
void Step::vAstackStk(MP_Task mpTask)
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
void Step::vAstackSwap(MP_Task mpTask)
{
  ENTRY("vAstackSwap"_Stz);
  
  GenVar xValA;
  GenVar xValB;
  
  if ( ! mpTask->m_pAuxStack->bPop(xValA,xValB) )
  THROW_INVALID_ASTACK("aswap"_Stz);
  
  mpTask->m_pAuxStack->vPushGenVar(xValA);
  mpTask->m_pAuxStack->vPushGenVar(xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >a -- move value from dstack to astack                                     */
/*----------------------------------------------------------------------------*/
void Step::vAstackToA(MP_Task mpTask)
{
  ENTRY("vAstackToA"_Stz);
  
  GenVar xValOpaque;
  if ( ! mpTask->m_pDataStack->bPop(xValOpaque) )
    THROW_INVALID_DSTACK(">a"_Stz);
  
  mpTask->m_pAuxStack->vPushGenVar(xValOpaque);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a> -- move value from astack to dstack                                     */
/*----------------------------------------------------------------------------*/
void Step::vAstackToP(MP_Task mpTask)
{
  ENTRY("vAstackToP"_Stz);
  
  GenVar xVal;
  if ( ! mpTask->m_pAuxStack->bPop(xVal) )
    THROW_INVALID_ASTACK("a>"_Stz);
  
  mpTask->m_pDataStack->vPushGenVar(xVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >auxStack -- pop stack of this task and push onto aux stack of that task   */
/*----------------------------------------------------------------------------*/
void Step::vAstackTransfer(MP_Task mpTask)
{
  ENTRY("vAstackTransfer"_Stz);
  
  Object* pPtrTask;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK(">auxStack"_Stz);
  Task* pThatTask = _pTask(pPtrTask);
  
  GenVar xVariable;
  if ( ! mpTask->m_pDataStack->bPop(xVariable) )
    THROW_INVALID_DSTACK(">auxStack"_Stz);
  
  pThatTask->m_oLock.vGrabWrite();
  pThatTask->m_pAuxStack->vPushGenVar(xVariable);
  pThatTask->m_oLock.vDropWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a0 -- place copy of 0th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack0(MP_Task mpTask)
{
  ENTRY("vAstack0"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pAuxStack->vPeekValWrk(0,xVariable);
  
  mpTask->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a1 -- place copy of 1st value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack1(MP_Task mpTask)
{
  ENTRY("vAstack1"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pAuxStack->vPeekValWrk(1,xVariable);
  
  mpTask->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r2 -- place copy of 2nd value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack2(MP_Task mpTask)
{
  ENTRY("vAstack2"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pAuxStack->vPeekValWrk(2,xVariable);
  
  mpTask->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r3 -- place copy of 3rd value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack3(MP_Task mpTask)
{
  ENTRY("vAstack3"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pAuxStack->vPeekValWrk(3,xVariable);
  
  mpTask->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r4 -- place copy of 4th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack4(MP_Task mpTask)
{
  ENTRY("vAstack4"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pAuxStack->vPeekValWrk(4,xVariable);
  
  mpTask->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r5 -- place copy of 0th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack5(MP_Task mpTask)
{
  ENTRY("vAstack5"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pAuxStack->vPeekValWrk(5,xVariable);
  
  mpTask->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r6 -- place copy of 6th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack6(MP_Task mpTask)
{
  ENTRY("vAstack6"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pAuxStack->vPeekValWrk(6,xVariable);
  
  mpTask->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r7 -- place copy of 7th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack7(MP_Task mpTask)
{
  ENTRY("vAstack7"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pAuxStack->vPeekValWrk(7,xVariable);
  
  mpTask->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r8 -- place copy of 8th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack8(MP_Task mpTask)
{
  ENTRY("vAstack8"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pAuxStack->vPeekValWrk(8,xVariable);
  
  mpTask->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r9 -- place copy of 9th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack9(MP_Task mpTask)
{
  ENTRY("vAstack9"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  mpTask->m_pAuxStack->vPeekValWrk(9,xVariable);
  
  mpTask->m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
