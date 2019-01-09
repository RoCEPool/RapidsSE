/*============================================================================*/
/* RegFrame.cpp -- Register Frame                                             */
/*============================================================================*/

#include "Forth.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RegFrame* Forth::_pRegFrame(void* aAddr)
{
  RegFrame* pRegFrame = reinterpret_cast<RegFrame*>(aAddr);
  
  RASSERT_TYPE(pRegFrame,CN_REGFRAME);

  return pRegFrame;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RegFrame::operator new(size_t)
{
  void* pRegFrame = HeapMem::pAllocate(CN_REGFRAME);
  
  return pRegFrame;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void RegFrame::operator delete(void* pRegFrameAddress)
{
  RegFrame* pRegFrame = _pRegFrame(pRegFrameAddress);
  
  HeapMem::vFree(pRegFrame);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RegFrame::RegFrame(void)
: Object(CN_REGFRAME)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGFRAME,_FOOTER_);

  //m_sName
  m_pNext     = nullptr;
  m_pStackPtr = nullptr;
  m_pOpDic    = nullptr;
  //m_xValReg
  
  _checkInitialised_(CN_REGFRAME);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_REGFRAME,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RegFrame::~RegFrame(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGFRAME,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump register frame                                                        */
/*----------------------------------------------------------------------------*/
void RegFrame::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RegFrame* pRegFrame = _pRegFrame(pObject);
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get register frame name                                                    */
/*----------------------------------------------------------------------------*/
s32& RegFrame::sName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGFRAME,_FOOTER_);
  
  return m_sName;
}

/*----------------------------------------------------------------------------*/
/* set register frame name                                                    */
/*----------------------------------------------------------------------------*/
void RegFrame::vName(s32& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGFRAME,_FOOTER_);
  
  m_sName = sName;
}

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
/* get operator dictionary                                                    */
/*----------------------------------------------------------------------------*/
Dic* RegFrame::pOpDic(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGFRAME,_FOOTER_);
  
  return m_pOpDic;
}

/*----------------------------------------------------------------------------*/
/* set operator dictionary                                                    */
/*----------------------------------------------------------------------------*/
void RegFrame::vOpDic(Dic* pOpDic)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGFRAME,_FOOTER_);
  
  m_pOpDic = pOpDic;
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
Footer* RegFrame::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGFRAME);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
