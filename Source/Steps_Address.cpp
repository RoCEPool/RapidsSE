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
  ENTRY(); // ENTRY("vAddressEql"_Stz);
  
  GenVar xAddressA;
  if ( ! m_pDataStack->bPopVal(xAddressA) ) THROW_DSTACK();
  
  GenVar xAddressB;
  if ( ! m_pDataStack->bPopVal(xAddressB) ) THROW_DSTACK();

  i8 iA = xAddressA.iInt();
  i8 iB = xAddressB.iInt();
  
  i8 r = iA == iB;
  
  m_pDataStack->vPushInt(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* @ -- fetch value using pointer on stack                                    */
/*----------------------------------------------------------------------------*/
void Step::vAddressFetch(void)
{
  ENTRY(); // ENTRY("vAddressFetch"_Stz);
  
  Object* pObject;
  if ( ! m_pDataStack->bPopPtr(pObject) ) THROW_DSTACK();
  AnyVal* aAnyVal = reinterpret_cast<AnyVal*>(pObject);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  GenVar xValFetched = aAnyVal; 
  
  m_pDataStack->vPushVal(xValFetched);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ! -- store value using pointer on stack                                    */
/*----------------------------------------------------------------------------*/
void Step::vAddressStore(void)
{
  ENTRY(); // ENTRY("vAddressStore"_Stz);
  
  Object* pObject;
  if ( ! m_pDataStack->bPopPtr(pObject) ) THROW_DSTACK();
  AnyVal* aAnyVal = reinterpret_cast<AnyVal*>(pObject);

  GenVar xValToStore;
  if ( ! m_pDataStack->bPopVal(xValToStore) ) THROW_DSTACK();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  aAnyVal->vCopy(xValToStore);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
