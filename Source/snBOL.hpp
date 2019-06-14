/*============================================================================*/
/* snBOL.hpp - Bol scan helper                                                */
/*============================================================================*/
#pragma once

namespace Transform
{
  class snBOL : public Scan
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    snBOL(void);
    ~snBOL(void) noexcept(false);
  
    i8 i(char** aacCrs, Column* pCol, Formatters* pScnFmt);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
