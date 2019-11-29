/*============================================================================*/
/* Stack.cpp -- Push/Pop Stack of Values                                      */
/*============================================================================*/

#include "SenVM.h"


// Note: to optimise stack operations look at directly using pDataStack->m_xTop.pStkVal
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
  
  m_eKind        = eKind;
  
  m_xTop.pStkVal = aStackBase();
  m_nValues      = 0;
  
  m_aRetryTop    = aStackBase();
  m_nRetryValues = 0;
  
#ifdef WITH_CHECK_INITIALISED
  m_xWsp.vFill(0x00,CN_STACK);
#endif
  
  _checkInitialised_(CN_STACK);
  
  _hazard_();

  _postcondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Stack::~Stack(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump stack                                                                 */
/*----------------------------------------------------------------------------*/
void Stack::vDump(eCN eClass, Object* pObj, Stz* aTab)
{
  Stack* pStack = _pStack(pObj);
  
  vTrace("values %lld top %p\n"_Stz,
         pStack->m_nValues,
         pStack->m_xTop.pStkVal);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump contents of the stack                                                 */
/*----------------------------------------------------------------------------*/
void Stack::vContentDump(DicStack* pDicStack, Stz* aTitle)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  vTrace("%s:\n"_Stz,aTitle);
  
  GenVar xVar;
  
  i8 nValues = this->nValues();
  for (i8 i_Val = 0; i_Val < nValues; i_Val++) {
    
    AnyVal* aAnyVal = aPeekVal(i_Val);
    i8 nStackBytes = aAnyVal->nStackBytes();
    
    vPeekVal(i_Val,xVar);
    
    eCN eValClass = xVar.eValClass();
    
    if (CN_STRVAL == eValClass)
      vTrace("[%2lld %p %02lldB %6s (8,8,%02lld)] "_Stz,
             i_Val,aAnyVal,nStackBytes,aCN[eValClass],
             _aStrVal(aAnyVal)->nSize());
    else
      vTrace("[%2lld %p %02lldB %6s (8,8,00)] "_Stz,
             i_Val,aAnyVal,nStackBytes,aCN[eValClass]);

    
    switch(eValClass) {
        
      case CN_FIPVAL: {
        Cell* aFip  = xVar.aFip();
        if (pDicStack) {
          Word* pWord = pDicStack->pLocateWord(aFip);

          vTrace("=%p word %s \n"_Stz,aFip,
                 pWord ? pWord->szWordNm() : "<none>"_Stz);
        }
        else {
          vTrace("%p word %s \n"_Stz,aFip,
                 "<UNKNOWN - No pDicStack>"_Stz);
        }
        break;
      }
        
      case CN_INTVAL: {
        i8 iInt = xVar.iInt();
        
        vTrace("%lld\n"_Stz,iInt);
        break;
      }
        
      case CN_PTRVAL: {
        Object* pObj  = xVar.pObj();
        eCN ePtdClass = pObj->eClass();
        
        vTrace("%p (%s)\n"_Stz,pObj,aCN[ePtdClass]);
        break;
      }
        
      case CN_STRVAL: {
        char* azChar = xVar.azChar();
        
        vTrace("%s\n"_Stz,azChar);
        break;
      }
        
      default: {
        
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
/* guard top value on stack has given class                                   */
/*----------------------------------------------------------------------------*/
bool Stack::bGuard(eCN eClass0)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if (nValues() < 1)
    return false;
  
  StkVal* aStkVal0 = aPeekVal(0);
  if (eClass0 != aStkVal0->eClass())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* guard top 2 values on stack have given classes                             */
/*----------------------------------------------------------------------------*/
bool Stack::bGuard(eCN eClass0, eCN eClass1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if (nValues() < 2)
    return false;
  
  StkVal* aStkVal0 = aPeekVal(0);
  if (eClass0 != aStkVal0->eClass())
    return false;
  
  StkVal* aStkVal1 = aPeekVal(1);
  if (eClass1 != aStkVal1->eClass())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* guard top 3 values on stack have given classes                             */
/*----------------------------------------------------------------------------*/
bool Stack::bGuard(eCN eClass0, eCN eClass1, eCN eClass2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if (nValues() < 3)
    return false;
  
  StkVal* aStkVal0 = aPeekVal(0);
  if (eClass0 != aStkVal0->eClass())
    return false;
  
  StkVal* aStkVal1 = aPeekVal(1);
  if (eClass1 != aStkVal1->eClass())
    return false;
  
  StkVal* aSrkVal2 = aPeekVal(2);
  if (eClass2 != aSrkVal2->eClass())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* peek at given value on stack                                               */
/*----------------------------------------------------------------------------*/
AnyVal* Stack::aPeekVal(i8 i_Val)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  StkVal* aStkVal = aTopVal();
  while (i_Val--) {
    char* aAddress = reinterpret_cast<char*>(aStkVal);
    i8 iSize = aStkVal->nStackBytes();
    aAddress += iSize;
    aStkVal = reinterpret_cast<StkVal*>(aAddress);
  }
  
  return (AnyVal*)aStkVal;
}

/*----------------------------------------------------------------------------*/
/* peek at given value on stack                                               */
/*----------------------------------------------------------------------------*/
void Stack::vPeekVal(i8 i_Val, GenVar& xGenVar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  StkVal* aPeekedValue = aPeekVal(i_Val);
  
  xGenVar.vCopy(aPeekedValue);
}

/*----------------------------------------------------------------------------*/
/* poke given value into stack                                                */
/*----------------------------------------------------------------------------*/
void Stack::vPokeVal(i8 i_Val, AnyVal* aAnyVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  StkVal* aPokeAddress = aPeekVal(i_Val);
  
  aPokeAddress->vCopy(aAnyVal);
}

/*----------------------------------------------------------------------------*/
/* pop value from stack as integer                                            */
/*----------------------------------------------------------------------------*/
bool Stack::bPopAsInt(i8& iInt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(1) )
    return false;
  
  iInt = m_xTop.pAnyVal->iInt();
  vTopDiscard();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop value from stack as integer                                            */
/*----------------------------------------------------------------------------*/
bool Stack::bPopAsInt_Quick(i8& iInt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(1) )
    return false;
  
  StkVal* aTopValue = aTopVal();
  
  vTopDiscard();
  
  eCN eClassNo = aTopValue->eClass();
  
  switch (eClassNo) {
      
    case CN_INTVAL:
      iInt = _aIntVal(aTopValue)->iInt();
      return true;
      
    case CN_PTRVAL:
      iInt = _aPtrVal(aTopValue)->pObj() ? 1 : 0;
      return true;
      
    default:
      NEVER_REACH;
  }
  
  NEVER_REACH;
  return false;
}

/*----------------------------------------------------------------------------*/
/* pop fip from stack                                                         */
/*----------------------------------------------------------------------------*/
bool Stack::bPopFip(Cell*& aFip)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_FIPVAL) )
    return false;
  
  aFip = aFipValPop();

  return true;
}

/*----------------------------------------------------------------------------*/
/* pop i8 from stack                                                          */
/*----------------------------------------------------------------------------*/
bool Stack::bPopInt(i8& iInt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_INTVAL) )
    return false;
  
  iInt = iIntValPop();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop Object* from stack                                                     */
/*----------------------------------------------------------------------------*/
bool Stack::bPopPtr(Object*& pObj)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_PTRVAL) && ! bTopIsZero() )
    return false;
  
  pObj = pPtrValPop();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop register from stack                                                    */
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
/* pop string from stack                                                      */
/*----------------------------------------------------------------------------*/
bool Stack::bPopStr(String* pString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_STRVAL) )
    return false;
  
  _hazard_();
  
  StrVal* aStrVal = _aStrVal(aTopVal());
  
  pString->vCopy(aStrVal->szStz());
  
  vTopDiscard();
  
  _hazard_();

  return true;
}

