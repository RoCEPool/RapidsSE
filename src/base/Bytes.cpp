/*============================================================================*/
/* Bytes.hpp -- A class that maps over the Bytes_t templates                  */
/*============================================================================*/

#include "BASE.h"
#include "Bytes.hpp"

using namespace BASE;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Bytes::operator new(size_t)
{
  vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
         "fatal: Bytes class cannot be new'd\n"_Stz);

  NEVER_REACH;
}


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Bytes::Bytes(void)
{
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Bytes::~Bytes(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append given string                                                        */
/*----------------------------------------------------------------------------*/
i8 Bytes::iAppend(Stz* szStr)
{
  m_xHeader.vAssertHeader();
  aFooter()->vAssertFooter();

  i8 i_String = m_i_Latter;

  for (byte* ayByte = (byte*) szStr; * ayByte; )
    m_yaArea[m_i_Latter++] = * ayByte++;

  m_yaArea[m_i_Latter++] = '\0';

  m_xHeader.vAssertHeader();
  aFooter()->vAssertFooter();

  return i_String;
}

/*----------------------------------------------------------------------------*/
/* return m_nArea                                                             */
/*----------------------------------------------------------------------------*/
i8 Bytes::nArea(void)
{
  m_xHeader.vAssertHeader();
  aFooter()->vAssertFooter();

  return m_nArea;
}

/*----------------------------------------------------------------------------*/
/* get get the address of the byte area                                       */
/*----------------------------------------------------------------------------*/
Stz* Bytes::szArea(void)
{
  m_xHeader.vAssertHeader();
  aFooter()->vAssertFooter();

  char* azStr = (char*) m_yaArea;

  return (Stz*)azStr;
}

/*----------------------------------------------------------------------------*/
/* get last accessible byte in the byte area                                  */
/*----------------------------------------------------------------------------*/
Stz* Bytes::szBoundary(void)
{
  m_xHeader.vAssertHeader();
  aFooter()->vAssertFooter();

  char* azStr = (char*) &m_yaArea[m_nArea - 1];

  return (Stz*)azStr;
}

/*----------------------------------------------------------------------------*/
/* returns true if the given address is within this object                    */
/*----------------------------------------------------------------------------*/
bool Bytes::bContains(Stz* szAddr)
{
  m_xHeader.vAssertHeader();
  aFooter()->vAssertFooter();

  ASSERT(szAddr);

  return (szAddr >= szArea() && szAddr <= szBoundary());
}

/*----------------------------------------------------------------------------*/
/* get address at given offset in m_yaArea                                    */
/*----------------------------------------------------------------------------*/
Stz* Bytes::szGet(i8 i_Area)
{
  m_xHeader.vAssertHeader();
  aFooter()->vAssertFooter();

  char* azStr = (char*) &m_yaArea[i_Area];

  return (Stz*)azStr;
}

/*----------------------------------------------------------------------------*/
/* give nBytes back to latter area                                            */
/*----------------------------------------------------------------------------*/
void Bytes::vGive(i8 nBytes)
{
  m_xHeader.vAssertHeader();
  aFooter()->vAssertFooter();

  m_i_Latter -= nBytes;
}

/*----------------------------------------------------------------------------*/
/* give nBytes back to latter area                                            */
/*----------------------------------------------------------------------------*/
bool Bytes::bIsEmpty(void)
{
  m_xHeader.vAssertHeader();
  aFooter()->vAssertFooter();

  return m_i_Latter == 0;
}

/*----------------------------------------------------------------------------*/
/* get the offset of the latter portion                                       */
/*----------------------------------------------------------------------------*/
i8 Bytes::i_Latter(void)
{
  m_xHeader.vAssertHeader();
  aFooter()->vAssertFooter();

  return m_i_Latter;
}

/*----------------------------------------------------------------------------*/
/* get address of the later portion                                           */
/*----------------------------------------------------------------------------*/
Stz* Bytes::szLatter(void)
{
  m_xHeader.vAssertHeader();
  aFooter()->vAssertFooter();

  Stz* szLater = szGet(m_i_Latter);

  return szLater;
}

/*----------------------------------------------------------------------------*/
/* list all embedded strings                                                  */
/*----------------------------------------------------------------------------*/
void Bytes::vListAll(void)
{
  m_xHeader.vAssertHeader();
  aFooter()->vAssertFooter();

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
void Bytes::vMoveLeft(Stz* szAtAddr, i8 nBytesRem)
{
  m_xHeader.vAssertHeader();
  aFooter()->vAssertFooter();

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
void Bytes::vMoveRight(Stz* szAtAddr, i8 nBytesIns)
{
  m_xHeader.vAssertHeader();
  aFooter()->vAssertFooter();

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
void Bytes::vPrintAll(void)
{
  m_xHeader.vAssertHeader();
  aFooter()->vAssertFooter();

  for (byte* ayByte = m_yaArea; ayByte < & m_yaArea[m_i_Latter]; ayByte++)
    if (* ayByte)
      vTrace("%c"_Stz,* ayByte);
    else
    if (ayByte + 1 < & m_yaArea[m_i_Latter])
      vTrace(","_Stz);

  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* return the remaining space                                                 */
/*----------------------------------------------------------------------------*/
i8 Bytes::iRemaining(void)
{
  m_xHeader.vAssertHeader();
  aFooter()->vAssertFooter();

  return m_nArea - i_Latter();
}

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void Bytes::vReset(void)
{
  m_xHeader.vAssertHeader();
  aFooter()->vAssertFooter();

  m_i_Latter = 0;
}

/*----------------------------------------------------------------------------*/
/* get size of the area in bytes                                              */
/*----------------------------------------------------------------------------*/
i8 Bytes::nSize(void)
{
  m_xHeader.vAssertHeader();
  aFooter()->vAssertFooter();

  return m_nArea;
}

/*----------------------------------------------------------------------------*/
/* take nBytes from latter area                                               */
/*----------------------------------------------------------------------------*/
void Bytes::vTake(i8 nBytes)
{
  m_xHeader.vAssertHeader();
  aFooter()->vAssertFooter();

  m_i_Latter += nBytes;
}

/*----------------------------------------------------------------------------*/
/* reset latter to given argument                                             */
/*----------------------------------------------------------------------------*/
void Bytes::vTrim(Stz* szTrim)
{
  m_xHeader.vAssertHeader();
  aFooter()->vAssertFooter();

  m_i_Latter = nDifference(szArea(),szTrim);

  m_yaArea[m_i_Latter] = '\0';
}

/*----------------------------------------------------------------------------*/
/* wipe all data in the buffer space                                          */
/*----------------------------------------------------------------------------*/
void Bytes::vWipe(void)
{
  m_xHeader.vAssertHeader();
  aFooter()->vAssertFooter();

  MemLib::vZero(m_yaArea,m_nArea,CN_BYTES_T);
}

/*----------------------------------------------------------------------------*/
/* wipe all data in the buffer space                                          */
/*----------------------------------------------------------------------------*/
void Bytes::vWipe(Stz* szAddr)
{
  m_xHeader.vAssertHeader();
  aFooter()->vAssertFooter();

  ASSERT(szAddr >= this->szArea());
  ASSERT(szAddr <= this->szBoundary());

  i8 nLen = szAddr->nLength();
  MemLib::vZero(szAddr,nLen,CN_BYTES_T);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Bytes::aFooter(void)
{
  m_xHeader.vAssertHeader();

  i8 nSizeHeaderBody = sizeof(m_xHeader)
                       + sizeof(m_nArea)
                       + sizeof(m_i_Latter)
                       + m_nArea;

  Footer* aFooter = aAdd<Footer*>(this,nSizeHeaderBody);

  return aFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
