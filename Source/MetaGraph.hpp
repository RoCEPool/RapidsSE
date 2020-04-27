/*============================================================================*/
/* MetaGraph.hpp - MetaGraph                                                  */
/*============================================================================*/
#pragma once

namespace Reflection
{
  class MetaGraph : public Object,
                    public List_t<MetaType>
  {
  public:
 
    ConstSet* m_pConstSet;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MetaGraph(void);
   ~MetaGraph(void) noexcept(false);
  
    static void vPrintTermSeq(TagSet& roTerms);
  
    void  vAddMetaType(MetaType* pMetaType);

    ConstSet* pConstSet(void);

    void vPrint(void);
    void vRecursivePrint(void);
  
    Footer* aFooter(void);
  };

  MetaGraph* _pMetaGraph(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
