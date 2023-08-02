/*============================================================================*/
/* XF_table_Analysis.hpp -- Table Prior                                    */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_table_Analysis : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_table_Analysis(CardIter* pCI, char* acTableSyn);
   ~XF_table_Analysis(void) noexcept(false);

    static void vXF_table_Analysis(CardIter* pCI, char* acTableSyn, Symbol* pSymbol_tableSyn);
    static void vXF_table_Analysis_passThru(CardIter* pCI, char* acTableSyn, Symbol* pSymbol_tableSyn);

    void vXF_table_Analysis(char* acTableSyn, Symbol* pSymbol_tableSyn);
    void vXF_table_Analysis_passThru(char* acTableSyn, Symbol* pSymbol_tableSyn);

  private:
    Footer* aFooter(void);
  };

  XF_table_Analysis* _pXF_table_Analysis(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
