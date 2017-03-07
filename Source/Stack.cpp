/*============================================================================*/
/* Stack.cpp -- Push/Pop Stack of Values                                      */
/*============================================================================*/

#include "Structures.h"

#include "Dictionaries.h"

extern BufferPool* gpBufferPool;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Stack* Stack::_pStack(void* aAddr)
{
  Stack* pStack = reinterpret_cast<Stack*>(aAddr);
  
  RASSERT( !pStack || CN_STACK == pStack->eClass() );
  
  return pStack;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Stack::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pStack = Heap::pAllocate(CN_STACK);
  
  return pStack;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Stack::operator delete(void* pBufferAddress)
{
  RASSERT(pBufferAddress);
  Stack* pStack = _pStack(pBufferAddress);
  
  Heap::vFree(pStack);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Stack::Stack(void)
: Buffer(CN_STACK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  m_pTop    = pStackBase();
  m_iValues = 0;
  
  m_pRetryTop    = pStackBase();
  m_iRetryValues = 0;
  
  m_xWsp.vFill(0x00);
  
  _checkInitialised_(CN_STACK);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  //m_xHazard.vAssertFooter();
  _hazard_();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Stack::~Stack(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump grid                                                                  */
/*----------------------------------------------------------------------------*/
void Stack::vDump(eCN eClass, Object* pObject, Str* aTab)
{
  ASSERT(eClass == CN_STACK);
  ASSERT(pObject->eClass() == CN_STACK);
  
  Stack* pStack = _pStack(pObject);
  
  vTrace("values %lld top %p\n"_Str,
         pStack->m_iValues,
         pStack->m_pTop);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* guard data stack                                                           */
/*----------------------------------------------------------------------------*/
bool Stack::bGuard(i8 iCount)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if (iValues() < iCount)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* guard data stack 1                                                         */
/*----------------------------------------------------------------------------*/
bool Stack::bGuard(eCN eClass0)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if (iValues() < 1)
    return false;
  
  Value* pValue0 = pPeek(0);
  if (eClass0 != pValue0->eClass())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* guard data stack 2                                                         */
/*----------------------------------------------------------------------------*/
bool Stack::bGuard(eCN eClass0, eCN eClass1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if (iValues() < 2)
    return false;
  
  Value* pValue0 = pPeek(0);
  if (eClass0 != pValue0->eClass())
    return false;
  
  Value* pValue1 = pPeek(1);
  if (eClass1 != pValue1->eClass())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* guard data stack 3                                                         */
/*----------------------------------------------------------------------------*/
bool Stack::bGuard(eCN eClass0, eCN eClass1, eCN eClass2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if (iValues() < 3)
    return false;
  
  Value* pValue0 = pPeek(0);
  if (eClass0 != pValue0->eClass())
    return false;
  
  Value* pValue1 = pPeek(1);
  if (eClass1 != pValue1->eClass())
    return false;
  
  Value* pValue2 = pPeek(2);
  if (eClass2 != pValue2->eClass())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* pop cell from data stack                                                   */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(Dictionaries::Cell*& pCell)
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
/* pop Header* from data stack                                                */
/*----------------------------------------------------------------------------*/
/*
bool Stack::bPop(Header*& pHeader)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_PTRVAL) )
    return false;
  
  PtrVal xPtrVal1;
  vPopVal(&xPtrVal1);
  
  pHeader = xPtrVal1.pHeader();
  
  return true;
}
*/
/*----------------------------------------------------------------------------*/
/* pop eCN from data stack                                                    */
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
/* pop i8 from data stack                                                     */
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
/* pop i8,i8 from data stack                                                  */
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
/* pop i8,val from data stack                                                 */
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
/* pop i8,val from data stack                                                 */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(i8& iArg1, Value& xArg2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_INTVAL,CN_STRVAL) )
    return false;
  
  IntVal xIntVal1;
  vPopVal(&xIntVal1);
  
  iArg1 = xIntVal1.iInt();
  
  vPopVal(&xArg2);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop i8,s from data stack                                                   */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(i8& iArg1, s48K& sArg2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_INTVAL,CN_STRVAL) )
    return false;
  
  IntVal xIntVal1;
  vPopVal(&xIntVal1);
  
  iArg1 = xIntVal1.iInt();
  
  ValWsp xValWsp2;
  vPopVal(&xValWsp2);
  
  sArg2 = _pStrVal(&xValWsp2)->aStr();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop i8,i8,val from data stack                                              */
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
/* pop Object* from data stack                                                */
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
/* pop Object*, i8 from data stack                                            */
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
/* pop Object*, Object* from data stack                                       */
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
/* pop Object* from data stack                                                */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(Object*& pObject, Value& xValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_PTRVAL) )
    return false;
  
  PtrVal xPtrVal1;
  vPopVal(&xPtrVal1);
  
  pObject = xPtrVal1.pPtr();
  
  return bPop(xValue);
}

/*----------------------------------------------------------------------------*/
/* pop s from data stack                                                      */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(s32& sArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_STRVAL) )
    return false;
  
  ValWsp xValWsp1;
  vPopVal(&xValWsp1);
  
  sArg1 = _pStrVal(&xValWsp1)->aStr();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop s from data stack                                                      */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(s256& sArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_STRVAL) )
    return false;
  
  ValWsp xValWsp1;
  vPopVal(&xValWsp1);
  
  sArg1 = _pStrVal(&xValWsp1)->aStr();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop s from data stack                                                      */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(s48K& sArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_STRVAL) )
    return false;
  
  ValWsp xValWsp1;
  vPopVal(&xValWsp1);
  
  sArg1 = _pStrVal(&xValWsp1)->aStr();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop s,i8 from data stack                                                   */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(s48K& sArg1, i8& iArg2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_STRVAL,CN_INTVAL) )
    return false;
  
  ValWsp xValWsp1;
  vPopVal(&xValWsp1);
  
  sArg1 = _pStrVal(&xValWsp1)->aStr();
  
  IntVal xIntVal2;
  vPopVal(&xIntVal2);
  
  iArg2 = xIntVal2.iInt();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop s,i8,i8 from data stack                                                */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(s48K& sArg1, i8& iArg2, i8& iArg3)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_STRVAL,CN_INTVAL,CN_INTVAL) )
    return false;
  
  ValWsp xValWsp1;
  vPopVal(&xValWsp1);
  sArg1 = _pStrVal(&xValWsp1)->aStr();
  
  IntVal xIntVal2;
  vPopVal(&xIntVal2);
  iArg2 = xIntVal2.iInt();
  

  IntVal xIntVal3;
  vPopVal(&xIntVal3);
  iArg3 = xIntVal3.iInt();
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop s,s from data stack                                                    */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(s48K& sArg1, s48K& sArg2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_STRVAL,CN_STRVAL) )
    return false;
  
  ValWsp xValWsp1;
  vPopVal(&xValWsp1);
  
  sArg1 = _pStrVal(&xValWsp1)->aStr();
  
  ValWsp xValWsp2;
  vPopVal(&xValWsp2);
  
  sArg2 = _pStrVal(&xValWsp2)->aStr();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop value from data stack                                                  */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(Value* pArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(1) )
    return false;
  
  vPopVal(pArg1);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop value from data stack                                                  */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(Value& xArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(1) )
    return false;
  
  vPopVal(&xArg1);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop value, value from data stack                                           */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(Value& pValue1, Value& pValue2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(2) )
    return false;
  
  vPopVal(&pValue1);
  
  vPopVal(&pValue2);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop value, value, value from data stack                                    */
