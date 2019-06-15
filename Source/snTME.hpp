/*============================================================================*/
/* snTME.hpp - Tme scan helper                                                */
/*============================================================================*/
#pragma once

namespace Transform
{
  class snTME : public Scan
  {
  public:
  
    Dtm m_xTme;
  
    // must be last
    Footer m_xFooter;
    
  public:
    snTME(void);
    ~snTME(void) noexcept(false);
  
    i8 i(char** aacCrs, Column* pCol, Formatters* pScnFmt);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
