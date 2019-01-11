/*============================================================================*/
/* Steps_Address.cpp - address step group                                     */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

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
void Step::vAddressEqual(MP_Task mpTask)
{
  ENTRY("vAddressEql"_Stz);
  
  GenVar xAddressA;
  GenVar xAddressB;
  
  if ( ! mpTask->m_pDataStack->bPop(xAddressA,xAddressB) )
    THROW_INVALID_DSTACK("eql"_Stz);
  
  i8 iA = xAddressA.iNestedAsInt();
  i8 iB = xAddressB.iNestedAsInt();
  
  i8 r = iA == iB;
  
  mpTask->m_pDataStack->vPushI8(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* @ -- fetch value using pointer on stack                                    */
/*----------------------------------------------------------------------------*/
void Step::vAddressFetch(MP_Task mpTask)
{
  ENTRY("vAddressFetch"_Stz);
  
  Object* pObject;
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("@"_Stz);
  
  GenVar xValFetched;
  
  eCN eClass = pObject->eClass();
  
  Value* pValue = reinterpret_cast<Value*>(pObject);
  
  switch (eClass) {
    case CN_INTVAL:
      xValFetched = _pIntVal(pValue)->iInt();
      break;
      
    case CN_STRVAL:
      xValFetched = _pStrVal(pValue);
      break;
      
    case CN_PTRVAL:
      xValFetched = _pPtrVal(pValue)->pObj();
      break;
      
    case CN_IPVAL:
      xValFetched = (Object*)_pIpVal(pValue)->pIpPtr();
      break;
  
    default:
      THROW("Invalid eClass %lld"_Stz,eClass);
  }
  
  mpTask->m_pDataStack->vPushGenVar(xValFetched);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ! -- store value using pointer on stack                                    */
/*----------------------------------------------------------------------------*/
void Step::vAddressStore(MP_Task mpTask)
{
  ENTRY("vAddressStore"_Stz);
  
  Object* pObject;
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("!"_Stz);
  
  GenVar xValStored;
  if ( ! mpTask->m_pDataStack->bPop(xValStored) )
    THROW_INVALID_DSTACK("!"_Stz);
  
  Value* pValue = reinterpret_cast<Value*>(pObject);
  
  eCN eClass = pObject->eClass();
  
  switch (eClass) {
    case CN_INTVAL:
      *_pIntVal(pValue) = _pIntVal(xValStored.pNestedValue())->iInt();
      break;
      
    case CN_STRVAR:
    case CN_STRVAL:
      _pStrVal(pValue)->vAssign( _pStrVal(xValStored.pNestedValue())->aStz() );
      break;
      
    case CN_PTRVAL:
      *_pPtrVal(pValue) = _pPtrVal(xValStored.pNestedValue())->pObj();
      break;
      
    case CN_IPVAL:
      *_pIpVal(pValue) = _pIpVal(xValStored.pNestedValue())->pIpPtr();
      break;
  
    default:
      THROW("Invalid eClass %lld"_Stz,eClass);
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
