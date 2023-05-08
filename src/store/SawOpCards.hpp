/*============================================================================*/
/* SawOpCards.hpp -- Saw Op Cards                                             */
/*============================================================================*/
#pragma once

#include "Bench.hpp"


namespace STORE
{
  class SawOpCards : public Object
  {
  public:

    eAL              m_eAggLevel;

    Card_Aggreg*     m_p_aggreg;
    Card_Filter*     m_p_filter;
    Card_GbyAgg*     m_p_gbyAgg;
    Card_Having*     m_p_having;
    Card_IndexJoin*  m_p_indexJoin;
    Card_IndexScan*  m_p_indexScan;
    Card_Project*    m_p_project;
    Card_Scan*       m_p_scan;
    Card_Sort*       m_p_sort;
    Card_Where*      m_p_where;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    SawOpCards(void);
   ~SawOpCards(void) noexcept(false);

    void vReset(eAL eAggLevel=AL_NONE);
    void vSaw(Card_OP* p_card_OP);

  protected:
    Footer* aFooter(void);
  };

  SawOpCards* _pSawOpCards(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
