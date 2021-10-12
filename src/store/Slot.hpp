/*============================================================================*/
/* Slot.hpp -- Stencil Slot                                                   */
/*============================================================================*/
#pragma once

#include "eLS.h"
#include "CodeSynth.hpp"

namespace STORE
{
  class Slot : public Object
  {
  private:
  
    eLS        m_eSection;
    CodeSynth* m_pStructSynth;
    CodeSynth* m_pCodeSynthA;
    CodeSynth* m_pCodeSynthB;
    CodeSynth* m_pCodeSynthC;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Slot(void);
   ~Slot(void) noexcept(false);

  private:
  
    Footer* aFooter(void);
  };
  
  Slot* _pSlot(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
