/*============================================================================*/
/* MetaInherit.hpp - Meta Inherit                                             */
/*============================================================================*/
#pragma once

namespace Reflection
{
  class MetaInherit : public Object
  {
  public:
 
    MetaInherits* m_p_MetaInherits;  // parent member set

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MetaInherit(MetaInherits* m_p_MetaInherits);
   ~MetaInherit(void) noexcept(false);

    Footer* aFooter(void);
  };

  MetaInherit* _pMetaInherit(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
