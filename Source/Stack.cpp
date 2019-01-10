/*============================================================================*/
/* Stack.cpp -- Push/Pop Stack of Values                                      */
/*============================================================================*/

#include "Forth.h"

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
Stack::Stack(void)
: Object(CN_STACK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  m_pTop    = pStackBase();
  m_nValues = 0;
  
  m_pRetryTop    = pStackBase();
  m_iRetryValues = 0;
  
#ifdef __DEBUG_CHECKS
  m_xWsp.vFill(0x00,CN_STACK);
#endif
  
  _checkInitialised_(CN_STACK);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  //m_xHazard.vAssertFooter();
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
         pStack->m_pTop);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump contents of the value stack                                           */
/*----------------------------------------------------------------------------*/
void Stack::vContentDump(Stz* aPrefix, Stz* aTitle)
{
  vTrace("[%s]%s:\n"_Stz,aPrefix,aTitle);
  
  GenVar xVariable;
  
  s48K sString;
  
  i8 nValues = this->nValues();
  for (i8 iVal = 0; iVal < nValues; iVal++) {
    
    vPeekValWrk(iVal,xVariable);
    
    xVariable.vToString(sString);
    
    Value* pNestedValue = xVariable.pNestedValue();
    eCN eClass = pNestedValue->eClass();
    
    switch(eClass) {
        
      case CN_IPVAL: {
        IpVal* pIpVal = _pIpVal(pNestedValue);
        Cell* pCell = pIpVal->pIpPtr();
        vTrace("[%s] [%2lld] [%s] %p\n"_Stz,aPrefix,iVal,aCN[eClass],pCell);
        break;
      }
        
      case CN_INTVAL: {
        i8 iIntVal = xVariable.iNestedAsInt();
        vTrace("[%s] [%2lld] [%s] %lld\n"_Stz,aPrefix,iVal,aCN[eClass],iIntVal);
        break;
      }
        
      case CN_PTRVAL: {
        PtrVal* pPtrVal = xVariable.pNestedPtrVal();
        Object* pObject = pPtrVal->pPtr();
        eCN ePtdClass   = pObject->eClass();
        vTrace("[%s] [%2lld] [%s] %s (%s)\n"_Stz,
               aPrefix,iVal,aCN[eClass],sString.aChar(),aCN[ePtdClass]);
        break;
      }
        
      case CN_STRVAL: {
        StrVal* pStrVal = _pStrVal(pNestedValue);
        vTrace("[%s] [%2lld] [%s] %s\n"_Stz,aPrefix,iVal,aCN[eClass],pStrVal->aChar());
        break;
      }
        
      default: {
        vTrace("[%s] [%2lld] [%s]  %s (%s)\n"_Stz,
               aPrefix,iVal,aCN[eClass],sString.aChar(),aCN[eClass]);
        break;
      }
        
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* discard value on top of value stack                                        */
/*----------------------------------------------------------------------------*/
void Stack::vDiscard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  //m_xHazard.vAssertFooter();
  _hazard_();
  
  Value* pValue = reinterpret_cast<Value*>(m_pTop);
  i8 iSize = pValue->nSize();
  
  vPopBy(iSize);
  m_nValues--;
  
  //m_xHazard.vAssertFooter();
  _hazard_();
}

/*----------------------------------------------------------------------------*/
/* get address of given value on value stack                                  */
/*----------------------------------------------------------------------------*/
Value* Stack::pGetValue(i8 iNthValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  Value* pValue = pPeekValue();
  while (iNthValue--) {
    char* aAddress = reinterpret_cast<char*>(pValue);
    i8 iSize = pValue->nSize();
    aAddress += iSize;
    pValue = reinterpret_cast<Value*>(aAddress);
  }
  
  return pValue;
}

/*----------------------------------------------------------------------------*/
/* guard value stack                                                          */
/*----------------------------------------------------------------------------*/
bool Stack::bGuard(i8 iCount)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if (nValues() < iCount)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* guard value stack 1                                                        */
/*----------------------------------------------------------------------------*/
bool Stack::bGuard(eCN eClass0)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if (nValues() < 1)
    return false;
  
  Value* pValue0 = pPeekValue(0);
  if (eClass0 != pValue0->eClass())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* guard value stack 2                                                        */
/*----------------------------------------------------------------------------*/
bool Stack::bGuard(eCN eClass0, eCN eClass1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if (nValues() < 2)
    return false;
  
  Value* pValue0 = pPeekValue(0);
  if (eClass0 != pValue0->eClass())
    return false;
  
  Value* pValue1 = pPeekValue(1);
  if (eClass1 != pValue1->eClass())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* guard value stack 3                                                        */
/*----------------------------------------------------------------------------*/
bool Stack::bGuard(eCN eClass0, eCN eClass1, eCN eClass2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if (nValues() < 3)
    return false;
  
  Value* pValue0 = pPeekValue(0);
  if (eClass0 != pValue0->eClass())
    return false;
  
  Value* pValue1 = pPeekValue(1);
  if (eClass1 != pValue1->eClass())
    return false;
  
  Value* pValue2 = pPeekValue(2);
  if (eClass2 != pValue2->eClass())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* peek at value on top of value stack                                        */
/*----------------------------------------------------------------------------*/
Value* Stack::pPeekValue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  Value* pValue = reinterpret_cast<Value*>(m_pTop);
  
  return pValue;
}

/*----------------------------------------------------------------------------*/
/* peek at given value on value stack                                         */
/*----------------------------------------------------------------------------*/
Value* Stack::pPeekValue(i8 iNthValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  return pGetValue(iNthValue);
}

/*----------------------------------------------------------------------------*/
/* peek at given value on value stack                                         */
/*----------------------------------------------------------------------------*/
void Stack::vPeekValWrk(i8 iNthValue, GenVar& xVariable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  Value* pPeekedValue = pPeekValue(iNthValue);
  
  i8 iSize = pPeekedValue->nSize();
  
  //vTrace("Peek: %lld %p %lld\n"_Stz,iNthValue,pPeekedValue,iSize);
  
  MemLib::vCopy(xVariable.pNestedValueSelf(),pPeekedValue,iSize,CN_STACK);
}

/*----------------------------------------------------------------------------*/
/* pop cell from value stack                                                  */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(Forth::Cell*& pCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_IPVAL) )
    return false;
  
  IpVal xIpVal;
  
  vPopVal(&xIpVal);
  
  pCell = xIpVal.pIpPtr();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop eCN from value stack                                                   */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(eCN& iArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_INTVAL) )
    return false;
  
  IntVal xIntVal1;
  vPopVal(&xIntVal1);
  
  iArg1 = static_cast<eCN>(xIntVal1.iInt());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop value from value stack                                                 */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(GenVar* pArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(1) )
    return false;
  
  vPopVal(pArg1->pNestedValueSelf());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop value from value stack                                                 */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(GenVar& xArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(1) )
    return false;
  
  vPopVal(xArg1.pNestedValueSelf());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop i8 from value stack                                                    */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(i8& iArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_INTVAL) )
    return false;
  
  IntVal xIntVal1;
  vPopVal(&xIntVal1);
  
  iArg1 = xIntVal1.iInt();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop i8,i8 from value stack                                                 */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(i8& iArg1, i8& iArg2)
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
/* pop i8,val from value stack                                                */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(i8& iArg1, Object*& pArg2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_INTVAL,CN_PTRVAL) )
    return false;
  
  IntVal xIntVal1;
  vPopVal(&xIntVal1);
  
  iArg1 = xIntVal1.iInt();
  
  PtrVal xPtrVal1;
  vPopVal(&xPtrVal1);
  
  pArg2 = xPtrVal1.pPtr();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop i8,val from value stack                                                */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(i8& iArg1, GenVar& xArg2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_INTVAL,CN_STRVAL) )
    return false;
  
  IntVal xIntVal1;
  vPopVal(&xIntVal1);
  
  iArg1 = xIntVal1.iInt();
  
  vPopVal(xArg2.pNestedValueSelf());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop i8,s from value stack                                                  */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(i8& iArg1, s48K& sArg2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_INTVAL,CN_STRVAL) )
    return false;
  
  IntVal xIntVal1;
  vPopVal(&xIntVal1);
  
  iArg1 = xIntVal1.iInt();
  
  GenVar xValWrk2;
  vPopVal(xValWrk2.pNestedValueSelf());
  
  sArg2 = _pStrVal(&xValWrk2)->aStz();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop i8,i8,val from value stack                                             */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(i8& iArg1, i8& iArg2, Value& xArg3)
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
  
  vPopVal(&xArg3);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop Object* from value stack                                               */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(Object*& pObject)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_PTRVAL) && ! bPeekZero() )
    return false;
  
  PtrVal xPtrVal1;
  vPopVal(&xPtrVal1);
  
  pObject = xPtrVal1.pPtr();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop Object*, i8 from value stack                                           */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(Object*& pObject, i8& iArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_PTRVAL,CN_INTVAL) )
    return false;
  
  PtrVal xPtrVal1;
  vPopVal(&xPtrVal1);
  
  pObject = xPtrVal1.pPtr();
  
  IntVal xIntVal1;
  vPopVal(&xIntVal1);
  
  iArg = xIntVal1.iInt();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop Object*, Object* from value stack                                      */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(Object*& pObject1, Object*& pObject2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_PTRVAL,CN_PTRVAL) )
    return false;
  
  PtrVal xPtrVal1;
  vPopVal(&xPtrVal1);
  
  pObject1 = xPtrVal1.pPtr();
  
  PtrVal xPtrVal2;
  vPopVal(&xPtrVal2);
  
  pObject2 = xPtrVal2.pPtr();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop Object* from value stack                                               */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(Object*& pObject, GenVar& xVariable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_PTRVAL) )
    return false;
  
  PtrVal xPtrVal1;
  vPopVal(&xPtrVal1);
  
  pObject = xPtrVal1.pPtr();
  
  return bPop(xVariable);
}

