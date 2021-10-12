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
  
    eLS        m_eSlot;
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
  
    eLS eSlot(void);
    void vSlot(eLS eSlot);
  
    CodeSynth* pStructSynth(void);
    void vStructSynth(CodeSynth* pStructSynth);

    CodeSynth* pCodeSynthA(void);
    void vCodeSynthA(CodeSynth* pCodeSynthA);

    CodeSynth* pCodeSynthB(void);
    void vCodeSynthB(CodeSynth* pCodeSynthB);

    CodeSynth* pCodeSynthC(void);
    void vCodeSynthC(CodeSynth* pCodeSynthC);

  private:
  
    Footer* aFooter(void);
  };
  
  Slot* _pSlot(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
