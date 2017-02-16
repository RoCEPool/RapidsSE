/*============================================================================*/
/* KeyWsp.hpp -- Key Workspace                                                */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class KeyWsp : public Header
  {
    static const i8 C_KEYWSP_DATA = C_1KB*15;
  
  private:
    i8     m_iDataBytes;  // number of data bytes
    char   m_mData[C_KEYWSP_DATA];
    
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, KeyWsp*);
    
    KeyWsp(void);
    ~KeyWsp(void);
  
    void vShow(void);

    void vAddElement(ValWsp& xValWsp);

    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
