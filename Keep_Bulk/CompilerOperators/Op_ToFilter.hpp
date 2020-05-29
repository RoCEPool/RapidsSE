/*============================================================================*/
/* Op_ToFilter.hpp -- To Data Op's operator                                     */
/*============================================================================*/
#pragma once

namespace CompilerOperators
{
  class Op_ToFilter : public Op
  {
  private:
  
    Filter_From*    m_pFilter_From;
    Filter_GroupBy* m_pFilter_GroupBy;
    Filter_Inflate* m_pFilter_Inflate;
    Filter_Join*    m_pFilter_Join;
    Filter_Limit*   m_pFilter_Limit;
    Filter_OrderBy* m_pFilter_OrderBy;
    Filter_Output*  m_pFilter_Output;
    Filter_Project* m_pFilter_Project;
    Filter_Scan*    m_pFilter_Scan;
    Filter_Select*  m_pFilter_Select;
    Filter_Where*   m_pFilter_Where;
    Filter_Zed*     m_pFilter_Zed;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Op_ToFilter(OpSpec* pOpSpec, Pipeline* pPipeline);
   ~Op_ToFilter(void) noexcept(false);
  
    static Op* pNew(OpSpec* pOpSpec, Pipeline* pPipeline);
    static eOR eCall(void* aThis);
    static eOR eDrain(void* aThis);

    eOR eOpLoop(void);
    eOR eDrainLoop(void);

  private:
    Footer* aFooter(void);
  };

  Op_ToFilter* _pOp_ToFilter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
