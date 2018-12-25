/*============================================================================*/
/* JobObjectName.hpp -- Object Name part for a Job                            */
/*============================================================================*/
#pragma once

namespace JobModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class JobObjectName
  {
  public:
  s32  m_sObjectName; // object name (e.g. schema, table etc)
  s32  m_sIndexName;  // index name
    
  public:
    void* operator new(size_t);
  
    JobObjectName(void);
   ~JobObjectName(void) noexcept(false);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
