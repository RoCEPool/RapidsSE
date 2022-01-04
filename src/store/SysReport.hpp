/*============================================================================*/
/* SysReport.hpp -- SysReport                                                 */
/*============================================================================*/
#pragma once

namespace STORE { class Tub; }

namespace STORE
{
  class SysReport : public Object
  {
  private:

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void* aAddr);
  
    SysReport(void);
   ~SysReport(void) noexcept(false);

    static void vSysMetrics(Tub* pTub);

  private:
    Footer* aFooter(void);
  };
  
   SysReport* _pSysReport(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
