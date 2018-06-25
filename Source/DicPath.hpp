/*============================================================================*/
/* DicPath.hpp -- Dictionary Path                                             */
/*============================================================================*/
#pragma once

namespace Dictionaries
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class DicPath : public Object
  {
  public:
    i8   m_iDicDepth;               // depth of dictionary path
    s256 m_sDicPath[C_DIC_STACK];   // dictionary path
  
  public:
    void* operator new(size_t);
  
    DicPath(void);
   ~DicPath(void);
    
    void vContentDump(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
