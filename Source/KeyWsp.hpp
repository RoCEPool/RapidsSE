/*============================================================================*/
/* KeyWsp.hpp -- Key Workspace                                                */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class KeyWsp : public Object,
                 public Wsp<C_1KB*15>
  {
  private:
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
