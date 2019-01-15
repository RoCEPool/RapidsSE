/*============================================================================*/
/* DicPath.hpp -- Dictionary Path                                             */
/*============================================================================*/
#pragma once

namespace Forth
{
  static const i8 C_DIC_STACK = 16;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class DicPath : public Object
  {
    i8   m_nDicNames;               // depth of dictionary path
    s256 m_sDicPath[C_DIC_STACK];   // dictionary path
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
  
    DicPath(void);
   ~DicPath(void) noexcept(false);
    
    void vContentDump(void);
  
    i8 nDicNames(void);
  
    s256& sDicName(i8 i_Dic);
    void vDicName(i8 i_Dic, s256& sName);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
