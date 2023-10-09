/*============================================================================*/
/* XF_Prolog_Analyse_table.hpp -- Analyse Table                               */
/*============================================================================*/
#pragma once

#include "XF_PROLOG.hpp"

namespace STORE
{
  class XF_Prolog_Analyse_table : public XF_PROLOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Prolog_Analyse_table(CardIter* pCI);
   ~XF_Prolog_Analyse_table(void) noexcept(false);

    static void vAnalyse(CardIter* pCI);
    static void vXF_Analyse_table_passThru(CardIter* pCI);

    void vXF_Analyse_table(Symbol* pSymbol_tableSyn);
    void vXF_Analyse_table_passThru(Symbol* pSymbol_tableSyn);

  private:
    Footer* aFooter(void);
  };

  XF_Prolog_Analyse_table* _pXF_Prolog_Analyse_table(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
