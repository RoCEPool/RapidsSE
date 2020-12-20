/*============================================================================*/
/* Stack.cpp -- Push/Pop Stack of Values                                      */
/*============================================================================*/

#include "SenVM.h"

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
  
  m_xTop.aAnyVal = aStackBase();
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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* discard value on top of stack                                              */
/*----------------------------------------------------------------------------*/
void Stack::vDiscardTop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  _hazard_();
  
  m_xTop.vRewind();
  m_nValues--;
  
  _hazard_();
}

/*----------------------------------------------------------------------------*/
/* dump contents of the stack                                                 */
/*----------------------------------------------------------------------------*/
void Stack::vDump(DicStack* pDicStack, Stz* aTitle)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  vTrace("%s:\n"_Stz,aTitle);
  
  i8 nValues = this->nValues();
  for (i8 i_Val = 0; i_Val < nValues; i_Val++) {
    
    AnyVal* aAnyVal = aPeekVal(i_Val);
    
    eCN eValClass  = aAnyVal->eClass();
    i8 nStackUsage = aAnyVal->nStackUsage();
    
    if (CN_STZVAL == eValClass)
      vTrace("[%2lld %p %02lldB %6s (8,8,%02lld)] "_Stz,
             i_Val,aAnyVal,nStackUsage,aCN[eValClass],
             _aTypedCell(aAnyVal)->nStzSize());
    else
      vTrace("[%2lld %p %02lldB %6s (8,8,00)] "_Stz,
             i_Val,aAnyVal,nStackUsage,aCN[eValClass]);
    
    switch(eValClass) {
        
      case CN_FIPVAL: {
        ForthCell* aFip  = aAnyVal->aFip();
        if (pDicStack) {
          Word* pWord = pDicStack->pLocateRoutineWord(aFip);
          
          vTrace("=%p word %s \n"_Stz,aFip,
                 pWord ? pWord->szWordNm() : C_NO_WORD);
        }
        else {
          vTrace("%p word %s \n"_Stz,aFip,
                 "<UNKNOWN - No pDicStack>"_Stz);
        }
        break;
      }
        
      case CN_INTVAL: {
        i8 iInt = aAnyVal->iInt();
        
        vTrace("%lld\n"_Stz,iInt);
        break;
      }
        
      case CN_PTRVAL: {
        Object* pObj  =aAnyVal->pObj();
        eCN ePtdClass = pObj->eClass();
        
        if (CN_WORD == ePtdClass) {
          Word* pPtdWord = _pWord(pObj);
          vTrace("%p (%s) %s\n"_Stz,pObj,aCN[ePtdClass],pPtdWord->szWordNm());
        }
        else
          vTrace("%p (%s)\n"_Stz,pObj,aCN[ePtdClass]);

        break;
      }
        
      case CN_STZVAL: {
        char* azChar = aAnyVal->azChar();
        
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
/* peek at given value on stack                                               */
/*----------------------------------------------------------------------------*/
AnyVal* Stack::aPeekVal(i8 i_Val)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  AnyVal* aAnyVal = aTopVal();
  while (i_Val--) {
    char* aAddress = reinterpret_cast<char*>(aAnyVal);
    i8 iSize = aAnyVal->nStackUsage();
    aAddress += iSize;
    aAnyVal = reinterpret_cast<AnyVal*>(aAddress);
  }
  
  return (AnyVal*)aAnyVal;
}

/*----------------------------------------------------------------------------*/
/* peek at given value on stack                                               */
/*----------------------------------------------------------------------------*/
void Stack::vPeekVal(i8 i_Val, AnyVal* aAnyVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  AnyVal* aPeekedValue = aPeekVal(i_Val);
  
  aAnyVal->vCopy(aPeekedValue);
}

/*----------------------------------------------------------------------------*/
/* poke given value into stack                                                */
/*----------------------------------------------------------------------------*/
void Stack::vPokeVal(i8 i_Val, AnyVal* aAnyVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  AnyVal* aPokeAddress = aPeekVal(i_Val);
  
  aPokeAddress->vCopy(aAnyVal);
}

/*----------------------------------------------------------------------------*/
/* pop value from stack as integer                                            */
/*----------------------------------------------------------------------------*/
i8 Stack::iPopAsInt(const char* azFunNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(1) )
    Resources::vThrow(FK_DSTACK,VL_FATAL,
                        azFunNm,__FILE__,__LINE__,
                        "Invalid d-stack"_Stz);
  
  i8 iInt = m_xTop.aAnyVal->iAsInt();
  vDiscardTop();
  
  return iInt;
}

/*----------------------------------------------------------------------------*/
/* pop fip from stack                                                         */
/*----------------------------------------------------------------------------*/
ForthCell* Stack::aPopFip(const char* azFunNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_FIPVAL) )
    Resources::vThrow(FK_DSTACK,VL_FATAL,
                        azFunNm,__FILE__,__LINE__,
                        "Invalid d-stack"_Stz);
  
  ForthCell* aFip = m_xTop.aFipVal->aFip();
  
  vDiscardTop();
  
  return aFip;
}

