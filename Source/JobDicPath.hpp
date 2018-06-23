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
    i8   m_iDicDepth;               // depth of dictionary path
    s256 m_sDicPath[C_DIC_STACK];   // dictionary path
  
  public:
    void* operator new(size_t);
  
    JobDicPath(Interface::SEitf* pSEitf);
   ~JobDicPath(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
