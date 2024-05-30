/*============================================================================*/
/* CallFrame.h -- Stack call information for a single frame                   */
/*============================================================================*/
#pragma once

#include "Object.hpp"
#include "Correct.hpp"
#include "StrLib.hpp"

namespace BASE {

  class CallFrame : public Object
  {
  public:
    void* m_pvFramePtr;
    char  m_acModules[256];
    char  m_acFileNames[256];
    char  m_acFuncNames[2000];
    i8    m_iLines;
    i8    m_iOffsets;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    CallFrame& operator=(CallFrame& other);

    CallFrame();
    CallFrame(CallFrame& oOther);
    ~CallFrame() noexcept(false);

    void vReset(void);
    void vToString(String* psDst);

    Footer* aFooter(void);
  };

  CallFrame* _pCallFrame(void* aAddr);
}