/*============================================================================*/
/* Forth.hpp -- Forth                                                         */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class Forth : public Object
  {
  public:
    Task*     m_pTask;        // which task
  
    Word*     m_pSeedWord;      // seed
    Word*     m_pExceptionWord; // seed
  
    DicStack  m_oDicStack;      // dictionary stack

    Cell*     m_aInxPtr;      // instruction/cell pointer
  
    Stack*    m_pAuxStack;    // auxiliary stack
    Stack*    m_pDataStack;   // data stack
    Stack*    m_pReturnStack; // return stack
 
    LocalChain* m_pLocalsFrame; // local register frame chain
  
    Compiler* m_pCompiler;    // word compiler (Job boot, create table etc)
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Forth(Task* pTask);
   ~Forth(void) noexcept(false);
  
    bool bCorrectLayout(void);
    void vReset(void);
    void vSeedForth(Word* pSeedWord);

  private:
    Footer* aFooter(void);
  };
  
  Forth* _pForth(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
