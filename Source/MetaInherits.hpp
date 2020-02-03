/*============================================================================*/
/* MetaInherits.hpp - Meta Inherits                                           */
/*============================================================================*/
#pragma once

namespace Reflection
{
  class MetaInherits : public Object,
                       public List_t<MetaMember>
  {
  public:
 
    MetaGraph*     m_p_MetaGraph;      // the _p_ sequence means not owned by
    MetaStructure* m_p_MetaStructure;  // parent member set

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MetaInherits(MetaGraph* p_MetaGraph, MetaStructure* p_MetaStructure);
   ~MetaInherits(void) noexcept(false);

    Footer* aFooter(void);
  };

  MetaInherits* _pMetaInherits(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