/*----------------------------------------------------------------------------*/
bool Stack::bPop(Value& pValue1, Value& pValue2, Value& pValue3)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(3) )
    return false;
  
  vPopVal(&pValue1);
  
  vPopVal(&pValue2);
  
  vPopVal(&pValue3);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop value from data stack  as integer                                      */
/*----------------------------------------------------------------------------*/
bool Stack::bPopParamInt(i8& iArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(1) )
    return false;
  
  ValWsp xWspVal1;
  vPopVal(&xWspVal1);
  
  iArg1 = xWspVal1.iAsInt();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop integer from stack                                                     */
/*----------------------------------------------------------------------------*/
i8 Stack::iPop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  i8 iArg = _pIntVal(m_pTop)->iInt();
  
  vPopBy(sizeof(IntVal));

  m_iValues--;
  
  return iArg;
}

/*----------------------------------------------------------------------------*/
/* pop binary from data stack                                                 */
/*----------------------------------------------------------------------------*/
bool Stack::bPopBinary(Object*& pObject)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if ( ! bGuard(CN_BINVAL) )
    return false;

  ValWsp xBinVal;
  vPopVal(&xBinVal);
  
  pObject = _pBinVal(&xBinVal)->pNewObject();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* push binary Object onto data stack                                         */
/*----------------------------------------------------------------------------*/
void Stack::vPushBinary(Object* pObject)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  ValWsp xBinVal;
  xBinVal.vAssignBinary(pObject);

  vPush(xBinVal);
  
}

/*----------------------------------------------------------------------------*/
/* push Object* onto data stack                                               */
/*----------------------------------------------------------------------------*/
void Stack::vPush(Object* pObject)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  PtrVal xPtrObject;
  
  xPtrObject = pObject;
  
  vPush(xPtrObject);
}

/*----------------------------------------------------------------------------*/
/* push Cell* onto data stack                                                 */
/*----------------------------------------------------------------------------*/
void Stack::vPush(Cell* pCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  IpVal xIpVal;
  
  xIpVal = pCell;
  
  vPush(xIpVal);
}

