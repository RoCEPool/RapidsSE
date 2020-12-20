/*============================================================================*/
/* Steps_Address.cpp - address step group                                     */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Address(void)
{
  vAddStep("eql"_Stz,             &Step::vAddressEqual);
  vAddStep("@"_Stz,               &Step::vAddressFetch);
  vAddStep("!"_Stz,               &Step::vAddressStore);
}

/*----------------------------------------------------------------------------*/
/* eql -- test top two address on data stack for equality                     */
/*----------------------------------------------------------------------------*/
void Step::vAddressEqual(void)
{
  ENTRY();
  
  CellSingle xAddressA;
  m_pDataStack->vPopVal(xAddressA,__func__);
  
  CellSingle xAddressB;
  m_pDataStack->vPopVal(xAddressB,__func__);

  i8 iA = xAddressA.iGetColVal();
  i8 iB = xAddressB.iGetColVal();
  
  i8 r = iA == iB;
  
  m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* @ -- fetch value using pointer on stack                                    */
/*----------------------------------------------------------------------------*/
void Step::vAddressFetch(void)
{
  ENTRY();
  
  TypedCell* aTypedCell = _aTypedCell(m_pDataStack->pPopPtr(__func__));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  m_pDataStack->vPushVal(aTypedCell);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ! -- store value using pointer on stack                                    */
/*----------------------------------------------------------------------------*/
void Step::vAddressStore(void)
{
  ENTRY();
  
  TypedCell* aTypedCell = _aTypedCell(m_pDataStack->pPopPtr(__func__));

  CellSingle xValToStore;
  m_pDataStack->vPopVal(xValToStore,__func__);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  aTypedCell->vCopy(xValToStore);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
