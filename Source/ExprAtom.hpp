/*============================================================================*/
/* ExprAtom.hpp - an abstract expression atom used in parsing.                */
/*============================================================================*/
#pragma once

namespace Parser
{  
  class ExprAtom : public Object, public ListItem_t<ExprAtom>
  {
  protected:
    ListNL_t<ExprAtom> m_oChildren;
    
  private:
    Footer m_xFooter;
    
   public:
    
    ExprAtom(eCN eClassNo);
    virtual ~ExprAtom(void);
    
    Footer* aFooter(void);
    
    virtual void vDescribe(s2K& buf) = 0;
    
    void vAddChild(ExprAtom* pAtom);
  };
  
  ExprAtom* _pExprAtom(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
