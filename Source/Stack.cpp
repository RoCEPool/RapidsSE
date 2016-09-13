/*============================================================================*/
/* Stack.cpp -- Push/pop Stack                                                */
/*============================================================================*/

#include "Tasks.h"

extern BufferPool* gpBufferPool;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Stack::operator new(size_t iSize)
{
  ASSERT(iSize);
  vFatal("fatal: Stack class cannot be new'd");
  
  ASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Stack::operator delete(void* pBufferAddress)
{
  ASSERT(pBufferAddress);
  vFatal("fatal: Stack class cannot be deleted");
  
  ASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Stack::Stack(void)
: Object(CN_STACK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  m_pTop    = pStackBase();
  m_iValues = 0;
  
  m_pRetryTop    = pStackBase();
  m_iRetryValues = 0;
  
  Memory::vZero(m_mSpace,C_STACK_AREA);
  
  Memory::vCheckInitialised(this,sizeof(Stack));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  m_xHazard.vAssertFooter(); 
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Stack::~Stack(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* guard parameter stack                                                      */
/*----------------------------------------------------------------------------*/
bool Stack::bGuardParam(i8 iCount)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  if (iValues() < iCount)
    return false;
  
  return true;
}



/*----------------------------------------------------------------------------*/
/* guard parameter stack 1                                                    */
/*----------------------------------------------------------------------------*/
bool Stack::bGuardParam(eCN eClass0)
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
/* guard parameter stack 2                                                    */
/*----------------------------------------------------------------------------*/
bool Stack::bGuardParam(eCN eClass0, eCN eClass1)
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
/* guard parameter stack 3                                                    */
/*----------------------------------------------------------------------------*/
bool Stack::bGuardParam(eCN eClass0, eCN eClass1, eCN eClass2)
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
/* push void* onto param stack                                                */
/*----------------------------------------------------------------------------*/
void Stack::vPushParam(void* pVoid)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  PtrVal xPtrVoid;
  
  xPtrVoid = pVoid;
  
  vPushParam(xPtrVoid);
}

/*----------------------------------------------------------------------------*/
/* push Header* onto param stack                                              */
/*----------------------------------------------------------------------------*/
void Stack::vPushParam(Header* pHeader1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  PtrVal xPtrHeader;
  xPtrHeader = pHeader1;
  
  vPushParam(&xPtrHeader);
}

/*----------------------------------------------------------------------------*/
/* push Object* onto param stack                                              */
/*----------------------------------------------------------------------------*/
void Stack::vPushParam(Object* pObject1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  PtrVal xPtrObject;
  xPtrObject = pObject1;
  
  vPushParam(&xPtrObject);
}

/*----------------------------------------------------------------------------*/
/* push i8 onto param stack                                                   */
/*----------------------------------------------------------------------------*/
void Stack::vPushParam(i8 iArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  IntVal xIntVal1;
  xIntVal1 = iArg1;
  
  vPushParam(&xIntVal1);
}

/*----------------------------------------------------------------------------*/
/* push s32 onto param stack                                                  */
/*----------------------------------------------------------------------------*/
void Stack::vPushParam(s32& sArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  ValWsp xStrVal1;
  xStrVal1 = sArg1.a();
  
  vPushParam(xStrVal1);
}

/*----------------------------------------------------------------------------*/
/* push s256 onto param stack                                                 */
/*----------------------------------------------------------------------------*/
void Stack::vPushParam(s256& sArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  ValWsp xStrVal1;
  xStrVal1 = sArg1.a();
  
  vPushParam(xStrVal1);
}

/*----------------------------------------------------------------------------*/
/* push s8K onto param stack                                                 */
/*----------------------------------------------------------------------------*/
void Stack::vPushParam(s8K& sArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  ValWsp xStrVal1;
  xStrVal1 = sArg1.a();
  
  vPushParam(xStrVal1);
}

/*----------------------------------------------------------------------------*/
/* push value onto parameter stack                                            */
/*----------------------------------------------------------------------------*/
void Stack::vPushParam(Value& xVal1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  vPush(&xVal1);
}

/*----------------------------------------------------------------------------*/
/* push value onto parameter stack                                            */
/*----------------------------------------------------------------------------*/
void Stack::vPushParam(Value* pValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  vPush(pValue);
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
  m_xHazard.vAssertFooter();

  Value* pValue = reinterpret_cast<Value*>(m_pTop);
  i8 iSize = pValue->iSize();
  
  vPopBy(iSize);
  m_iValues--;
  
  m_xHazard.vAssertFooter();
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

  i8 iSize = pPeekedValue->iSize();
  
  //vTrace("Peek: %lld %p %lld\n",iNthValue,pPeekedValue,iSize);
  
  Memory::vCopy(pValue,pPeekedValue,iSize);
}

/*----------------------------------------------------------------------------*/
/* poke at given value on stack                                               */
/*----------------------------------------------------------------------------*/
void Stack::vPoke(i8 iNthValue, Value* pValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);

  i8 iSize = pValue->iSize();
  
  Value* pPokeAddress = pPeek(iNthValue);
  
  Memory::vCopy(pPokeAddress,pValue,iSize);
}

/*----------------------------------------------------------------------------*/
/* pop value from stack                                                       */
/*----------------------------------------------------------------------------*/
void Stack::vPop(Value* pValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  m_xHazard.vAssertFooter();
  
  i8 iSize = pTopValue()->iSize();

  Memory::vCopy(pValue,m_pTop,iSize);
  vPopBy(iSize);
  m_iValues--;

  m_xHazard.vAssertFooter();
}

/*----------------------------------------------------------------------------*/
/* push value onto stack                                                      */
/*----------------------------------------------------------------------------*/
void Stack::vPush(Value* pValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  m_xHazard.vAssertFooter();
  
  i8 iSize = pValue->iSize();
  
  vPushBy(iSize);
  Memory::vCopy(m_pTop,pValue,iSize);
  m_iValues++;

  m_xHazard.vAssertFooter();
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
  
  if (CN_INTVAL == pValue->eClass() && 0 == pValue->iInt())
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
    char* pAddress = reinterpret_cast<char*>(pValue);
    i8 iSize = pValue->iSize();
    pAddress += iSize;
    pValue = reinterpret_cast<Value*>(pAddress);
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
/* base of this stack                                                         */
/*----------------------------------------------------------------------------*/
Value* Stack::pStackBase(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  char* pBase = &m_mSpace[C_STACK_AREA-1];
  
  return pPoolCheck<Value*>(pBase);
}

/*----------------------------------------------------------------------------*/
/* limit of this stack                                                        */
/*----------------------------------------------------------------------------*/
Value* Stack::pStackLimit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STACK,_FOOTER_);
  
  char* pLimit = &m_mSpace[0];
  
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