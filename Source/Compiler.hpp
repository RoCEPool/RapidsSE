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
    eCM   m_eMode;           // current mode of compiler
  
    Tokeniser*   m_pTokeniser;   // input stream tokenizer
    TokenStream* m_pTokenStream; // input stream 

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
    void  vCppTokens(void);
    Word* pCurrentWord(void);
    void  vCurrentWord(Word* pWord);
    void  vDump(void);
    void  vInspect(void);
    bool  bInterpreting(void);
    void  vLineMode(bool bLineMode);
    i8    nLocals(void);
    void  vLocals(i8 nLocals);
    eCM   eMode(void);
    void  vMode(eCM eMode);
    Word* pNestedWord(i8 i_Nested);
    void  vNestedWord(i8 i_Nested, Word* pNested);
    void  vPop(void);
    bool  bPostponing(void);
    void  vPush(void);
    void  vReadToken_cpl(void);
    void  vSenTokens(void);
    eTN   eToken(void);
    Stz*  szToken(void);
    Tokeniser* pTokeniser(void);
    void  vUseBegin(void);
    void  vUseClose(void);
    void  vUseEnd(void);
    void  vUseFile(Stz* szPath);
    void  vUseStdin(void);
    void  vUseString(Stz* szString);

    Footer* aFooter(void);
  };

  Compiler* _pCompiler(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
