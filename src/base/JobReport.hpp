/*============================================================================*/
/*  JobReport.hpp --  JobReport                                               */
/*============================================================================*/
#pragma once

namespace BASE
{
  class  JobReport : public Object
  {
  public:
  
  private:
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void* pJobReportAddress);
  
     JobReport(void);
   ~ JobReport(void) noexcept(false);

    Footer* aFooter(void);
  };
  
   JobReport* _pJobReport(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
