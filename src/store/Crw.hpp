/*============================================================================*/
/* Crw.hpp -- Compact Row                                                     */
/*============================================================================*/
#pragma once

namespace STORE
{

  static const i8 C_CRW_HEADBYTES = 32;

  static const i8 C_CRW_FIRSTHEAD  = 0x290;

  class Crw : public HeadLine
  {
  public:

  public:
    Crw(void);
   ~Crw(void) noexcept(false);
  
    static void vDumpCrw(Stz* szTitle, HeadLine* aHeadLine, StreamDef* pStreamDef);
    static void vDumpCrw_v2(Stz* szTitle, HeadLine* aHeadLine, StreamDef* pStreamDef);
  };

  /*--------------------------------------------------------------------------*/
  /* safe cast                                                                */
  /*--------------------------------------------------------------------------*/
  inline Crw* _aCrw(void* aAddr)
  {
    Crw* aCrw = reinterpret_cast<Crw*>(aAddr);
    
    return aCrw;
  }


  //----------------------------------------------------------------------------


  static const i8 C_TRAY_ROWS = (C_SIZE_LARGE_OBJECT - C_1KB) / sizeof(HeadLine);

  typedef    Nub_t <CN_ROWTRAY,HeadLine,OverLine,TailLine,StreamDef> RowNub;

  typedef TrayNL_t <CN_ROWTRAY,HeadLine,OverLine,TailLine,StreamDef,
                    (C_TRAY_ROWS * sizeof(HeadLine))>               RowTray;

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
