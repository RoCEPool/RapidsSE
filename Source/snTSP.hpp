/*============================================================================*/
/* snTSP.hpp - Tme scan helper                                                */
/*============================================================================*/
#pragma once

namespace Transform
{
  class snTSP : public Scan
  {
  public:
  
    Tsp m_xTsp;
  
    // must be last
    Footer m_xFooter;
    
  public:
    snTSP(void);
    ~snTSP(void) noexcept(false);
  
    i8 i(char** aacCrs, Column* pCol, Formatters* pScnFmt);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
