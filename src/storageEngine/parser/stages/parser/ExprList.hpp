/*============================================================================*/
/* ExprList.hpp - a list of expression atoms                                  */
/*============================================================================*/
#pragma once

namespace Parser
{  
  class ExprList : public ListNL_t<ExprAtom>
  {
  private:
    // must be last
    Footer m_xFooter;
    
   public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ExprList();
    ~ExprList(void);
    
    Footer* aFooter(void);
    
  };
  
  ExprList* _pExprList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
