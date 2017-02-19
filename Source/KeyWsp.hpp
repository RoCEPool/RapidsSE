/*============================================================================*/
/* KeyWsp.hpp -- Key Workspace                                                */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class KeyWsp : public Header
  {
    static const i8 C_KEYWSP_DATAAREA = C_1KB*15;
  
  private:
    i8   m_nDataBytes;  // number of used data bytes
    char m_mDataArea[C_KEYWSP_DATAAREA]; // data area
  
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, KeyWsp*);
    
    KeyWsp(void);
    ~KeyWsp(void);
  
    void vShow(void);

    void vAddElement(ValWsp& xValWsp);
  
    char* pDataArea(void);
    i8    nDataBytes(void);
    char* pDatumNext(void);

    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
