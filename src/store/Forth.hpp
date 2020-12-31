/*============================================================================*/
/* Forth.hpp -- Forth                                                         */
/*============================================================================*/
#pragma once

namespace STORE
{
  class Forth : public Object
  {
    friend class STORE::Task;
  
  protected:
    Task*     m_pTask;        // which task

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Forth(Task* pTask, LogicUnit* pLogicUnit);
   ~Forth(void) noexcept(false);
  
    void vNewInput(void);
    void vReset(void);
    void vSeedForth(Word* pSeedWord);
    
    bool bCorrectLayout(void);
    Task* pTask(void);

  private:
    Footer* aFooter(void);
  };
  
  Forth* _pForth(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
