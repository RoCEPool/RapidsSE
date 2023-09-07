

#pragma once


namespace BASE
{

  template<class _CharT, class _Traits = std::char_traits<_CharT>>
  class Stz1 {

  public:
    using value_type            = _CharT;
    using size_type             = size_t;
    using traits_type		        = _Traits;
    static constexpr size_type npos = size_type(-1);

    constexpr
    Stz1() _NOEXCEPT : m_pData(nullptr), m_iDataSize(0) {}

    constexpr
    Stz1(const Stz1&) _NOEXCEPT = default;

    constexpr
    Stz1& operator=(const Stz1&) _NOEXCEPT = default;

    constexpr
     Stz1(const _CharT* pChar, size_type iLen) _NOEXCEPT
        : m_pData(pChar), m_iDataSize(iLen)
    {
//#if _LIBCPP_STD_VER > 11
//      _LIBCPP_ASSERT(iLen == 0 || pChar != nullptr, "Stz1::Stz1(_CharT *, size_t): received nullptr");
//#endif
    }

    constexpr
    Stz1(const _CharT* pChar) _NOEXCEPT
        : m_pData(pChar), m_iDataSize(traits_type::length(pChar))
    { }


    // public methods
    constexpr char* azChar(void) const _NOEXCEPT  { return (char*)m_pData;  }

    //i8 iCompare(Stz1 szArg);
    //{      i8 iCmp = StrLib::iCompare(azChar(),szArg.azChar(),C_NO_CASE);      return iCmp;    }

    constexpr size_type
    iCompare(Stz1<_CharT> stzb) const noexcept
    {
      size_type iLen = std::min(iSize(), stzb.iSize());
      int iVal = _Traits::compare(pData(), stzb.pData(), iLen);
      if (iVal == 0)
        iVal = iSize() == stzb.iSize() ? 0 : (iSize() < stzb.iSize() ? -1 : 1);
      return iVal;
    }

    constexpr
        size_type iCompare(const _CharT* pC) const _NOEXCEPT
    {
      Stz1 <_CharT> Stz(pC);
      return iCompare(Stz);
    }

    constexpr
    size_type     iCompare(size_type iPos, size_type iLen, Stz1 Stz2) const
    {
      return this->stzSubstr(iPos, iLen).compare(Stz2);
    }

    //constexpr bool
    //operator==(Stz1<_CharT> Stz1a,
    //           Stz1<_CharT> Stz1b) noexcept
    //{ return Stz1a.iSize() == Stz1b.iSize() && Stz1a.iCompare(Stz1b) == 0; }




    constexpr size_type iSize() const _NOEXCEPT
    {
      if (m_pData!=nullptr && m_iDataSize == 0 )
        return (size_type) strlen(m_pData);
      return m_iDataSize;
    }

    constexpr size_type iMaxSize() const _NOEXCEPT { return std::numeric_limits<size_type>::max() / sizeof(value_type); }

    constexpr size_type nLength(void) const _NOEXCEPT { return m_iDataSize; }

    constexpr bool bIsEmpty(void) const _NOEXCEPT { return m_iDataSize == 0; }

    constexpr bool bIsInteger() const _NOEXCEPT
    {
      if ('-' == *m_pData)
        (m_pData)++;

      if ( ! *m_pData )
        return false;

      for ( ; *m_pData; (m_pData)++)
        if (!isdigit(*m_pData))
          return false;

      return true;
    }


    const value_type* pData() const _NOEXCEPT { return m_pData; }

    constexpr size_type iOrdinal(void) const _NOEXCEPT  {    size_type iOrdinal = iToI8(m_pData);    return iOrdinal;  }

    constexpr Stz1 sztSubstr(size_type iPos = 0, size_type iLen = npos) const
    {
      return iPos > iSize()
             ? printf("string_view::substr out of range\n")
             : Stz1(pData() + iPos, _VSTD::min(iLen, iSize() - iPos));
    }

    constexpr i8 iToI8(const char* s)
    {
      i8 i = atoll(s);

      return i;

      /* i8 i = 0;
      bool bNegative = false;

      if (*s && '-' == *s) {
        bNegative = true;
        s++;
      }

      // skip leading 0's
      while (*s && '0' == *s)
        s++;

      if (*s && ('x' == *s || 'X' == *s)) {
        s++;
        while (*s && bIsHex(*s)) {
          i8 j = iHexToInw(*s++);
          i = i * 16 + j;
        }
      }
      else {
        while (*s && bIsDigit(*s)) {
          i8 j = (*s++ - '0');
          i = i * 10 + j;
        }
      }

      if (bNegative)
        i *= -1;

      return i; */
    }

    constexpr size_type
    iCopy(_CharT* pChar, size_type iLen, size_type iPos = 0) const
    {
      //__glibcxx_requires_string_len(pChar, iLen);

      const size_type iRlen = std::min<size_t>(iLen, iSize() - iPos);

      traits_type::copy(pChar, pData() + iPos, iRlen);
      return iRlen;
    }



  private:
    const   value_type* m_pData;
    size_type           m_iDataSize;
  };


  inline constexpr
  Stz1<char> operator "" _Stz1(const char *__str, size_t __len)
  {
    return Stz1<char> (__str, __len);
  }

  inline constexpr
  Stz1<wchar_t> operator "" _Stz1(const wchar_t *pszStr, size_t iLen)
  {
    return Stz1<wchar_t> (pszStr, iLen);
  }



  constexpr
  Stz1<char16_t> operator "" _Stz1(const char16_t *pszStr, size_t iLen);

  constexpr
  Stz1<char32_t> operator "" _Stz1(const char32_t *pszStr, size_t iLen) ;





}



