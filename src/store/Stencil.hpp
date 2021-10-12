/*============================================================================*/
/* Stencil.hpp -- Op Stencil                                                  */
/*============================================================================*/
#pragma once

#include "eLS.h"
#include "CodeSynth.hpp"

namespace STORE
{
static const i8 C_MAX_STENCIL_SLOTS = 32;

  class Stencil : public Object
  {
  private:
  
    struct {
      eLS        m_eSection;
      CodeSynth* m_pStructSynth;
      CodeSynth* m_pCodeSynthA;
      CodeSynth* m_pCodeSynthB;
      CodeSynth* m_pCodeSynthC;
      
    } m_xaPart[C_MAX_STENCIL_SLOTS];
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Stencil(void);
   ~Stencil(void) noexcept(false);

  private:
  
    Footer* aFooter(void);
  };
  
  Stencil* _pStencil(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