/*----------------------------------------------------------------------------*/
/* pop register from value stack                                              */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(Register* pArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(1) )
    return false;
  
  vPopVal(pArg1);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop s from value stack                                                     */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(s32& sArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_STRVAL) )
    return false;
  
  GenVar xValWrk1;
  vPopVal(xValWrk1.pNestedValueSelf());
  
  sArg1 = _pStrVal(xValWrk1.pNestedValueSelf())->aStz();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop s from value stack                                                     */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(s256& sArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_STRVAL) )
    return false;
  
  GenVar xValWrk1;
  vPopVal(xValWrk1.pNestedValueSelf());
  
  sArg1 = _pStrVal(xValWrk1.pNestedValueSelf())->aStz();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop s from value stack                                                     */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(s4K& sArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_STRVAL) )
    return false;
  
  GenVar xValWrk1;
  vPopVal(xValWrk1.pNestedValueSelf());
  
  sArg1 = _pStrVal(xValWrk1.pNestedValueSelf())->aStz();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop s from value stack                                                     */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(s16KB& sArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_STRVAL) )
    return false;
  
  GenVar xValWrk1;
  vPopVal(xValWrk1.pNestedValueSelf());
  
  sArg1 = _pStrVal(xValWrk1.pNestedValueSelf())->aStz();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop s from value stack                                                     */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(s48K& sArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_STRVAL) )
    return false;
  
  GenVar xValWrk1;
  vPopVal(xValWrk1.pNestedValueSelf());
  
  sArg1 = _pStrVal(xValWrk1.pNestedValueSelf())->aStz();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop s,i8 from value stack                                                  */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(s48K& sArg1, i8& iArg2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_STRVAL,CN_INTVAL) )
    return false;
  
  GenVar xValWrk1;
  vPopVal(xValWrk1.pNestedValueSelf());
  
  sArg1 = _pStrVal(xValWrk1.pNestedValueSelf())->aStz();
  
  IntVal xIntVal2;
  vPopVal(&xIntVal2);
  
  iArg2 = xIntVal2.iInt();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop s,i8,i8 from value stack                                               */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(s48K& sArg1, i8& iArg2, i8& iArg3)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_STRVAL,CN_INTVAL,CN_INTVAL) )
    return false;
  
  GenVar xValWrk1;
  vPopVal(xValWrk1.pNestedValueSelf());
  sArg1 = _pStrVal(xValWrk1.pNestedValueSelf())->aStz();
  
  IntVal xIntVal2;
  vPopVal(&xIntVal2);
  iArg2 = xIntVal2.iInt();
  

  IntVal xIntVal3;
  vPopVal(&xIntVal3);
  iArg3 = xIntVal3.iInt();
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop s,s from value stack                                                   */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(s48K& sArg1, s48K& sArg2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_STRVAL,CN_STRVAL) )
    return false;
  
  GenVar xValWrk1;
  vPopVal(xValWrk1.pNestedValueSelf());
  
  sArg1 = _pStrVal(xValWrk1.pNestedValueSelf())->aStz();
  
  GenVar xValWrk2;
  vPopVal(xValWrk2.pNestedValueSelf());
  
  sArg2 = _pStrVal(xValWrk2.pNestedValueSelf())->aStz();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop value, value from value stack                                          */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(GenVar& pValue1, GenVar& pValue2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(2) )
    return false;
  
  vPopVal(pValue1.pNestedValueSelf());
  
  vPopVal(pValue2.pNestedValueSelf());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop value, value, value from value stack                                   */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(GenVar& pValue1, GenVar& pValue2, GenVar& pValue3)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(3) )
    return false;
  
  vPopVal(pValue1.pNestedValueSelf());
  
  vPopVal(pValue2.pNestedValueSelf());
  
  vPopVal(pValue3.pNestedValueSelf());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop value from value stack  as integer                                     */
