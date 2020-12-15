/*============================================================================*/
/* Compiler.hpp -- Sen Word Compiler                                          */
/*============================================================================*/
#pragma once

#include "eCM.h"

namespace SenVM
{
  class Compiler : public Object
  {
  private:
    Word* m_pCurrent;        // current word being compiled
    eCM   m_eMode;           // current mode of compiler
  
    i8    m_nLocals;         // number of locals/params variables
    Word* m_paWord[C_MAX_NESTED_WORDS]; // array of nested words
 
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Compiler(void);
   ~Compiler(void) noexcept(false);
  
    bool  bCompiling(void);
    Word* pCurrentWord(void);
    void  vCurrentWord(Word* pWord);
    bool  bInterpreting(void);
    i8    nLocals(void);
    void  vLocals(i8 nLocals);
    eCM   eMode(void);
    void  vMode(eCM eMode);
    Word* pNestedWord(i8 i_Nested);
    void  vNestedWord(i8 i_Nested, Word* pNested);
    bool  bPostponing(void);

    Footer* aFooter(void);
  };

  Compiler* _pCompiler(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
