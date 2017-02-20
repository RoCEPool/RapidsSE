/*============================================================================*/
/* EntWsp.hpp -- entry workspace                                              */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class EntWsp : public Header
  {
    static const i8 C_ENTWSP_DATAAREA = C_1KB*15;
  
  private:
    RowId  m_xRowId;      // row id
    i8     m_nDataBytes;  // number of data bytes
    char   m_mDataArea[C_ENTWSP_DATAAREA];
    
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, EntWsp*);
    
    EntWsp(void);
    ~EntWsp(void);
  
    void vShow(void);
  
    RowId& xRowId(void);
    void vRowId(RowId& xRowId);
  
    void vAddKey(KeyWsp& xKeyWsp);
  
    char* pDataArea(void);
    i8    nDataBytes(void);
    char* pDataNext(void);
  
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
