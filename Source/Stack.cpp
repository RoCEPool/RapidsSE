/*============================================================================*/
/* Stack.cpp -- Push/Pop Stack of Values                                      */
/*============================================================================*/

#include "SenVM.h"


// Note: to optimise stack operations look at directly using pDataStack->m_aTop
//       refer to Step::vFlowJumpFalse
 
 

/*----------------------------------------------------------------------------*/
/*-- Lifecycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Stack* Stack::_pStack(void* aAddr)
{
  Stack* pStack = reinterpret_cast<Stack*>(aAddr);
  
  RASSERT_TYPE(pStack,CN_STACK);
  
  return pStack;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Stack::operator new(size_t)
{
  void* pStack = HeapMem::pAllocate(CN_STACK);
  
  return pStack;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Stack::operator delete(void* pBufferAddress)
{
  Stack* pStack = _pStack(pBufferAddress);
  
  HeapMem::vFree(pStack);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Stack::Stack(eKK eKind)
: Object(CN_STACK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  m_eKind   = eKind;
  
  m_aTop    = aStackBase();
  m_nValues = 0;
  
  m_aRetryTop    = aStackBase();
  m_nRetryValues = 0;
  
#ifdef WITH_CHECK_INITIALISED
  m_xWsp.vFill(0x00,CN_STACK);
#endif
  
  _checkInitialised_(CN_STACK);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  _hazard_();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Stack::~Stack(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump value stack                                                           */
/*----------------------------------------------------------------------------*/
void Stack::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Stack* pStack = _pStack(pObject);
  
  vTrace("values %lld top %p\n"_Stz,
         pStack->m_nValues,
         pStack->m_aTop);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump contents of the stack                                                 */
