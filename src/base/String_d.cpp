/*============================================================================*/
/* String_d.cpp -- dynamic-length string                                      */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
void* BASE::_pStrd(void* aAddr)
{
  Object* pStrVar = reinterpret_cast<Object*>(aAddr);

  RASSERT_TYPE(pStrVar,CN_STRING_D);

  return pStrVar;
}


/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* String_d::operator new(size_t, void* pArea)
{
  return pArea;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
String_d::String_d(i8 nBytes)
    : Object(CN_STRING_D)
{
  _precondition_(_MAGIC_,CN_STRING_D);

  m_nArea = nBytes;
  m_caCharArea[0] = '\0';
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caCharArea,nBytes,CN_STRING_D);
#endif
  i8 iTotalSizeOfStz = iBytesToAlloc(nBytes);
  Footer* pFooter;
  i8 iFooterOffset = iTotalSizeOfStz - sizeof(Footer);
  pFooter = aAdd<Footer*>(this, iFooterOffset);

  pFooter->vSetFooter();
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STRING_D,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
String_d::~String_d(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_STRING_D,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* return the number of bytes to allocate for a string of a given size        */
/*----------------------------------------------------------------------------*/
i8 String_d::iBytesToAlloc(i8 nBytes)
{
  _static_
  i8 iTotalSize = 0;
  if (nBytes <= 16) {
    iTotalSize = sizeof(String_d) + sizeof(Footer);
  } else {
    i8 iQuotent = nBytes / 16;
    i8 iRemainder = nBytes % 16;

    if (iRemainder == 0) {
      iQuotent--;
      iTotalSize = sizeof(String_d) + sizeof(Footer) + (16 * iQuotent);
    } else {
      iTotalSize = sizeof(String_d) + sizeof(Footer) + (16 * iQuotent);
    }
  }

  RASSERT(iTotalSize % 16 == 0);
  return iTotalSize;
}
/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get size of data area                                                      */
/*----------------------------------------------------------------------------*/
i8 String_d::nAreaBytes(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  return m_nArea;
}

/*----------------------------------------------------------------------------*/
/* test if arg is a leading sub-string of this string                         */
/*----------------------------------------------------------------------------*/
bool String_d::bBeginsWith(Stz* szArg)
{
  _precondition_(_MAGIC_,_FOOTER_);

  return StrLib::bBeginsWith(m_caCharArea,szArg->azChar());
}

/*----------------------------------------------------------------------------*/
/* get char pointer to str area                                               */
/*----------------------------------------------------------------------------*/
char* String_d::azChar(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  return (char*)m_caCharArea;
}

/*----------------------------------------------------------------------------*/
/* test if arg is a sub-string in this string                                 */
/*----------------------------------------------------------------------------*/
bool String_d::bContains(Stz* szArg)
{
  return StrLib::bContains(m_caCharArea,szArg->azChar());
}

/*----------------------------------------------------------------------------*/
/* copy given stz* into this string (silent truncate)                         */
/*----------------------------------------------------------------------------*/
void String_d::vCopy(Stz* szArg)
{
  _precondition_(_MAGIC_,_FOOTER_);

  // Copy over max dstbufsize - 1, for the null char
  StrLib::vCopyLen(m_caCharArea, m_nArea, szArg->azChar(), m_nArea-1);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* copy given stz* up to a given length into this string                      */
/*----------------------------------------------------------------------------*/
void String_d::vCopy(Stz* szArg, i8 iLen)
{
  _precondition_(_MAGIC_,_FOOTER_);

  StrLib::vCopyLen(m_caCharArea, m_nArea, szArg->azChar(), iLen);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* is equal to given argument? (same case)                                    */
/*----------------------------------------------------------------------------*/
bool String_d::bEqual(Stz* szArg)
{
  _precondition_(_MAGIC_,_FOOTER_);

  bool b = StrLib::bEqualTo(m_caCharArea,(const char*)szArg);

  return b;
}

/*----------------------------------------------------------------------------*/
/* get size of data area                                                      */
/*----------------------------------------------------------------------------*/
i8 String_d::nFreeBytes(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  i8 nFreeBytes = m_nArea - nLength();

  return nFreeBytes;
}


/*----------------------------------------------------------------------------*/
/* is this an empty string (i.e. "". Not to be confused with a null ptr)      */
/*----------------------------------------------------------------------------*/
bool String_d::bIsEmpty(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  return m_caCharArea[0] == 0;
}

/*----------------------------------------------------------------------------*/
/* is this string an integer?                                                 */
/*----------------------------------------------------------------------------*/
bool String_d::bIsInteger(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  bool b = StrLib::bIsInteger(m_caCharArea);

  return b;
}

/*----------------------------------------------------------------------------*/
/* get length of this str                                                     */
/*----------------------------------------------------------------------------*/
i8 String_d::nLength(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  return StrLib::nLength(azChar());
}

/*----------------------------------------------------------------------------*/
/* reset string                                                               */
/*----------------------------------------------------------------------------*/
void String_d::vReset(void)
{
  m_caCharArea[0] = '\0';
}

/*----------------------------------------------------------------------------*/
/* is same as given argument?  (ignores case)                                 */
/*----------------------------------------------------------------------------*/
bool String_d::bSame(Stz* szArg)
{
  _precondition_(_MAGIC_,_FOOTER_);

  bool b = StrLib::bSame(m_caCharArea,(const char*)szArg);

  return b;
}

/*----------------------------------------------------------------------------*/
/* get size of str in data area                                               */
/*----------------------------------------------------------------------------*/
i8 String_d::nSize(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  return StrLib::nSize(azChar());
}

/*----------------------------------------------------------------------------*/
/* get space of data area                                                     */
/*----------------------------------------------------------------------------*/
i8 String_d::nSpace(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  return m_nArea;
}

/*----------------------------------------------------------------------------*/
/* get m_caCharArea                                                           */
/*----------------------------------------------------------------------------*/
Stz* String_d::szStz(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  return (Stz*)m_caCharArea;
}

/*----------------------------------------------------------------------------*/
/* Calcualte the total size of this string_d                                  */
/*----------------------------------------------------------------------------*/
i8 String_d::iTotalSize(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  return iBytesToAlloc(m_nArea);
}

/*----------------------------------------------------------------------------*/
/* zero the string area                                                       */
/*----------------------------------------------------------------------------*/
void String_d::vZero(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  MemLib::vZero(m_caCharArea,m_nArea,CN_STRING_D);

  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* String_d::aFooter(void)
{
  _precondition_(_MAGIC_,CN_STRING_D);

  i8 iTotalSize = iBytesToAlloc(m_nArea);
  i8 iFooterOffset = iTotalSize - sizeof(Footer);

  Footer* aFooter = aAdd<Footer*>(this,iFooterOffset);

  return aFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/