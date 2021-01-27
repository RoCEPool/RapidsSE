/*============================================================================*/
/* SetPix.hpp -- Set Pix                                                      */
/*============================================================================*/
#pragma once

namespace BASE
{
  class SetPix : public Object
  {
  protected:
    s256   m_sPixNm;       // pix name
    i8     m_nColumns;     // total columns

    ColPix m_xaColPix[1];  // array of column pix, one per column

  public:
    void* operator new(size_t);
    void* operator new(size_t, SetPix*);
    void operator delete(void*);
  
    SetPix(void);
   ~SetPix(void) noexcept(false);
  
    i8 nColumns(void);
    void vColumns(i8 nColumns);

    ColPix* axColPix(i8 i_Col);

    ColPix* axaColPixArray(void);

    Stz* szPixNm(void);
    void vPixNm(Stz* szPixNm);

    void vPrint(void);
  };

  SetPix* _pSetPix(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
