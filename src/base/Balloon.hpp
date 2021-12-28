/*============================================================================*/
/* Balloon.hpp -- Balloon                                                     */
/*============================================================================*/
#pragma once

namespace BASE
{
static const i8 C_BALLOON_MAGN = 16;

  class Balloon : public Object
  {
  private:

    eCN m_eAssocClass;
  
    i8  m_iCurrentSize;
  
    i8  m_iaWhenMagn_first[C_BALLOON_MAGN];
    i8  m_iaWhenMagn_last[C_BALLOON_MAGN];

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void*);
  
    Balloon(void);
   ~Balloon() noexcept(false);
  
    void vAddSize(i8 iBy);
  
    eCN  eAssocClass(void);
    void vAssocClass(eCN eAssocClass);
  
    i8   iCurrentSize(void);
    void vCurrentSize(i8 iSize);

    void vNextSequence(void);
  
    i8   iWhenMagn_first(i8 i_Magn);
    i8   iWhenMagn_last(i8 i_Magn);

  private:

    Footer* aFooter(void);
  };
  
  Balloon* _pBalloon(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
