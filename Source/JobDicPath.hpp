/*============================================================================*/
/* JobDicPath.hpp -- Dictionary Path part for a Job                           */
/*============================================================================*/
#pragma once

namespace JobModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class JobDicPath
  {
  public:
    DicPath m_oDicPath; // dictionary path
  
    Interface::Session* m_pSession;
  
  public:
    void* operator new(size_t);
  
    JobDicPath(void);
    JobDicPath(Interface::Session* pSession);
   ~JobDicPath(void) noexcept(false);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
