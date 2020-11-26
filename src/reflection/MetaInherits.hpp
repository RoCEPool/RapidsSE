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
    MetaType* m_p_MetaType;  // parent member set

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MetaInherits(MetaType* p_MetaType);
   ~MetaInherits(void) noexcept(false);

    MetaInherit* pAddInherit(Stz* szBaseClassNm);

    Footer* aFooter(void);
  };

  MetaInherits* _pMetaInherits(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
