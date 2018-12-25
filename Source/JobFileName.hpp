/*============================================================================*/
/* JobFileName.hpp -- File Name part for a Job                                */
/*============================================================================*/
#pragma once

namespace JobModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class JobFileName
  {
  public:
    s256  m_sFileName; // file name
  
  public:
    void* operator new(size_t);
  
    JobFileName(void);
   ~JobFileName(void) noexcept(false);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
