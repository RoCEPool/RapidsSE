/*============================================================================*/
/* Flatten.hpp -- Code flattening                                             */
/*============================================================================*/
#pragma once

#include "eAM.h"
#include "eEO.h"

#include "CodeSynth.hpp"
#include "TinyC.hpp"
#include "SE_interface.h"

namespace STORE
{

  struct SESymbol
  {
    Stz* m_szSymbolNm;
    const void *m_afSymbol;
  };

//----------------------------------------------------------------------------

  class Flatten : public Object
  {
  private:

    CodeSynth* m_pCodeSynth;
    TinyC*     m_pTinyC;

    bool (*m_afFlatten)(void*);

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Flatten(void);
   ~Flatten(void) noexcept(false);

    void vTestFlatten(CrwBuffer* pCrwBuffer);
    bool bCallFlatten(void* pCrwBuffer);
    CodeSynth* pCodeSynth(void);

  private:

    bool bCompileCode(CodeSynth* pCodeSynth);

    Footer* aFooter(void);
  };

  Flatten* _pFlatten(void* aAddr);
}

  void vTccError(void *aOpaque, const char *azMsg);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