/*----------------------------------------------------------------------------*/
bool Stack::bPopAsInt(i8& iArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(1) )
    return false;
  
  GenVar xWspVal1;
  vPopVal(xWspVal1.pNestedValueSelf());
  
  iArg1 = xWspVal1.iNestedAsInt();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop value from value stack  as integer                                     */
/*----------------------------------------------------------------------------*/
bool Stack::bPopAsInt_Quick(i8& iArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(1) )
    return false;
  
  // get pointer to value on the top of the stack
  Value* pTop = pPeekValue();

  // discard the value
  vDiscard();

  eCN eClassNo = pTop->eClass();
  
  switch (eClassNo) {
      
    case Base::CN_INTVAL:
      iArg1 = _pIntVal(pTop)->iInt();
      return true;
      
    case Base::CN_PTRVAL:
      iArg1 = _pPtrVal(pTop)->pPtr() ? 1 : 0;
      return true;
      
    default:
      NEVER_REACH;
  }

  NEVER_REACH;
  return false;
}

/*----------------------------------------------------------------------------*/
/* pop integer from stack                                                     */
/*----------------------------------------------------------------------------*/
i8 Stack::iPop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  i8 iArg = _pIntVal(m_pTop)->iInt();
  
  vPopBy(sizeof(IntVal));

  m_nValues--;
  
  return iArg;
}