/*----------------------------------------------------------------------------*/
void Stack::vContentDump(DicStack* pDicStack, Stz* aTitle)
{
  vTrace("%s:\n"_Stz,aTitle);
  
  GenVar xVariable;
  
  i8 nValues = this->nValues();
  for (i8 i_Val = 0; i_Val < nValues; i_Val++) {
    
    vPeekGenVar(i_Val,xVariable);
    
    StkVal* aEmbeddedStkVal = xVariable.aEmbeddedStkVal();
    eCN eClass = aEmbeddedStkVal->eClass();
    
    switch(eClass) {
        
      case CN_FIPVAL: {
        FipVal* aFipVal = _aFipVal(aEmbeddedStkVal);
        Cell* aFip = aFipVal->aFip();
        Word* pWord = pDicStack->pLocateWord(aFip);


        vTrace("[%2lld] [%s] %p word %s \n"_Stz,
               i_Val,aCN[eClass],aFip,
               pWord ? pWord->szWordNm() : "<none>"_Stz);
        break;
      }
        
      case CN_INTVAL: {
        IntVal *aIntVal = _aIntVal(aEmbeddedStkVal); //xVariable.iInt();
        i8 iInt = aIntVal->iInt();
        vTrace("[%2lld] [%s] %lld\n"_Stz,
               i_Val,aCN[eClass],iInt);
        break;
      }
        
      case CN_PTRVAL: {
        PtrVal* aPtrVal = _aPtrVal(aEmbeddedStkVal);
        Object* pObject = aPtrVal->pObj();
        eCN ePtdClass   = pObject->eClass();
        vTrace("[%2lld] [%s] %p (%s)\n"_Stz,
               i_Val,aCN[eClass],pObject,aCN[ePtdClass]);
        break;
      }
        
      case CN_STRVAL: {
        StrVal* aStrVal = _aStrVal(aEmbeddedStkVal);
        vTrace("[%2lld] [%s] %s\n"_Stz,
               i_Val,aCN[eClass],aStrVal->azChar());
        break;
      }
        
      default: {
        vTrace("[%2lld] [%s]\n"_Stz,
               i_Val,aCN[eClass]);
        break;
      }
        
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* guard stack has at least nValues on it                                     */
/*----------------------------------------------------------------------------*/
bool Stack::bGuard(i8 nValues)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if (m_nValues < nValues)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* guard top value on value stack has given class                             */
/*----------------------------------------------------------------------------*/
bool Stack::bGuard(eCN eClass0)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if (nValues() < 1)
    return false;
  
  StkVal* aStkVal0 = aPeekStkVal(0);
  if (eClass0 != aStkVal0->eClass())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* guard top 2 values on value stack have given classes                       */
/*----------------------------------------------------------------------------*/
bool Stack::bGuard(eCN eClass0, eCN eClass1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if (nValues() < 2)
    return false;
  
  StkVal* aStkVal0 = aPeekStkVal(0);
  if (eClass0 != aStkVal0->eClass())
    return false;
  
  StkVal* aStkVal1 = aPeekStkVal(1);
  if (eClass1 != aStkVal1->eClass())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* guard top 3 values on value stack have given classes                       */
/*----------------------------------------------------------------------------*/
bool Stack::bGuard(eCN eClass0, eCN eClass1, eCN eClass2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if (nValues() < 3)
    return false;
  
  StkVal* aStkVal0 = aPeekStkVal(0);
  if (eClass0 != aStkVal0->eClass())
    return false;
  
  StkVal* aStkVal1 = aPeekStkVal(1);
  if (eClass1 != aStkVal1->eClass())
    return false;
  
  StkVal* aSrkVal2 = aPeekStkVal(2);
  if (eClass2 != aSrkVal2->eClass())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* peek at given value on value stack                                         */
/*----------------------------------------------------------------------------*/
StkVal* Stack::aPeekStkVal(i8 i_Val)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  StkVal* aStkVal = aTopStkVal();
  while (i_Val--) {
    char* aAddress = reinterpret_cast<char*>(aStkVal);
    i8 iSize = aStkVal->nStackBytes();
    aAddress += iSize;
    aStkVal = reinterpret_cast<StkVal*>(aAddress);
  }
  
  return aStkVal;
}

/*----------------------------------------------------------------------------*/
/* peek at given value on value stack                                         */
/*----------------------------------------------------------------------------*/
void Stack::vPeekGenVar(i8 i_Val, GenVar& xVariable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  StkVal* aPeekedValue = aPeekStkVal(i_Val);
  
  i8 iSize = aPeekedValue->nStackBytes();
  
  //vTrace("Peek: %lld %p %lld\n"_Stz,iNthValue,pPeekedValue,iSize);
  
  MemLib::vCopy(xVariable.aEmbeddedStkVal(),aPeekedValue,iSize,CN_STACK);
}

/*----------------------------------------------------------------------------*/
/* pop eClassNo from value stack                                              */
/*----------------------------------------------------------------------------*/
bool Stack::bPopCN(eCN& eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_INTVAL) )
    return false;
  
  IntVal xIntVal;
  vPopVal(&xIntVal);
  
  eClassNo = static_cast<eCN>(xIntVal.iInt());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop fip from value stack                                                   */
/*----------------------------------------------------------------------------*/
bool Stack::bPopFip(Cell*& aFip)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_FIPVAL) )
    return false;
  
  FipVal xFipVal;
  
  vPopVal(&xFipVal);
  
  aFip = xFipVal.aFip();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop value from value stack into given variable                             */
/*----------------------------------------------------------------------------*/
bool Stack::bPopGenVar(GenVar* pVariable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(1) )
    return false;
  
  vPopVal(pVariable->aEmbeddedStkVal());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop value from value stack into given variable                             */
/*----------------------------------------------------------------------------*/
bool Stack::bPopGenVar(GenVar& xVar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(1) )
    return false;
  
  vPopVal(xVar.aEmbeddedStkVal());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop values from value stack into given variables                           */
/*----------------------------------------------------------------------------*/
bool Stack::bPopGenVar(GenVar& xVar1, GenVar& xVar2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(2) )
    return false;
  
  vPopVal(xVar1.aEmbeddedStkVal());
  
  vPopVal(xVar2.aEmbeddedStkVal());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop values from value stack into given variables                           */
/*----------------------------------------------------------------------------*/
bool Stack::bPopGenVar(GenVar& xVar1, GenVar& xVar2, GenVar& xVar3)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(3) )
    return false;
  
  vPopVal(xVar1.aEmbeddedStkVal());
  
  vPopVal(xVar2.aEmbeddedStkVal());
  
  vPopVal(xVar3.aEmbeddedStkVal());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop i8 from value stack                                                    */
/*----------------------------------------------------------------------------*/
bool Stack::bPopInt(i8& iArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_INTVAL) )
    return false;
  
