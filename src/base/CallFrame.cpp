/*============================================================================*/
/* CallFrame.h -- Stack call information for a single frame                   */
/*============================================================================*/

#include "BASE.h"

#include "CallFrame.hpp"
#include "String.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,CallFrame,CN_CALLFRAME)
__aNewFunction(CallFrame,CN_CALLFRAME)
__vDeleteFunction(CallFrame,CN_CALLFRAME)
__aFooterFunction(CallFrame)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CallFrame::CallFrame()
    : Object(CN_CALLFRAME)
{
  _precondition_(_MAGIC_,CN_CALLFRAME,_FOOTER_);

  m_pvFramePtr = nullptr;
  m_acModules[0] = 0;
  m_acFileNames[0] = 0;
  m_acFuncNames[0] = 0;
  m_iLines = 0;
  m_iOffsets = 0;

  _postcondition_(_MAGIC_,CN_CALLFRAME,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* copy constructor                                                           */
/*----------------------------------------------------------------------------*/
CallFrame::CallFrame(CallFrame& oOther)
    : Object(CN_CALLFRAME)
{
  _precondition_(_MAGIC_,CN_CALLFRAME,_FOOTER_);

  m_pvFramePtr = oOther.m_pvFramePtr;
  StrLib::vCopy(m_acModules,   sizeof(m_acModules),   oOther.m_acModules);
  StrLib::vCopy(m_acFileNames, sizeof(m_acFileNames), oOther.m_acFileNames);
  StrLib::vCopy(m_acFuncNames, sizeof(m_acFuncNames), oOther.m_acFuncNames);
  m_iLines = oOther.m_iLines;
  m_iOffsets = oOther.m_iOffsets;

  _postcondition_(_MAGIC_,CN_CALLFRAME,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CallFrame::~CallFrame(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_CALLFRAME,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* copy assignment operator                                                   */
/*----------------------------------------------------------------------------*/
CallFrame& CallFrame::operator=(CallFrame& oOther)
{
  m_pvFramePtr = oOther.m_pvFramePtr;
  StrLib::vCopy(m_acModules,   sizeof(m_acModules),   oOther.m_acModules);
  StrLib::vCopy(m_acFileNames, sizeof(m_acFileNames), oOther.m_acFileNames);
  StrLib::vCopy(m_acFuncNames, sizeof(m_acFuncNames), oOther.m_acFuncNames);
  m_iLines = oOther.m_iLines;
  m_iOffsets = oOther.m_iOffsets;

  return *this;
}

/*----------------------------------------------------------------------------*/
/* Reset all info for this frame                                              */
/*----------------------------------------------------------------------------*/
void CallFrame::vReset()
{
  m_pvFramePtr = nullptr;
  m_acModules[0] = 0;
  m_acFileNames[0] = 0;
  m_acFuncNames[0] = 0;
  m_iLines = 0;
  m_iOffsets = 0;
}

/*----------------------------------------------------------------------------*/
/* Write the frame information to a string                                    */
/*----------------------------------------------------------------------------*/
void CallFrame::vToString(String* psDst)
{
  ASSERT(psDst);

  if (StrLib::nLength(m_acFileNames) > 0) {
    StrLib::vSprint(psDst->azStr(),
                    psDst->nAreaBytes(),
                    nullptr,
                    "%-26s  %018p  %s  (%s:%lld)"_Stz,
                    m_acModules,
                    m_pvFramePtr,
                    m_acFuncNames,
                    m_acFileNames,
                    m_iLines);
  } else {
    StrLib::vSprint(psDst->azStr(),
                    psDst->nAreaBytes(),
                    nullptr,
                    "%-26s  %018p  %s  (+%lld)"_Stz,
                    m_acModules,
                    m_pvFramePtr,
                    m_acFuncNames,
                    m_iOffsets);
  }
}