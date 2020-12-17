/*============================================================================*/
/* Forth.hpp -- Forth                                                         */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class Forth : public Object
  {
    friend class Tasking::Task;
  
  protected:
    Task*     m_pTask;        // which task
    LogicUnit*   m_pLogicUnit;      // associated register set

    Word*     m_pSeedWord;      // seed
    Word*     m_pExceptionWord; // seed
  
    bool      m_bSeedInitially; // seed_initially sentinel has been executed
    bool      m_bDoneFinally;   // done_finally sentinel has been executed

    DicStack  m_oDicStack;      // dictionary stack
    DicStack  m_oSaveDicStack;  // save dictionary stack

    ForthCell*     m_aInxPtr;      // instruction/cell pointer
  
    Stack*    m_pAuxStack;    // auxiliary stack
    Stack*    m_pDataStack;   // data stack
    Stack*    m_pReturnStack; // return stack
 
    LocalChain* m_pLocalsFrame; // locals| | register frame chain
  
    Compiler*   m_pCompiler;    // word compiler (Job boot, create table etc)
    Input*      m_pInput;       // input stream

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Forth(Task* pTask, LogicUnit* pLogicUnit);
   ~Forth(void) noexcept(false);
  
    void vCheckBreakPt(void);
    Compiler* pCompiler(void);
    void vCompiler(Compiler* pCompiler);
    void vContentDump(void);
    bool bCorrectLayout(void);
    void vDeleteInput(void);
    DicStack* pDicStack(void);
    void vNewInput(void);
    void vReset(void);
    DicStack* pSaveDicStack(void);
    void vSeedForth(Word* pSeedWord);
    Task* pTask(void);
    Input* pInput(void);

  private:
    Footer* aFooter(void);
  };
  
  Forth* _pForth(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
