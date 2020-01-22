/*============================================================================*/
/* MemTest.hpp -- Memory pattern test                                         */
/*============================================================================*/

#include "Gadgets.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
MemTest* Gadgets::_pMemTest(void* aAddr)
{
  MemTest* pMemTest = reinterpret_cast<MemTest*>(aAddr);
  
  RASSERT_TYPE(pMemTest,CN_MEMTEST);
  
  return pMemTest;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* MemTest::operator new(size_t)
{
  void* pMutex = HeapMem::pAllocate(CN_MEMTEST);
  
  return pMutex;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void MemTest::operator delete(void* aAddr)
{
  Object* pObject = reinterpret_cast<Object*>(aAddr);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MemTest::MemTest(void)
: Object(CN_MEMTEST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MEMTEST,_FOOTER_);
  
  m_iInt = 73;
  m_iIntFlipped = IntLib::iFlipEndian(m_iInt);
  m_iNull = C_DATUM_NULL;
  m_iNullFlipped = IntLib::iFlipEndian(m_iNull);
  StrLib::vCopy(m_caCharA,"AbCdEfG");
  StrLib::vCopy(m_caCharB,"aBcDeFg");
  StrLib::vCopy(m_caCharC,"aBc");

  StrLib::vCopy(m_caCharUpper,"AbCdEfGhIjKlMnO");
  StrLib::iWordFlip(m_caCharUpperFlipped, m_caCharUpper);
  
  StrLib::vCopy(m_caCharLower,"aBcDeFgHiJkLmNo");
  StrLib::iWordFlip(m_caCharLowerFlipped, m_caCharLower);
 
  _checkInitialised_(CN_MEMTEST);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_MEMTEST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MemTest::~MemTest(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MEMTEST,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* MemTest::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MEMTEST);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