/*----------------------------------------------------------------------------*/
/* pop value from stack into given variable                                   */
/*----------------------------------------------------------------------------*/
bool Stack::bPopVal(GenVar& xGenVar)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(1) )
    return false;
  
  vAnyValPop(xGenVar);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* push Cell* onto stack                                                      */
/*----------------------------------------------------------------------------*/
void Stack::vPushFip(Cell* aFip)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  FipVal* aFipVal = aAllotFipVal();
  
  * aFipVal = aFip;
}

/*----------------------------------------------------------------------------*/
/* push i8 onto stack                                                         */
/*----------------------------------------------------------------------------*/
void Stack::vPushInt(i8 iInt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  IntVal* aIntVal = aAllotIntVal();
  
  * aIntVal = iInt;
}

/*----------------------------------------------------------------------------*/
/* push pointer to proper object onto stack                                   */
/*----------------------------------------------------------------------------*/
void Stack::vPushPtr(Object* pObj)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  PtrVal* aPtrVal = aAllotPtrVal();
  
  * aPtrVal = pObj;
}

/*----------------------------------------------------------------------------*/
/* push contents of local onto stack                                          */
/*----------------------------------------------------------------------------*/
void Stack::vPushLocal(Local* pLocal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  AnyVal* aAnyVal = _aAnyVal(pLocal->aLocalValue());
  
  vPushVal(aAnyVal);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* push string onto stack                                                     */
/*----------------------------------------------------------------------------*/
void Stack::vPushStr(String* pStr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  //vContentDump(nullptr,"Before:"_Stz);

  i8 nSize = StrLib::iSize(pStr->azStr());
  StrVal* aStrVal = aAllotStrVal(nSize);

  aStrVal->vAssign(pStr->szStz());
  //aStrVal->vInspect();

  //vContentDump(nullptr,"After:"_Stz);
}

/*----------------------------------------------------------------------------*/
/* push value onto stack                                                      */
/*----------------------------------------------------------------------------*/
void Stack::vPushVal(AnyVal* aAnyVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);

  //aStkVal->vInspect();
  
  vAnyValPush(aAnyVal);
}

