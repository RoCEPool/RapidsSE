/*============================================================================*/
/* MetaClass.hpp - MetaClass                                                  */
/*============================================================================*/
#pragma once

namespace Reflection
{
  class MetaClass : public Object,
                    public ListItem_t<MetaClass>
  {
  public:
 
    s32 m_sClassNm;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    MetaClass(void);
   ~MetaClass(void) noexcept(false);
  
    Stz* szClassNm(void);
    void vClassNm(Stz* szClassNm);
      
    Footer* aFooter(void);
  };

  MetaClass* _pMetaClass(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