/*----------------------------------------------------------------------------*/
/* push Header* onto data stack                                               */
/*----------------------------------------------------------------------------*/
/*
void Stack::vPush(Header* pHeader1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  PtrVal xPtrHeader;
  xPtrHeader = pHeader1;
  
  vPush(&xPtrHeader);
}
*/
/*----------------------------------------------------------------------------*/
/* push i8 onto data stack                                                    */
/*----------------------------------------------------------------------------*/
void Stack::vPush(i8 iArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  IntVal xIntVal1;
  xIntVal1 = iArg1;
  
  vPush(&xIntVal1);
}

/*----------------------------------------------------------------------------*/
/* push s32 onto data stack                                                   */
/*----------------------------------------------------------------------------*/
void Stack::vPush(s32& sArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  ValWsp xStrVal1;
  xStrVal1 = sArg1.aStr();
  
  vPush(xStrVal1);
}

/*----------------------------------------------------------------------------*/
/* push s256 onto data stack                                                  */
/*----------------------------------------------------------------------------*/
void Stack::vPush(s256& sArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  ValWsp xStrVal1;
  xStrVal1 = sArg1.aStr();
  
  vPush(xStrVal1);
}

/*----------------------------------------------------------------------------*/
/* push s48K onto data stack                                                  */
/*----------------------------------------------------------------------------*/
void Stack::vPush(s48K& sArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  ValWsp xStrVal1;
  xStrVal1 = sArg1.aStr();
  
  vPush(xStrVal1);
}

/*----------------------------------------------------------------------------*/
/* push value onto data stack                                                 */
/*----------------------------------------------------------------------------*/
void Stack::vPush(Value& xVal1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  vPushVal(&xVal1);
}

/*----------------------------------------------------------------------------*/
/* push value onto data stack                                                 */
/*----------------------------------------------------------------------------*/
void Stack::vPush(Value* pValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  vPushVal(pValue);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* discard value on top of stack                                              */
/*----------------------------------------------------------------------------*/
void Stack::vDiscard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  //m_xHazard.vAssertFooter();
  _hazard_();

  Value* pValue = reinterpret_cast<Value*>(m_pTop);
  i8 iSize = ValLib::iSize(pValue);
  
  vPopBy(iSize);
  m_iValues--;
  
  //m_xHazard.vAssertFooter();
  _hazard_();
}

/*----------------------------------------------------------------------------*/
/* stk -- dump the data stack                                                 */
/*----------------------------------------------------------------------------*/
void Stack::vContentDump(Str* aPrefix, Str* aTitle)
{
  vTrace("[%s]%s:\n"_Str,aPrefix,aTitle);
  
  ValWsp xValWsp;
  
  s48K sWorkspace;
  
  i8 nValues = iValues();
  for (i8 iVal = 0; iVal < nValues; iVal++) {
    
    vPeek(iVal,&xValWsp);
    
    ValLib::vToString(&xValWsp,sWorkspace.aStr());
    char* aClass = xValWsp.aClass();
    
    eCN eClass = xValWsp.eClass();
    
    if (CN_PTRVAL == eClass) {
      PtrVal* pPtrVal = _pPtrVal(&xValWsp);
      Object* pObject = pPtrVal->pPtr();
      eCN ePtdClass   = pObject->eClass();
      vTrace("[%s] [%2lld] %s (%s %s)\n"_Str,
        aPrefix,iVal,sWorkspace.aStr(),aClass,aCN[ePtdClass]);
    }
    else if (CN_IPVAL == eClass) {                                       //RAY??
      IpVal* pIpVal = _pIpVal(&xValWsp);
      Cell* pCell = pIpVal->pIpPtr();
      vTrace("[%s] [%2lld] nullptr\n"_Str,aPrefix,iVal);  //RAY: should trace pIpVal
//    vTrace("[%s] [%2lld] %p\n"_Str,aPrefix,iVal,pIpVal);
    }
    else {
      vTrace("[%s] [%2lld] %s (%s)\n"_Str,
             aPrefix,iVal,sWorkspace.aStr(),aClass);
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* peek at value on top of stack                                              */
/*----------------------------------------------------------------------------*/
Value* Stack::pPeek(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  return pTopValue();
}

/*----------------------------------------------------------------------------*/
/* peek at given value on stack                                               */
/*----------------------------------------------------------------------------*/
Value* Stack::pPeek(i8 iNthValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  return pGetValue(iNthValue);
}

/*----------------------------------------------------------------------------*/
/* peek at given value on stack                                               */
/*----------------------------------------------------------------------------*/
void Stack::vPeek(i8 iNthValue, Value* pValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  Value* pPeekedValue = pPeek(iNthValue);

  i8 iSize = ValLib::iSize(pPeekedValue);
  
  //vTrace("Peek: %lld %p %lld\n"_Str,iNthValue,pPeekedValue,iSize);
  
  MemLib::vCopy(pValue,pPeekedValue,iSize);
}

/*----------------------------------------------------------------------------*/
/* poke at given value on stack                                               */
/*----------------------------------------------------------------------------*/
void Stack::vPoke(i8 iNthValue, Value* pValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);

  i8 iSize = ValLib::iSize(pValue);
  
  Value* pPokeAddress = pPeek(iNthValue);
  
  MemLib::vCopy(pPokeAddress,pValue,iSize);
}

/*----------------------------------------------------------------------------*/
/* reset stack                                                                */
/*----------------------------------------------------------------------------*/
void Stack::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  m_pTop    = pStackBase();
  m_iValues = 0;
  
  m_pRetryTop    = pStackBase();
  m_iRetryValues = 0;
}

/*----------------------------------------------------------------------------*/
/* set stack back to retry point                                              */
/*----------------------------------------------------------------------------*/
void Stack::vRetry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);

  m_pTop    = m_pRetryTop;
  m_iValues = m_iRetryValues;
}

