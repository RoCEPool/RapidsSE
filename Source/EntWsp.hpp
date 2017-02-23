/*============================================================================*/
/* EntWsp.hpp -- entry workspace                                              */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class EntWsp : public Object,
                 public Wsp<C_1KB*15>
  {
  private:
    RowId  m_xRowId;   // row id
  
    Footer m_xFooter;  // footer
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, EntWsp*);
    
    EntWsp(void);
    ~EntWsp(void);
  
    void vShow(void);
  
    RowId& xRowId(void);
    void vRowId(RowId& xRowId);
  
    void vAddKey(KeyWsp& xKeyWsp);
    Key* pKey(void);
    i8   iSize(void);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
