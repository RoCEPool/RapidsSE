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
  
    bool  m_bCompiling;    // currently in compilation state?
  
    Word* m_pCurrent;      // current word being compiled
    i8    m_nLocals;       // number of locals/params variables
  
  public:
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
  
    bool  bCompiling(void);
    void  vCompiling(bool bCompiling);

    Word* pCurrentWord(void);
    void  vCurrentWord(Word* pWord);

    i8    nLocals(void);
    void  vLocals(i8 nLocals);

    void  vNextToken(void);
    Stz*  szToken(void);

        
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