//  IntVal xIntVal;
//
//  vPopVal(&xIntVal);
//
//  iArg = xIntVal.iInt();

  iArg = iPopInt();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop i8,i8 from value stack                                                 */
/*----------------------------------------------------------------------------*/
bool Stack::bPopInt(i8& iArg1, i8& iArg2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_INTVAL,CN_INTVAL) )
    return false;
  
  IntVal xIntVal1;
  vPopVal(&xIntVal1);
  
  iArg1 = xIntVal1.iInt();
  
  IntVal xIntVal2;
  vPopVal(&xIntVal2);
  
  iArg2 = xIntVal2.iInt();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop i8,pObject from value stack                                            */
/*----------------------------------------------------------------------------*/
bool Stack::bPopInt(i8& iArg, Object*& pObject)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_INTVAL,CN_PTRVAL) )
    return false;
  
  IntVal xIntVal;
  vPopVal(&xIntVal);
  
  iArg = xIntVal.iInt();
  
  PtrVal xPtrVal;
  vPopVal(&xPtrVal);
  
  pObject = xPtrVal.pObj();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop i8,val from value stack                                                */
/*----------------------------------------------------------------------------*/
bool Stack::bPopInt(i8& iArg, GenVar& xGenVar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_INTVAL,CN_STRVAL) )
    return false;
  
  IntVal xIntVal;
  vPopVal(&xIntVal);
  
  iArg = xIntVal.iInt();
  
  vPopVal(xGenVar.aEmbeddedStkVal());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop i8,s from value stack                                                  */
/*----------------------------------------------------------------------------*/
bool Stack::bPopInt(i8& iArg, s48K& sArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_INTVAL,CN_STRVAL) )
    return false;
  
  IntVal xIntVal;
  vPopVal(&xIntVal);
  
  iArg = xIntVal.iInt();
  
  GenVar xGenVar;
  vPopVal(xGenVar.aEmbeddedStkVal());
  
  sArg = _aStrVal(&xGenVar)->szStz();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop i8,i8,val from value stack                                             */
/*----------------------------------------------------------------------------*/
bool Stack::bPopInt(i8& iArg1, i8& iArg2, GenVar& xGenVar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_INTVAL,CN_INTVAL,CN_STRVAL) )
    return false;
  
  IntVal xIntVal1;
  vPopVal(&xIntVal1);
  
  iArg1 = xIntVal1.iInt();
  
  IntVal xIntVal2;
  vPopVal(&xIntVal2);
  
  iArg2 = xIntVal2.iInt();
  
  vPopVal(xGenVar.aEmbeddedStkVal());

  return true;
}

/*----------------------------------------------------------------------------*/
/* pop Object* from value stack                                               */
/*----------------------------------------------------------------------------*/
bool Stack::bPopPtr(Object*& pObject)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_PTRVAL) && ! bTopIsZero() )
    return false;
  
  PtrVal xPtrVal;
  vPopVal(&xPtrVal);
  
  pObject = xPtrVal.pObj();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop Object*, i8 from value stack                                           */
/*----------------------------------------------------------------------------*/
bool Stack::bPopPtr(Object*& pObject, i8& iArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_PTRVAL,CN_INTVAL) )
    return false;
  
  PtrVal xPtrVal;
  vPopVal(&xPtrVal);
  
  pObject = xPtrVal.pObj();
  
  IntVal xIntVal;
  vPopVal(&xIntVal);
  
  iArg = xIntVal.iInt();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop Object*, Object* from value stack                                      */
/*----------------------------------------------------------------------------*/
bool Stack::bPopPtr(Object*& pObject1, Object*& pObject2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_PTRVAL,CN_PTRVAL) )
    return false;
  
  PtrVal xPtrVal1;
  vPopVal(&xPtrVal1);
  
  pObject1 = xPtrVal1.pObj();
  
  PtrVal xPtrVal2;
  vPopVal(&xPtrVal2);
  
  pObject2 = xPtrVal2.pObj();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop Object* from value stack                                               */
/*----------------------------------------------------------------------------*/
bool Stack::bPopPtr(Object*& pObject, GenVar& xGenVar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_PTRVAL) )
    return false;
  
  PtrVal xPtrVal1;
  vPopVal(&xPtrVal1);
  
  pObject = xPtrVal1.pObj();
  
  return bPopGenVar(xGenVar);
}

