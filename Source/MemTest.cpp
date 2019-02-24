/*============================================================================*/
/* MemTest.hpp -- Memory pattern test                                         */
/*============================================================================*/

#include "System.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
MemTest* System::_pMemTest(void* aAddr)
{
  MemTest* pMutex = reinterpret_cast<MemTest*>(aAddr);
  
  RASSERT_TYPE(pMutex,CN_MEMTEST);
  
  return pMutex;
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
  m_iNull = C_NULL_INT;
  m_iNullFlipped = IntLib::iFlipEndian(m_iNull);
  StrLib::vCopy(m_caCharA,"AbCdEfG");
  StrLib::vCopy(m_caCharB,"aBcDeFg");
  StrLib::vCopy(m_caCharC,"aBc");

  StrLib::vCopy(m_caCharUpper16,"AbCdEfGhIjKlMnO");
  m_iCharUpperFlipA = 0x4162436445664768;
  m_iCharUpperFlipB = 0x496a4b6c4d6e4f00;
  
  StrLib::vCopy(m_caCharLower16,"aBcDeFgHiJkLmNo");
  m_iCharLowerFlipA = 0x6142634465466748;
  m_iCharLowerFlipB = 0x694a6b4c6d4e6f00;

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
