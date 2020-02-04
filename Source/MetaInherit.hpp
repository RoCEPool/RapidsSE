/*============================================================================*/
/* MetaInherit.hpp - Inheritance of another Meta Type                         */
/*============================================================================*/
#pragma once

namespace Reflection
{
  class MetaInherit : public Object,
                      public ListItem_t<MetaInherit>
  {
  public:
    MetaInherits* m_p_WithinInherits; // this inherit is within which interits set
  
    s32           m_sInheritedType; // type that is inherited
    EnameSet*     m_pTplArgs;       // opt. with these template arguments

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MetaInherit(Stz* szInheritedType, MetaInherits* p_WithinInherits);
   ~MetaInherit(void) noexcept(false);
  
    Stz* sInheritedType(void);
  
    EnameSet* pTplArgs(void);
    void vTplArgs(EnameSet* pTplArgs);
  
    Footer* aFooter(void);
  };

  MetaInherit* _pMetaInherit(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
