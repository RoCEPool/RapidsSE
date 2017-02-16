/*============================================================================*/
/* EntWsp.hpp -- Entry Workspace                                              */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class EntWsp : public Header
  {
    static const i8 C_ENTWSP_DATA = C_1KB*15;
  
  private:
    RowId  m_xRowId;      // row id
    i8     m_iDataBytes;  // number of data bytes
    char   m_mData[C_ENTWSP_DATA];
    
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, EntWsp*);
    
    EntWsp(void);
    ~EntWsp(void);
  
    void vShow(void);
  
    RowId& xRowId(void);
    void vRowId(RowId& xRowId);
  
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
