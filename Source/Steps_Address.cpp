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
  
  if ( ! m_pDataStack->bPopVar(xAddressA,xAddressB) )
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
  
  GenVar xValFetched;
  
  eCN eClass = pObject->eClass();
  
  StkVal* aStkVal = reinterpret_cast<StkVal*>(pObject);
  
  switch (eClass) {
    case CN_INTVAL:
      xValFetched = _aIntVal(aStkVal)->iInt();
      break;
      
    case CN_STRVAL:
      xValFetched = _aStrVal(aStkVal)->szStz();
      break;
      
    case CN_PTRVAL:
      xValFetched = _aPtrVal(aStkVal)->pObj();
      break;
      
    case CN_FIPVAL:
      xValFetched = (Object*)_aFipVal(aStkVal)->aFip();
      break;
  
    default:
      vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
             "Invalid eClass %lld"_Stz,eClass);
  }
  
  m_pDataStack->vPushGenVar(xValFetched);
  
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
  
  GenVar xValStored;
  if ( ! m_pDataStack->bPopVar(xValStored) )
    THROW_DSTACK();
  
  StkVal* aStkVal = reinterpret_cast<StkVal*>(pObject);
  
  eCN eClass = pObject->eClass();
  
  switch (eClass) {
    case CN_FIPVAL:
      *_aFipVal(aStkVal) = _aFipVal(xValStored.aEmbeddedStkVal())->aFip();
      break;
      
    case CN_INTVAL:
      *_aIntVal(aStkVal) = _aIntVal(xValStored.aEmbeddedStkVal())->iInt();
      break;
      
    case CN_PTRVAL:
      *_aPtrVal(aStkVal) = _aPtrVal(xValStored.aEmbeddedStkVal())->pObj();
      break;
      
   case CN_STRVAL:
      _aStrVal(aStkVal)->vAssign( _aStrVal(xValStored.aEmbeddedStkVal())->szStz() );
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
