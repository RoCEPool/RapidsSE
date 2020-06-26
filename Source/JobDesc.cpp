/*============================================================================*/
/* JobDesc.cpp -- the class which populates the operator catalog               */
/*============================================================================*/

#include "Jobs.h"

extern JobCatalog gxJobCatalog;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
JobDesc::JobDesc(
                 eCN  eClass,     // enum of the class
                 eJK  eKind,      // job kind
                 Stz* aComment,   // comment about the job
                 Stz* aCommand1,  // first word of prompt command
                 Stz* aCommand2   // optional second word of prompt command
              )
{
  JobClass* pJobClass = & gxJobCatalog.m_xJobClass[gxJobCatalog.m_nJobClasses++];

  pJobClass->m_eClass     = eClass;
  pJobClass->m_eKind      = eKind;
  pJobClass->m_aComment   = aComment;
  pJobClass->m_aCommand1  = aCommand1;
  pJobClass->m_aCommand2  = aCommand2;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
