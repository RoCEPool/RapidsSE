/*============================================================================*/
/* Stack.cpp - push/pop stack                                                 */
/*============================================================================*/

#include "Tasks.h"

extern BufferPool* gpBufferPool;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Stack::operator new(size_t)
{
  vFatal("fatal: Stack class cannot be new'd");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Stack::operator delete(void* pBufferAddress)
{
  vFatal("fatal: Stack class cannot be deleted");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Stack::Stack(void)
: Object(CN_STACK)
{
  _poolAddress_(this);
  //_magic_();
  
  m_pTop    = pStackBase();
  m_iValues = 0;
  
  m_pRetryTop    = pStackBase();
  m_iRetryValues = 0;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Stack::~Stack(void)
{
  _poolAddress_(this);
  //_magic_();
}

/*----------------------------------------------------------------------------*/
/* discard value on top of stack                                              */
/*----------------------------------------------------------------------------*/
void Stack::vDiscard(void)
{
  _poolAddress_(this);
  //_magic_();

  OnStack* pValue = reinterpret_cast<OnStack*>(m_pTop);
  i8 iSize = pValue->iSize();
  
  vPopBy(iSize);
  m_iValues--;
}

/*----------------------------------------------------------------------------*/
/* peek at value on top of stack                                              */
/*----------------------------------------------------------------------------*/
OnStack* Stack::pPeek(void) const
{
  _poolAddress_(this);
  //_magic_();
  
  return pTopValue();
}

/*----------------------------------------------------------------------------*/
/* peek at given value on stack                                               */
/*----------------------------------------------------------------------------*/
OnStack* Stack::pPeek(i8 iNthValue) const
{
  _poolAddress_(this);
  //_magic_();
  
  return pGetValue(iNthValue);
}

/*----------------------------------------------------------------------------*/
/* peek at given value on stack                                               */
/*----------------------------------------------------------------------------*/
void Stack::vPeek(i8 iNthValue, OnStack* pValue)
{
  _poolAddress_(this);
  //_magic_();
  
  OnStack* pPeekedValue = pPeek(iNthValue);

  i8 iSize = pPeekedValue->iSize();
  
  //vTrace("Peek: %lld %p %lld\n",iNthValue,pPeekedValue,iSize);
  
  Memory::vCopy(pValue,pPeekedValue,iSize);
}

/*----------------------------------------------------------------------------*/
/* poke at given value on stack                                               */
/*----------------------------------------------------------------------------*/
void Stack::vPoke(i8 iNthValue, OnStack* pValue)
{
  _poolAddress_(this);
  //_magic_();

  i8 iSize = pValue->iSize();
  
  OnStack* pPokeAddress = pPeek(iNthValue);
  
  Memory::vCopy(pPokeAddress,pValue,iSize);
}

/*----------------------------------------------------------------------------*/
/* pop value from stack                                                       */
/*----------------------------------------------------------------------------*/
void Stack::vPop(OnStack* pValue)
{
  _poolAddress_(this);
  //_magic_();
  
  i8 iSize = pTopValue()->iSize();

  Memory::vCopy(pValue,m_pTop,iSize);
  vPopBy(iSize);
  m_iValues--;
}

/*----------------------------------------------------------------------------*/
/* push value onto stack                                                      */
/*----------------------------------------------------------------------------*/
void Stack::vPush(OnStack* pValue)
{
  _poolAddress_(this);
  //_magic_();
  
  i8 iSize = pValue->iSize();
  
  vPushBy(iSize);
  Memory::vCopy(m_pTop,pValue,iSize);
  m_iValues++;
}

/*----------------------------------------------------------------------------*/
/* reset stack                                                                */
/*----------------------------------------------------------------------------*/
void Stack::vReset(void)
{
  _poolAddress_(this);
  //_magic_();
  
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
  _poolAddress_(this);
  //_magic_();

  m_pTop    = m_pRetryTop;
  m_iValues = m_iRetryValues;
}

/*----------------------------------------------------------------------------*/
/* mark retry point                                                           */
/*----------------------------------------------------------------------------*/
void Stack::vRetryPoint(void)
{
  _poolAddress_(this);
  //_magic_();
  
  m_pRetryTop    = m_pTop;
  m_iRetryValues = m_iValues;
}

/*----------------------------------------------------------------------------*/
/* get number of values on stack                                              */
/*----------------------------------------------------------------------------*/
i8 Stack::iValues(void) const
{
  _poolAddress_(this);
  //_magic_();
  
  return m_iValues;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* get address of given value on stack                                        */
/*----------------------------------------------------------------------------*/
OnStack* Stack::pGetValue(i8 iNthValue) const
{
  _poolAddress_(this);
  //_magic_();
  
  OnStack* pValue = pTopValue();
  while (iNthValue--) {
    char* pAddress = reinterpret_cast<char*>(pValue);
    i8 iSize = pValue->iSize();
    pAddress += iSize;
    pValue = reinterpret_cast<OnStack*>(pAddress);
  }
  
  return pValue;
}

/*----------------------------------------------------------------------------*/
/* push top by given bytes                                                    */
/*----------------------------------------------------------------------------*/
void Stack::vPushBy(i8 iBytes)
{
  _poolAddress_(this);
  //_magic_();

  char* pTop = reinterpret_cast<char*>(m_pTop);
  
  pTop -= iBytes;
  
  m_pTop = reinterpret_cast<OnStack*>(pTop);
}

/*----------------------------------------------------------------------------*/
/* pop top by given bytes                                                     */
/*----------------------------------------------------------------------------*/
void Stack::vPopBy(i8 iBytes)
{
  _poolAddress_(this);
  //_magic_();

  char* pTop = reinterpret_cast<char*>(m_pTop);
  
  pTop += iBytes;
  
  m_pTop = reinterpret_cast<OnStack*>(pTop);
}

/*----------------------------------------------------------------------------*/
/* base of this stack                                                         */
/*----------------------------------------------------------------------------*/
OnStack* Stack::pStackBase(void)
{
  _poolAddress_(this);
  //_magic_();
  
  char* pBase = &m_mSpace[C_STACK_AREA-1];
  
  return pPoolCheck<OnStack*>(pBase);
}

/*----------------------------------------------------------------------------*/
/* limit of this stack                                                        */
/*----------------------------------------------------------------------------*/
OnStack* Stack::pStackLimit(void) const
{
  _poolAddress_(this);
  //_magic_();
  
  char* pLimit = (char*)this + sizeof(Stack);
  
  return pPoolCheck<OnStack*>(pLimit);
}

/*----------------------------------------------------------------------------*/
/* get address of value on top of stack                                       */
/*----------------------------------------------------------------------------*/
OnStack* Stack::pTopValue(void) const
{
  _poolAddress_(this);
  //_magic_();
  
  OnStack* pValue = reinterpret_cast<OnStack*>(m_pTop);
  
  return pValue;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/