/*----------------------------------------------------------------------------*/
/* pop register from value stack                                              */
/*----------------------------------------------------------------------------*/
bool Stack::bPopLocal(Local*& pLocal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(1) )
    return false;
  
  vPopLocal(pLocal);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop s from value stack                                                     */
/*----------------------------------------------------------------------------*/
bool Stack::bPopS32(s32& sArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_STRVAL) )
    return false;
  
  GenVar xGenVar;
  vPopVal(xGenVar.aEmbeddedStkVal());
  
  sArg = _aStrVal(xGenVar.aEmbeddedStkVal())->szStz();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop s from value stack                                                     */
/*----------------------------------------------------------------------------*/
bool Stack::bPopS256(s256& sArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_STRVAL) )
    return false;
  
  GenVar xGenVar;
  vPopVal(xGenVar.aEmbeddedStkVal());
  
  sArg = _aStrVal(xGenVar.aEmbeddedStkVal())->szStz();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop s from value stack                                                     */
/*----------------------------------------------------------------------------*/
bool Stack::bPopS4K(s4K& sArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_STRVAL) )
    return false;
  
  GenVar xGenVar;
  vPopVal(xGenVar.aEmbeddedStkVal());
  
  sArg = _aStrVal(xGenVar.aEmbeddedStkVal())->szStz();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop s from value stack                                                     */
/*----------------------------------------------------------------------------*/
bool Stack::bPopS48K(s48K& sArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_STRVAL) )
    return false;
  
  GenVar xGenVar;
  vPopVal(xGenVar.aEmbeddedStkVal());
  
  sArg = _aStrVal(xGenVar.aEmbeddedStkVal())->szStz();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop s,i8 from value stack                                                  */
/*----------------------------------------------------------------------------*/
bool Stack::bPopS48K(s48K& sArg, i8& iArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_STRVAL,CN_INTVAL) )
    return false;
  
  GenVar xGenVar;
  vPopVal(xGenVar.aEmbeddedStkVal());
  
  sArg = _aStrVal(xGenVar.aEmbeddedStkVal())->szStz();
  
  IntVal xIntVal;
  vPopVal(&xIntVal);
  
  iArg = xIntVal.iInt();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop s,i8,i8 from value stack                                               */
/*----------------------------------------------------------------------------*/
bool Stack::bPopS48K(s48K& sArg, i8& iArg1, i8& iArg2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_STRVAL,CN_INTVAL,CN_INTVAL) )
    return false;
  
  GenVar xGenVar;
  vPopVal(xGenVar.aEmbeddedStkVal());
  sArg = _aStrVal(xGenVar.aEmbeddedStkVal())->szStz();
  
  IntVal xIntVal1;
  vPopVal(&xIntVal1);
  iArg1 = xIntVal1.iInt();
  

  IntVal xIntVal2;
  vPopVal(&xIntVal2);
  iArg2 = xIntVal2.iInt();
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop s,s from value stack                                                   */
/*----------------------------------------------------------------------------*/
bool Stack::bPopS48K(s48K& sArg1, s48K& sArg2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_STRVAL,CN_STRVAL) )
    return false;
  
  GenVar xGenVar1;
  vPopVal(xGenVar1.aEmbeddedStkVal());
  
  sArg1 = _aStrVal(xGenVar1.aEmbeddedStkVal())->szStz();
  
  GenVar xGenVar2;
  vPopVal(xGenVar2.aEmbeddedStkVal());
  
  sArg2 = _aStrVal(xGenVar2.aEmbeddedStkVal())->szStz();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop string from stack                                                      */
/*----------------------------------------------------------------------------*/
bool Stack::bPopStr(String* pString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_STRVAL) )
    return false;
  
  _hazard_();
  
  StrVal* aStrVal = _aStrVal(aTopStkVal());
  
  pString->vCopy(aStrVal->szStz());
  
  vDiscard();
  
  _hazard_();

  return true;
}

/*----------------------------------------------------------------------------*/
/* pop value from value stack  as integer                                     */
/*----------------------------------------------------------------------------*/
bool Stack::bPopAsInt(i8& iArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(1) )
    return false;
  
  GenVar xGenVar;
  vPopVal(xGenVar.aEmbeddedStkVal());
  
  iArg = xGenVar.iInt();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop value from value stack  as integer                                     */
