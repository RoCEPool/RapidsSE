/*============================================================================*/
/* RegSet.hpp -- Task RegSet                                                  */
/*============================================================================*/
#pragma once

namespace Registers
{
  class RegSet : public Object
  {
  private:
    Task* m_pTask;  // which task

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
