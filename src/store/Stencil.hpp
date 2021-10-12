/*============================================================================*/
/* Stencil.hpp -- Op Stencil                                                  */
/*============================================================================*/
#pragma once

#include "eLS.h"
#include "CodeSynth.hpp"
#include "Slot.hpp"

namespace STORE
{
static const i8 C_MAX_STENCIL_SLOTS = 32;

  class Stencil : public VectorNL_t<CN_STENCIL,Slot,C_MAX_STENCIL_SLOTS>
  {
  private:
  
    CodeSynth* m_pBannerHead;
    CodeSynth* m_pBannerTail;
  
    CodeSynth* m_pOpSource;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Stencil(void);
   ~Stencil(void) noexcept(false);

    char* azOpSource(void);

  private:
  
    Footer* aFooter(void);
  };
  
  Stencil* _pStencil(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
