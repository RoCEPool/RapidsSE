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
  
  m_rTop.aTypedCell = (TypedCell*)aStackBase();
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
  
  m_rTop.vRewind();
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
void Stack::vPeekVal(i8 i_Val, TypedCell* aTypedCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  TypedCell* aPeekedValue = (TypedCell*)aPeekVal(i_Val);
  
  aTypedCell->vCopy(aPeekedValue);
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
  
  i8 iInt = m_rTop.aTypedCell->iAsInt();
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
  
  ForthCell* aFip = m_rTop.aTypedCell->aFip();
  
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
  
  i8 iInt = m_rTop.aTypedCell->iInt();
  
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
  
  //Object* pObj = m_rTop.aPtrVal->pObj();
  Object* pObj = m_rTop.aTypedCell->pObj();

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
void Stack::vPopVal(TypedCell* aTypedCell, const char* azFunNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(1) )
    Resources::vThrow(FK_DSTACK,VL_FATAL,
                        azFunNm,__FILE__,__LINE__,
                        "Invalid d-stack"_Stz);
  
  _hazard_();
  
  i8 nBytes = aTopVal()->nStackUsage();
  
  aTypedCell->vCopy(m_rTop.aTypedCell,nBytes);
  
  m_rTop.vRewind(nBytes);
  
  m_nValues--;
  
  _hazard_();
}

/*----------------------------------------------------------------------------*/
/* push ForthCell* onto stack                                                      */
/*----------------------------------------------------------------------------*/
void Stack::vPushFip(ForthCell* aFip)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  TypedCell* aTypedCell = aAllotFipVal();
  
  aTypedCell->vFip(aFip);
}

/*----------------------------------------------------------------------------*/
/* push i8 onto stack                                                         */
/*----------------------------------------------------------------------------*/
void Stack::vPushInt(i8 iInt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  TypedCell* aTypedCell = aAllotIntVal();
  
  aTypedCell->vInt(iInt);
}

/*----------------------------------------------------------------------------*/
/* push pointer to proper object onto stack                                   */
/*----------------------------------------------------------------------------*/
void Stack::vPushPtr(Object* pObj)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  TypedCell* aTypedCell = aAllotPtrVal();
  
  aTypedCell->vObj(pObj);
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
void Stack::vPushVal(TypedCell* aTypedCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);

  _hazard_();
  
  i8 nBytes = aTypedCell->nStackUsage();
  
  m_rTop.vAdvance(nBytes);
  
  m_rTop.aTypedCell->vCopy(aTypedCell,nBytes);
  
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
  
  m_rTop.aTypedCell  = (TypedCell*)aStackBase();
  m_nValues       = 0;
}

/*----------------------------------------------------------------------------*/
/* set stack back to retry point                                              */
/*----------------------------------------------------------------------------*/
void Stack::vRetry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);

  m_rTop.aTypedCell = (TypedCell*)m_aRetryTop;
  m_nValues      = m_nRetryValues;
}

/*----------------------------------------------------------------------------*/
/* mark retry point                                                           */
/*----------------------------------------------------------------------------*/
void Stack::vRetryPoint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  m_aRetryTop    = m_rTop.aTypedCell;
  m_nRetryValues = m_nValues;
}

/*----------------------------------------------------------------------------*/
/* is top value a zero?                                                       */
/*----------------------------------------------------------------------------*/
bool Stack::bTopIsZero(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if (CN_INTVAL == m_rTop.aTypedCell->eType() &&
      0 == m_rTop.aTypedCell->iInt())
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* peek at value on top of stack                                              */
/*----------------------------------------------------------------------------*/
AnyVal* Stack::aTopVal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  return (AnyVal*) m_rTop.aTypedCell;
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
TypedCell* Stack::aAllotTypedCell(eCN eType, i8 nWithAdditionalBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  i8 nAllotBytes = sizeof(_Header) + sizeof(Dtm) + nWithAdditionalBytes;
  
  m_rTop.vAdvance(nAllotBytes);
  ASSERT(m_rTop.ayByte >= (byte*)aStackLimit());
  
  m_rTop.aTypedCell->vMagic();

  m_rTop.aTypedCell->vType(eType);

  m_nValues++;
  
  return m_rTop.aTypedCell;
}

/*----------------------------------------------------------------------------*/
/* allot a fip value on top of stack                                          */
/*----------------------------------------------------------------------------*/
TypedCell* Stack::aAllotFipVal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  TypedCell* aTypedCell = aAllotTypedCell(CN_FIPVAL,C_NO_ADDITIONAL_BYTES);
  
  //aTypedCell->vType(CN_FIPVAL);
  //aFipVal->vMagic();
  
  return aTypedCell;
}

/*----------------------------------------------------------------------------*/
/* allot an int value on top of stack                                         */
/*----------------------------------------------------------------------------*/
TypedCell* Stack::aAllotIntVal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  TypedCell* aTypedCell = aAllotTypedCell(CN_INTVAL,C_NO_ADDITIONAL_BYTES);
  
  //aTypedCell->vType(CN_INTVAL);
  //aIntVal->vMagic();
  
  return aTypedCell;
}

/*----------------------------------------------------------------------------*/
/* allot a ptr value on top of stack                                          */
/*----------------------------------------------------------------------------*/
TypedCell* Stack::aAllotPtrVal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  TypedCell* aTypedCell = aAllotTypedCell(CN_PTRVAL,C_NO_ADDITIONAL_BYTES);
  
  //aTypedCell->vType(CN_PTRVAL);
  //aPtrVal->vMagic();
  
  return aTypedCell;
}

/*----------------------------------------------------------------------------*/
/* allot a str value on top of stack                                          */
/*----------------------------------------------------------------------------*/
TypedCell* Stack::aAllotStzVal(i8 nStrSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  i8 nWithAdditionalBytes = nStrSize;
  
  TypedCell* aTypedCell = aAllotTypedCell(CN_STZVAL,nWithAdditionalBytes);
  
  //aTypedCell->vType(CN_STZVAL);
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
TypedCell* Stack::aStackBase(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  char* aBase = (char*)&m_xWsp.ayArea()[C_WSP_STACK - 1];
  
  return pPoolCheck<TypedCell*>(aBase);
}

/*----------------------------------------------------------------------------*/
/* compute the limit of this stack                                            */
/*----------------------------------------------------------------------------*/
TypedCell* Stack::aStackLimit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  char* aLimit = (char*)&m_xWsp.ayArea()[0];
  
  return pPoolCheck<TypedCell*>(aLimit);
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
