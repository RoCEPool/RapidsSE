/*============================================================================*/
/* Compiler.hpp -- Sen Word Compiler                                          */
/*============================================================================*/
#pragma once

namespace SenVM
{  
  class Compiler : public Object
  {
  private:
    Word* m_pCurrent;        // current word being compiled
    bool  m_bCompiling;      // currently in compilation state?
    eCM   m_eMode;           // current mode of compiler
  
    Tokeniser* m_pTokeniser; // input stream tokenizer

    i8    m_nLocals;         // number of locals/params variables
    Word* m_paWord[C_MAX_NESTED_WORDS]; // array of nested words

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

    eCM   eMode(void);
    void  vMode(eCM eMode);
  
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
