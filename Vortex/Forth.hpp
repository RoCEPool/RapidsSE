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
    RegSet*   m_pRegSet;      // associated register set

    Word*     m_pSeedWord;      // seed
    Word*     m_pExceptionWord; // seed
  
    bool      m_bSeedInitially; // seed_initially sentinel has been executed
    bool      m_bDoneFinally;   // done_finally sentinel has been executed

    DicStack  m_oDicStack;      // dictionary stack

    Cell*     m_aInxPtr;      // instruction/cell pointer
  
    Stack*    m_pAuxStack;    // auxiliary stack
    Stack*    m_pDataStack;   // data stack
    Stack*    m_pReturnStack; // return stack
 
    LocalChain* m_pLocalsFrame; // locals| | register frame chain
  
    Compiler* m_pCompiler;    // word compiler (Job boot, create table etc)
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Forth(Task* pTask, RegSet* pRegSet);
   ~Forth(void) noexcept(false);
  
    void vContentDump(void);
    bool bCorrectLayout(void);
    DicStack* pDicStack(void);
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