/*----------------------------------------------------------------------------*/
/* pop i8 from stack                                                          */
/*----------------------------------------------------------------------------*/
i8 Stack::iPopInt(const char* azFunNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_INTVAL) )
    Resources::vThrow(FK_DSTACK,VL_FATAL,
                        azFunNm,__FILE__,__LINE__,
                        "Invalid d-stack"_Stz);
  
  i8 iInt = m_xTop.aIntVal->iInt();
  
  vDiscardTop();
  
  return iInt;
}

/*----------------------------------------------------------------------------*/
/* pop Object* from stack                                                     */
/*----------------------------------------------------------------------------*/
Object* Stack::pPopPtr(const char* azFunNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if (! bGuard(CN_PTRVAL) && ! bTopIsZero() )
    Resources::vThrow(FK_DSTACK,VL_FATAL,
                        azFunNm,__FILE__,__LINE__,
                        "Invalid d-stack"_Stz);
  
  Object* pObj = m_xTop.aPtrVal->pObj();
  
  vDiscardTop();
  
  return pObj;
}

/*----------------------------------------------------------------------------*/
/* pop string from stack                                                      */
/*----------------------------------------------------------------------------*/
void Stack::vPopStr(String* pString, const char* azFunNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_STZVAL) )
    Resources::vThrow(FK_DSTACK,VL_FATAL,
                        azFunNm,__FILE__,__LINE__,
                        "Invalid d-stack"_Stz);
  
  _hazard_();
  
  TypedCell* aTypedCell = _aTypedCell(aTopVal());
  
  pString->vCopy(aTypedCell->szStz());
  
//  if (aStzVal->szStz()->bContains("/liteOp/_PulseMarker"_Stz)) {
//    vTrace("vPopStr: %s"_Stz,aStzVal->szStz());
//    Resources::vDebugPoint();
//  }
  
  vDiscardTop();
  
  _hazard_();
}

/*----------------------------------------------------------------------------*/
/* pop value from stack                                                       */
/*----------------------------------------------------------------------------*/
void Stack::vPopVal(AnyVal* aAnyVal, const char* azFunNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(1) )
    Resources::vThrow(FK_DSTACK,VL_FATAL,
                        azFunNm,__FILE__,__LINE__,
                        "Invalid d-stack"_Stz);
  
  _hazard_();
  
  i8 nBytes = aTopVal()->nStackUsage();
  
  aAnyVal->vCopy(m_xTop.aAnyVal,nBytes);
  
  m_xTop.vRewind(nBytes);
  
  m_nValues--;
  
  _hazard_();
}

/*----------------------------------------------------------------------------*/
/* push ForthCell* onto stack                                                      */
/*----------------------------------------------------------------------------*/
void Stack::vPushFip(ForthCell* aFip)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  FipVal* aFipVal = aAllotFipVal();
  
  aFipVal->vFip(aFip);
}

/*----------------------------------------------------------------------------*/
/* push i8 onto stack                                                         */
/*----------------------------------------------------------------------------*/
void Stack::vPushInt(i8 iInt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  IntVal* aIntVal = aAllotIntVal();
  
  aIntVal->vInt(iInt);
}

