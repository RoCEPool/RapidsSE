/*============================================================================*/
/* SubCall.hpp -- Sub-Pipeline Call                                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class SubCall : public Object
  {
  private:

    Decider  m_oAcksRmg;  // number of acks still waiting for
  
    Task*    m_pTask;     // which task to notify
  
    // must be last
    Footer m_xFooter;
 
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    SubCall(void);
   ~SubCall(void) noexcept(false);
  
    static void vRepair(Object* pObject);
  
    bool bDecRmg(i8& nAcksWereRmg);
  
    void vSetRmg(i8 nAcksRmg);
  
    Task* pTask(void);
    void vTask(Task* pTask);

  private:

    Footer* aFooter(void);
  };
  
  SubCall* _pSubCall(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
