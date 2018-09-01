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
  
    static pJOBCLASSFUN pFirstFun(void);
  
    static JobClass_s* pFindJobClass_s(eCN eClass);
    static JobClass_s* pFindJobClass_s(Stz* aCommand1, Stz* aCommand2);
    static void vTraceJobs_s(eCN eClass);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
