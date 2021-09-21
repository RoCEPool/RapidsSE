/*============================================================================*/
/* JobReport.hpp -- JobReport                                                */
/*============================================================================*/
#pragma once

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
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void* pJobReportAddress);
  
    JobReport(i8 iJobId);
   ~JobReport(void) noexcept(false);

    static void vReport(i8 iJobNo);

    void vCraftReport(void);
    bool bFindParts(void);
    void vReportJob(void);

    Footer* aFooter(void);
  };
  
   JobReport* _pJobReport(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
