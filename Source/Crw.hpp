/*============================================================================*/
/* Crw.hpp -- Compact Row                                                     */
/*============================================================================*/
#pragma once

namespace Rows
{
  class Crw : public HeadLine
  {
  public:

  public:
    Crw(void);
    ~Crw(void) noexcept(false);
  
    static void vDumpCrw(Stz* szTitle, HeadLine* aHeadLine, ColumnVec* pColVec);
    static void vDumpCrw_IV(Stz* szTitle, HeadLine* aHeadLine, ColumnVec* pColVec);

  };

  /*--------------------------------------------------------------------------*/
  /* safe cast                                                                */
  /*--------------------------------------------------------------------------*/
  inline Crw* _aCrw(void* aAddr)
  {
    Crw* aCrw = reinterpret_cast<Crw*>(aAddr);
    
    return aCrw;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
