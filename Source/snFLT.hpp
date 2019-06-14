/*============================================================================*/
/* snFLT.hpp - Flt scan helper                                                */
/*============================================================================*/
#pragma once

namespace Transform
{
  class snFLT : public Scan
  {
  public:
  
    Flt m_xFlt;
  
    // must be last
    Footer m_xFooter;
    
  public:
    snFLT(void);
    ~snFLT(void) noexcept(false);
  
    d8 d(char** aacCrs, Column* pCol, Formatters* pScnFmt);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
