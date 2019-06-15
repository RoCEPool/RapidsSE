/*============================================================================*/
/* snDTE.hpp - Dte scan helper                                                */
/*============================================================================*/
#pragma once

namespace Transform
{
  class snDTE : public Scan
  {
  public:
  
    Dtm m_xDte;
  
    // must be last
    Footer m_xFooter;
    
  public:
    snDTE(void);
    ~snDTE(void) noexcept(false);
  
    i8 i(char** aacCrs, Column* pCol, Formatters* pScnFmt);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
