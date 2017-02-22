/*============================================================================*/
/* EntWsp.hpp -- entry workspace                                              */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class EntWsp : public Object
  {
  private:
    RowId  m_xRowId;         // row id
    Wsp<C_1KB*15> m_xArea;  // byte area
  
    Footer m_xFooter;        // footer
  
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
