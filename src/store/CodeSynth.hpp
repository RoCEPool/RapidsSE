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
  
    char   m_caCode[C_FLATTEN_SOURCE];
    char*  m_azCodeCrs;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    CodeSynth(void);
   ~CodeSynth(void) noexcept(false);

    void  vAppendFragment(Stz* szFragment);
    void  vAppendFragment(CodeSynth* pFragment);
    char* azCode(void);
    void  vSynthStruct(StreamDef* pStreamDef);
    void  vReset(void);

  private:
  
    Footer* aFooter(void);
  };
  
  CodeSynth* _pCodeSynth(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
