/*============================================================================*/
/* StrLib.cpp -- char* string utility                                         */
/*============================================================================*/

#include "Base.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* wAllocate: allocate a workspace/string of given size                       */
/*----------------------------------------------------------------------------*/
char* StrLib::wAllocate(i8 nBytes)
{
  char* w = static_cast<char*>(malloc(nBytes));
  *w = '\0';
  return w;
}

/*----------------------------------------------------------------------------*/
/* wAllocate: allocate a workspace/string and initialize with given string    */
/*----------------------------------------------------------------------------*/
char* StrLib::wAllocate(const char* s)
{
  i8 nBytes = iSize(s);
  
  char* w = wAllocate(nBytes);
  
  vCopy(w,s);
  
  return w;
}

/*----------------------------------------------------------------------------*/
/* vAppendChar: append character to end of workspace                          */
/*----------------------------------------------------------------------------*/
void StrLib::vAppendChar(char** pz, char cChar)
{
  *(*pz)++ = cChar;
}

/*----------------------------------------------------------------------------*/
/* vAppendDecimal: append number in decimal to workspace                      */
/*----------------------------------------------------------------------------*/
void StrLib::vAppendDecimal(char** pz, i8 iNumber)
{
  char* pBwd = *pz;
  char* pFwd = *pz;
  
  i8 v = iNumber;
  
  do
  {
    *pBwd++ = (v % 10) + '0';
    v /= 10;
  } while (v);
  
  char* pEnd = pBwd;
  
  while (--pBwd > pFwd)
  {
    char cTmp = *pBwd;
    *pBwd = *pFwd;
    *pFwd++ = cTmp;
  }
  
  *pz = pEnd;
}

/*----------------------------------------------------------------------------*/
/* vAppendHex: append number in hexadecimal to workspace                      */
/*----------------------------------------------------------------------------*/
void StrLib::vAppendHex(char** pz, i8 iNumber)
{
  char* pBwd = *pz;
  char* pFwd = *pz;
  
  i8 v = iNumber;
  
  do {
    *pBwd++ = cToHex(v % 16);
    v /= 16;
  } while (v);
  
  char* pEnd = pBwd;
  
  while (--pBwd > pFwd)
  {
    char cTmp = *pBwd;
    *pBwd = *pFwd;
    *pFwd++ = cTmp;
  }
  
  *pz = pEnd;
}

/*----------------------------------------------------------------------------*/
/* vAppendNull: append null character to end of workspace                     */
/*----------------------------------------------------------------------------*/
void StrLib::vAppendNull(char** pz)
{
  *(*pz)++ = '\0';
}
  
/*----------------------------------------------------------------------------*/
/* vAppendString: append string to workspace                                  */
/*----------------------------------------------------------------------------*/
void StrLib::vAppendString(char** pz, const char* pString)
{
  char *z = *pz;
  
  while (*pString)
    *z++ = *pString++;
  
  *z = '\0';
  
  *pz = z;
}

/*----------------------------------------------------------------------------*/
/* azAscii: get 3-letter ascii abbreviation for char                          */
/*----------------------------------------------------------------------------*/
char* StrLib::azAscii(i8 i_Char)
{
  return (char*)szaAscii[i_Char];
}

