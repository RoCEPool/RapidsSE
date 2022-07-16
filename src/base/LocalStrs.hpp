/*============================================================================*/
/* LocalStrs.hpp -- Local Strings                                             */
/*============================================================================*/
#pragma once

namespace BASE
{
  static const i8 C_LOCAL_STR  = 512 - (8 * sizeof(i8));

  struct _SizeArea {
    i8         m_jStrSize;
    char       m_caArea[C_LOCAL_STR];
  };

  struct _LclStr {
    
    LocalStrs* m_pLocalStrs;
    i8         m_i_LclStr;
    i8         m_i_NextFree;  // C_UNKNOWN indicates end
    
//    i8         m_jStrSize;
//    char       m_caArea[C_LOCAL_STR];
    
    _SizeArea  m_xSizeArea;
  };
  
  static const i8 C_LOCAL_STRS = (C_SIZE_MEDIUM_OBJECT - 128) / sizeof(_LclStr);

  class LocalVars;

  class LocalStrs : public Object
  {
  public:
    
    LocalVars* m_pLocalVars;

    i8         m_i_FirstFree;  // C_UNKNOWN indicates end
  
    _LclStr    m_xaLclStr[C_LOCAL_STRS];

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    LocalStrs(LocalVars* pLocalVars);
   ~LocalStrs(void) noexcept(false);
      
    _LclStr* axAllotLclStr(void);
    void     vFreeLclStr(_LclStr* axLclStr);
    //_LclStr* axGetLclStr(i_LclStr);

    // must be last
    Footer* aFooter(void);
  };
  
  LocalStrs* _pLocalStrs(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
