/*============================================================================*/
/* InfoPix.hpp -- InfoSet Pix                                                 */
/*============================================================================*/
#pragma once

#include "eOI.h"

#include "InfoSetBase.hpp"

namespace STORE
{
  
  class InfoPix : public Object
  {
  protected:
    
  public:
    void* operator new(size_t, InfoPix*);
    void operator delete(void*);
  
    InfoPix(StreamDef* pStreamDef);
   ~InfoPix(void) noexcept(false);
  
    void vFormInfoPix(StreamDef* pStreamDef);
    
  };

  InfoPix* _pInfoPix(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
