/*============================================================================*/
/* InfoSet.hpp -- Information Set                                             */
/*============================================================================*/
#pragma once

#include "eOI.h"

namespace InfoData
{
  
  class InfoSet : public InfoSetBase
  {
  protected:
    eOI  m_eOpInfoSet;
    
  public:
    void* operator new(size_t);
    void operator delete(void*);
  
    InfoSet(void);
    InfoSet(StreamDef* pStreamDef);
    ~InfoSet(void) noexcept(false);
    
    void vOpInfoSet(eOI eOpInfoSet);
    eOI  eOpInfoSet(void);
  };

  InfoSet* _pInfoSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
