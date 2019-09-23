/*============================================================================*/
/* JobObjectName.hpp -- Object Name part for a Job                            */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class JobObjectName
  {
  public:
    s32  m_sObjectName; // object name (e.g. schema, table etc)
    s32  m_sIndexName;  // index name
    
  public:
    void* operator new(size_t);
  
    Stz* szIndexName(void);
    void vIndexName(Stz* szIndexName);
  
    Stz* szObjectName(void);
    void vObjectName(Stz* szObjectName);

    JobObjectName(void);
   ~JobObjectName(void) noexcept(false);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
