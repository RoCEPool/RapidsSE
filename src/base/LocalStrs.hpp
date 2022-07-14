/*============================================================================*/
/* LocalStrs.hpp -- Local Strings                                             */
/*============================================================================*/
#pragma once

namespace BASE
{
  static const i8 C_LOCAL_STR  = 512 - (2 * sizeof(i8));

  struct _LclStr {
    i8   m__dummy0;
    i8   m__dummy1;
    char m__strspace[C_LOCAL_STR];
  };
  
  static const i8 C_LOCAL_STRS = (C_SIZE_MEDIUM_OBJECT - 128) / sizeof(_LclStr);

  class LocalVars;

  class LocalStrs : public Object
  {
  public:
    
    LocalVars* m_pLocalVars;

    _LclStr m_xaLclStr[C_LOCAL_STRS];

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    LocalStrs(void);
   ~LocalStrs(void) noexcept(false);
      
    _LclStr* aAllotLclStr(void* );
    _LclStr* aGetLclStr(i8 i_Local);
    void vFreeLclStr(_LclStr* aLclStr);

    // must be last
    Footer* aFooter(void);
  };
  
  LocalStrs* _pLocalStrs(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
