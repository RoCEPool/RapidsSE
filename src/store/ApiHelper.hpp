/*============================================================================*/
/* ApiHelper.hpp -- SE Api Helper                                             */
/*============================================================================*/
#pragma once

namespace STORE
{
  class ApiHelper
  {
  protected:
    Session* m_pSession;  // rapidsse session

  public:
    ApiHelper(void);
   ~ApiHelper(void) noexcept(false);

    void vAllocateSession(void);

    void vDeleteJob(void);
  
    void vExecuteJob(void);

    void vFreeSession(void);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
