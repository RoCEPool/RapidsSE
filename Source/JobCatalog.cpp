/*============================================================================*/
/* JobCatalog.cpp -- the catalog of job classes                               */
/*============================================================================*/

#include "Jobs.h"


/*----------------------------------------------------------------------------*/
/* find class definition given command                                        */
/*----------------------------------------------------------------------------*/
JobClass* JobCatalog::aFindClass(eCN eClass)
{
  JobClass* aJobClass = nullptr;
  
  for (i8 i_Class = 0; i_Class < m_nJobClasses; i_Class++) {
    
    aJobClass = & m_xJobClass[i_Class];
    
    if (aJobClass->m_eClass == eClass)
      return aJobClass;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find class definition given command                                        */
/*----------------------------------------------------------------------------*/
JobClass* JobCatalog::aFindClass(Stz* szCmd1, Stz* szCmd2)
{
  JobClass* aJobClass = nullptr;

  for (i8 i_Class = 0; i_Class < m_nJobClasses; i_Class++) {
    
    aJobClass = & m_xJobClass[i_Class];
    
    if (! aJobClass->m_aCommand1 ||
        ! StrLib::bSame(aJobClass->m_aCommand1->azChar(),szCmd1->azChar()) )
      continue;

    if (aJobClass->m_aCommand2) {
      if ( ! StrLib::nLength(aJobClass->m_aCommand2->azChar()) )
        continue;
      if ( ! StrLib::bSame(aJobClass->m_aCommand2->azChar(),szCmd2->azChar()) )
        continue;
    }
    
    return nullptr;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* help                                                                       */
/*----------------------------------------------------------------------------*/
void JobCatalog::vHelp(eCN eClass)
{

}

/*----------------------------------------------------------------------------*/
/* print catalog                                                              */
/*----------------------------------------------------------------------------*/
void JobCatalog::vPrintCatalog(void)
{
}

/*----------------------------------------------------------------------------*/
/* print catalog                                                              */
/*----------------------------------------------------------------------------*/
void JobCatalog::vPrintClass(JobClass* aJobClass)
{
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