/*----------------------------------------------------------------------------*/
/* mark retry point                                                           */
/*----------------------------------------------------------------------------*/
void Stack::vRetryPoint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  m_pRetryTop    = m_pTop;
  m_iRetryValues = m_iValues;
}

/*----------------------------------------------------------------------------*/
/* get number of values on stack                                              */
/*----------------------------------------------------------------------------*/
i8 Stack::iValues(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  return m_iValues;
}

/*----------------------------------------------------------------------------*/
/* is top value a zero?                                                       */
/*----------------------------------------------------------------------------*/
bool Stack::bPeekZero(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);

  Value* pValue = pTopValue();
  
  if (CN_INTVAL == pValue->eClass() && 0 == _pIntVal(pValue)->iInt())
    return true;
  
  return false;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* get address of given value on stack                                        */
/*----------------------------------------------------------------------------*/
Value* Stack::pGetValue(i8 iNthValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  Value* pValue = pTopValue();
  while (iNthValue--) {
    char* aAddress = reinterpret_cast<char*>(pValue);
    i8 iSize = ValLib::iSize(pValue);
    aAddress += iSize;
    pValue = reinterpret_cast<Value*>(aAddress);
  }
  
  return pValue;
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
/* pop top by given bytes                                                     */
/*----------------------------------------------------------------------------*/
void Stack::vPopBy(i8 iBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);

  char* pTop = reinterpret_cast<char*>(m_pTop);
  
  pTop += iBytes;
  
  m_pTop = reinterpret_cast<Value*>(pTop);
}

/*----------------------------------------------------------------------------*/
/* pop value from stack                                                       */
/*----------------------------------------------------------------------------*/
void Stack::vPopVal(Value* pValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  //m_xHazard.vAssertFooter();
  _hazard_();
  
  i8 iSize = ValLib::iSize(pTopValue());
  
  MemLib::vCopy(pValue,m_pTop,iSize);
  vPopBy(iSize);
  m_iValues--;
  
  //m_xHazard.vAssertFooter();
  _hazard_();
}

/*----------------------------------------------------------------------------*/
/* push value onto stack                                                      */
/*----------------------------------------------------------------------------*/
void Stack::vPushVal(Value* pValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  //m_xHazard.vAssertFooter();
  _hazard_();
  
  i8 iSize = ValLib::iSize(pValue);
  
  vPushBy(iSize);
  MemLib::vCopy(m_pTop,pValue,iSize);
  m_iValues++;
  
  //m_xHazard.vAssertFooter();
  _hazard_();
}

/*----------------------------------------------------------------------------*/
/* base of this stack                                                         */
/*----------------------------------------------------------------------------*/
Value* Stack::pStackBase(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  char* pBase = &m_xWsp.sArea()[C_STACK_LENGTH-1];
  
  return pPoolCheck<Value*>(pBase);
}

/*----------------------------------------------------------------------------*/
/* limit of this stack                                                        */
/*----------------------------------------------------------------------------*/
Value* Stack::pStackLimit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  char* pLimit = &m_xWsp.sArea()[0];
  
  return pPoolCheck<Value*>(pLimit);
}

/*----------------------------------------------------------------------------*/
/* get address of value on top of stack                                       */
/*----------------------------------------------------------------------------*/
Value* Stack::pTopValue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  Value* pValue = reinterpret_cast<Value*>(m_pTop);
  
  return pValue;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Stack::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/* get location of hazard                                                     */
/*----------------------------------------------------------------------------*/
Footer* Stack::pHazard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK);
  
  return &m_xHazard;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
