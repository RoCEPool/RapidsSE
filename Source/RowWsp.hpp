/*============================================================================*/
/* RowWsp.hpp -- Row Workspace                                                */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class RowWsp : public Header
  {
    static const i8 C_ROWWSP_DATA = C_1KB*15;
  private:
    i8     m_iTxId;       // transaction id
    i8     m_iDataBytes;  // number of data bytes (incl null teminator)
    char   m_mData[C_ROWWSP_DATA];
    
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, RowWsp*);
    
    RowWsp(i8 iDataBytes);
    ~RowWsp(void);
        
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/