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
void Step::vAddressEqual(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vAddressEql"_Stz);
  
  GenVar xAddressA;
  GenVar xAddressB;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(xAddressA,xAddressB) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  i8 iA = xAddressA.iInt();
  i8 iB = xAddressB.iInt();
  
  i8 r = iA == iB;
  
  mpTask->m_pForth->m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* @ -- fetch value using pointer on stack                                    */
/*----------------------------------------------------------------------------*/
void Step::vAddressFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vAddressFetch"_Stz);
  
  Object* pObject;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pObject) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
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
      vThrow(__func__,__FILE__,__LINE__,"Invalid eClass %lld"_Stz,eClass);
  }
  
  mpTask->m_pForth->m_pDataStack->vPushGenVar(xValFetched);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ! -- store value using pointer on stack                                    */
/*----------------------------------------------------------------------------*/
void Step::vAddressStore(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vAddressStore"_Stz);
  
  Object* pObject;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pObject) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  GenVar xValStored;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(xValStored) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
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
      vThrow(__func__,__FILE__,__LINE__,"Invalid eClass %lld"_Stz,eClass);
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
