/*============================================================================*/
/* Compiler.hpp -- Sen ForthWord Compiler                                          */
/*============================================================================*/
#pragma once

namespace Forth
{
  class Compiler : public Object
  {
  friend class Task;
  
  public:
  
    bool  m_bCompiling;    // currently compiling a word?
    ForthWord* m_pCurrentWord;  // current word being compiled
    i8    m_nSubDic;       // number of sub dictionaries
    i8    m_nLocals;       // number of locals/params variables
  
    Cell* m_pBeginIP;
    Cell* m_pRetryIP;


    Tokeniser* m_pTokeniser; // input stream tokenizer

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Compiler(void);
    ~Compiler(void) noexcept(false);
        
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
