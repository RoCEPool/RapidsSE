/*============================================================================*/
/* String_d.hpp -- dynamic-length string                                      */
/*============================================================================*/
#pragma once

#include <cstdarg>
/*
 * String_d does not allocate space for itself.
 * 1) The caller must first call the static function iBytesToAlloc(nBytes),
 * where nBytes is equal to the max string size that they want (including null term).
 * This function returns the total number of bytes that String_d will occupy.
 * For example, a string of size 4 bytes needs 48 bytes of space.
 * string of size 32 bytes needs 64 bytes of space, and so on.
 *
 * 2) The caller must allocate space and provide the address to this class.
 * String_d* pStringd = new(aAddr) String_d(nBytes)
 * where aAddr is the starting address of the allocated space,
 * and nBytes is the max size of the string.
 * Note: nBytes is NOT the total size returned by iBytesToAlloc(), but rather,
 * the number of bytes given to iBytesToAlloc().
 *
 */
namespace BASE
{
  class String_d : public Object
  {
  private:
    i8   m_nArea;             // byte length of char area/array
    char m_caCharArea[16];    // minimum of 16 bytes

    // ..CharArea..
    // Char area can extend here till footer


    // Last 16 bytes are the footer
  public:
    void* operator new(size_t, void* pArea);
    String_d(i8 nBytes);
    ~String_d(void) noexcept(false);

    static i8 iBytesToAlloc(i8 nBytes);

    i8   nAreaBytes(void);
    bool bBeginsWith(Stz* szArg);
    char* azChar(void);
    bool bContains(Stz* szArg);
    void vCopy(Stz* szArg);
    void vCopy(Stz* szArg, i8 iLen);
    bool bEqual(Stz* szArg);
    i8   nFreeBytes(void);
    bool bIsEmpty(void);
    bool bIsInteger(void);
    i8   nLength(void);
    void vReset(void);
    bool bSame(Stz* szArg);
    i8   nSize(void);
    i8   nSpace(void);
    Stz* szStz(void);
    i8   iTotalSize(void);
    void vZero(void);

    void vSetFooter(void);
    Footer* aFooter(void);
  };

  void* _pStrd(void* aAddr);
}

using namespace BASE;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
