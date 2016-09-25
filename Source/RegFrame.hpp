/*============================================================================*/
/* RegFrame.hpp -- Register Frame                                             */
/*============================================================================*/

#pragma once

#include "Tasks.h"

namespace Tasks
{
  static const i8 C_VALREGS = 16;
  
  class RegFrame : public Object
  {
  public:
    RegFrame* m_pNext;
    ValReg         m_xValReg[C_VALREGS];
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    RegFrame(void);
    ~RegFrame(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    RegFrame* pNext(void);
    void      vNext(RegFrame* pRegFrame);
    
    Footer* pFooter(void);
  };
  
  RegFrame* _pRegFrame(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/