/*----------------------------------------------------------------------------*/
/* completely reset stack                                                     */
/*----------------------------------------------------------------------------*/
void Stack::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  m_xTop.pStkVal  = aStackBase();
  m_nValues       = 0;
}

/*----------------------------------------------------------------------------*/
/* set stack back to retry point                                              */
/*----------------------------------------------------------------------------*/
void Stack::vRetry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);

  m_xTop.pStkVal = m_aRetryTop;
  m_nValues      = m_nRetryValues;
}

/*----------------------------------------------------------------------------*/
/* mark retry point                                                           */
/*----------------------------------------------------------------------------*/
void Stack::vRetryPoint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  m_aRetryTop    = m_xTop.pStkVal;
  m_nRetryValues = m_nValues;
}

/*----------------------------------------------------------------------------*/
/* get number of values currently on stack                                    */
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
/* allot a value on top of stack                                               */
/*----------------------------------------------------------------------------*/
AnyVal* Stack::aAllotAnyVal(i8 nWithAdditionalBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  i8 nAllotBytes = sizeof(AnyVal) + nWithAdditionalBytes;
  
  m_xTop.vAdvance(nAllotBytes);
  ASSERT(m_xTop.ayByte >= (byte*)aStackLimit());
  
  m_nValues++;
  
  return m_xTop.pAnyVal;
}

/*----------------------------------------------------------------------------*/
/* allot a fip value on top of stack                                          */
/*----------------------------------------------------------------------------*/
FipVal* Stack::aAllotFipVal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  FipVal* aFipVal = (FipVal*) aAllotAnyVal(C_NO_ADDITIONAL_BYTES);
  
  aFipVal->vClass(CN_FIPVAL);
  aFipVal->vMagic();
  
  return aFipVal;
}

/*----------------------------------------------------------------------------*/
/* allot an int value on top of stack                                         */
/*----------------------------------------------------------------------------*/
IntVal* Stack::aAllotIntVal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  IntVal* aIntVal = (IntVal*) aAllotAnyVal(C_NO_ADDITIONAL_BYTES);
  
  aIntVal->vClass(CN_INTVAL);
  aIntVal->vMagic();
  
  return aIntVal;
}

/*----------------------------------------------------------------------------*/
/* allot a ptr value on top of stack                                          */
/*----------------------------------------------------------------------------*/
PtrVal* Stack::aAllotPtrVal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  PtrVal* aPtrVal = (PtrVal*) aAllotAnyVal(C_NO_ADDITIONAL_BYTES);
  
  aPtrVal->vClass(CN_PTRVAL);
  aPtrVal->vMagic();
  
  return aPtrVal;
}

