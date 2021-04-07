/*============================================================================*/
/* InternalTables.hpp -- Internal Tables                                      */
/*============================================================================*/
#pragma once

#include "eTI.h"

namespace STORE
{
  class InternalTables : public Object
  {
  private:
  
    StreamDef* m_apStreamDef[TI_ENUMS];
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    InternalTables(void);
   ~InternalTables(void) noexcept(false);
  
    StreamDef* pStreamDef(eTI eTable);
    void vStreamDef(eTI eTable, StreamDef* pStreamDef);

  private:
  
    Footer* aFooter(void);
  };
  
  InternalTables* _pInternalTables(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
