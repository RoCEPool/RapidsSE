/*============================================================================*/
/* NmStr.hpp -- name-string pair                                              */
/*============================================================================*/
#pragma once

namespace Transform
{
  class NmStr : public Object,
                public ListItem_t<NmStr>
  {
  public:
  
    s32 m_sName;
    s4K m_sValue;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    NmStr(void);
    NmStr(Stz* szName);
    NmStr(Stz* szName, Stz* szValue);
   ~NmStr() noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
