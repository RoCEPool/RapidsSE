/*============================================================================*/
/* CodeSynth.hpp -- Code Synthesizer                                          */
/*============================================================================*/
#pragma once

#include "TinyC.hpp"

namespace STORE
{
  static const i8 C_FLATTEN_SOURCE  = C_SIZE_LARGE_OBJECT - C_1KB;


  //----------------------------------------------------------------------------

  class CodeSynth : public Object
  {
  private:
  
    char        m_caCode[C_FLATTEN_SOURCE];
    char*       m_azCodeCrs;
    TypedStack* m_pTypedStack;
    TokenVec*   m_pInFixTknVec;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    CodeSynth(void);
   ~CodeSynth(void) noexcept(false);

    void  vAppendFragment(Stz* szFragment);
    void  vAppendFragment(CodeSynth* pFragment);
    void  vPrint(void);
    char* azCode(void);
    void  vSynthFilter(Op* pOp);
    void  vSynthStruct(Op* pOp);
    void  vReset(void);

  private:
    char* szToInfix(char* sExpr, eSR eStencilRow);

    Footer* aFooter(void);
  };
  
  CodeSynth* _pCodeSynth(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