/*----------------------------------------------------------------------------*/
/* allot a str value on top of stack                                          */
/*----------------------------------------------------------------------------*/
StrVal* Stack::aAllotStrVal(i8 nStrSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  i8 nWithAdditionalBytes = nStrSize;
  
  StrVal* aStrVal = (StrVal*) aAllotAnyVal(nWithAdditionalBytes);
  
  aStrVal->vClass(CN_STRVAL);
  aStrVal->vMagic();
  
  return aStrVal;
}

/*----------------------------------------------------------------------------*/
/* pop any value from stack                                                   */
/*----------------------------------------------------------------------------*/
void Stack::vAnyValPop(AnyVal* aAnyVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  _hazard_();
  
  i8 nBytes = aTopVal()->nStackBytes();
  
  aAnyVal->vCopy(m_xTop.pAnyVal,nBytes);
  
  m_xTop.vRewind(nBytes);
  
  m_nValues--;
  
  _hazard_();
}

/*----------------------------------------------------------------------------*/
/* push any value onto stack                                                  */
/*----------------------------------------------------------------------------*/
void Stack::vAnyValPush(AnyVal* aAnyVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  _hazard_();
  
  ASSERT(CN_GENVAR != aAnyVal->eClass());
  
  i8 nBytes = aAnyVal->nStackBytes();
  
  m_xTop.vAdvance(nBytes);
  
  m_xTop.pStkVal->vCopy(aAnyVal,nBytes);
  
  m_nValues++;
  
  _hazard_();
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* pop fip from stack                                                         */
/*----------------------------------------------------------------------------*/
Cell* Stack::aFipValPop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  Cell* aFip = m_xTop.pFipVal->aFip();
  
  vTopDiscard();
  
  return aFip;
}

/*----------------------------------------------------------------------------*/
/* pop int from stack                                                         */
/*----------------------------------------------------------------------------*/
i8 Stack::iIntValPop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  i8 iInt = m_xTop.pIntVal->iInt();
  
  vTopDiscard();
  
  return iInt;
}

/*----------------------------------------------------------------------------*/
/* pop obj from stack                                                         */
/*----------------------------------------------------------------------------*/
Object* Stack::pPtrValPop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  Object* pObj = m_xTop.pPtrVal->pObj();
  
  vTopDiscard();
  
  return pObj;
}

/*----------------------------------------------------------------------------*/
/* pop local from stack                                                       */
/*----------------------------------------------------------------------------*/
void Stack::vPopLocal(Local* aLocal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  vAnyValPop((AnyVal*)aLocal->aLocalValue());
}

/*----------------------------------------------------------------------------*/
/* compute the base of this stack                                             */
/*----------------------------------------------------------------------------*/
AnyVal* Stack::aStackBase(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  char* aBase = (char*)&m_xWsp.ayArea()[C_WSP_STACK - 1];
  
  return pPoolCheck<AnyVal*>(aBase);
}

/*----------------------------------------------------------------------------*/
/* compute the limit of this stack                                            */
/*----------------------------------------------------------------------------*/
AnyVal* Stack::aStackLimit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  char* aLimit = (char*)&m_xWsp.ayArea()[0];
  
  return pPoolCheck<AnyVal*>(aLimit);
}

/*----------------------------------------------------------------------------*/
/* discard value on top of stack                                              */
/*----------------------------------------------------------------------------*/
void Stack::vTopDiscard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  _hazard_();
  
  m_xTop.vRewind();
  m_nValues--;
  
  _hazard_();
}

/*----------------------------------------------------------------------------*/
/* is top value a zero?                                                       */
/*----------------------------------------------------------------------------*/
bool Stack::bTopIsZero(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if (CN_INTVAL == m_xTop.pStkVal->eClass() && 0 == m_xTop.pIntVal->iInt())
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* peek at value on top of stack                                              */
/*----------------------------------------------------------------------------*/
AnyVal* Stack::aTopVal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
    
  return (AnyVal*) m_xTop.pStkVal;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Stack)

/*----------------------------------------------------------------------------*/
/*-- Hazard ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aHazardFunction(Stack,CN_STACK)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
