/*============================================================================*/
/* ConstSet.hpp - Constant Set                                                */
/*============================================================================*/
#pragma once

namespace Reflection
{
  class ConstSet : public Object,
                   public List_t<ConstDef>
  {
  public:
 
    MetaGraph* m_p_MetaGraph;    // the _p_ sequence means not owned by

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    ConstSet(MetaGraph* p_MetaGraph);
   ~ConstSet(void) noexcept(false);

    ConstDef* pAddConstDef(Stz* szConstNm);
   
    void vPrint(i8 i_Tab);
  
    Footer* aFooter(void);
  };

  ConstSet* _pConstSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