/*----------------------------------------------------------------------------*/
/* bBackward: scan backwards to find given character                          */
/*----------------------------------------------------------------------------*/
bool StrLib::bBackward(const char** ps, char* pBegin, const char c)
{
  const char *p = *ps;
  
//  do
//  {
//    if (*p == c)
//    {
//      *ps = p;
//      return true;
//    }
//  } while (p-- != pBegin);
//

  while (*p-- != c)
  {
    if (p == pBegin)
      return false;
  }
  
  *ps = ++p;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* click arg                                                                  */
/*----------------------------------------------------------------------------*/
void StrLib::vClick(char* aArg)
{
  char* aChar = sLast(aArg);
  
  for (;;) {
    
    if ('z' == *aChar) {
      *aChar = 'a';
      aChar--;
      continue;
    }
    if ('Z' == *aChar) {
      *aChar = 'A';
      aChar--;
      continue;
    }

    *aChar += 1;
    break;
  }
  
}

/*----------------------------------------------------------------------------*/
/* comma seperate the integer argument                                        */
/*----------------------------------------------------------------------------*/
void StrLib::vComma(char* azDst, i8 iArg)
{
  char caInteger[32];
  vToString(caInteger,iArg);
  
  char caReverse[32];
  vReverse(caReverse,caInteger);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  char caRevCommas[32];

  char* aChar = caRevCommas;
  char* aDigit = caReverse;
  
  i8 nDigit = 0;
  while (*aDigit) {
    if (nDigit == 3) {
      *aChar++ = C_COMMA;
      nDigit = 0;
    }
    *aChar++ = *aDigit++;
    nDigit++;
  }
  
  if (iArg < 0)
    *aChar++ = '-';
  
  *aChar = '\0';

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vReverse(azDst,caRevCommas);
}

/*----------------------------------------------------------------------------*/
/* iCompare: standard compare of the two args                                 */
/*----------------------------------------------------------------------------*/
i8 StrLib::iCompare(const char* pArg1, const char* pArg2)
{
  i8 iDiff = strcmp(pArg1,pArg2);
  
  return iDiff;
}

/*----------------------------------------------------------------------------*/
/* iCompare: standard case-insensitive compare of the two args                */
/*----------------------------------------------------------------------------*/
bool StrLib::iCompare(const char* pArg1, const char* pArg2, i8 iCase)
{
  i8 iDiff;
  
  if (C_NO_CASE == iCase)
    iDiff = strcasecmp(pArg1,pArg2);
  else
    iDiff = strcmp(pArg1,pArg2);
  
  return iDiff;
}

/*----------------------------------------------------------------------------*/
/* iCompare: standard case-insensitive compare of the two args                */
/*----------------------------------------------------------------------------*/
bool StrLib::iCompare(const char* pArg1, const char* pArg2, i8 nBytes, i8 iCase)
{
  i8 iDiff;
  
  if (C_NO_CASE == iCase)
    iDiff = strncasecmp(pArg1,pArg2,nBytes);
  else
    iDiff = strncmp(pArg1,pArg2,nBytes);
  
  return iDiff;
}

/*----------------------------------------------------------------------------*/
/* vConcat: concatenate character to workspace                                */
/*----------------------------------------------------------------------------*/
void StrLib::vConcat(char* pWS, char cChar)
{
  i8 i = iLength(pWS);
  pWS[i] = cChar;
  pWS[i+1] = '\0';
}

/*----------------------------------------------------------------------------*/
/* vConcat: concatenate arg onto dst string                                   */
/*----------------------------------------------------------------------------*/
void StrLib::vConcat(char* psDst, const char* psArg)
{
  strcat(psDst,psArg);
}

/*----------------------------------------------------------------------------*/
/* vConcat: concatenate arg1 & arg2 onto dst string                           */
/*----------------------------------------------------------------------------*/
void StrLib::vConcat(char* psDst, const char* psArg1, const char* psArg2)
{
  strcat(psDst,psArg1);
  strcat(psDst,psArg2);
}

/*----------------------------------------------------------------------------*/
/* vConcat: concatenate i8 arg onto dst string                                */
/*----------------------------------------------------------------------------*/
void StrLib::vConcat(char* psDst, i8 iArg)
{
  char aInt[32];
  
  wToString(iArg,aInt);
  vConcat(psDst,aInt);
}

/*----------------------------------------------------------------------------*/
/* sConcat: concatenate two strings                                           */
/*----------------------------------------------------------------------------*/
char* StrLib::wConcat(const char* s1, const char* s2)
{
  i8 iLen1 = iLength(s1);
  i8 iLen2 = iLength(s2);
  
  i8 iSize = iLen1 + iLen2 + 1;
  
  char* w = wAllocate(iSize);
  
  strcpy(w,s1);
  strcat(w,s2);
  
  return w;
}

/*----------------------------------------------------------------------------*/
/* bContains: test if arg is in substr                                        */
/*----------------------------------------------------------------------------*/
bool StrLib::bContains(char* s, const char cArg)
{
  char* f = strchr(s,cArg);
  
  if (f)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bContains: test if arg is in substr                                        */
/*----------------------------------------------------------------------------*/
bool StrLib::bContains(char* s, const char* sArg)
{
  char* f = strstr(s,sArg);
  
  if (f)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* copy src into dst, return bytes copied (not including '\0' terminator)     */
/*----------------------------------------------------------------------------*/
i8 StrLib::nCopy(char* psDst, const char* psSrc)
{
  i8 nBytes = 0;
  
  while ( (*psDst++ = *psSrc++))
    nBytes++; // won't be executed when \0 is copied
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* vCopy: copy src into dst                                                   */
/*----------------------------------------------------------------------------*/
void StrLib::vCopy(char* psDst, const char* psSrc)
{
  strcpy(psDst,psSrc);
}

/*----------------------------------------------------------------------------*/
/* vCopyLen: copy src into dst uptill nBytes                                  */
/*----------------------------------------------------------------------------*/
void StrLib::vCopyLen(char* psDst, const char* psSrc, i8 nBytes)
{
  char *d = psDst;
  const char *s = psSrc;
  
  i8 n = nBytes;
  
  while (*s && n--)
    *d++ = *s++;
  
  *d = '\0';
}

/*----------------------------------------------------------------------------*/
/* vCopySplit: copy src into w1, then w2 divided at cSplit                    */
/*----------------------------------------------------------------------------*/
void StrLib::vCopySplit(char* w1, char* w2, const char* sSrc, char cSplit)
{
  const char *s = sSrc;
  
  char *d1 = w1;
  while (*s && *s != cSplit)
    *d1++ = *s++;
  *d1 = '\0';
  
  if ( ! *s ) {
    *w2 = '\0';
    return;
  }
  
  // skip the split character
  s++;
  
  char* d2 = w2;
  while (*s)
    *d2++ = *s++;
  *d2 = '\0';
}
  
/*----------------------------------------------------------------------------*/
/* vCopyTill: copy src into dst uptill but not including ccTill               */
/*----------------------------------------------------------------------------*/
void StrLib::vCopyTill(char* psDst, const char* psSrc, char ccTill)
{
  char *d = psDst;
  const char *s = psSrc;
  
  while (*s && ccTill != *s)
    *d++ = *s++;
  
  *d = '\0';
}

/*----------------------------------------------------------------------------*/
/* convert digit arg to i8                                                    */
/*----------------------------------------------------------------------------*/
i8 StrLib::iDigitToI8(const char cDigit)
{
  i8 i = cDigit - '0';
    
  return i;
}

/*----------------------------------------------------------------------------*/
/* vDump: dump string                                                         */
/*----------------------------------------------------------------------------*/
void StrLib::vDump(const char* s, i8 nMax, const char* pSuffix)
{
  char caString[C_STRING_ARRAY];
  char *z = caString;
  
  for (int i = 0; i < nMax && i < C_16KB - 1; i++)
  {
    if (isprint(s[i]))
    {
      *z++ = s[i];
    }
    else
    if ('\n' == s[i])
    {
      *z++ = '\\';
      *z++ = 'n';
    }
    else
    if ('\0' == s[i])
    {
      *z++ = '\\';
      *z++ = '0';
    }
    else
    {
      *z++ = '.';
    }
    
    if ('\0' == s[i])
      break;
  }
  
  vAppendString(&z,pSuffix);
  
  *z = '\0';
  
  printf("%s",caString);
}

/*----------------------------------------------------------------------------*/
/* sDupe: duplicate string                                                    */
/*----------------------------------------------------------------------------*/
char* StrLib::sDupe(const char* psArg)
{
  i8 iSize = StrLib::iSize(psArg);

  char* psString = wAllocate(iSize);

  StrLib::vCopy(psString,psArg);

  return psString;
}

/*----------------------------------------------------------------------------*/
/* sDupeStripped: duplicate stripped string                                   */
/*----------------------------------------------------------------------------*/
char* StrLib::sDupeStripped(const char* s)
{
  if ('"' != *s)
    return sDupe(s);
  
  i8 iSize = StrLib::iSize(s) - 2;
  
  char* psString = wAllocate(iSize);
  
  char* p = psString;
  
  for (s++; '"' != *s; )
  {
    if ('\\' == *s)
      s++;
    *p++ = *s++;
  }
  *p = '\0';
  
  return psString;
}

/*----------------------------------------------------------------------------*/
/* bEnds: test if string ends with given char                                 */
/*----------------------------------------------------------------------------*/
bool StrLib::bEachLine(char** aacInputChar, char* acOutputChar)
{
  char* acInputChar = *aacInputChar;
  
  for (;;) {
    
    if ('\n' == *acInputChar) {
      *acOutputChar++ = '\0';
      acInputChar++;
      break;
    }
    
    if ('\0' == *acInputChar) {
      if (acInputChar == *aacInputChar)
        return false;
      else {
        *acOutputChar++ = '\0';
        break;
      }
    }
    
    *acOutputChar++ = *acInputChar++;
  }
  
  *aacInputChar = acInputChar;
  return true;
}

/*----------------------------------------------------------------------------*/
/* bEnds: test if string ends with given char                                 */
/*----------------------------------------------------------------------------*/
bool StrLib::bEnds(const char* s, const char c)
{
  const char* z = sLast(s);
  
  if (c == *z)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bEnds: test if string ends with given substr arg                           */
/*----------------------------------------------------------------------------*/
bool StrLib::bEnds(const char* sArg, const char* sEnd)
{
  i8 nLen  = iLength(sArg);
  i8 nBack = iLength(sEnd);
  
  if (nBack > nLen)
    return false;
  
  const char* sFrom = sArg + nLen - nBack;
  
  if (bEqualTo(sFrom,sEnd))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* vEscape: put a '\' before an '"'                                           */
/*----------------------------------------------------------------------------*/
void StrLib::vEscape(const char* sArg, char* sEscaped)
{
  const char* aSrc = sArg;
  char* aDst = sEscaped;
  
  while (*aSrc) {
    if ('\"' == *aSrc)
      *aDst++ = '\\';
    else
    if ('\\' == *aSrc)
      *aDst++ = '\\';

    *aDst++ = *aSrc++;
  }
  
  *aDst = '\0';
}

/*----------------------------------------------------------------------------*/
/* bEscapeConvert: test if cArg is an escape character                        */
/*----------------------------------------------------------------------------*/
bool StrLib::bEscapeConvert(const char cArg,
                            char cEnclosedBy_Leading,
                            char cEnclosedBy_Trailing,
                            char cEscapeChar,
                            char cDelimiter,
                            char cTerminator)
{
  if (cEnclosedBy_Leading == cArg) return true;
  if (cEnclosedBy_Trailing == cArg) return true;
  if (cEscapeChar == cArg) return true;
  if (cDelimiter == cArg) return true;
  if (cTerminator == cArg) return true;
  
  if ('b' == cArg) return true;
  if ('f' == cArg) return true;
  if ('n' == cArg) return true;
  if ('r' == cArg) return true;
  if ('t' == cArg) return true;
  if ('Z' == cArg) return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* bEscapeConvert: test if cArg is an escape character                        */
/*----------------------------------------------------------------------------*/
bool StrLib::bEscapeConvertDefault(const char cArg,
                            char cEscapeChar,
                            char cDelimiter)
{
  if (cEscapeChar == cArg) return true;
  if (cDelimiter == cArg) return true;
  
  if ('b' == cArg) return true;
  if ('f' == cArg) return true;
  if ('n' == cArg) return true;
  if ('r' == cArg) return true;
  if ('t' == cArg) return true;
  if ('Z' == cArg) return true;
  
  return false;
}
/*----------------------------------------------------------------------------*/
/* cEscapeConvert: convert an escaped character to actual special character   */
/*----------------------------------------------------------------------------*/
char StrLib::cEscapeConvert(const char cArg)
{
  if ('b' == cArg) return '\b';
  if ('f' == cArg) return '\f';
  if ('n' == cArg) return '\n';
  if ('r' == cArg) return '\r';
  if ('t' == cArg) return '\t';
  if ('Z' == cArg) return 26;
  
  return cArg;
}

/*----------------------------------------------------------------------------*/
/* cEscapeInsert: convert a special character to escaped character form       */
/*----------------------------------------------------------------------------*/
char StrLib::cEscapeInverse(const char cArg)
{
  if ('\b' == cArg) return 'b';
  if ('\f' == cArg) return 'f';
  if ('\n' == cArg) return 'n';
  if ('\r' == cArg) return 'r';
  if ('\t' == cArg) return 't';
  if ((char)26 == cArg) return 'Z';
  
  return cArg;
}

/*----------------------------------------------------------------------------*/
/* escape character processing                                                */
/*----------------------------------------------------------------------------*/
char StrLib::cEscapeProcessing(char** aaSrc, char cEscapeChar)
{
  // test for escape
  if (cEscapeChar != **aaSrc) {
    // not escaped, return char, advance pointer
    return *(*aaSrc)++;
  }
  
  // skip escape character
  (*aaSrc)++;
  
  // must have some non-zero character
  if ( ! **aaSrc )
    return '\0';
  
  // convert the char, advance pointer
  char cConverted = StrLib::cEscapeConvert(*(*aaSrc)++);
  
  return cConverted;
}

/*----------------------------------------------------------------------------*/
/* bEscapedSpecial: test for special character which should be escaped        */
/*----------------------------------------------------------------------------*/
bool StrLib::bEscapedSpecial(const char cArg)
{
  if ('\b' == cArg) return true;
  if ('\f' == cArg) return true;
  if ('\n' == cArg) return true;
  if ('\r' == cArg) return true;
  if ('\t' == cArg) return true;
  if ((char)26 == cArg) return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* bEqualTo: test if the two args match                                       */
/*----------------------------------------------------------------------------*/
bool StrLib::bEqualTo(const char* s1, const char* s2)
{
  i8 iDiff = strcmp(s1,s2);
  
  if (0 == iDiff)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bEqualTo: are two arguments equal (case or no case)                        */
/*----------------------------------------------------------------------------*/
bool StrLib::bEqualTo(const char* pArg1, const char* pArg2, i8 iCase)
{
  i8 iDiff;
  
  if (C_NO_CASE == iCase)
    iDiff = strcasecmp(pArg1,pArg2);
  else
    iDiff = strcmp(pArg1,pArg2);
  
  if (0 == iDiff)
    return true;
  else
    return false;
}
  
/*----------------------------------------------------------------------------*/
/* bExtractArg: extract an argument from substr                               */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractArg(char** ps, char* pArg)
{
  if ( ! *ps )
    return false;
  
  bool bOnce = false;
  if (**ps && ':' != **ps)
    bOnce = true;
  
  char *a = pArg;
  while (**ps && ':' != **ps)
    *a++ = *(*ps)++;
  *a = '\0';
  
  return bOnce;
}

/*----------------------------------------------------------------------------*/
/* bExtractBoolean: extract a boolean from substr                             */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractBoolean(char** aaSrc, i8& iBoolean)
{
  // assume the boolean is false
  iBoolean = false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // numeric processing

  // test for number
  if (StrLib::bIsDigit(**aaSrc)) {
    
    // - numeric processing
    
    // collect all numeric digits
    i8 iResult = 0;
    while (StrLib::bIsDigit(**aaSrc)) {
      iResult = iResult * 10 + StrLib::iDigitToI8(**aaSrc);
      (*aaSrc)++;
    }

    // a non-zero number is true
    if (iResult > 0)
      iBoolean = true;
    
    // saw a valid boolean
    return true;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // true processing
  
  // test for possible true
  if ('t' ==  **aaSrc || 'T' == **aaSrc) {

    // - true processing
    
    iBoolean = true;
    
    // next 3 characters could be rue to make true
    (*aaSrc)++;
    if ('r' !=  **aaSrc && 'R' != **aaSrc) return true;  // saw only t
    (*aaSrc)++;
    if ('u' !=  **aaSrc && 'U' != **aaSrc) return false; // saw only tr
    (*aaSrc)++;
    if ('e' !=  **aaSrc && 'E' != **aaSrc) return false; // saw only tru
    (*aaSrc)++;
    
    return true; // saw true
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // false processing
  
  // test for possible false
  if ('f' ==  **aaSrc || 'F' == **aaSrc) {
    
    // - false processing
    
    // next 4 characters could be alse to make alse
    (*aaSrc)++;
    if ('a' !=  **aaSrc && 'A' != **aaSrc) return true;  // saw only f
    (*aaSrc)++;
    if ('l' !=  **aaSrc && 'L' != **aaSrc) return false; // saw only fa
    (*aaSrc)++;
    if ('s' !=  **aaSrc && 'S' != **aaSrc) return false; // saw only fal
    (*aaSrc)++;
    if ('e' !=  **aaSrc && 'E' != **aaSrc) return false; // saw only fals
    (*aaSrc)++;
    
    return true; // saw false
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // saw nothing that could be recognised as a boolean
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* bExtractDate: extract a date from substr                                   */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractDate(char** aaSrc, i8& iYear, i8& iMonth, i8& iDay)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  u8 uSubField;
  
  iYear = iMonth = iDay = 0;
  
  // extract year
  if ( ! StrLib::bExtractU8(aaSrc,&uSubField, 4) )
    return false;
  
  // assume a leap year
  bool bLeapYear = true;
  if (uSubField % 4 == 0) {
    if ((uSubField % 100) == 0 && (uSubField % 400) != 0) {
      // not a leap year
      bLeapYear = false;
    }
  }
  iYear = uSubField;
  
  // skip - separator
  if ('-' != *(*aaSrc)++)
    return false;
  
  // extract month
  if ( ! StrLib::bExtractU8(aaSrc,&uSubField,2) )
    return false;
  if (uSubField < 1 || uSubField > 12)
    return false;
  iMonth = uSubField;
  
  // skip - separator
  if ('-' != *(*aaSrc)++) return false;
  
  // extract day
  if ( ! StrLib::bExtractU8(aaSrc,&uSubField,2) )
    return false;
  if (bLeapYear) {
    if ((i8)uSubField > iDaysPerMonthLeap[iMonth-1])
      return false;
  }
  else {
    if ((i8)uSubField > iDaysPerMonth[iMonth-1])
      return false;
  }
  iDay = uSubField;

  return true;
}

/*----------------------------------------------------------------------------*/
/* bExtractDate: extract a date from substr                                   */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractDate2(char** aaSrc, i8& iYear, i8& iMonth, i8& iDay,
                           eDC* pDateComponents, char cDateSeparator)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  for (i8 i_Component = 0; i_Component < 3; i_Component++) {
  
    eDC eDateComponent = pDateComponents[i_Component];
    
    switch (eDateComponent) {
      
      case DC_DD:
        if (! bExtractDay(aaSrc,iDay))
          return false;
        break;
      
      case DC_MM:
        if (! bExtractMonth(aaSrc,iMonth))
          return false;
        break;
      
      case DC_YYYY:
        if (! bExtractYear(aaSrc,iYear))
          return false;
        break;
      
      default:
        return false;
    }
    
    // skip separator after components  0 and 1
    if (i_Component < 2) {
      if (cDateSeparator != *(*aaSrc)++)
        return false;
    }
    
  }
  
  if (! bValidateMonth(iDay, iMonth, iYear))
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* bExtractDay: extract a day from substr                                     */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractDay(char** aaSrc, i8& iDay)
{
  u8 uSubField;

  iDay = 0;

  if ( ! StrLib::bExtractU8(aaSrc,&uSubField,2) )
    return false;

  iDay = uSubField;

  return true;
}

/*----------------------------------------------------------------------------*/
/* bExtractDecimal: extract a decimal from substr                             */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractDecimal(char** aaSrc, i8& iDecimal, i8 iLength, i8 iScale)
{
  i8 iPrecision = iLength - iScale;
  
  u8 uUnsignedResult = 0;
  
  // test for negative or positive sign
  bool bNegative = false;
  if (**aaSrc && '-' == **aaSrc ) {
    bNegative = true;
    (*aaSrc)++;
  }
  else
    if (**aaSrc && '+' == **aaSrc ) {
      (*aaSrc)++;
    }
  
  bool bOnce = false;
  if (**aaSrc && (isdigit(**aaSrc) || '.' == **aaSrc))
    bOnce = true;
  
  // ignore leading 0's
  while (StrLib::bIsDigit(**aaSrc) && StrLib::iDigitToI8(**aaSrc) == 0) {
    (*aaSrc)++;
  }
  
  // collect leading digits (till '.')
  while (StrLib::bIsDigit(**aaSrc)) {
    uUnsignedResult = uUnsignedResult * 10 + StrLib::iDigitToI8(**aaSrc);
    (*aaSrc)++;
    iPrecision--;
  }
  
  // have we exceeded maximum precision digits?
  if (iPrecision < 0)
    return false;
  
  if ('.' == **aaSrc) {
    
    (*aaSrc)++; // skip '.'
    
    while (StrLib::bIsDigit(**aaSrc) && iScale) {
      uUnsignedResult = uUnsignedResult * 10 + StrLib::iDigitToI8(**aaSrc);
      (*aaSrc)++;
      iScale--;
    }
  }
  
  while (iScale--)
    uUnsignedResult = uUnsignedResult * 10 + 0;
  
  // have we exceeded the max ?
  if (uUnsignedResult > (u8)C_I8_MAX)
    return false;
  
  // round up if there any more digits after decimal
  if (StrLib::bIsDigit(**aaSrc)) {
    if (StrLib::iDigitToI8(**aaSrc) >= 5)
      uUnsignedResult += 1;
    while (StrLib::bIsDigit(**aaSrc))
      (*aaSrc)++;
  }
  
  // compute final result
  uUnsignedResult = bNegative ? - uUnsignedResult : uUnsignedResult;
  
  // have we gone beyond the min ?
  if (uUnsignedResult != 0 && bNegative && (uUnsignedResult < (u8)C_I8_MIN))
    return false;
  
  iDecimal = uUnsignedResult;
  
  return bOnce;
}

/*----------------------------------------------------------------------------*/
/* bExtractFloat: extract a float from substr                                 */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractFloat(char** aaSrc, d8& dFloat)
{
  d8 dUnsignedResult = 0.0;
  char xBuffer[256];
  char* aChar = xBuffer;
  
  // test for negative or positive sign
  bool bNegative = false;
  if (**aaSrc && '-' == **aaSrc ) {
    bNegative = true;
    (*aaSrc)++;
  }
  else
    if (**aaSrc && '+' == **aaSrc ) {
      (*aaSrc)++;
    }
  
  bool bOnce = false;
  if (**aaSrc && (isdigit(**aaSrc) || '.' == **aaSrc))
    bOnce = true;
  
  // collect leading portion (till '.')
  while (StrLib::bIsDigit(**aaSrc))
    *aChar++ = *(*aaSrc)++;
  
  // collect trailing portion (after '.')
  if ('.' == **aaSrc) {
    *aChar++ = *(*aaSrc)++;
    while (StrLib::bIsDigit(**aaSrc))
      *aChar++ = *(*aaSrc)++;
  }
  
  // test for exponent
  if (**aaSrc && ('e' == **aaSrc || 'E' == **aaSrc) ) {
    
    *aChar++ = *(*aaSrc)++;
    
    // test for -
    if (**aaSrc && '-' == **aaSrc )
      *aChar++ = *(*aaSrc)++;
    else
    // test for +
    if (**aaSrc && '+' == **aaSrc )
      *aChar++ = *(*aaSrc)++;
    
    // collect exponent digits
    while (StrLib::bIsDigit(**aaSrc))
      *aChar++ = *(*aaSrc)++;
  }
  
  // terminate
  *aChar++ = '\0';
  
  // compute final result
  dUnsignedResult = atof(xBuffer);
  dFloat = bNegative ? - dUnsignedResult : dUnsignedResult;
  
  return bOnce;
}

/*----------------------------------------------------------------------------*/
/* bExtractFraction: extract an unsigned integer as a fraction from substr    */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractFraction(char** ps, u8* aU8, i8 iDigits)
{
  char *s = *ps;
  
  if ( ! *s )
    return false;
  
  bool bOnce = false;
  if (*s && isdigit(*s))
    bOnce = true;
  
  u8 uFraction = 0;
  
  for ( ; *s && iDigits > 0; s++) {
    
    char cDigit = *s;
    
    if (cDigit < '0' || cDigit > '9')
      break;
    
    // decrement no of digits extracted
    iDigits--;
    
    uFraction = (uFraction * 10) + (cDigit - '0');
  }
  
  while (iDigits--)
    uFraction *= 10;
  
  *aU8 = uFraction;
  
  *ps = s;
  
  return bOnce;
}

/*----------------------------------------------------------------------------*/
/* bExtractInteger: extract an integer from substr                            */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractInteger(char** aaSrc, i8* aI8, bool bWithSign)
{
  char *aSrc = *aaSrc;
  
  if ( ! *aSrc )
    return false;
 
  // test for negative or positive sign
  bool bNegative = false;
  if (bWithSign) {
    if (*aSrc && '-' == *aSrc) {
      bNegative = true;
      aSrc++;
    }
    else
    if (*aSrc && '+' == *aSrc) {
      aSrc++;
    }
  }
  
  bool bOnce = false;
  if (*aSrc && isdigit(*aSrc))
    bOnce = true;
  
  u8 uUnsignedResult = 0;
  for ( ; *aSrc; aSrc++) {
    
    char c = *aSrc;
    
    if ( c < '0' || c > '9' )
      break;
    
    uUnsignedResult = (uUnsignedResult * 10) + (c - '0');
    
    // have we exceeded the max ?
    if (uUnsignedResult > (u8)C_I8_MAX)
      return false;
  }
  
  if (bNegative)
    *aI8 = - uUnsignedResult;
  else
    *aI8 = uUnsignedResult;
  
  *aaSrc = aSrc;
  
  return bOnce;
}

/*----------------------------------------------------------------------------*/
/* bExtractInteger: extract an integer from substr, counting no of digits     */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractInteger(char** aaSrc, i8* aI8, i8& nDigits)
{
  char *aSrc = *aaSrc;
  
  if ( ! *aSrc )
    return false;
  
  bool bOnce = false;
  if (*aSrc && isdigit(*aSrc))
    bOnce = true;
  
  nDigits = 0;
  
  u8 uUnsignedResult = 0;
  for ( ; *aSrc; aSrc++) {
    
    char c = *aSrc;
    
    if ( c < '0' || c > '9' )
      break;
    
    uUnsignedResult = (uUnsignedResult * 10) + (c - '0');
    nDigits++;
    
    // have we exceeded the max ?
    if (uUnsignedResult > (u8)C_I8_MAX)
      return false;
  }
  
  *aI8 = uUnsignedResult;
  
  *aaSrc = aSrc;
  
  return bOnce;
}

/*----------------------------------------------------------------------------*/
/* bExtractInterval: extract an interval from substr                          */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractInterval(char** aaSrc, i8& iSecond, i8& iMilli)
{
  u8 uSubField;
  
  iSecond = iMilli = 0;
  
  // extract second
  if ( ! StrLib::bExtractU8(aaSrc,&uSubField) )
    return false;
  
  iSecond = uSubField;
  
  // extract fraction (assuming milliseconds)
  if ('.' == **aaSrc) {
    (*aaSrc)++;
    if ( StrLib::bExtractU8(aaSrc,&uSubField) )
      iMilli = uSubField;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* bExtractItem: extract an item from substr                                  */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractItem(char** ps, char* pItem)
{
  if ( ! *ps )
    return false;
  
  char *a = pItem;
  bool bOnce = false;
  
  for ( ; **ps; (*ps)++)
  {
    if (' ' == **ps || ',' == **ps)
      break;

    bOnce = true;

    *a++ = **ps;
  }
  
  *a = '\0';
  
  return bOnce;
}

/*----------------------------------------------------------------------------*/
/* bExtractItem: extract an item from substr                                  */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractItem(char** ps, char* pItem, char cDelimiter)
{
  if ( ! *ps )
    return false;
  
  char *a = pItem;
  bool bOnce = false;
  
  for ( ; **ps; (*ps)++)
  {
    //if (' ' == **ps || cDelimiter == **ps)
    bOnce = true;
    
  if (cDelimiter == **ps)
    break;
  
    *a++ = **ps;
  }
  
  *a = '\0';
  
  return bOnce;
}

/*----------------------------------------------------------------------------*/
/* bExtractMonth: extract a month from substr                                 */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractMonth(char** aaSrc, i8& iMonth)
{
  u8 uSubField;
  
  iMonth = 0;
  
  if ( ! StrLib::bExtractU8(aaSrc,&uSubField,2) )
    return false;
  
  if (uSubField < 1 || uSubField > 12)
    return false;

  iMonth = uSubField;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* bExtractPath: extract a path from substr                                   */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractPath(char** ps, char* pPath)
{
  if ( ! *ps )
    return false;
  
  bool bOnce = false;
  if (**ps && ' ' != **ps)
    bOnce = true;
  
  char *a = pPath;
  while (**ps && ' ' != **ps)
    *a++ = *(*ps++);
  *a = '\0';
  
  return bOnce;
}

/*----------------------------------------------------------------------------*/
/* bExtractPort: extract a port number from substr                            */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractPort(char** ps, u2* pPort)
{
  if ( ! *ps )
    return false;
  
  bool bOnce = false;
  if (**ps && isdigit(**ps))
    bOnce = true;
  
  i8 v = 0;
  for ( ; **ps; (*ps)++)
  {
    char c = **ps;
    
    if ( ! isdigit(c) )
      break;
    
    v = (v * 10) + c - '0';
  }
  
  *pPort = v;
  
  return bOnce;
}

/*----------------------------------------------------------------------------*/
/* bExtractString: extract a quoted string from substr                        */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractString(char** ps, char* pString)
{
  if ( ! *ps )
    return false;
  
  if ('"' != **ps)
    return false;
  
  char *a = pString;
  //*a++ = *((*ps)++);
  
  (*ps)++;
  for ( ; **ps; (*ps)++) {
    if ('"' == **ps) {
      (*ps)++;
      break;
    }
    *a++ = **ps;
  }
  
  *a = '\0';
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* bExtractTime: extract a time from substr                                   */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractTime(char** aaSrc,
                          i8& iHour, i8& iMinute, i8& iSecond, i8& iMicro)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  u8 uSubField;
  
  iHour = iMinute = iSecond = iMicro = 0;
  
  // extract hour
  if ( ! StrLib::bExtractU8(aaSrc,&uSubField,2) )
    return false;
  if (uSubField > 23)
    return false;
  iHour = uSubField;
  
  // skip : separator
  if (':' != *(*aaSrc)++)
    return false;
  
  // extract minute
  if ( ! StrLib::bExtractU8(aaSrc,&uSubField,2) )
    return false;
  
  if (uSubField > 59)
    return false;
  iMinute = uSubField;
  
  // skip : separator
  if (':' != *(*aaSrc)++)
    return false;
  
  // extract second
  if ( ! StrLib::bExtractU8(aaSrc,&uSubField,2) )
    return false;
  if (uSubField > 59)
    return false;
  iSecond = uSubField;
  
  // extract fraction (assuming microseconds)
  if ('.' == **aaSrc) {
    (*aaSrc)++;
    
    if ( StrLib::bExtractFraction(aaSrc,&uSubField,6) ) {
      iMicro = uSubField;
    }
    
    // skip/truncate any remaining digits
    while (bIsDigit(**aaSrc))
      (*aaSrc)++;
  }
  
  // skip optional Z (UTC zone designator)
  if ('Z' == **aaSrc)
    (*aaSrc)++;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* bExtractTimestamp: extract a timestamp from substr                         */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractTimestamp(char** aaSrc,
                               i8& iYear, i8& iMonth,  i8& iDay,
                               i8& iHour, i8& iMinute, i8& iSecond, i8& iMicro)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
    
  // - date portion

  if ( ! bExtractDate(aaSrc,iYear,iMonth,iDay) )
    return false;
  //if ( ! StrLib::bExtractDate2(aaSrc, iYear, iMonth, iDay,
  //                             pFormatters->m_eDateComponent,
  //                             pFormatters->m_cDateSeparator) )
  //  return false;
  
  // - time portion
  
  // skip T or space separator
  if ( ! ('T' == **aaSrc || ' ' == **aaSrc) ) {
    iHour = iMinute = iSecond = iMicro = 0;
    return true;
  }
  
  (*aaSrc)++;
  
  if ( ! bExtractTime(aaSrc,iHour,iMinute,iSecond,iMicro) )
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* bExtractTimestamp: extract a timestamp from substr                         */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractTimestamp2(char** aaSrc,
                                i8& iYear, i8& iMonth,  i8& iDay,
                                i8& iHour, i8& iMinute, i8& iSecond, i8& iMicro,
                                eDC* pDateComponents, char cDateSeparator)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // date portion
  
  if ( ! StrLib::bExtractDate2(aaSrc, iYear, iMonth, iDay,
                               pDateComponents,cDateSeparator) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // time portion
  
  // skip T or space separator
  if ( ! ('T' == **aaSrc || ' ' == **aaSrc) ) {
    iHour = iMinute = iSecond = iMicro = 0;
    return true;
  }
  
  (*aaSrc)++;
  
  if ( ! bExtractTime(aaSrc,iHour,iMinute,iSecond,iMicro) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* bExtractU8: extract an unsigned integer from substr                        */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractU8(char** aaSrc, u8* aU8)
{
  char *aSrc = *aaSrc;
  
  if ( ! *aSrc )
    return false;
  
  bool bOnce = false;
  if (*aSrc && isdigit(*aSrc))
    bOnce = true;
  
  u8 uUnsignedResult = 0;
  for ( ; *aSrc; aSrc++) {
    
    char cChar = *aSrc;
    
    if (cChar < '0' || cChar > '9')
      break;
    
    uUnsignedResult = (uUnsignedResult * 10) + cChar - '0';
    
    // have we exceeded the max ?
    if (uUnsignedResult > (u8)C_I8_MAX)
      return false;
  }
  
  *aU8 = uUnsignedResult;
  
  *aaSrc = aSrc;
  
  return bOnce;
}

/*----------------------------------------------------------------------------*/
/* bExtractU8: extract an unsigned integer from substr                        */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractU8(char** ps, u8* aU8, i8 iDigits)
{
  char *s = *ps;
  
  if ( ! *s )
    return false;
  
  bool bOnce = false;
  if (*s && isdigit(*s))
    bOnce = true;
  
  u8 v = 0;
  for ( ; *s && iDigits > 0; s++)
  {
    char c = *s;
    
    // decrement no of digits extracted
    iDigits--;
    
    //    if ( ! isdigit(c) )
    if ( c < '0' || c > '9' )
      break;
    
    v = (v * 10) + c - '0';
  }
  
  *aU8 = v;
  
  *ps = s;
  
  return bOnce;
}

/*----------------------------------------------------------------------------*/
/* bExtractVarchar: extract a varchar from substr                             */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractVarchar(char** aaSrc,
                             char* aVarChar,
                             i8 iMaxLength,
                             bool bSawLeadingEnclosed,
                             char cEnclosedBy_Leading,
                             char cEnclosedBy_Trailing,
                             char cEscapeChar,
                             char cFieldSeparator,
                             char cRecordTerminator,
                             bool bAllowNonPrintable,
                             i8& nChars)
{
  i8 iLength = 0;
  char *aDst = (char*)aVarChar;
  
  bool bSawTrailingEnclosed = false;
  
  char cSrc = **aaSrc;
  
  while (cSrc) {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // special case of double enclosedBy
    
    if (cEnclosedBy_Trailing == cEnclosedBy_Leading) {
      if (cSrc == cEnclosedBy_Leading && *(*aaSrc+1) == cEnclosedBy_Leading) {
        
        // skip the first enclosed character
        (*aaSrc)++;
       
        // append one instance of the enclosedBy character
        if (iLength++ < iMaxLength)
          *aDst++ = cSrc;
      
        (*aaSrc)++;

        cSrc = **aaSrc;
        
        continue;
      }
      //falls thru if not the case of double enclosedBy
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // test for a non-printable character (e.g. \n or \r)
   
    // Alka: Changed check from ! isprint (not-printable) to isspace and then allowing space
    // this ways we allow for UTF-8 characters to pass this test
    // I think there is more to supporting UTF-8 but this will let us allow for UTF-8 characters to
    // be loaded from files.
    
    //if (! bAllowNonPrintable && ! isprint(**aaSrc) )
    
    // isprint: checks for any printable character including space.
    
    // isspace : checks for white-space characters. In the C and POSIX locales, these are:
    // space, form-feed ('\f'), newline ('\n'), carriage return ('\r'), horizontal tab ('\t'),
    // and vertical tab ('\v').
      
    if (! bAllowNonPrintable && isspace(cSrc) && (cSrc != ' ') )
        break;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // test for normal end
    
    if (bSawLeadingEnclosed) {
      // we saw a leading enclodedBy so test for trailing enclosedBy
      if (cSrc == cEnclosedBy_Trailing) {
        bSawTrailingEnclosed = true;
        break;
      }
      //falls thru if not trailing enclosedBy
    }
    // test for field separator (because its not enclosed)
    else if (cFieldSeparator && cSrc == cFieldSeparator)
      break;
    // test for record terminator (because its not enclosed)
    else if (cRecordTerminator && cSrc == cRecordTerminator)
      break;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // test for the escape character
    
    if (cSrc == cEscapeChar) {
      
      if (StrLib::bEscapeConvert( *((*aaSrc)+1),
                                 cEnclosedBy_Leading, cEnclosedBy_Trailing,
                                 cEscapeChar, cFieldSeparator, cRecordTerminator)) {
        
        // skip the escape character
        (*aaSrc)++;
        
        // append the escaped character
        if (iLength++ < iMaxLength)
          *aDst++ = StrLib::cEscapeConvert(*(*aaSrc)++);
        else
          (*aaSrc)++;
        
        cSrc = **aaSrc;
        continue;
      }
      else {
        (*aaSrc)++;
        cSrc = **aaSrc;
      }
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // normal character
    
    // append the normal character
    if (iLength++ < iMaxLength)
      *aDst++ = cSrc;
    
    (*aaSrc)++;
    cSrc = **aaSrc;
  }
  
  // terminate
  *aDst++ = '\0';
  
  // its not an enclosed varchar so we consider it to be valid
  if (iLength < iMaxLength)
    nChars = iLength;
  else
    nChars = iMaxLength;
  
  // if this an enclosed varchar we should have expected to
  // see the trailing enclosedBy
  if (bSawLeadingEnclosed)
    return bSawTrailingEnclosed;

  return true;
}

/*----------------------------------------------------------------------------*/
/* bExtractVarchar: extract a varchar from substr                             */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractVarcharDefault(char** aaSrc,
                                    char* aVarChar,
                                    i8 iMaxLength,
                                    i8& nChars)
{
  i8 iLength = 0;
  
  char cEscapeChar = gpAttachMap->m_oFormatters.m_cEscapeChar;
  char cFieldDelimiter = gpAttachMap->m_oFormatters.m_cFieldDelimiter;
  
  char *aDst = (char*)aVarChar;
  
  char cSrc = **aaSrc;
  
  while (cSrc) {
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // test for a non-printable character (e.g. \n or \r)
    
    // Alka: Changed check from ! isprint (not-printable) to isspace and then allowing space
    // this ways we allow for UTF-8 characters to pass this test
    // I think there is more to supporting UTF-8 but this will let us allow for UTF-8 characters to
    // be loaded from files.
    
    //if (! bAllowNonPrintable && ! isprint(**aaSrc) )
    
    // isprint: checks for any printable character including space.
    
    // isspace : checks for white-space characters. In the C and POSIX locales, these are:
    // space, form-feed ('\f'), newline ('\n'), carriage return ('\r'), horizontal tab ('\t'),
    // and vertical tab ('\v').
    
    if (isspace(cSrc) && (cSrc != ' ') )
      break;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // test for normal end
    
    // test for field separator (because its not enclosed)
    if (cSrc == cFieldDelimiter)
      break;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // test for the escape character
    
    if (cSrc == cEscapeChar) {
      
      if (StrLib::bEscapeConvertDefault( *((*aaSrc)+1),
                                 cEscapeChar, cFieldDelimiter)) {
        
        // skip the escape character
        (*aaSrc)++;
        
        // append the escaped character
        if (iLength++ < iMaxLength)
          *aDst++ = StrLib::cEscapeConvert(*(*aaSrc)++);
        else
          (*aaSrc)++;
        
        cSrc = **aaSrc;
        continue;
      }
      else {
        (*aaSrc)++;
        cSrc = **aaSrc;
      }
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // normal character
    
    // append the normal character
    if (iLength++ < iMaxLength)
      *aDst++ = cSrc;
    
    (*aaSrc)++;
    cSrc = **aaSrc;
  }
  
  // terminate
  *aDst++ = '\0';
  
  // its not an enclosed varchar so we consider it to be valid
  if (iLength < iMaxLength)
    nChars = iLength;
  else
    nChars = iMaxLength;
  return true;
}

/*----------------------------------------------------------------------------*/
/* bExtractWord: extract a word from substr                                   */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractWord(char** ps, char* pWord)
{
  if ( ! *ps )
    return false;
  
  bool bOnce = false;
  if (**ps && isalnum(**ps))
    bOnce = true;
  
  char *a = pWord;
  for ( ; **ps; (*ps)++)
  {
    if ( ! isalnum(**ps) )
      break;
    
    *a++ = **ps;
  }

  *a = '\0';

  return bOnce;
}

/*----------------------------------------------------------------------------*/
/* bExtractWord: extract a word or quoted string                              */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractWordString(char** ps, char* pWord)
{
  if ( ! *ps )
    return false;
  
  if ('"' == **ps)
    return bExtractString(ps,pWord);
  
  if ( !isalnum(**ps) && !('_' == **ps))
    return bExtractItem(ps,pWord);
  
  bool bOnce = false;
  if (**ps && isalnum(**ps))
    bOnce = true;
  
  char *a = pWord;
  for ( ; **ps; (*ps)++)
  {
    if ( !isalnum(**ps) && !('_' == **ps) )
      break;
    
    *a++ = **ps;
  }
  
  *a = '\0';
  
  return bOnce;
}

/*----------------------------------------------------------------------------*/
/* bExtractYear: extract a month from substr                                  */
/*----------------------------------------------------------------------------*/
bool StrLib::bExtractYear(char** aaSrc, i8& iYear)
{
  /*
  u8 uSubField;
  
  // extract year
  if ( ! StrLib::bExtractU8(aaSrc,&uSubField, 4) )
    return false;

  iYear = uSubField;

  return true;
  */

  i8 nDigits;
  
  // extract year
  if ( ! bExtractInteger(aaSrc,&iYear,nDigits) )
    return false;
  
  if (2 == nDigits) {
    iYear += 2000;
    return true;
  }
  
  if (4 == nDigits)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* bFind: find start of ScanChar in substr                                    */
/*----------------------------------------------------------------------------*/
bool StrLib::bFind(char** ps, const char cArg)
{
  char* f = strchr(*ps,cArg);
  
  if (f)
  {
    *ps = f;
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bFind: find nth ScanChar in substr                                         */
/*----------------------------------------------------------------------------*/
bool StrLib::bFind(char** ps, const char cArg, i8 iOccurence)
{
  bool b;
  
  for (i8 i_Count = 0; i_Count < iOccurence; i_Count++) {
    b = bFind(ps,cArg);
    if (! b)
      return false;
    (*ps)++;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* bFind: find start of arg in substr                                         */
/*----------------------------------------------------------------------------*/
bool StrLib::bFind(char** ps, const char* sArg)
{
  char* f = strstr(*ps,sArg);
  
  if (f)
  {
    *ps = f;
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bFind: find start of arg in substr                                         */
/*----------------------------------------------------------------------------*/
bool StrLib::bFind(char** ps, const char* sArg, i8 iLenArg)
{
  char *s = *ps;
  
  for (;;)
  {
    s = strchr(s,*sArg);
    
    // end of substr?
    if (nullptr == s)
      return false;
    
    // match?
    if (0 == strncmp(s,sArg,iLenArg))
    {
      *ps = s;
      return true;
    }
    
    // advance
    s++;
  }
}

/*----------------------------------------------------------------------------*/
/* vFormatFraction: format a fraction                                         */
/*----------------------------------------------------------------------------*/
void StrLib::vFormatFraction(char* sDst, u8 uFraction, i8 iDigits)
{
  char caFormat[16];
  
  // create the format
  sprintf(caFormat,"%%0%lldlld",iDigits);
  
  // sprint the number as an integer
  sprintf(sDst,caFormat,uFraction);
  
  // strip off trailing zero's
//  char *z = sDst;
//  while (*z)
//    z++;
//  z--;
//  
//  while ('0' == *z)
//    *z-- = '\0';
}

/*----------------------------------------------------------------------------*/
/* vFree: free a dynamic string                                               */
/*----------------------------------------------------------------------------*/
void StrLib::vFree(char** ps)
{
  free(*ps);
  
  *ps = nullptr;
}

/*----------------------------------------------------------------------------*/
/* iHexToInw: convert hex character to i8 (-1 on error)                       */
/*----------------------------------------------------------------------------*/
i8 StrLib::iHexToInw(const char c)
{
  if (c >= 'a' && c <= 'f') return c - 'a' + 10;
  if (c >= 'A' && c <= 'F') return c - 'A' + 10;
  if (c >= '0' && c <= '9') return c - '0';
  
  return -1;
}

/*----------------------------------------------------------------------------*/
/* bIsAlpha: is char an alphabetic character?                                 */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsAlpha(const char c)
{
  if (isalpha(c))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsAlphaDigit: is char an alphabetic or numeric digit character?           */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsAlphaDigit(const char c)
{
  if (isalpha(c) || isdigit(c))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is char an alphabetic, numeric digit or '_' character?                     */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsAlphaDigitUnder(const char c)
{
  if (isalpha(c) || isdigit(c) || '_' == c)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsAlphaUnder: is char an alphabetic or '_' character?                     */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsAlphaUnder(const char c)
{
  if (isalpha(c) || '_' == c)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsDigit: is char a numeric decimal digit?                                 */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsDigit(const char c)
{
  if (isdigit(c))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsNumeric: is the string numeric ?                                        */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsNumeric(char* ps)
{
  if ('-' == *ps)
    (ps)++;
  
  for ( ; *ps; (ps)++)
    if (!isdigit(*ps))
      return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* bIsHex: is char a hexadecimal digit?                                       */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsHex(const char c)
{
  if (c >= 'a' && c <= 'f') return true;
  if (c >= 'A' && c <= 'F') return true;
  if (c >= '0' && c <= '9') return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* bIsWhite: is char white space?                                             */
/*----------------------------------------------------------------------------*/
bool StrLib::bIsWhite(const char c)
{
  if (c == ' ')  return true;
  if (c == '\n') return true;
  if (c == '\r') return true;
  if (c == '\f') return true;
  if (c == '\t') return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* sLast: return pointer to last char in s                                    */
/*----------------------------------------------------------------------------*/
char* StrLib::sLast(const char* s)
{
  i8 nLen  = iLength(s);
  
  const char* z = s + nLen - 1;
  
  return (char*)z;
}

/*----------------------------------------------------------------------------*/
/* vSkipLeading: skip leading sequence                                        */
/*----------------------------------------------------------------------------*/
bool StrLib::bSkipLeading(char** ps, char cEnclosedBy_Leading)
{
  // skip leading spaces first
  while (' ' == **ps)
    (*ps)++;
  
  // do we see the leading enclose character?
  if (cEnclosedBy_Leading == **ps) {
    // skip the leading enclose character
    (*ps)++;
    return true;
  }
  else {
    return false;
  }
}

/*----------------------------------------------------------------------------*/
/* iLength: get length in bytes of arg (not including terminating null)       */
/*----------------------------------------------------------------------------*/
i8 StrLib::iLength(const char* s)
{
  i8 iLen = strlen(s);
  
  return iLen;
}

/*----------------------------------------------------------------------------*/
/* iLength: get length in bytes of arg (not including terminating null)       */
/*----------------------------------------------------------------------------*/
i8 StrLib::iLength(const char* s, const char cTerm, i8 iMax)
{
  if (! *s)
    return 0;
  
  i8 iLen = 1;
  
  for (const char* p = s; iLen < iMax; iLen++)
    if (! *p || *p++ == cTerm)
      break;
  
  return iLen;
}

/*----------------------------------------------------------------------------*/
/* sNew: new a string from formatted args                                     */
/*----------------------------------------------------------------------------*/
char* StrLib::sNew(const char* sFormat, ...)
{
  char w[C_STRING_ARRAY];
  
  va_list args;
  
  va_start(args,sFormat);
  vsprintf(w,sFormat,args);
  va_end(args);
  
  char* s = sDupe(w);
  
  return s;
}

/*----------------------------------------------------------------------------*/
/* vPad: pad string to given length with char                                 */
/*----------------------------------------------------------------------------*/
void StrLib::vPad(char* w, i8 iToLength, const char cWith)
{
  i8 iCurrLength = iLength(w);
  
  if (iCurrLength >= iToLength)
    return;
  
  char* z = zTerminator(w);
  i8 nChars = iToLength - iCurrLength;
  
  for (i8 iChar = 0; iChar < nChars; iChar++)
    *z++ = cWith;
  *z = '\0';
}

/*----------------------------------------------------------------------------*/
/* vRemove: remove any leading character                                      */
/*----------------------------------------------------------------------------*/
void StrLib::vRemove(char *w, const char cLeading)
{
  if (*w != cLeading || cLeading == '\0')
    return;
  
  char* pTo = w;
  char* pFrom = w;
  while (*pFrom == cLeading)
    pFrom++;
  
  for (;;) {
    char cCopied = (*pTo++ = *pFrom++);
    if ( ! cCopied )
      return;
  }
}
  
/*----------------------------------------------------------------------------*/
/* bReplace: replace sub-string                                               */
/*----------------------------------------------------------------------------*/
bool StrLib::bReplace(char *pString, const char *pThis, const char *pThat)
{
  //note: this function does the replacement within the given string
  //      therefore it must be large enough to handle expancesion
  
  // is this in tbe string?
  char* pSubstr = pString;
  if ( ! bFind(&pSubstr,pThis) )
    return false;
  
  // compute lengths and difference
  i8 iLenThis = iLength(pThis);
  i8 iLenThat = iLength(pThat);
  i8 iLenDiff = abs((int)iLenThis - (int)iLenThat);
  
  // which of 3 variants is it?
  if (iLenThis == iLenThat)
  {
    // (1) replacing same length
    
    // simply copy this into string at that substr
    while (*pThat)
      *pSubstr++ = *pThat++;
  }
  else
  if (iLenThis < iLenThat)
  {
    // (2) replacing with longer
    
    // shuffle to the right to make space
    char* pFrom  = pSubstr + iLength(pSubstr) - 1;
    char* pTo    = pFrom   + iLenDiff;
    char* pLimit = pSubstr + iLenThis;
    while (pFrom >= pLimit)
      *pTo-- = *pFrom--;

    // copy this into string at that substr
    while (*pThat)
      *pSubstr++ = *pThat++;
  }
  else
  {
    // (3) replacing with shorter
    
    // shuffle to the left to close space
    char* pFrom = pSubstr + iLenThis;
    char* pTo   = pFrom - iLenDiff;
    while (*pFrom)
      *pTo++ = *pFrom++;
    *pTo = '\0';
    
    // copy this into string at that substr
    while (*pThat)
      *pSubstr++ = *pThat++;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* bReplace: replace sub-string                                               */
/*----------------------------------------------------------------------------*/
bool StrLib::bReplace(char *pDst, const char* pSrc,
                      const char *pThis, const char *pThat)
{
  bool bReplaced = false;
  
NextIteration:
  
  // scan source string looking for matches
  while (*pSrc) {
  
    // non-matching case
    if (*pSrc != *pThis) {
      // just copy one src char to dst
      *pDst++ = *pSrc++;
      continue;
    }
    
    // mark start of possible match & replace
    const char* pMatch   = pSrc;
    const char* pReplace = pThis;
    
    // attempt to collect matching chars from src
    while (*pReplace) {
      
      // false match?
      if (*pSrc != *pReplace) {
        // false match, therefore copy the unmatching chars
        do {
          *pDst++ = *pMatch++;
        } while (pMatch < pSrc);
        goto NextIteration;
      }
      
      // matching so far...
      pSrc++;
      pReplace++;
    }
    
    // copy the replacement substring
    for (const char* pCopy = pThat; *pCopy; )
      *pDst++ = *pCopy++;
    
    // a replacement has been made
    bReplaced = true;
  }
  
  return bReplaced;
}

/*----------------------------------------------------------------------------*/
/* vReverse: reverse a string                                                 */
/*----------------------------------------------------------------------------*/
void StrLib::vReverse(char* azDst, char* azSrc)
{
  char* acSrc = azSrc + strlen(azSrc) - 1;
  char* acDst = azDst;

  while (acSrc >= azSrc)
    *acDst++ = *acSrc--;
  
  *acDst = '\0';
}

/*----------------------------------------------------------------------------*/
/* vRotate: rotate a string by given number of characters                     */
/*----------------------------------------------------------------------------*/
void StrLib::vRotate(char* zArg, i8 nBy)
{
  i8 nLen = iLength(zArg);
  ASSERT(nBy < nLen);
  
  char caFront[nBy];
  
  MemLib::vCopy(caFront,zArg,nBy,CN_NONE);
  
  MemLib::vMove(zArg,zArg+nBy,nBy,CN_NONE);
  
  MemLib::vCopy(zArg + nLen - nBy,caFront,nBy,CN_NONE);
}

/*----------------------------------------------------------------------------*/
/* bSame: are two arguments equal (no case)                                   */
/*----------------------------------------------------------------------------*/
bool StrLib::bSame(const char* pArg1, const char* pArg2)
{
  i8 iDiff = strcasecmp(pArg1,pArg2);
  
  if (0 == iDiff)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* iSize: get size in bytes of arg                                            */
/*----------------------------------------------------------------------------*/
i8 StrLib::iSize(const char* s)
{
  i8 nBytes = strlen(s) + 1;

  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* bSkipDigits: skip digit sequence in substr                                 */
/*----------------------------------------------------------------------------*/
bool StrLib::bSkipNonDigit(char** ps)
{
  char *s = *ps;
  
  for ( ; *s; s++)
    if (isdigit(*s))
      break;
  
  *ps = s;
  
  if (*s)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bSkipEOL: skip till end of line                                            */
/*----------------------------------------------------------------------------*/
bool StrLib::bSkipEOL(char** ps)
{
  while (**ps && '\n' != **ps)
    (*ps)++;
  (*ps)++;
  
  if (**ps)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bSkipNonAlnum: skip non-alphanumeric sequence in substr                    */
/*----------------------------------------------------------------------------*/
bool StrLib::bSkipNonAlnum(char** ps)
{
  for ( ; **ps; (*ps)++)
    if (isalnum(**ps))
      break;
  
  if (**ps)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bSkipDigits: skip non digit sequence in substr                             */
/*----------------------------------------------------------------------------*/
void StrLib::vSkipDigits(char** aaSrc)
{
  char *aSrc = *aaSrc;
  
  for ( ; *aSrc; aSrc++)
    if ( ! isdigit(*aSrc) )
      break;

  *aaSrc = aSrc;
}

/*----------------------------------------------------------------------------*/
/* vSkipSpaces: skip spaces                                                   */
/*----------------------------------------------------------------------------*/
void StrLib::vSkipSpaces(char** ps)
{
  while (' ' == **ps)
    (*ps)++;
}

/*----------------------------------------------------------------------------*/
/* bSkipTrailing: skip trailing sequence                                      */
/*----------------------------------------------------------------------------*/
bool StrLib::bSkipTrailing(char** ps,  bool bSawEnclosedBy_Leading,
                           char cEnclosedBy_Trailing)
{
  // skip any blanks
  while (' ' == **ps)
    (*ps)++;
  
  // if no leading enclosed then we have a valid end of the field
  if (! bSawEnclosedBy_Leading)
    return true;
  
  // test we see the trailing enclosed
  if (cEnclosedBy_Trailing == **ps) {
    // skip trailing enclosed
    (*ps)++;
    //skip any blanks
    while (' ' == **ps)
      (*ps)++;
    return true;
  }

  // we expected to see a trailing enclosed but didn't
  return false;
}

/*----------------------------------------------------------------------------*/
/* vSkipWhite: skip whitespace                                                */
/*----------------------------------------------------------------------------*/
void StrLib::vSkipWhite(char** ps)
{
  for ( ; **ps; (*ps)++)
    if (!bIsWhite(**ps))
      break;
}

/*----------------------------------------------------------------------------*/
/* vSprint: formatted sprint                                                  */
/*----------------------------------------------------------------------------*/
void StrLib::vSprint(char* sDst, Stz* aFormat, ...)
{
  va_list args;
  
  va_start(args,aFormat);
  vsprintf(sDst,aFormat->aChar(),args);
  va_end(args);
}

/*----------------------------------------------------------------------------*/
/* vSprint: formatted sprint                                                  */
/*----------------------------------------------------------------------------*/
void StrLib::vSprint(Stz* sDst, Stz* aFormat, ...)
{
  va_list args;
  
  va_start(args,aFormat);
  vsprintf(sDst->aChar(),aFormat->aChar(),args);
  va_end(args);
}

/*----------------------------------------------------------------------------*/
/* vSprint: formatted sprint                                                  */
/*----------------------------------------------------------------------------*/
void StrLib::vSprint(char* sDst, va_list args, const char* sFormat)
{
  vsprintf(sDst,sFormat,args);
}

/*----------------------------------------------------------------------------*/
/* vSprintCat: formatted concatenated sprint                                  */
/*----------------------------------------------------------------------------*/
void StrLib::vSprintCat(char* sDst, const char* sFormat, ...)
{
  va_list args;
  
  char* sEnd = sDst + iLength(sDst);
  
  va_start(args,sFormat);
  vsprintf(sEnd,sFormat,args);
  va_end(args);
}

/*----------------------------------------------------------------------------*/
/* vSprintCat: formatted concatenated sprint                                  */
/*----------------------------------------------------------------------------*/
void StrLib::vSprintCat(char* sDst, va_list args, const char* sFormat)
{
  char* sEnd = sDst + iLength(sDst);
  
  vsprintf(sEnd,sFormat,args);
}

/*----------------------------------------------------------------------------*/
/* vStrip: test last char and remove                                          */
/*----------------------------------------------------------------------------*/
void StrLib::vStrip(char* s, char cTrailing)
{
  i8 nBytes = strlen(s);
  
  if ( ! nBytes )
    return;
  
  i8 iOffset = nBytes - 1;
  
  if (s[iOffset] == cTrailing)
    s[iOffset] = '\0';
}
  
/*----------------------------------------------------------------------------*/
/* bToBool: convert arg to boolean                                            */
/*----------------------------------------------------------------------------*/
bool StrLib::bToBool(const char* s)
{
  if (StrLib::bEqualTo(s,"true" ,C_NO_CASE)) return true;
  if (StrLib::bEqualTo(s,"1"    ,C_NO_CASE)) return true;
  
  if (StrLib::bEqualTo(s,"false",C_NO_CASE)) return false;
  if (StrLib::bEqualTo(s,"0"    ,C_NO_CASE)) return false;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* cToHex: convert value to hexadecimal character                             */
/*----------------------------------------------------------------------------*/
char StrLib::cToHex(i8 i)
{
  char c;
  
  if (i >= 10)
    c = 'a' + i - 10;
  else
    c = '0' + i;
  
  return c;
}

/*----------------------------------------------------------------------------*/
/* iToI8: convert arg to i8                                                   */
/*----------------------------------------------------------------------------*/
i8 StrLib::iToI8(const char* s)
{
  //i8 i = atoll(s);

  //return i;
  
  i8 i = 0;
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
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* vToLower: convert arg to lower case                                        */
/*----------------------------------------------------------------------------*/
void StrLib::vToLower(char* s)
{
  while (*s) {
    *s = (char)tolower((unsigned char)*s);
    s++;
  }
}

/*----------------------------------------------------------------------------*/
/* wToPointer: convert given integer into its char/str form                   */
/*----------------------------------------------------------------------------*/
char* StrLib::wToPointer(void* pArg, char* wArg)
{
  i8 i = reinterpret_cast<i8>(pArg);
  
  *wArg++ = '0';
  *wArg++ = 'x';
  
  char* w = wArg;
  
  char *p = w, *q = w;
  do {
    *p++ = cToHex(i%16); //(i % 16) + '0';
    i /= 16ll;
  } while (i);
  
  
  *p = '\0';
  while (--p > q) {
    char c = *p;
    *p = *q;
    *q++ = c;
  }
  
  return w;
}

/*----------------------------------------------------------------------------*/
/* vToString: convert given float into its char/str form                      */
/*----------------------------------------------------------------------------*/
void StrLib::vToString(char* psDst, i8 iArg)
{
  wToString(iArg,psDst);
}

/*----------------------------------------------------------------------------*/
/* wToString: convert given float into its char/str form                      */
/*----------------------------------------------------------------------------*/
char* StrLib::wToString(d8 d, char* w)
{
  char caFloat[32];
  
  snprintf(caFloat,sizeof(caFloat),"%g",d);
  
  vAppendString(&w,caFloat);
  
  return w;
}

/*----------------------------------------------------------------------------*/
/* wToString: convert given integer into its char/str form                    */
/*----------------------------------------------------------------------------*/
char* StrLib::wToString(i8 i, char* w)
{
  bool bNegative = false;
  
  if (i < 0) {
    i = -i;
    bNegative = true;
  }
  
  char *p = w, *q = w;
  do {
    *p++ = (i % 10) + '0';
    i /= 10ll;
  } while (i);
  
  if (bNegative)
    *p++ = '-';
  
  *p = '\0';
  while (--p > q) {
    char c = *p;
    *p = *q;
    *q++ = c;
  }
  
  return w;
}

/*----------------------------------------------------------------------------*/
/* zTerminator: get ptr to null terminator                                    */
/*----------------------------------------------------------------------------*/
char* StrLib::zTerminator(char* s)
{
  i8 nLen  = iLength(s);
  
  char* z = s + nLen;
  
  return z;
}

/*----------------------------------------------------------------------------*/
/* bValidateMonth: validate month                                             */
/*----------------------------------------------------------------------------*/
bool StrLib::bValidateMonth(i8 iDay, i8 iMonth, i8 iYear)
{
  // assume a leap year
  bool bLeapYear = true;
  if (iYear % 4 == 0) {
    if ((iYear % 100) == 0 && (iYear % 400) != 0) {
      // not a leap year
      bLeapYear = false;
    }
  }

  if (bLeapYear) {
    if (iDay > (i8)iDaysPerMonthLeap[iMonth-1])
      return false;
  }
  else {
    if (iDay > (i8)iDaysPerMonth[iMonth-1])
      return false;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* flip the bytes in each word of src string into dst                         */
/*----------------------------------------------------------------------------*/
i8 StrLib::iWordFlip(char* aDst, char* aSrc)
{
  i8 iLenSrc  = iLength(aSrc);
  //vTrace("Flipping %s\n"_Stz, aSrc);
  
  i8 iRoundUp = 0;
  i8 iRemainder = iLenSrc % C_BYTES_PER_WORD;
  if (iRemainder)
    iRoundUp = C_BYTES_PER_WORD - iRemainder;
  
  i8 iDstSize = iLenSrc + iRoundUp;
  ASSERT(0 == iDstSize % C_BYTES_PER_WORD);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 iSrcLen = StrLib::iSize(aSrc);
  MemLib::vZero(aDst,iDstSize,CN_KEY);
  MemLib::vCopy(aDst, aSrc, iSrcLen, CN_KEY);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  w8* aWrd = (w8*)aDst;
  for (i8 nWords = iDstSize / C_BYTES_PER_WORD; nWords; nWords--)
    MemLib::vWordFlip(aWrd++);
  
  if (iLenSrc == iDstSize) {
     MemLib::vZero(aWrd,8,CN_KEY);
    iDstSize += C_BYTES_PER_WORD;
  }
  
  //MemLib::vDumpWords(aDst, iDstSize);
  
  return iDstSize;
}

/*----------------------------------------------------------------------------*/
/* flip the bytes in each word of src string into dst                         */
/*----------------------------------------------------------------------------*/
i8 StrLib::iWordFlipBack(char* aDst, char* aSrc)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //vTrace("WORDFLIPBACK\n"_Stz);
  //MemLib::vDumpWords(aSrc, 16);
  
  i8 iLenSrc = StrLib::iSize(aSrc);
  i8 iDstLen = 0;
  
  i8 iRoundUp = 0;
  i8 iRemainder = iLenSrc % C_BYTES_PER_WORD;
  if (iRemainder)
    iRoundUp = C_BYTES_PER_WORD - iRemainder;
  
  i8 iDstSize = iLenSrc + iRoundUp;
  ASSERT(0 == iDstSize % C_BYTES_PER_WORD);
  
  MemLib::vZero(aDst,iDstSize,CN_KEY);
  MemLib::vCopy(aDst, aSrc, iDstSize, CN_KEY);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  w8* aWrd = (w8*)aDst;
  for (i8 nWords = iDstSize / C_BYTES_PER_WORD; nWords; nWords--)
    MemLib::vWordFlip(aWrd++);
  
  
  //vTrace("AFTER BACK : %s\n"_Stz, aDst);
  //MemLib::vDumpWords(aDst, iDstSize);
  
  return iDstLen;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
