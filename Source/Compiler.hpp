/*============================================================================*/
/* Compiler.hpp -- Sen Word Compiler                                          */
/*============================================================================*/
#pragma once

namespace SenVM
{
  
  class Compiler : public Object
  {
  friend class Task;
  
  private:
    Word* m_pCurrent;        // current word being compiled
    Word* m_paWord[C_MAX_NESTED_WORDS]; // array of nested words
  
    bool  m_bCompiling;      // currently in compilation state?
    i8    m_nLocals;         // number of locals/params variables
  
    Tokeniser* m_pTokeniser; // input stream tokenizer
  
  public:
    Cell* m_pBeginIP;
    Cell* m_pRetryIP;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Compiler(void);
   ~Compiler(void) noexcept(false);
  
    bool  bCompiling(void);
    void  vCompiling(bool bCompiling);

    Word* pCurrentWord(void);
    void  vCurrentWord(Word* pWord);
  
    i8    nLocals(void);
    void  vLocals(i8 nLocals);

    Word* pNestedWord(i8 i_Nested);
    void  vNestedWord(i8 i_Nested, Word* pNested);
  
    void  vNextToken(void);
  
    Stz*  szToken(void);
  
    Tokeniser* pTokeniser(void);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
