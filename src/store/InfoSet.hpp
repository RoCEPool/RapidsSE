/*============================================================================*/
/* InfoSet.hpp -- Information Set                                             */
/*============================================================================*/
#pragma once

#include "eOI.h"

#include "InfoSetBase.hpp"

namespace STORE { class InfoPix; }

namespace STORE
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
    
    void vFormInfoPix(StreamDef* pStreamDef);
  
    InfoPix* pInfoPix(void);
  
    void vOpInfoSet(eOI eOpInfoSet);
    eOI  eOpInfoSet(void);
  
  };

  InfoSet* _pInfoSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
