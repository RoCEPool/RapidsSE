/*============================================================================*/
/* RegFrame.cpp -- Register Frame                                             */
/*============================================================================*/

#include "Tasks.h"

extern Heap* gpMediumHeap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RegFrame* Tasks::_pRegFrame(void* pAddr)
{
  RegFrame* pRegFrame = reinterpret_cast<RegFrame*>(pAddr);
  
  RASSERT( !pRegFrame || CN_REGFRAME == pRegFrame->eClass() );

  return pRegFrame;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RegFrame::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pRegFrame = Heap::pAllocate(CN_REGFRAME);
  
  return pRegFrame;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void RegFrame::operator delete(void* pRegFrameAddress)
{
  RegFrame* pRegFrame = _pRegFrame(pRegFrameAddress);
  
  Heap::vFree(pRegFrame);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RegFrame::RegFrame(void)
: Buffer(CN_REGFRAME)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGFRAME,_FOOTER_);

  m_pNext     = nullptr;
  m_pStackPtr = nullptr;
  //m_xValReg
  
  _checkInitialised_(CN_REGFRAME);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_REGFRAME,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RegFrame::~RegFrame(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGFRAME,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump register frame                                                        */
/*----------------------------------------------------------------------------*/
void RegFrame::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_REGFRAME);
  ASSERT(pObject->eClass() == CN_REGFRAME);
  
  RegFrame* pRegFrame = _pRegFrame(pObject);
  
  vTrace("%sRegFrame %p:\n\n", pTab, pRegFrame);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get next register frame                                                    */
/*----------------------------------------------------------------------------*/
RegFrame* RegFrame::pNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGFRAME,_FOOTER_);
  
  return m_pNext;
}

/*----------------------------------------------------------------------------*/
/* set next register frame                                                    */
/*----------------------------------------------------------------------------*/
void RegFrame::vNext(RegFrame* pNext)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGFRAME,_FOOTER_);
  
  m_pNext = pNext;
}

/*----------------------------------------------------------------------------*/
/* get stack pointer                                                          */
/*----------------------------------------------------------------------------*/
void* RegFrame::pStackPtr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGFRAME,_FOOTER_);
  
  return m_pStackPtr;
}

/*----------------------------------------------------------------------------*/
/* set stack pointer                                                          */
/*----------------------------------------------------------------------------*/
void RegFrame::vStackPtr(void* pStackPtr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGFRAME,_FOOTER_);
  
  m_pStackPtr = pStackPtr;
}

/*----------------------------------------------------------------------------*/
/* get address of value register                                              */
/*----------------------------------------------------------------------------*/
ValReg* RegFrame::pValReg(i8 iReg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGFRAME,_FOOTER_);
  
  ASSERT(iReg >= 0 && iReg < C_VALREGS);
  
  return &m_xValReg[iReg];
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* RegFrame::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGFRAME);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
