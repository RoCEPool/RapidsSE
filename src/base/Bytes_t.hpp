/*============================================================================*/
/* Bytes_t.hpp -- fixed-length byte array variable (template)                 */
/*============================================================================*/
#pragma once

namespace BASE
{
  template <i8 AREA>
  class Bytes_t : public Object
  {
  private:
    i8   m_nArea;        // byte length of byte area/array
    i8   m_i_Latter;     // offset of next available byte
    byte m_yaArea[AREA]; // byte area (_ya =: byte array)
    
    // must be last
    Footer m_xFooter;
    
  public:    
    Bytes_t(void);
   ~Bytes_t(void) noexcept(false);
  
    i8   iAppend(Stz* szStr);
    i8   nArea(void);
    Stz* szArea(void);
    Stz* szBoundary(void);
    Stz* szGet(i8 i_Area);
    void vGive(i8 nBytes);
    i8   i_Latter(void);
    Stz* szLatter(void);
    void vListAll(void);
    void vMoveLeft(Stz* szAtAddr, i8 nBytesRem);
    void vMoveRight(Stz* szAtAddr, i8 nBytesIns);
    void vPrintAll(void);
    void vReset(void);
    i8   nSize(void);
    void vTake(i8 nBytes);
    void vTrim(Stz* szTrim);
  
    Footer* aFooter(void);
  };
  
  void* _pBytes_t(void* aAddr);
}

using namespace BASE;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
Bytes_t<AREA>::Bytes_t(void)
: Object(CN_BYTES_T)
{
  _precondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
  
  m_i_Latter = 0;
  
  m_nArea = AREA;
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_yaArea,AREA,CN_BYTES_T);
#else
  m_yaArea[0] = '\0';
#endif

  _checkInitialisedSize_(AREA);
  
  _postcondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
