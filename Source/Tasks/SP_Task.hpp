/*============================================================================*/
/* SP_Task.hpp - Task sprint helper                                           */
/*============================================================================*/
#pragma once

namespace Tasks
{
  class SP_Task : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_Task(void);
   ~SP_Task(void) noexcept(false);
  
    Stz* sz(Task* pTask);
    Stz* sz_log(Task* pTask);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
