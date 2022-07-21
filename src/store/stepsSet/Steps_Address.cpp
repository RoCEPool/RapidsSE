/*============================================================================*/
/* Steps_Address.cpp - address step group                                     */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Address(void)
{
  vAddStep("equal"_Stz,             &Step::vAddressEqual);
  vAddStep("@"_Stz,               &Step::vAddressFetch);
  vAddStep("!"_Stz,               &Step::vAddressStore);
}

/*----------------------------------------------------------------------------*/
/* equal -- test top two address on data stack for equality                     */
/*----------------------------------------------------------------------------*/
void Step::vAddressEqual(void)
{
  ENTRY();
  
  SenCell xAddressA;
  m_pDataStack->vPopSenCell(xAddressA,__func__);
  
  SenCell xAddressB;
  m_pDataStack->vPopSenCell(xAddressB,__func__);

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
  
  TypeValueArea* aTypeValueArea = _aTypeValueArea(m_pDataStack->pPopPtr(__func__));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  m_pDataStack->vPushTypeValueArea(aTypeValueArea);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ! -- store value using pointer on stack                                    */
/*----------------------------------------------------------------------------*/
void Step::vAddressStore(void)
{
  ENTRY();
  
  TypeValueArea* aTypedCell = _aTypeValueArea(m_pDataStack->pPopPtr(__func__));

  SenCell xValToStore;
  m_pDataStack->vPopSenCell(xValToStore,__func__);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  aTypedCell->vCopy(xValToStore);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
