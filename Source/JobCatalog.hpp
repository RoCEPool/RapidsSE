/*============================================================================*/
/* JobCatalog.hpp -- the catalog of job classes                               */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class JobCatalog
  {
  public:
    i8       m_nClasses;       // number of job classes
    JobClass m_xJobClass[128]; // array of job class descriptions
  
  public:
    JobClass* pFindJob(Stz* aCommand1, Stz* aCommand2);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
