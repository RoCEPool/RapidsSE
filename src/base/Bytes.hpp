/*============================================================================*/
/* Bytes.hpp -- A class that maps over the Bytes_t templates                  */
/*============================================================================*/
#pragma once

namespace BASE
{
  class Bytes
  {
  private:
    Header m_xHeader;

    i8   m_nArea;        // byte length of byte area/array
    i8   m_i_Latter;     // offset of next available byte
    byte m_yaArea[1];    // byte area (_ya =: byte array)

    // m_xFooter the address of which is calculated
    
  public:
    void* operator new(size_t);

  private:
    Bytes(void);
   ~Bytes(void) noexcept(false);

  public:
    i8   iAppend(const Stz* szStr);
    i8   iAppend(i8 iVal);
    i8   iAppend(byte* pySrc, i8 nLen);
    i8   nArea(void);
    Stz* szArea(void);
    Stz* szBoundary(void);
    bool bContains(const Stz* szAddr);
    i8   iGet(i8 i_Area);
    Stz* szGet(i8 i_Area);
    byte* pyGet(i8 i_Area);
    void vGive(i8 nBytes);
    bool bIsEmpty(void);
    i8   i_Latter(void);
    Stz* szLatter(void);
    void vListAll(void);
    void vMoveLeft(Stz* szAtAddr, i8 nBytesRem);
    void vMoveRight(Stz* szAtAddr, i8 nBytesIns);
    void vPrintAll(void);
    i8   iRemaining(void);
    void vReset(void);
    i8   nSize(void);
    void vTake(i8 nBytes);
    void vTrim(Stz* szTrim);
    void vWipe(void);
    void vWipe(Stz* szAddr);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
