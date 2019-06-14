/*============================================================================*/
/* snIVL.hpp - Ivl scan helper                                                */
/*============================================================================*/
#pragma once

namespace Transform
{
  class snIVL : public Scan
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    snIVL(void);
    ~snIVL(void) noexcept(false);
  
    i8 i(char** aacCrs, Column* pCol, Formatters* pScnFmt);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
