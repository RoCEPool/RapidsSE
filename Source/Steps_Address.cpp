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
  ENTRY("vAddressEql"_Stz);
  
  GenVar xAddressA;
  GenVar xAddressB;
  
  if ( ! m_pDataStack->bPopGenVar(xAddressA,xAddressB) )
    THROW_DSTACK();
  
  i8 iA = xAddressA.iInt();
  i8 iB = xAddressB.iInt();
  
  i8 r = iA == iB;
  
  m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* @ -- fetch value using pointer on stack                                    */
/*----------------------------------------------------------------------------*/
void Step::vAddressFetch(void)
{
  ENTRY("vAddressFetch"_Stz);
  
  Object* pObject;
  if ( ! m_pDataStack->bPopObject(pObject) )
    THROW_DSTACK();
  
  GenVar xValFetched;
  
  eCN eClass = pObject->eClass();
  
  ForthValue* aValue = reinterpret_cast<ForthValue*>(pObject);
  
  switch (eClass) {
    case CN_INTVAL:
      xValFetched = _aIntVal(aValue)->iInt();
      break;
      
    case CN_STRVAL:
      xValFetched = _aStrVal(aValue)->szStz();
      break;
      
    case CN_PTRVAL:
      xValFetched = _aPtrVal(aValue)->pObj();
      break;
      
    case CN_FIPVAL:
      xValFetched = (Object*)_aFipVal(aValue)->aFipPtr();
      break;
  
    default:
      vThrow(FK_FAILED,__func__,__FILE__,__LINE__,"Invalid eClass %lld",eClass);
  }
  
  m_pDataStack->vPushGenVar(xValFetched);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ! -- store value using pointer on stack                                    */
/*----------------------------------------------------------------------------*/
void Step::vAddressStore(void)
{
  ENTRY("vAddressStore"_Stz);
  
  Object* pObject;
  if ( ! m_pDataStack->bPopObject(pObject) )
    THROW_DSTACK();
  
  GenVar xValStored;
  if ( ! m_pDataStack->bPopGenVar(xValStored) )
    THROW_DSTACK();
  
  ForthValue* aValue = reinterpret_cast<ForthValue*>(pObject);
  
  eCN eClass = pObject->eClass();
  
  switch (eClass) {
    case CN_FIPVAL:
      *_aFipVal(aValue) = _aFipVal(xValStored.aNestedValue())->aFipPtr();
      break;
      
    case CN_INTVAL:
      *_aIntVal(aValue) = _aIntVal(xValStored.aNestedValue())->iInt();
      break;
      
    case CN_PTRVAL:
      *_aPtrVal(aValue) = _aPtrVal(xValStored.aNestedValue())->pObj();
      break;
      
   case CN_STRVAL:
      _aStrVal(aValue)->vAssign( _aStrVal(xValStored.aNestedValue())->szStz() );
      break;
      
    default:
      vThrow(FK_FAILED,__func__,__FILE__,__LINE__,"Invalid eClass %lld",eClass);
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