/*----------------------------------------------------------------------------*/
/* poke at given value on value stack                                         */
/*----------------------------------------------------------------------------*/
void Stack::vPokeValWrk(i8 iNthValue, GenVar& xVariable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  i8 iSize = xVariable.pNestedValueSelf()->nSize();
  
  Value* pPokeAddress = pPeekValue(iNthValue);
  
  MemLib::vCopy(pPokeAddress,xVariable.pNestedValueSelf(),iSize,CN_STACK);
}

/*----------------------------------------------------------------------------*/
/* push bool onto value stack                                                 */
/*----------------------------------------------------------------------------*/
void Stack::vPushBool(bool bArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  IntVal xIntVal1;
  xIntVal1 = bArg1;
  
  vPushValue(xIntVal1);
}

/*----------------------------------------------------------------------------*/
/* push Cell* onto value stack                                                */
/*----------------------------------------------------------------------------*/
void Stack::vPushCell(Cell* pCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  IpVal xIpVal;
  
  xIpVal = pCell;
  
  vPushValue(xIpVal);
}

/*----------------------------------------------------------------------------*/
/* push buffer entry onto value stack                                         */
/*----------------------------------------------------------------------------*/
void Stack::vPushEntry(DBobjects::Entry* aEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  vPushEntry(aEntry);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* push value onto value stack                                                */
/*----------------------------------------------------------------------------*/
void Stack::vPushGenVar(GenVar* pValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  vPushVal(pValue->pNestedValueSelf());
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* push value onto value stack                                                */
/*----------------------------------------------------------------------------*/
void Stack::vPushGenVar(GenVar& xVariable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  vPushVal(xVariable.pNestedValue());
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* push i8 onto value stack                                                   */
/*----------------------------------------------------------------------------*/
void Stack::vPushI8(i8 iArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  IntVal xIntVal1;
  xIntVal1 = iArg1;
  
  vPushValue(xIntVal1);
}

/*----------------------------------------------------------------------------*/
/* push pointer to proper object onto value stack                             */
/*----------------------------------------------------------------------------*/
void Stack::vPushPointer(Object* pObject)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  PtrVal xPtrObject;
  
  xPtrObject = pObject;
  
  vPushValue(xPtrObject);
}

/*----------------------------------------------------------------------------*/
/* push value onto value stack                                                */
/*----------------------------------------------------------------------------*/
void Stack::vPushRegister(Register* pRegister)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  vPushVal(pRegister);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* push s32 onto value stack                                                  */
/*----------------------------------------------------------------------------*/
void Stack::vPushS32(s32& sArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  GenVar xStrVal1;
  xStrVal1 = sArg1;
  
  vPushGenVar(xStrVal1);
}

/*----------------------------------------------------------------------------*/
/* push s256 onto value stack                                                 */
/*----------------------------------------------------------------------------*/
void Stack::vPushS256(s256& sArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  GenVar xStrVal1;
  xStrVal1 = sArg1;
  
  vPushGenVar(xStrVal1);
}

/*----------------------------------------------------------------------------*/
/* push s4K onto value stack                                                  */
/*----------------------------------------------------------------------------*/
void Stack::vPushS4K(s4K& sArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  GenVar xStrVal1;
  xStrVal1 = sArg1;
  
  vPushGenVar(xStrVal1);
}

/*----------------------------------------------------------------------------*/
/* push s48K onto value stack                                                 */
/*----------------------------------------------------------------------------*/
void Stack::vPushS48K(s48K& sArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  GenVar xStrVal1;
  xStrVal1 = sArg1;
  
  vPushGenVar(xStrVal1);
}

/*----------------------------------------------------------------------------*/
/* push s16KB onto value stack                                                */
/*----------------------------------------------------------------------------*/
void Stack::vPushS16KB(s16KB& sArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  GenVar xStrVal1;
  xStrVal1 = sArg1;
  
  vPushGenVar(xStrVal1);
}

/*----------------------------------------------------------------------------*/
/* push value onto value stack                                                */
/*----------------------------------------------------------------------------*/
void Stack::vPushValue(Value& xVal1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  vPushVal(&xVal1);
}

/*----------------------------------------------------------------------------*/
/* push value onto value stack                                                */
/*----------------------------------------------------------------------------*/
void Stack::vPushValue(Value* pValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  vPushVal(pValue);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* set stack back to retry point                                              */
/*----------------------------------------------------------------------------*/
void Stack::vRetry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);

  m_pTop    = m_pRetryTop;
  m_nValues = m_iRetryValues;
}

/*----------------------------------------------------------------------------*/
/* mark retry point                                                           */
/*----------------------------------------------------------------------------*/
void Stack::vRetryPoint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  m_pRetryTop    = m_pTop;
  m_iRetryValues = m_nValues;
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
/* is top value a zero?                                                       */
/*----------------------------------------------------------------------------*/
bool Stack::bPeekZero(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);

  Value* pValue = pPeekValue();
  
  if (CN_INTVAL == pValue->eClass() && 0 == _pIntVal(pValue)->iInt())
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* pop value from value stack                                                 */
/*----------------------------------------------------------------------------*/
void Stack::vPopVal(Value* pValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  //m_xHazard.vAssertFooter();
  _hazard_();
  
  i8 iSize = pPeekValue()->nSize();
  
  MemLib::vCopy(pValue,m_pTop,iSize,CN_STACK);
  vPopBy(iSize);
  m_nValues--;
  
  //m_xHazard.vAssertFooter();
  _hazard_();
}

/*----------------------------------------------------------------------------*/
/* push top by given bytes                                                    */
/*----------------------------------------------------------------------------*/
void Stack::vPushBy(i8 iBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  char* pTop = reinterpret_cast<char*>(m_pTop);
  
  pTop -= iBytes;
  
  ASSERT(pTop >= (char*)pStackLimit());
  
  m_pTop = reinterpret_cast<Value*>(pTop);
}

/*----------------------------------------------------------------------------*/
/* push value onto value stack                                                */
/*----------------------------------------------------------------------------*/
void Stack::vPushVal(Value* pValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  //m_xHazard.vAssertFooter();
  _hazard_();
  
  ASSERT(CN_VALWRK != pValue->eClass());
  
  i8 iSize = pValue->nSize();
  
  vPushBy(iSize);
  MemLib::vCopy(m_pTop,pValue,iSize,CN_STACK);
  m_nValues++;
  
  //m_xHazard.vAssertFooter();
  _hazard_();
}

/*----------------------------------------------------------------------------*/
/* base of this value stack                                                   */
/*----------------------------------------------------------------------------*/
Value* Stack::pStackBase(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  char* pBase = &m_xWsp.aArea()[C_STACK_LENGTH-1];
  
  return pPoolCheck<Value*>(pBase);
}

/*----------------------------------------------------------------------------*/
/* limit of this value stack                                                  */
/*----------------------------------------------------------------------------*/
Value* Stack::pStackLimit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  char* pLimit = &m_xWsp.aArea()[0];
  
  return pPoolCheck<Value*>(pLimit);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Stack::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK);
  
  return &m_xFooter;
}

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
