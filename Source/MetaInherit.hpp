/*============================================================================*/
/* MetaInherit.hpp - Meta Inheritance of another MetaStructure                */
/*============================================================================*/
#pragma once

namespace Reflection
{
  class MetaInherit : public Object,
                      public ListItem_t<MetaInherit>
  {
  public:
 
    MetaInherits* m_p_MetaInherits;  // parent member set
   
    s32           m_sMetaStructure;  // structure that is inherited
  
    EnameSet*     m_pTplArgs;        // with these template arguments

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MetaInherit(Stz* szStructure, MetaInherits* m_p_MetaInherits);
   ~MetaInherit(void) noexcept(false);
  
    Stz* sMetaStructure(void);
  
    EnameSet* pTplArgs(void);
    void vTplArgs(EnameSet* pTplArgs);
  
    Footer* aFooter(void);
  };

  MetaInherit* _pMetaInherit(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
