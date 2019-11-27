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
  GenVar xAddressB;
  
  if ( ! m_pDataStack->bPopVal(xAddressA,xAddressB) )
    THROW_DSTACK();
  
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
  if ( ! m_pDataStack->bPopPtr(pObject) )
    THROW_DSTACK();
  StkVal* aStkVal = reinterpret_cast<StkVal*>(pObject);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  GenVar xValFetched;
  
  eCN eClass = aStkVal->eClass();
  
  switch (eClass) {
    case CN_FIPVAL:
      xValFetched = aStkVal->aFip();
      break;
      
    case CN_INTVAL:
      xValFetched = aStkVal->iInt();
      break;
      
    case CN_PTRVAL:
      xValFetched = aStkVal->pObj();
      break;
      
    case CN_STRVAL:
      xValFetched = aStkVal->szStz();
      break;
      
    default:
      vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
             "Invalid eClass %lld"_Stz,eClass);
  }
  
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
  if ( ! m_pDataStack->bPopPtr(pObject) )
    THROW_DSTACK();
  StkVal* aStkVal = reinterpret_cast<StkVal*>(pObject);

  GenVar xValToStore;
  if ( ! m_pDataStack->bPopVal(xValToStore) )
    THROW_DSTACK();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  eCN eClass = aStkVal->eClass();
  
  switch (eClass) {
    case CN_FIPVAL:
      *_aFipVal(aStkVal) = _aFipVal(xValToStore.aEmbeddedStkVal())->aFip();
      break;
      
    case CN_INTVAL:
      *_aIntVal(aStkVal) = _aIntVal(xValToStore.aEmbeddedStkVal())->iInt();
      break;
      
    case CN_PTRVAL:
      *_aPtrVal(aStkVal) = _aPtrVal(xValToStore.aEmbeddedStkVal())->pObj();
      break;
      
   case CN_STRVAL:
      _aStrVal(aStkVal)->vAssign( _aStrVal(xValToStore.aEmbeddedStkVal())->szStz() );
      break;
      
    default:
      vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
             "Invalid eClass %lld"_Stz,eClass);
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
