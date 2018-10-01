/*============================================================================*/
/* Compiler.hpp -- Sen Word Compiler                                          */
/*============================================================================*/
#pragma once

namespace Tasks
{
  class Compiler : public Object
  {
  friend class Task;
  
  public:
  
    bool  m_bCompiling;    // currently compiling a word?
    Word* m_pCurrentWord;  // current word being compiled
    i8    m_nVariables;    // number of locals/params variables

    Tokeniser* m_pTokeniser; // input stream tokenizer

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Compiler(void);
    ~Compiler(void);
        
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
