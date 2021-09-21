/*============================================================================*/
/* JobReport.cpp -- JobReport                                                 */
/*============================================================================*/

#include "BASE.h"
#include "JobReport.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,JobReport,CN_JOBREPORT)
__aNewFunction(JobReport,CN_JOBREPORT)
__vDeleteFunction(JobReport,CN_JOBREPORT)
__aFooterFunction(JobReport)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
JobReport::JobReport(i8 iJobNo)
: Object(CN_JOBREPORT)
{
  _precondition_(_MAGIC_,CN_JOBREPORT,_FOOTER_);

  m_iJobNo   = iJobNo;
  m_tJobId   = C_UNKNOWN;
  m_pJob     = nullptr;
  m_pJobTags = nullptr;

  _checkInitialised_(CN_JOBREPORT);

  _postcondition_(_MAGIC_,CN_JOBREPORT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
JobReport::~JobReport(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_JOBREPORT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* report                                                                     */
/*----------------------------------------------------------------------------*/
void JobReport::vReport(i8 iJobNo)
{
  _static_

  vTrace("JobReport: %lld\n"_Stz,iJobNo);
  
  JobReport* pJobReport = new JobReport(iJobNo);
  
  pJobReport->vReportJob();
  
  delete pJobReport;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* craft the report for job                                                   */
/*----------------------------------------------------------------------------*/
void JobReport::vCraftReport(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBREPORT,_FOOTER_);
  
  vTrace("vCraftReport: %lld\n"_Stz,m_iJobNo);

}

/*----------------------------------------------------------------------------*/
/* find all the parts                                                         */
/*----------------------------------------------------------------------------*/
bool JobReport::bFindParts(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBREPORT,_FOOTER_);

  vTrace("bFindParts: %lld\n"_Stz,m_iJobNo);

  return true;
}

/*----------------------------------------------------------------------------*/
/* report on job                                                              */
/*----------------------------------------------------------------------------*/
void JobReport::vReportJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOBREPORT,_FOOTER_);
  
  vTrace("vReportJob: %lld\n"_Stz,m_iJobNo);

  if (bFindParts())
    vCraftReport();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
