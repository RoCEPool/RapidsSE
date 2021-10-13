/*============================================================================*/
/* Stencil.hpp -- Op Stencil                                                  */
/*============================================================================*/
#pragma once

#include "eLp.h"
#include "eLS.h"
#include "CodeSynth.hpp"
#include "Slot.hpp"

namespace STORE
{
static const i8 C_MAX_STENCIL_SLOTS = 32;

  class Stencil : public VectorNL_t<CN_STENCIL,Slot,C_MAX_STENCIL_SLOTS>
  {
  private:
  
    eLP        m_ePattern;
  
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

    void vCombine(Op* pOp);
  
    char* azOpSource(void);

    eLP ePattern(void);
    void vPattern(eLP ePattern);

  private:
  
    void vEtch(void);

    Footer* aFooter(void);
  };
  
  Stencil* _pStencil(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
