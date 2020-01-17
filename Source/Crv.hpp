/*============================================================================*/
/* Crw.hpp -- Consistent Row Version                                          */
/*============================================================================*/
#pragma once

namespace ReadConsistency
{
  class Crv
  {
  public:
    i8  m_nCrvSize; // size of this crv in bytes
  
    Ref m_xwRef;    // the reference of the current row in a CrwBuffer
  
    Crw m_xCrw_RC;  // read consistent version version of crw

  public:
    Crv(void);
   ~Crv(void) noexcept(false);
  
    static i8 nCrvBytesNeed(i8 nCrwBytes);
  
    static void vFormKey(EntWrk* aEntWrk, Ref xwRef, t8 tPerspective);
  
    Crw* aCrw(void);
    Ref  xwRef(void);

    void vTakeVersion(Ref xwRef, Crw* aCrw, i8 nCrwBytes);
  };

  /*--------------------------------------------------------------------------*/
  /* safe cast                                                                */
  /*--------------------------------------------------------------------------*/
  inline Crv* _aCrv(void* aAddr)
  {
    Crv* aCrv = reinterpret_cast<Crv*>(aAddr);
    
    return aCrv;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
