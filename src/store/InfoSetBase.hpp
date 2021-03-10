/*============================================================================*/
/* InfoSetBase.hpp -- Information Set Base                                    */
/*============================================================================*/
#pragma once

#include "RowBuffer.hpp"

namespace STORE
{
  static const i8 C_INFO_VALUES = 256;
  
  class InfoSetBase : public RowBuffer
  {
  protected:
    //s32 m_sInfoSetNm;
    
    // must be last
    Footer m_xFooter;

  public:
  
    InfoSetBase(eCN eClassNo);
    InfoSetBase(eCN eClassNo, StreamDef* pStreamDef);
   ~InfoSetBase(void) noexcept(false);
    
    
  //private:
    i8   iToOffset(Stz* aItem);
  
    Footer* aFooter(void);
  };

  InfoSetBase* _pInfoSetBase(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
