/*============================================================================*/
/* SN_Int.hpp - Int scan helper                                               */
/*============================================================================*/
#pragma once

namespace ScanSprint
{
  class SN_Int : public Scan
  {
  public:
  
   // must be last
    Footer m_xFooter;
    
  public:
    SN_Int(void);
    ~SN_Int(void) noexcept(false);
  
    i8 i(char** aacCrs, Column* pCol, Format* pFmt);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
