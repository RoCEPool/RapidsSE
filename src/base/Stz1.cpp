

#include "BASE.h"
#if 0
//template <class T>
//T Number<T>::

//===========================================
//_LIBCPP_CONSTEXPR _LIBCPP_INLINE_VISIBILITY
// Stz1<char>::Stz1() _NOEXCEPT : m_pData(nullptr), m_iDataSize(0) {}

template<class _CharT, class _Traits >  Stz1<_CharT>::Stz1() _NOEXCEPT : m_pData(nullptr), m_iDataSize(0)  {}

_LIBCPP_CONSTEXPR  _LIBCPP_INLINE_VISIBILITY
 Stz1<_CharT, _Traits>::Stz1(const Stz1&) _NOEXCEPT = default;

_LIBCPP_CONSTEXPR _LIBCPP_INLINE_VISIBILITY
     template<class _CharT, class _Traits > Stz1& Stz1<_CharT, _Traits>::operator=(const Stz1&) _NOEXCEPT = default;

_LIBCPP_CONSTEXPR _LIBCPP_INLINE_VISIBILITY
template<class _CharT, class _Traits > Stz1<_CharT, _Traits>::Stz1(const _CharT* __s, size_type __len) _NOEXCEPT
: m_pData(__s), m_iDataSize(__len)
{
#if _LIBCPP_STD_VER > 11
  _LIBCPP_ASSERT(__len == 0 || __s != nullptr, "string_view::string_view(_CharT *, size_t): received nullptr");
#endif
}



/*----------------------------------------------------------------------------*/
/* get as char*                                                               */
/*----------------------------------------------------------------------------*/
value_type* Stz1::azChar(void) const _NOEXCEPT  { return (char*)m_pData;  }

size_type Stz1::iSize()     const _NOEXCEPT { return m_iDataSize; }

size_type Stz1::iMaxSize() const _NOEXCEPT { return std::numeric_limits<size_type>::max() / sizeof(value_type); }



/*----------------------------------------------------------------------------*/
/* iLength: get length in bytes of arg (not including terminating null)       */
/*----------------------------------------------------------------------------*/
size_type Stz1::nLength(void) const _NOEXCEPT { return m_iDataSize; }


/*----------------------------------------------------------------------------*/
/* bIsEmpty: test if this Stz is "" (NB: not a null ptr)                      */
/*----------------------------------------------------------------------------*/
bool Stz1::bIsEmpty(void) const _NOEXCEPT { return m_iDataSize == 0; }

/*----------------------------------------------------------------------------*/
/* bIsInteger: is the string an integer digit sequence ?                      */
/*----------------------------------------------------------------------------*/
bool Stz1::bIsInteger() const _NOEXCEPT
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

/*----------------------------------------------------------------------------*/
/* get ordinal value                                                          */
/*----------------------------------------------------------------------------*/
size_type Stz1::iOrdinal(void) const _NOEXCEPT  {    size_type iOrdinal = StrLib::iToI8(m_pData);    return iOrdinal;  }



_LIBCPP_CONSTEXPR _LIBCPP_INLINE_VISIBILITY
const value_type* Stz1::pData() const _NOEXCEPT { return m_pData; }


Stz1 Stz1::substr(size_type __pos = 0, size_type __n = npos) const
{
  return __pos > size()
         ? printf("string_view::substr out of range\n")
         : Stz1(data() + __pos, _VSTD::min(__n, size() - __pos));
}

Stz1<char> BASE::operator "" _Stz1(const char *__str, size_t __len)
{
  return Stz1<char> (__str, __len);
}

#endif

//template <class T>
//T Number<T>::

/*
constexpr
Stz1<char> BASE::operator "" _Stz1(const char *pszStr, size_t iLen)
{
  return Stz1<char> (pszStr, iLen);
}

constexpr
Stz1<wchar_t> operator "" _Stz1(const wchar_t *pszStr, size_t iLen)
{
return Stz1<wchar_t> (pszStr, iLen);
}
 */

constexpr
Stz1<char16_t> operator "" _Stz1(const char16_t *pszStr, size_t iLen)
{
return Stz1<char16_t> (pszStr, iLen);
}

constexpr
Stz1<char32_t> operator "" _Stz1(const char32_t *pszStr, size_t iLen)
{
return Stz1<char32_t> (pszStr, iLen);
}
