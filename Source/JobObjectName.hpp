/*============================================================================*/
/* JobObjectName.hpp -- Object Name part for a Job                            */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class JobObjectName
  {
  public:
  s32  m_sObjectName; // object name (e.g. schema, table etc)
  
  public:
    void* operator new(size_t);
  
    JobObjectName(void);
   ~JobObjectName(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
