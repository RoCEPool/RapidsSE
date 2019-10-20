/*============================================================================*/
/* RegSet.hpp -- Task RegSet                                                  */
/*============================================================================*/
#pragma once

namespace Registers
{
  class RegSet : public Object
  {
  friend class Tasks::Task;
  
  public:
    Task* m_pTask;  // which task
  
    OperandQueue* m_pOperandQueue;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    RegSet(Task* pTask);
   ~RegSet(void) noexcept(false);

  private:
    Footer* aFooter(void);
  };
  
  RegSet* _pRegSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
