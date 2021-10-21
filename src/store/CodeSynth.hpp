/*============================================================================*/
/* CodeSynth.hpp -- Code Synthesizer                                          */
/*============================================================================*/
#pragma once

#include "TinyC.hpp"
#include "Tub.hpp"
#include "eAG.h"
#include "ePJ.h"

namespace STORE
{
  static const i8 C_FLATTEN_SOURCE  = C_SIZE_LARGE_OBJECT - C_1KB;

  //----------------------------------------------------------------------------

  class CodeSynth : public Object
  {
  private:
  
    char        m_caCode[C_FLATTEN_SOURCE];
    char*       m_azCodeCrs;

    TypedStack* m_pExprStack;
    TypedStack* m_pPostExpr;

    IntStack* m_pInitStack;

    TokenVec*   m_pInFixTknVec;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    CodeSynth(void);
   ~CodeSynth(void) noexcept(false);

    void  vAppendFragment(Stz* szFragment, i8* piLineNumber);
    void  vAppendFragment(CodeSynth* pFragment, i8* piLineNumber);
    void  vPrint(void);
    char* azCode(void);
    void  vSynthFilter(Stz* sFilterExpr, eSR eStencilRow);
    void  vSynthInflate(StreamDef* pStreamDefOut);
    void  vSynthProject(StreamDef* pStreamDefOut, eSR eStencilFrom, eSR eStencilTo,
                        IntStack* pAggTypeStack, ePJ eProjectKind, Stz* sProjectExpr);
    void  vSynthStruct(StreamDef* pStreamDefOut, eSR eStencilRow);
    void  vReset(void);

  private:
    void vToInfix(char* sExpr, eSR eStencilRow, IntStack* pAggTypeStack, ePJ eProjectKind);

    Footer* aFooter(void);
  };
  
  CodeSynth* _pCodeSynth(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
