/*============================================================================*/
/* SetPix.hpp -- InfoSet Pix                                                 */
/*============================================================================*/
#pragma once

#include "eOI.h"

#include "InfoSetBase.hpp"

namespace STORE
{
  class InfoPix : public Object
  {
  protected:
    s256   m_sPixNm;       // pix name
    i8     m_nColumns;     // total columns

    ColPix m_xaColPix[1];  // array of column pix

  public:
    void* operator new(size_t, InfoPix*);
    void operator delete(void*);
  
    InfoPix(void);
   ~InfoPix(void) noexcept(false);
  
    i8 nColumns(void);
    void vColumns(i8 nColumns);

    ColPix* aColPixArray(void);
  
    Stz* szPixNm(void);
    void vPixNm(Stz* szPixNm);

    void vPrint(void);
  };

  InfoPix* _pInfoPix(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
