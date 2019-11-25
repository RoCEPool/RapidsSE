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
void Step::vAstackClass(void)
{
  ENTRY(); // ENTRY("vAstackClass"_Stz);
  
  i8 nTh;
  if ( ! m_pDataStack->bPopI8(nTh) )
    THROW_DSTACK();
  
  GenVar xVariable;
  
  m_pAuxStack->vPeekGenVar(nTh,xVariable);
  
  eCN eClass = xVariable.eClass();
  
  m_pAuxStack->vPushI8(eClass);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adepth -- put number of values in aux stack onto data stack                */
/*----------------------------------------------------------------------------*/
void Step::vAstackDepth(void)
{
  ENTRY(); // ENTRY("vAstackDepth"_Stz);
  
  i8 iDepth = m_pAuxStack->nValues();
  
  m_pDataStack->vPushI8(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adrop -- discard top value off astack                                      */
/*----------------------------------------------------------------------------*/
void Step::vAstackDrop(void)
{
  ENTRY(); // ENTRY("vAstackDrop"_Stz);
  
  // should just discard on stack
  
  GenVar xValA;
  if ( ! m_pAuxStack->bPopGenVar(xValA) )
    THROW_ASTACK();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adup -- dup top of aux stack                                               */
/*----------------------------------------------------------------------------*/
void Step::vAstackDup(void)
{
  ENTRY(); // ENTRY("vAstackDup"_Stz);
  
  // could just copy from stack back into stack
  
  GenVar xValA;
  if ( ! m_pAuxStack->bPopGenVar(xValA) )
  THROW_DSTACK();
  
  m_pAuxStack->vPushGenVar(xValA);
  m_pAuxStack->vPushGenVar(xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a@ -- place copy of top value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstackFetch(void)
{
  ENTRY(); // ENTRY("vAstackFetch"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  m_pAuxStack->vPeekGenVar(0,xVariable);
  
  m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nip -- drop the 2nd value from the aux stack ( B A -- A )                  */
/*----------------------------------------------------------------------------*/
void Step::vAstackNip(void)
{
  ENTRY(); // ENTRY("vAstackNip"_Stz);
  
  GenVar xValA;
  GenVar xValB;
  
  if ( ! m_pAuxStack->bPopGenVar(xValA,xValB) )
  THROW_ASTACK();
  
  m_pAuxStack->vPushGenVar(xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* apick -- place copy of nth value in aux stack onto data stack              */
/*----------------------------------------------------------------------------*/
void Step::vAstackPick(void)
{
  ENTRY(); // ENTRY("vAstackPick"_Stz);
  
  i8 iPick;
  if ( ! m_pDataStack->bPopAsInt(iPick) )
    THROW_DSTACK();
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  m_pAuxStack->vPeekGenVar(iPick,xVariable);
  
  m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* areverse -- reverse astack values ( ... D C B A n -- A B C D ... )         */
/*----------------------------------------------------------------------------*/
void Step::vAstackReverse(void)
{
  ENTRY(); // ENTRY("vAstackReverse"_Stz);
  
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
  if ( ! m_pDataStack->bPopI8(nValues) )
    THROW_DSTACK();
  
  for (i8 i = 0; i < nValues; i++) {
    if ( ! m_pAuxStack->bPopGenVar(paVal[i]) )
      THROW_ASTACK();
  }
  
  for (i8 i = 0; i < nValues; i++) {
    m_pAuxStack->vPushGenVar(paVal[i]);
  }
}

/*----------------------------------------------------------------------------*/
/* astk -- dump the aux stack                                                 */
/*----------------------------------------------------------------------------*/
void Step::vAstackStk(void)
{
  ENTRY(); // ENTRY("vAstackStk"_Stz);
  
  m_pAuxStack->vContentDump(&m_oDicStack,"AuxStack:"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* swap -- swap top two values on aux stack                                   */
/*----------------------------------------------------------------------------*/
void Step::vAstackSwap(void)
{
  ENTRY(); // ENTRY("vAstackSwap"_Stz);
  
  GenVar xValA;
  GenVar xValB;
  
  if ( ! m_pAuxStack->bPopGenVar(xValA,xValB) )
  THROW_ASTACK();
  
  m_pAuxStack->vPushGenVar(xValA);
  m_pAuxStack->vPushGenVar(xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >a -- move value from dstack to astack                                     */
/*----------------------------------------------------------------------------*/
void Step::vAstackToA(void)
{
  ENTRY(); // ENTRY("vAstackToA"_Stz);
  
  GenVar xValOpaque;
  if ( ! m_pDataStack->bPopGenVar(xValOpaque) )
    THROW_DSTACK();
  
  m_pAuxStack->vPushGenVar(xValOpaque);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a> -- move value from astack to dstack                                     */
/*----------------------------------------------------------------------------*/
void Step::vAstackToD(void)
{
  ENTRY(); // ENTRY("vAstackToD"_Stz);
  
  GenVar xVal;
  if ( ! m_pAuxStack->bPopGenVar(xVal) )
    THROW_ASTACK();
  
  m_pDataStack->vPushGenVar(xVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >auxStack -- pop stack of this task and push onto aux stack of that task   */
/*----------------------------------------------------------------------------*/
void Step::vAstackTransfer(void)
{
  ENTRY(); // ENTRY("vAstackTransfer"_Stz);
  
  Object* pPtrTask;
  if ( ! m_pDataStack->bPopObject(pPtrTask) )
    THROW_DSTACK();
  Task* pThatTask = _pTask(pPtrTask);
  
  GenVar xVariable;
  if ( ! m_pDataStack->bPopGenVar(xVariable) )
    THROW_DSTACK();
  
  pThatTask->m_okLock.vGrabWrite();
  pThatTask->m_pForth->m_pAuxStack->vPushGenVar(xVariable);
  pThatTask->m_okLock.vDropWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a0 -- place copy of 0th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack0(void)
{
  ENTRY(); // ENTRY("vAstack0"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  m_pAuxStack->vPeekGenVar(0,xVariable);
  
  m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a1 -- place copy of 1st value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack1(void)
{
  ENTRY(); // ENTRY("vAstack1"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  m_pAuxStack->vPeekGenVar(1,xVariable);
  
  m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r2 -- place copy of 2nd value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack2(void)
{
  ENTRY(); // ENTRY("vAstack2"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  m_pAuxStack->vPeekGenVar(2,xVariable);
  
  m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r3 -- place copy of 3rd value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack3(void)
{
  ENTRY(); // ENTRY("vAstack3"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  m_pAuxStack->vPeekGenVar(3,xVariable);
  
  m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r4 -- place copy of 4th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack4(void)
{
  ENTRY(); // ENTRY("vAstack4"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  m_pAuxStack->vPeekGenVar(4,xVariable);
  
  m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r5 -- place copy of 0th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack5(void)
{
  ENTRY(); // ENTRY("vAstack5"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  m_pAuxStack->vPeekGenVar(5,xVariable);
  
  m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r6 -- place copy of 6th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack6(void)
{
  ENTRY(); // ENTRY("vAstack6"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  m_pAuxStack->vPeekGenVar(6,xVariable);
  
  m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r7 -- place copy of 7th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack7(void)
{
  ENTRY(); // ENTRY("vAstack7"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  m_pAuxStack->vPeekGenVar(7,xVariable);
  
  m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r8 -- place copy of 8th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack8(void)
{
  ENTRY(); // ENTRY("vAstack8"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  m_pAuxStack->vPeekGenVar(8,xVariable);
  
  m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r9 -- place copy of 9th value in aux stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vAstack9(void)
{
  ENTRY(); // ENTRY("vAstack9"_Stz);
  
  // could do this directly on stack
  
  GenVar xVariable;
  
  m_pAuxStack->vPeekGenVar(9,xVariable);
  
  m_pDataStack->vPushGenVar(xVariable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
