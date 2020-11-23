/*============================================================================*/
/* JobFilePath.hpp -- File Path part for a Job                                */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class JobFilePath
  {
  private:
    s256  m_sFilePath; // file path
  
  public:
    void* operator new(size_t);
  
    JobFilePath(void);
   ~JobFilePath(void) noexcept(false);
  
    Stz* szFilePath(void);
    void vFilePath(Stz* szFilePath);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
