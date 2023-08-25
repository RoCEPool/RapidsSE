/*============================================================================*/
/* XF_Prolog_table_Analysis.hpp -- Table Prior                                    */
/*============================================================================*/
#pragma once

#include "XF_PROLOG.hpp"

namespace STORE
{
  class XF_Prolog_table_Analysis : public XF_PROLOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Prolog_table_Analysis(CardIter* pCI);
   ~XF_Prolog_table_Analysis(void) noexcept(false);

    static void vProlog(CardIter* pCI, Symbol* pSymbol_tableSyn);
    static void vXF_table_Analysis_passThru(CardIter* pCI, Symbol* pSymbol_tableSyn);

    void vXF_table_Analysis(Symbol* pSymbol_tableSyn);
    void vXF_table_Analysis_passThru(Symbol* pSymbol_tableSyn);

  private:
    Footer* aFooter(void);
  };

  XF_Prolog_table_Analysis* _pXF_Prolog_table_Analysis(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
