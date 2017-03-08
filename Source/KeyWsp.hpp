/*============================================================================*/
/* KeyWsp.hpp -- Key Workspace                                                */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class KeyWsp : public Object
  {
  private:
    WspFxd<C_1KB*15> m_xWsp;
    Footer           m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, KeyWsp*);
    
    KeyWsp(void);
    ~KeyWsp(void);
  
    void vShow(void);

    void vAddElement(ValWsp& xValWsp);
  
    char* sWspArea(void);
    Str*  aWspArea(void);
    i8    nWspUsed(void);

    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
