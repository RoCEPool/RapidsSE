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
  
    Stack*    m_pAuxStack;    // auxiliary stack
    Stack*    m_pDataStack;   // data stack
    Stack*    m_pReturnStack; // return stack
 
    LocalSet* m_pLocalsFrame; // local register frame chain
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Forth(Task* pTask);
   ~Forth(void) noexcept(false);

  private:
    Footer* aFooter(void);
  };
  
  Forth* _pForth(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