/*----------------------------------------------------------------------------*/
bool Stack::bPopAsInt_Quick(i8& iArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(1) )
    return false;
  
  // get pointer to value on the top of the stack
  StkVal* aTopValue = aTopStkVal();

  // discard the value
  vDiscard();

  eCN eClassNo = aTopValue->eClass();
  
  switch (eClassNo) {
      
    case CN_INTVAL:
      iArg = _aIntVal(aTopValue)->iInt();
      return true;
      
    case CN_PTRVAL:
      iArg = _aPtrVal(aTopValue)->pObj() ? 1 : 0;
      return true;
      
    default:
      NEVER_REACH;
  }

  NEVER_REACH;
  return false;
}

/*----------------------------------------------------------------------------*/
/* poke given value into value stack                                          */
/*----------------------------------------------------------------------------*/
void Stack::vPokeGenVar(i8 i_Val, GenVar& xGenVar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  i8 iSize = xGenVar.aEmbeddedStkVal()->nStackBytes();
  
  StkVal* aPokeAddress = aPeekStkVal(i_Val);
  
  MemLib::vCopy(aPokeAddress,xGenVar.aEmbeddedStkVal(),iSize,CN_STACK);
}

/*----------------------------------------------------------------------------*/
/* push bool onto value stack                                                 */
/*----------------------------------------------------------------------------*/
void Stack::vPushBool(bool bArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  IntVal xIntVal;
  xIntVal = bArg;
  
  vPushStkVal(xIntVal);
}

/*----------------------------------------------------------------------------*/
/* push Cell* onto value stack                                                */
/*----------------------------------------------------------------------------*/
void Stack::vPushFip(Cell* aFip)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
//  FipVal xFipVal;
//
//  xFipVal = aFip;
//
//  vPushValue(xFipVal);
  
  FipVal* aFipVal = aAllotFipVal();
  
  * aFipVal = aFip;
}

/*----------------------------------------------------------------------------*/
/* push value onto value stack                                                */
/*----------------------------------------------------------------------------*/
void Stack::vPushGenVar(GenVar* pGenVar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  vPushVal(pGenVar->aEmbeddedStkVal());
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* push value onto value stack                                                */
/*----------------------------------------------------------------------------*/
void Stack::vPushGenVar(GenVar& xGenVar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  vPushVal(xGenVar.aEmbeddedStkVal());
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* push i8 onto value stack                                                   */
/*----------------------------------------------------------------------------*/
void Stack::vPushInt(i8 iArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  IntVal xIntVal1;
  xIntVal1 = iArg;
  
  vPushStkVal(xIntVal1);
}

/*----------------------------------------------------------------------------*/
/* push pointer to proper object onto value stack                             */
/*----------------------------------------------------------------------------*/
void Stack::vPushPtr(Object* pObject)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  PtrVal xPtrObject;
  
  xPtrObject = pObject;
  
  vPushStkVal(xPtrObject);
}

/*----------------------------------------------------------------------------*/
/* push pointer to register onto value stack                                  */
/*----------------------------------------------------------------------------*/
void Stack::vPushLocal(Local* pLocal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  StkVal* aStkVal = pLocal->aLocalValue();
  
  vPushStkVal(aStkVal);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* push s32 onto value stack                                                  */
/*----------------------------------------------------------------------------*/
void Stack::vPushS32(s32& sArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  GenVar xStrVar;
  xStrVar = sArg;
  
  vPushGenVar(xStrVar);
}

/*----------------------------------------------------------------------------*/
/* push s256 onto value stack                                                 */
/*----------------------------------------------------------------------------*/
void Stack::vPushS256(s256& sArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  GenVar xStrVar;
  xStrVar = sArg;
  
  vPushGenVar(xStrVar);
}

/*----------------------------------------------------------------------------*/
/* push s4K onto value stack                                                  */
/*----------------------------------------------------------------------------*/
void Stack::vPushS4K(s4K& sArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  GenVar xStrVar;
  xStrVar = sArg;
  
  vPushGenVar(xStrVar);
}

/*----------------------------------------------------------------------------*/
/* push s48K onto value stack                                                 */
/*----------------------------------------------------------------------------*/
void Stack::vPushS48K(s48K& sArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  GenVar xStrVar;
  xStrVar = sArg;
  
  vPushGenVar(xStrVar);
}

/*----------------------------------------------------------------------------*/
/* push s16KB onto value stack                                                */
/*----------------------------------------------------------------------------*/
void Stack::vPushS16KB(s16KB& sArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  GenVar xStrVar;
  xStrVar = sArg;
  
  vPushGenVar(xStrVar);
}

/*----------------------------------------------------------------------------*/
/* push value onto value stack                                                */
/*----------------------------------------------------------------------------*/
void Stack::vPushStkVal(StkVal& xVal1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  vPushVal(&xVal1);
}

/*----------------------------------------------------------------------------*/
/* push value onto value stack                                                */
/*----------------------------------------------------------------------------*/
void Stack::vPushStkVal(StkVal* aStkVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  vPushVal(aStkVal);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* completely reset stack                                                     */
/*----------------------------------------------------------------------------*/
void Stack::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  //! maybe this would be better as a re-init() ?
  
  while (nValues())
    vDiscard();
}

/*----------------------------------------------------------------------------*/
/* set stack back to retry point                                              */
/*----------------------------------------------------------------------------*/
void Stack::vRetry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);

  m_aTop    = m_aRetryTop;
  m_nValues = m_nRetryValues;
}

