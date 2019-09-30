/*============================================================================*/
/* JobCatalog.hpp -- the catalog of job classes                               */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class JobCatalog
  {
  public:
    i8       m_nJobClasses;     // number of job classes
    JobClass m_xJobClass[1024]; // array of job classes
  
  public:
    JobClass* aFindClass(eCN eClass);
    JobClass* aFindClass(Stz* szCmd1, Stz* szCmd2);

    void    vHelp(eCN eClass);
    void    vPrintCatalog(void);
    void    vPrintClass(JobClass* aJobClass);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
