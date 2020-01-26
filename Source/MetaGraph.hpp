/*============================================================================*/
/* MetaGraph.hpp - MetaGraph                                                  */
/*============================================================================*/
#pragma once

namespace Reflection
{
  class MetaGraph : public Object,
                    List_t<MetaClass>
  {
  public:
 
    ConstSet* m_pConsSet;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MetaGraph(void);
   ~MetaGraph(void) noexcept(false);
  
    MetaClass* pAddMetaClass(Stz* szClassNm);
    
    ConstSet* pConstSet(void);

    void vPrint(void);
    void vRecPrint(void);
  
    Footer* aFooter(void);
  };

  MetaGraph* _pMetaGraph(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
