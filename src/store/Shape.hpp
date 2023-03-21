/*============================================================================*/
/* Shape.hpp -- Stencil Shape                                                 */
/*============================================================================*/
#pragma once

#include "eSK.h"
#include "CodeSynth.hpp"

namespace STORE
{
  // shape reference information
  struct ShapeRef
  {
    Stz* m_szInclude;
  };

  // stencil row reference information
  struct RowRef
  {
    Stz* m_szInclude;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  class Shape : public Object
  {
  private:

    CodeSynth* m_pStructSynthA;
    CodeSynth* m_pStructSynthB;
    CodeSynth* m_pCodeSynthA;
    CodeSynth* m_pCodeSynthB;
    CodeSynth* m_pCodeSynthC;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Shape(void);
   ~Shape(void) noexcept(false);
  
    CodeSynth* pStructSynthA(void);
    void vStructSynthA(CodeSynth* pStructSynth);

    CodeSynth* pStructSynthB(void);
    void vStructSynthB(CodeSynth* pStructSynth);

    CodeSynth* pCodeSynthA(void);
    void vCodeSynthA(CodeSynth* pCodeSynthA);

    CodeSynth* pCodeSynthB(void);
    void vCodeSynthB(CodeSynth* pCodeSynthB);

    CodeSynth* pCodeSynthC(void);
    void vCodeSynthC(CodeSynth* pCodeSynthC);

  private:
  
    Footer* aFooter(void);
  };
  
  Shape* _pShape(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
