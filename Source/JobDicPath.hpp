/*============================================================================*/
/* JobDicPath.hpp -- Dictionary Path part for a job                           */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class JobDicPath
  {
  public:
    i8   m_iDicDepth;               // depth of dictionary path
    s256 m_sDicPath[C_MAX_FOLDERS]; // dictionary path
  
  public:
    JobDicPath(void);
   ~JobDicPath(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
