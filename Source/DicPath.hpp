/*============================================================================*/
/* DicPath.hpp -- Dictionary Path (array of dic names)                        */
/*============================================================================*/
#pragma once

namespace Forth
{
  static const i8 C_DIC_PATHDEPTH = 8; // 8 is probably enough
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class DicPath : public Object
  {
    i8  m_nDicNames;                 // number of dic names in array
    s32 m_sDicName[C_DIC_PATHDEPTH]; // array of dictionary names
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
  
    DicPath(void);
   ~DicPath(void) noexcept(false);
    
    void vContentDump(void);
  
    i8 nDicNames(void);
  
    s32& sDicName(i8 i_Dic);
    void vDicName(i8 i_Dic, s32& sName);
  
    void vAsString(s256& sString);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
