/*============================================================================*/
/* snBCD.hpp - Bcd scan helper                                                */
/*============================================================================*/
#pragma once

namespace Transform
{
  class snBCD : public Scan
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    snBCD(void);
    ~snBCD(void) noexcept(false);
  
    i8 i(char** aacCrs, Column* pCol, Formatters* pScnFmt);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
