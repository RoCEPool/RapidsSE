/*============================================================================*/
/* MetaInherits.hpp - Meta Inheritance List                                   */
/*============================================================================*/
#pragma once

namespace Reflection
{
  class MetaInherits : public Object,
                       public List_t<MetaInherit>
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

    MetaInherit* pAddInherit(Stz* szBaseClassNm);

    Footer* aFooter(void);
  };

  MetaInherits* _pMetaInherits(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