/*----------------------------------------------------------------------------*/
/* mark retry point                                                           */
/*----------------------------------------------------------------------------*/
void Stack::vRetryPoint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  m_aRetryTop    = m_aTop;
  m_nRetryValues = m_nValues;
}

/*----------------------------------------------------------------------------*/
/* get number of values currently on value stack                              */
/*----------------------------------------------------------------------------*/
i8 Stack::nValues(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  return m_nValues;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot a fip value on top of value stack                                    */
/*----------------------------------------------------------------------------*/
FipVal* Stack::aAllotFipVal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  FipVal* aFipVal = static_cast<FipVal*>(aAllotStkVal(sizeof(FipVal)));
  
  aFipVal->vClass(CN_FIPVAL);
  aFipVal->vMagic();
  
  return aFipVal;
}

/*----------------------------------------------------------------------------*/
/* allot an int value on top of value stack                                   */
/*----------------------------------------------------------------------------*/
IntVal* Stack::aAllotIntVal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  IntVal* aIntVal = static_cast<IntVal*>(aAllotStkVal(sizeof(IntVal)));
  
  aIntVal->vClass(CN_INTVAL);
  aIntVal->vMagic();
  
  return aIntVal;
}

/*----------------------------------------------------------------------------*/
/* allot a ptr value on top of value stack                                    */
/*----------------------------------------------------------------------------*/
PtrVal* Stack::aAllotPtrVal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  PtrVal* aPtrVal = static_cast<PtrVal*>(aAllotStkVal(sizeof(PtrVal)));
  
  aPtrVal->vClass(CN_PTRVAL);
  aPtrVal->vMagic();
  
  return aPtrVal;
}

/*----------------------------------------------------------------------------*/
/* pop StkVal on top of value stack, inc number of values on stack            */
/*----------------------------------------------------------------------------*/
StkVal* Stack::aAllotStkVal(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  char* aTop = reinterpret_cast<char*>(m_aTop);
  
  aTop -= nBytes;
  
  ASSERT(aTop >= (char*)aStackLimit());
  
  m_aTop = reinterpret_cast<StkVal*>(aTop);
  
  m_nValues++;

  return m_aTop;
}

/*----------------------------------------------------------------------------*/
/* allot a str value on top of value stack                                    */
/*----------------------------------------------------------------------------*/
StrVal* Stack::aAllotStrVal(i8 nSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  StrVal* aStrVal = static_cast<StrVal*>(aAllotStkVal(sizeof(StrVal) + nSize - 1));
  
  aStrVal->vClass(CN_STRVAL);
  aStrVal->vMagic();
  
  return aStrVal;
}

/*----------------------------------------------------------------------------*/
/* discard value on top of value stack                                        */
/*----------------------------------------------------------------------------*/
void Stack::vDiscard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  _hazard_();
  
  StkVal* aStkVal = reinterpret_cast<StkVal*>(m_aTop);
  
  i8 iSize = aStkVal->nStackBytes();
  
  vPopBy(iSize);
  
  m_nValues--;
  
  _hazard_();
}

