/*============================================================================*/
/* MetaInherit.hpp - Meta Inheritance of another MetaType                     */
/*============================================================================*/
#pragma once

namespace Reflection
{
  class MetaInherit : public Object,
                      public ListItem_t<MetaInherit>
  {
  public:
    MetaInherits* m_p_MetaInherits; // which inherits set this inherit is in
   
    s32           m_sMetaType;      // type that is inherited
    EnameSet*     m_pTplArgs;       // opt. with these template arguments

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MetaInherit(Stz* szStructure, MetaInherits* m_p_MetaInherits);
   ~MetaInherit(void) noexcept(false);
  
    Stz* sMetaType(void);
  
    EnameSet* pTplArgs(void);
    void vTplArgs(EnameSet* pTplArgs);
  
    Footer* aFooter(void);
  };

  MetaInherit* _pMetaInherit(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
