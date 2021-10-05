/*============================================================================*/
/* JobReport.hpp -- JobReport                                                 */
/*============================================================================*/
#pragma once

#include "NamedBuffer.hpp"

namespace STORE
{
  class JobReport : public Object
  {
  private:
    i8       m_iJobNo;
    JobId    m_tJobId;
    Job*     m_pJob;
    TagSet*  m_pJobTags;
    i8       m_iReponseTime;
    i8       m_iMagnitude;

    i8       m_i_pu_test;
    i8       m_i_pu_subset;
    i8       m_i_pu_t;
    i8       m_i_pu_ft;
    i8       m_i_pu_j;
    i8       m_i_pu_fj;
    i8       m_i_pu_groups;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void* pJobReportAddress);
  
    JobReport(i8 iJobId, Job* pJob);
   ~JobReport(void) noexcept(false);

    static Job* pFindOriginalJob(i8 iJobNo);
    static Job* pFindParticipantJob(i8 iJobNo);
    static eEC eOperators(i8 iJobNo, Tub* pTub);
    static eEC eReport(Job* pJob, Tub* pTub);
    static eEC eReport(i8 iJobNo, Tub* pTub);

  private:
    void vGenerateReport(Tub* pTub);
    void vStatsFromJobTags(void);

    Footer* aFooter(void);
  };
  
   JobReport* _pJobReport(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