/*----------------------------------------------------------------------------*/
/* pop fip from stack                                                         */
/*----------------------------------------------------------------------------*/
Cell* Stack::aPopFip(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  Cell* aFip = static_cast<FipVal*>(m_aTop)->aFip();
  
  vDiscard();
  
  return aFip;
}

/*----------------------------------------------------------------------------*/
/* pop int from stack                                                         */
/*----------------------------------------------------------------------------*/
i8 Stack::iPopInt(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  i8 iInt = static_cast<IntVal*>(m_aTop)->iInt();
  
  vDiscard();
  
  return iInt;
}

/*----------------------------------------------------------------------------*/
/* pop obj from stack                                                         */
/*----------------------------------------------------------------------------*/
Object* Stack::pPopObj(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  Object* pObj = static_cast<PtrVal*>(m_aTop)->pObj();
  
  vDiscard();
  
  return pObj;
}

/*----------------------------------------------------------------------------*/
/* pop local from value stack                                                 */
/*----------------------------------------------------------------------------*/
void Stack::vPopLocal(Local* aLocal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);

  _hazard_();
  
  i8 iSize = aTopStkVal()->nStackBytes();
  
  MemLib::vCopy(aLocal->aLocalValue(),m_aTop,iSize,CN_STACK);
  vPopBy(iSize);
  m_nValues--;
  
  _hazard_();
}

/*----------------------------------------------------------------------------*/
/* pop value from value stack                                                 */
/*----------------------------------------------------------------------------*/
void Stack::vPopVal(StkVal* aStkVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);

  _hazard_();
  
  i8 iSize = aTopStkVal()->nStackBytes();
  
  MemLib::vCopy(aStkVal,m_aTop,iSize,CN_STACK);
  vPopBy(iSize);
  m_nValues--;
  
  _hazard_();
}

/*----------------------------------------------------------------------------*/
/* push top by given bytes                                                    */
/*----------------------------------------------------------------------------*/
void Stack::vPushBy(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  char* pTop = reinterpret_cast<char*>(m_aTop);
  
  pTop -= nBytes;
  
  ASSERT(pTop >= (char*)aStackLimit());
  
  m_aTop = reinterpret_cast<StkVal*>(pTop);
}

/*----------------------------------------------------------------------------*/
/* push value onto value stack                                                */
/*----------------------------------------------------------------------------*/
void Stack::vPushVal(StkVal* aStkVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);

  _hazard_();
  
  ASSERT(CN_GENVAR != aStkVal->eClass());
  
  i8 iSize = aStkVal->nStackBytes();
  
  vPushBy(iSize);
  MemLib::vCopy(m_aTop,aStkVal,iSize,CN_STACK);
  m_nValues++;
  
 _hazard_();
}

/*----------------------------------------------------------------------------*/
/* base of this value stack                                                   */
/*----------------------------------------------------------------------------*/
StkVal* Stack::aStackBase(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  char* aBase = (char*)&m_xWsp.ayArea()[C_WSP_STACK - 1];
  
  return pPoolCheck<StkVal*>(aBase);
}

/*----------------------------------------------------------------------------*/
/* limit of this value stack                                                  */
/*----------------------------------------------------------------------------*/
StkVal* Stack::aStackLimit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  char* aLimit = (char*)&m_xWsp.ayArea()[0];
  
  return pPoolCheck<StkVal*>(aLimit);
}

/*----------------------------------------------------------------------------*/
/* is top value a zero?                                                       */
/*----------------------------------------------------------------------------*/
bool Stack::bTopIsZero(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if (CN_INTVAL == m_aTop->eClass() && 0 == _aIntVal(m_aTop)->iInt())
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* peek at value on top of value stack                                        */
/*----------------------------------------------------------------------------*/
StkVal* Stack::aTopStkVal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  StkVal* aStkVal = reinterpret_cast<StkVal*>(m_aTop);
  
  return aStkVal;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Stack)

/*----------------------------------------------------------------------------*/
/* get location of hazard                                                     */
/*----------------------------------------------------------------------------*/
Footer* Stack::aHazard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK);
  
  return &m_xHazard;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