/*----------------------------------------------------------------------------*/
/* push pointer to proper object onto stack                                   */
/*----------------------------------------------------------------------------*/
void Stack::vPushPtr(Object* pObj)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  PtrVal* aPtrVal = aAllotPtrVal();
  
  aPtrVal->vObj(pObj);
}

/*----------------------------------------------------------------------------*/
/* push string onto stack                                                     */
/*----------------------------------------------------------------------------*/
void Stack::vPushStr(String* pStr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  //vDump(nullptr,"Before:"_Stz);

  i8 nSize = StrLib::nSize(pStr->azStr());
  TypedCell* aTypedCell = aAllotStzVal(nSize);

  aTypedCell->vStz(pStr->szStz());
  //aStzVal->vInspect();

  //vDump(nullptr,"After:"_Stz);
}

/*----------------------------------------------------------------------------*/
/* push stz onto stack                                                        */
/*----------------------------------------------------------------------------*/
void Stack::vPushStz(Stz* szStz)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  //vDump(nullptr,"Before:"_Stz);
  
  i8 nSize = szStz->nSize();
  TypedCell* aTypedCell = aAllotStzVal(nSize);
  
  aTypedCell->vStz(szStz);
  //aStzVal->vInspect();
  
  //vDump(nullptr,"After:"_Stz);
}

/*----------------------------------------------------------------------------*/
/* push value onto stack                                                      */
/*----------------------------------------------------------------------------*/
void Stack::vPushVal(AnyVal* aAnyVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);

  _hazard_();
  
  i8 nBytes = aAnyVal->nStackUsage();
  
  m_xTop.vAdvance(nBytes);
  
  m_xTop.aAnyVal->vCopy(aAnyVal,nBytes);
  
  m_nValues++;
  
  _hazard_();
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* completely reset stack                                                     */
/*----------------------------------------------------------------------------*/
void Stack::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  m_xTop.aAnyVal  = aStackBase();
  m_nValues       = 0;
}

/*----------------------------------------------------------------------------*/
/* set stack back to retry point                                              */
/*----------------------------------------------------------------------------*/
void Stack::vRetry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);

  m_xTop.aAnyVal = m_aRetryTop;
  m_nValues      = m_nRetryValues;
}

/*----------------------------------------------------------------------------*/
/* mark retry point                                                           */
/*----------------------------------------------------------------------------*/
void Stack::vRetryPoint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  m_aRetryTop    = m_xTop.aAnyVal;
  m_nRetryValues = m_nValues;
}

/*----------------------------------------------------------------------------*/
/* is top value a zero?                                                       */
/*----------------------------------------------------------------------------*/
bool Stack::bTopIsZero(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if (CN_INTVAL == m_xTop.aAnyVal->eClass() && 0 == m_xTop.aIntVal->iInt())
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* peek at value on top of stack                                              */
/*----------------------------------------------------------------------------*/
AnyVal* Stack::aTopVal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  return (AnyVal*) m_xTop.aAnyVal;
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
  
  return m_xTop.aAnyVal;
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
TypedCell* Stack::aAllotStzVal(i8 nStrSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  i8 nWithAdditionalBytes = nStrSize;
  
  TypedCell* aTypedCell = (TypedCell*) aAllotAnyVal(nWithAdditionalBytes);
  
  aTypedCell->vType(CN_STZVAL);
  //aStzVal->vMagic();
  
  return aTypedCell;
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
  
  AnyVal* aAnyVal0 = aPeekVal(0);
  if (eClass0 != aAnyVal0->eClass())
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
  
  AnyVal* aAnyVal0 = aPeekVal(0);
  if (eClass0 != aAnyVal0->eClass())
    return false;
  
  AnyVal* aAnyVal1 = aPeekVal(1);
  if (eClass1 != aAnyVal1->eClass())
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
  
  AnyVal* aAnyVal0 = aPeekVal(0);
  if (eClass0 != aAnyVal0->eClass())
    return false;
  
  AnyVal* aAnyVal1 = aPeekVal(1);
  if (eClass1 != aAnyVal1->eClass())
    return false;
  
  AnyVal* aSrkVal2 = aPeekVal(2);
  if (eClass2 != aSrkVal2->eClass())
    return false;
  
  return true;
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
