/*============================================================================*/
/* JobAbs.hpp -- Job Absolute                                                 */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class JobAbs : public Object,
                 public JobDicPath,
                 public JobObjectName,
                 public JobResult
  {
  public:
  
  public:
    void* operator new(size_t);
  
    JobAbs(eCN eClass, Interface::SEitf* pSEitf);
   ~JobAbs(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
