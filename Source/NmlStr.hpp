/*============================================================================*/
/* NmlStr.hpp -- Normalized String Set                                        */
/*============================================================================*/
#pragma once

namespace Rows
{
  static const i8 C_WSP_NMLSTR = C_8KB - 128;

  class NmlStr : public Object
  {
  private:
      
    Wsp_t<C_WSP_NMLSTR> m_xWsp; // workspace
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    NmlStr(void);
   ~NmlStr(void) noexcept(false);
      
  private:
    Footer* aFooter(void);
  };
  
  NmlStr* _pNmlStr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