Bytes_t<AREA>::~Bytes_t(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
  
  _postcondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append given string                                                        */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
i8 Bytes_t<AREA>::iAppend(Stz* szStr)
{
  _precondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
  
  i8 i_String = m_i_Latter;
  
  for (byte* ayByte = (byte*) szStr; * ayByte; )
    m_yaArea[m_i_Latter++] = * ayByte++;

  m_yaArea[m_i_Latter++] = '\0';

  _postcondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
  
  return i_String;
}

/*----------------------------------------------------------------------------*/
/* return m_nArea                                                             */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
i8 Bytes_t<AREA>::nArea(void)
{
  _precondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
  
  ASSERT(AREA == m_nArea);
    
  return m_nArea;
}

/*----------------------------------------------------------------------------*/
/* get get the address of the byte area                                       */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
Stz* Bytes_t<AREA>::szArea(void)
{
  _precondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
  
  char* azStr = (char*) m_yaArea;
  
  return (Stz*)azStr;
}

/*----------------------------------------------------------------------------*/
/* get last accessible byte in the byte area                                  */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
Stz* Bytes_t<AREA>::szBoundary(void)
{
  _precondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
  
  char* azStr = (char*) &m_yaArea[AREA - 1];
  
  return (Stz*)azStr;
}

/*----------------------------------------------------------------------------*/
/* get address at given offset in m_yaArea                                    */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
Stz* Bytes_t<AREA>::szGet(i8 i_Area)
{
  _precondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
  
  char* azStr = (char*) &m_yaArea[i_Area];
  
  return (Stz*)azStr;
}

/*----------------------------------------------------------------------------*/
/* give nBytes back to latter area                                            */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void Bytes_t<AREA>::vGive(i8 nBytes)
{
  _precondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
  
  m_i_Latter -= nBytes;
}

/*----------------------------------------------------------------------------*/
/* get the offset of the latter portion                                       */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
i8 Bytes_t<AREA>::i_Latter(void)
{
  _precondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
    
  return m_i_Latter;
}

/*----------------------------------------------------------------------------*/
/* get address of the later portion                                           */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
Stz* Bytes_t<AREA>::szLatter(void)
{
  _precondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
  
  Stz* szLater = szGet(m_i_Latter);
  
  return szLater;
}

/*----------------------------------------------------------------------------*/
/* list all embedded strings                                                  */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void Bytes_t<AREA>::vListAll(void)
{
  _precondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
  
  byte* ayWild = & m_yaArea[m_i_Latter];
  
  bool bMarkup = true;

  i8 i_String = 0;
  for (byte* ayByte = m_yaArea; ayByte < ayWild; ayByte++) {
  
    if (bMarkup) {
      bMarkup = false;
      i8 nLen = StrLib::nLength((char*)ayByte);
      vTrace("[%02lld-%04lld+1B] %p : "_Stz,i_String,nLen,ayByte);
    }

    if (* ayByte) {
      vTrace("%c"_Stz,* ayByte);
      continue;
    }
    
    vTrace("\n"_Stz);
    bMarkup = true;
    i_String++;
  }
}

/*----------------------------------------------------------------------------*/
/* cut nBytes starting at given offset                                        */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void Bytes_t<AREA>::vMoveLeft(Stz* szAtAddr, i8 nBytesRem)
{
  _precondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
  
  Stz* szDst = szAtAddr;
  Stz* szSrc = aAdd<Stz*>(szAtAddr,nBytesRem);
  
  Stz* szLat = szLatter();
  
  i8 nBytesMove = nDifference(szSrc,szLat);
  
  //vListAll();
  //vTrace("\nMoving to %p from %p -- %lld bytes\n\n"_Stz,szDst,szSrc,nBytesMove);
  
  MemLib::vMove(szDst,szSrc,nBytesMove,CN_BYTES_T);
  
  m_i_Latter -= nBytesRem;
  
  //vListAll();
}

/*----------------------------------------------------------------------------*/
/* insert space for n-bytes starting at given offset                          */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void Bytes_t<AREA>::vMoveRight(Stz* szAtAddr, i8 nBytesIns)
{
  _precondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
  
  Stz* szSrc = szAtAddr;
  Stz* szDst = aAdd<Stz*>(szAtAddr,nBytesIns);
  
  Stz* szLat = szLatter();
  
  i8 nBytesMove = nDifference(szSrc,szLat);
  
  //vListAll();
  //vTrace("Moving to %p from %p -- %lld bytes\n"_Stz,szDst,szSrc,nBytesMove);
  
  MemLib::vMove(szDst,szSrc,nBytesMove,CN_BYTES_T);
  
  m_i_Latter += nBytesIns;
}

/*----------------------------------------------------------------------------*/
/* print all embedded strings                                                 */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void Bytes_t<AREA>::vPrintAll(void)
{
  _precondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
  
  for (byte* ayByte = m_yaArea; ayByte < & m_yaArea[m_i_Latter]; ayByte++)
    if (* ayByte)
      vTrace("%c"_Stz,* ayByte);
    else
    if (ayByte + 1 < & m_yaArea[m_i_Latter])
      vTrace(","_Stz);

  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void Bytes_t<AREA>::vReset(void)
{
  _precondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
  
  m_i_Latter = 0;
}

/*----------------------------------------------------------------------------*/
/* get size of the area in bytes                                              */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
i8 Bytes_t<AREA>::nSize(void)
{
  _precondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
  
  return AREA;
}

/*----------------------------------------------------------------------------*/
/* take nBytes from latter area                                               */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void Bytes_t<AREA>::vTake(i8 nBytes)
{
  _precondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
  
  m_i_Latter += nBytes;
}

/*----------------------------------------------------------------------------*/
/* reset latter to given argument                                             */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void Bytes_t<AREA>::vTrim(Stz* szTrim)
{
  _precondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
  
  m_i_Latter = nDifference(szArea(),szTrim);
  
  m_yaArea[m_i_Latter] = '\0';
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
Footer* Bytes_t<AREA>::aFooter(void)
{
  _precondition_(_MAGIC_,CN_BYTES_T);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
