/*============================================================================*/
/* JobDicPath.hpp -- Dictionary Path part for a Job                           */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class JobDicPath
  {
  public:
    DicPath m_xDicPath; // dictionary path
  
    Interface::SEitf* m_pSEitf;
  
  public:
    void* operator new(size_t);
  
    JobDicPath(Interface::SEitf* pSEitf);
   ~JobDicPath(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
