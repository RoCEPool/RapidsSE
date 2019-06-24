/*============================================================================*/
/* SN_Int.hpp - Int scan helper                                               */
/*============================================================================*/
#pragma once

namespace ScanSprint
{
  class SN_Int : public Scan
  {
  public:
  
    Dtm m_xInt;
  
   // must be last
    Footer m_xFooter;
    
  public:
    SN_Int(void);
    ~SN_Int(void) noexcept(false);
  
    i8 i(char** aacCrs, Column* pCol, Formatters* pScnFmt);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
