/*============================================================================*/
/* String.cpp - char* string utility                                          */
/*============================================================================*/

#include "String.hpp"

/*----------------------------------------------------------------------------*/
/* wAllocate: allocate a workspace/string of given size                       */
/*----------------------------------------------------------------------------*/
char* String::wAllocate(i8 nBytes)
{
  char* w = static_cast<char*>(malloc(nBytes));
  
  *w = '\0';
  
  return w;
}

/*----------------------------------------------------------------------------*/
/* wAllocate: allocate a workspace/string and initialize with given string    */
/*----------------------------------------------------------------------------*/
char* String::wAllocate(const char* s)
{
  i8 nBytes = iSize(s);
  
  char* w = wAllocate(nBytes);
  
  vCopy(w,s);
  
  return w;
}

/*----------------------------------------------------------------------------*/
/* vAppendChar: append character to end of workspace                          */
/*----------------------------------------------------------------------------*/
void String::vAppendChar(char** pz, char cChar)
{
  *(*pz)++ = cChar;
}

/*----------------------------------------------------------------------------*/
/* vAppendDecimal: append number in decimal to workspace                      */
/*----------------------------------------------------------------------------*/
void String::vAppendDecimal(char** pz, i8 iNumber)
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
void String::vAppendHex(char** pz, i8 iNumber)
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
void String::vAppendNull(char** pz)
{
  *(*pz)++ = '\0';
}
  
/*----------------------------------------------------------------------------*/
/* vAppendString: append string to workspace                                  */
/*----------------------------------------------------------------------------*/
void String::vAppendString(char** pz, const char* pString)
{
  char *z = *pz;
  
  while (*pString)
    *z++ = *pString++;
  
  *pz = z;
}

/*----------------------------------------------------------------------------*/
/* bBackward: scan backwards to find given character                          */
/*----------------------------------------------------------------------------*/
bool String::bBackward(const char** ps, char* pBegin, const char c)
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
/* iCompare: standard compare of the two args                                 */
/*----------------------------------------------------------------------------*/
i8 String::iCompare(const char* pArg1, const char* pArg2)
{
  i8 iDiff = strcmp(pArg1,pArg2);
  
  return iDiff;
}

/*----------------------------------------------------------------------------*/
/* iCompare: standard case-insensitive compare of the two args                */
/*----------------------------------------------------------------------------*/
bool String::iCompare(const char* pArg1, const char* pArg2, i8 iCase)
{
  i8 iDiff;
  
  if (C_NO_CASE == iCase)
    iDiff = strcasecmp(pArg1,pArg2);
  else
    iDiff = strcmp(pArg1,pArg2);
  
  return iDiff;
}

/*----------------------------------------------------------------------------*/
/* vConcat: concatenate character to workspace                                */
/*----------------------------------------------------------------------------*/
void String::vConcat(char* pWS, char cChar)
{
  i8 i = iLength(pWS);
  pWS[i] = cChar;
  pWS[i+1] = '\0';
}

/*----------------------------------------------------------------------------*/
/* vConcat: concatenate arg onto dst string                                   */
/*----------------------------------------------------------------------------*/
void String::vConcat(char* psDst, const char* psArg)
{
  strcat(psDst,psArg);
}

/*----------------------------------------------------------------------------*/
/* vConcat: concatenate arg1 & arg2 onto dst string                           */
/*----------------------------------------------------------------------------*/
void String::vConcat(char* psDst, const char* psArg1, const char* psArg2)
{
  strcat(psDst,psArg1);
  strcat(psDst,psArg2);
}

/*----------------------------------------------------------------------------*/
/* vConcat: concatenate i8 arg onto dst string                                */
/*----------------------------------------------------------------------------*/
void String::vConcat(char* psDst, i8 iArg)
{
  char aInt[32];
  
  wToString(iArg,aInt);
  vConcat(psDst,aInt);
}

/*----------------------------------------------------------------------------*/
/* sConcat: concatenate two strings                                           */
/*----------------------------------------------------------------------------*/
char* String::wConcat(const char* s1, const char* s2)
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
bool String::bContains(char* s, const char cArg)
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
bool String::bContains(char* s, const char* sArg)
{
  char* f = strstr(s,sArg);
  
  if (f)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* vCopy: copy src into dst                                                   */
/*----------------------------------------------------------------------------*/
void String::vCopy(char* psDst, const char* psSrc)
{
  strcpy(psDst,psSrc);
}

/*----------------------------------------------------------------------------*/
/* vCopyLen: copy src into dst uptill nBytes                                  */
/*----------------------------------------------------------------------------*/
void String::vCopyLen(char* psDst, const char* psSrc, i8 nBytes)
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
void String::vCopySplit(char* w1, char* w2, const char* sSrc, char cSplit)
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
void String::vCopyTill(char* psDst, const char* psSrc, char ccTill)
{
  char *d = psDst;
  const char *s = psSrc;
  
  while (*s && ccTill != *s)
    *d++ = *s++;
  
  *d = '\0';
}

/*----------------------------------------------------------------------------*/
/* vDump: dump string                                                         */
/*----------------------------------------------------------------------------*/
void String::vDump(const char* s, i8 nMax, const char* pSuffix)
{
  char w[C_STACK_WSP];
  char *z = w;
  
  for (int i = 0; i < nMax && i < 16_KB - 1; i++)
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
  
  printf("%s",w);
}

/*----------------------------------------------------------------------------*/
/* sDupe: duplicate string                                                    */
/*----------------------------------------------------------------------------*/
char* String::sDupe(const char* psArg)
{
  i8 iSize = String::iSize(psArg);

  char* psString = wAllocate(iSize);

  String::vCopy(psString,psArg);

  return psString;
}

/*----------------------------------------------------------------------------*/
/* sDupeStripped: duplicate stripped string                                   */
/*----------------------------------------------------------------------------*/
char* String::sDupeStripped(const char* s)
{
  if ('"' != *s)
    return sDupe(s);
  
  i8 iSize = String::iSize(s) - 2;
  
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
bool String::bEnds(const char* s, const char c)
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
bool String::bEnds(const char* sArg, const char* sEnd)
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
/* vEscape: put a '\' before and '"'                                          */
/*----------------------------------------------------------------------------*/
void String::vEscape(const char* sArg, char* sEscaped)
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
/* bEqualTo: test if the two args match                                       */
/*----------------------------------------------------------------------------*/
bool String::bEqualTo(const char* s1, const char* s2)
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
bool String::bEqualTo(const char* pArg1, const char* pArg2, i8 iCase)
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
bool String::bExtractArg(char** ps, char* pArg)
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
/* bExtractI8: extract an integer from substr                                 */
/*----------------------------------------------------------------------------*/
bool String::bExtractI8(char** ps, i8* pInw)
{
  char *s = *ps;
  
  if ( ! *s )
    return false;
  
  bool bOnce = false;
  if (*s && isdigit(*s))
    bOnce = true;
  
  i8 v = 0;
  for ( ; *s; s++)
  {
    char c = *s;
    
//    if ( ! isdigit(c) )
    if ( c < '0' || c > '9' )
      break;
    
    v = (v * 10) + c - '0';
  }
  
  *pInw = v;
  
  *ps = s;
  
  return bOnce;
}

/*----------------------------------------------------------------------------*/
/* bExtractItem: extract an item from substr                                  */
/*----------------------------------------------------------------------------*/
bool String::bExtractItem(char** ps, char* pItem)
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
/* bExtractPath: extract a path from substr                                   */
/*----------------------------------------------------------------------------*/
bool String::bExtractPath(char** ps, char* pPath)
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
bool String::bExtractPort(char** ps, u2* pPort)
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
/* bExtractUTC: extract a utc time from substr       yyyy-mm-ddThh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
//bool String::bExtractUTC(char** ps, Time* pTime)
//{
//  // refer: http://en.wikipedia.org/wiki/ISO_8601
//  
//  if ( ! *ps )
//    return false;
//  
//  pTime->m_iYear   = 0; pTime->m_iMonth  = 0; pTime->m_iDay    = 0;
//  pTime->m_iHour   = 0; pTime->m_iMinute = 0; pTime->m_iSecond = 0;
//  pTime->m_iMicro  = 0;
//  
//  i8 v;
//  
//  if ( ! bExtractInw(ps,&v) ) return false;
//  pTime->m_iYear = (unsigned int)v - 1900;
//
//  if ('-' != *(*ps)++) return false;
//  
//  if ( ! bExtractInw(ps,&v) ) return false;
//  pTime->m_iMonth = (unsigned int)v;
//  
//  if ('-' != *(*ps)++) return false;
//  
//  if ( ! bExtractInw(ps,&v) ) return false;
//  pTime->m_iDay = (unsigned int)v;
//  
//  // - - -
//  
//  if ('T' != **ps && ' ' != **ps) return true;
//  
//  (*ps)++;
//  
//  if ( ! bExtractInw(ps,&v) ) return true;
//  pTime->m_iHour = (unsigned int)v;
//
//  if (':' != *(*ps)++) return true;
//  
//  if ( ! bExtractInw(ps,&v) ) return true;
//  pTime->m_iMinute = (unsigned int)v;
//
//  if (':' != *(*ps)++) return true;
//  
//  if ( ! bExtractInw(ps,&v) ) return true;
//  pTime->m_iSecond = (unsigned int)v;
//
//  if ('.' == **ps)
//  {
//    (*ps)++;
//    if ( bExtractInw(ps,&v) )
//      pTime->m_iMicro  = (unsigned int)(v*1000);
//  }
//  
//  if ('Z' == **ps)
//    (*ps)++;
//  
//  return true;
//}

/*----------------------------------------------------------------------------*/
/* bExtractString: extract a quoted string from substr                        */
/*----------------------------------------------------------------------------*/
bool String::bExtractString(char** ps, char* pString)
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
/* bExtractWord: extract a word from substr                                   */
/*----------------------------------------------------------------------------*/
bool String::bExtractWord(char** ps, char* pWord)
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
bool String::bExtractWordString(char** ps, char* pWord)
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
/* bFind: find start of ScanChar in substr                                    */
/*----------------------------------------------------------------------------*/
bool String::bFind(char** ps, const char cArg)
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
/* bFind: find start of arg in substr                                         */
/*----------------------------------------------------------------------------*/
bool String::bFind(char** ps, const char* sArg)
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
bool String::bFind(char** ps, const char* sArg, i8 iLenArg)
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
/* vFree: free a dynamic string                                               */
/*----------------------------------------------------------------------------*/
void String::vFree(char** ps)
{
  free(*ps);
  
  *ps = nullptr;
}

/*----------------------------------------------------------------------------*/
/* iHexToInw: convert hex character to i8 (-1 on error)                       */
/*----------------------------------------------------------------------------*/
i8 String::iHexToInw(const char c)
{
  if (c >= 'a' && c <= 'f') return c - 'a' + 10;
  if (c >= 'A' && c <= 'F') return c - 'A' + 10;
  if (c >= '0' && c <= '9') return c - '0';
  
  return -1;
}

/*----------------------------------------------------------------------------*/
/* bIsAlpha: is char an alphabetic character?                                 */
/*----------------------------------------------------------------------------*/
bool String::bIsAlpha(const char c)
{
  if (isalpha(c))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsAlphaDigit: is char an alphabetic or numeric digit character?           */
/*----------------------------------------------------------------------------*/
bool String::bIsAlphaDigit(const char c)
{
  if (isalpha(c) || isdigit(c))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is char an alphabetic, numeric digit or '_' character?                     */
/*----------------------------------------------------------------------------*/
bool String::bIsAlphaDigitUnder(const char c)
{
  if (isalpha(c) || isdigit(c) || '_' == c)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsAlphaUnder: is char an alphabetic or '_' character?                     */
/*----------------------------------------------------------------------------*/
bool String::bIsAlphaUnder(const char c)
{
  if (isalpha(c) || '_' == c)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsDigit: is char a numeric decimal digit?                                 */
/*----------------------------------------------------------------------------*/
bool String::bIsDigit(const char c)
{
  if (isdigit(c))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* bIsNumeric: is the string numeric ?                                        */
/*----------------------------------------------------------------------------*/
bool String::bIsNumeric(char* ps)
{
  for ( ; *ps; (ps)++)
    if (!isdigit(*ps))
      return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* bIsHex: is char a hexadecimal digit?                                       */
/*----------------------------------------------------------------------------*/
bool String::bIsHex(const char c)
{
  if (c >= 'a' && c <= 'f') return true;
  if (c >= 'A' && c <= 'F') return true;
  if (c >= '0' && c <= '9') return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* bIsWhite: is char white space?                                             */
/*----------------------------------------------------------------------------*/
bool String::bIsWhite(const char c)
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
const char* String::sLast(const char* s)
{
  i8 nLen  = iLength(s);
  
  const char* z = s + nLen - 1;
  
  return z;
}

/*----------------------------------------------------------------------------*/
/* iLength: get length in bytes of arg (not including terminating null)       */
/*----------------------------------------------------------------------------*/
i8 String::iLength(const char* s)
{
  i8 iLen = strlen(s);
  
  return iLen;
}

/*----------------------------------------------------------------------------*/
/* iLength: get length in bytes of arg (not including terminating null)       */
/*----------------------------------------------------------------------------*/
i8 String::iLength(const char* s, const char cTerm, i8 iMax)
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
char* String::sNew(const char* sFormat, ...)
{
  char w[C_STACK_WSP];
  
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
void String::vPad(char* w, i8 iToLength, const char cWith)
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
void String::vRemove(char *w, const char cLeading)
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
bool String::bReplace(char *pString, const char *pThis, const char *pThat)
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
/* bSame: are two arguments equal (no case)                                   */
/*----------------------------------------------------------------------------*/
bool String::bSame(const char* pArg1, const char* pArg2)
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
i8 String::iSize(const char* s)
{
  i8 nBytes = strlen(s) + 1;

  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* bSkipEOL: skip till end of line                                            */
/*----------------------------------------------------------------------------*/
bool String::bSkipEOL(char** ps)
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
bool String::bSkipNonAlnum(char** ps)
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
/* bSkipNonDigit: skip non digit sequence in substr                           */
/*----------------------------------------------------------------------------*/
bool String::bSkipNonDigit(char** ps)
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
/* vSkipWhite: skip whitespace                                                */
/*----------------------------------------------------------------------------*/
void String::vSkipWhite(char** ps)
{
  for ( ; **ps; (*ps)++)
    if (!bIsWhite(**ps))
      break;
}

/*----------------------------------------------------------------------------*/
/* vSprint: formatted sprint                                                  */
/*----------------------------------------------------------------------------*/
void String::vSprint(char* sDst, const char* sFormat, ...)
{
  va_list args;
  
  va_start(args,sFormat);
  vsprintf(sDst,sFormat,args);
  va_end(args);
}

/*----------------------------------------------------------------------------*/
/* vSprint: formatted sprint                                                  */
/*----------------------------------------------------------------------------*/
void String::vSprint(char* sDst, va_list args, const char* sFormat)
{
  vsprintf(sDst,sFormat,args);
}

/*----------------------------------------------------------------------------*/
/* vSprintCat: formatted concatenated sprint                                  */
/*----------------------------------------------------------------------------*/
void String::vSprintCat(char* sDst, const char* sFormat, ...)
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
void String::vSprintCat(char* sDst, va_list args, const char* sFormat)
{
  char* sEnd = sDst + iLength(sDst);
  
  vsprintf(sEnd,sFormat,args);
}

/*----------------------------------------------------------------------------*/
/* vStrip: test last char and remove                                          */
/*----------------------------------------------------------------------------*/
void String::vStrip(char* s, char c)
{
  i8 nBytes = strlen(s);
  
  if ( ! nBytes )
    return;
  
  i8 iOffset = nBytes - 1;
  
  if (s[iOffset] == c)
    s[iOffset] = '\0';
}
  
/*----------------------------------------------------------------------------*/
/* bToBool: convert arg to boolean                                            */
/*----------------------------------------------------------------------------*/
bool String::bToBool(const char* s)
{
  if (String::bEqualTo(s,"true" ,C_NO_CASE)) return true;
  if (String::bEqualTo(s,"1"    ,C_NO_CASE)) return true;
  
  if (String::bEqualTo(s,"false",C_NO_CASE)) return false;
  if (String::bEqualTo(s,"0"    ,C_NO_CASE)) return false;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* cToHex: convert value to hexadecimal character                             */
/*----------------------------------------------------------------------------*/
char String::cToHex(i8 i)
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
i8 String::iToI8(const char* s)
{
  //i8 i = atoll(s);

  //return i;
  
  i8 i = 0;
  
  // skip leading 0's
  while (*s && '0' == *s)
    s++;
  
  if (*s && ('x' == *s || 'X' == *s)) {
    s++;
    while (*s) {
      i8 j = iHexToInw(*s++);
      i = i * 16 + j;
    }
  }
  else {
    while (*s) {
      i8 j = (*s++ - '0');
      i = i * 10 + j;
    }
  }
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* vToLower: convert arg to lower case                                        */
/*----------------------------------------------------------------------------*/
void String::vToLower(char* s)
{
  while (*s) {
    *s = (char)tolower((unsigned char)*s);
    s++;
  }
}

/*----------------------------------------------------------------------------*/
/* wToPointer: convert given integer into its char/str form                   */
/*----------------------------------------------------------------------------*/
char* String::wToPointer(void* pArg, char* wArg)
{
  i8 i = reinterpret_cast<i8>(pArg);
  
  *wArg++ = '0';
  *wArg++ = 'x';
  
  char* w = wArg;
  
  char *p = w, *q = w;
  do {
    *p++ = cToHex(i%16); //(i % 16) + '0';
    i /= 16ll;
  } while(i);
  
  
  *p = '\0';
  while(--p > q) {
    char c = *p;
    *p = *q;
    *q++ = c;
  }
  
  return w;
}

/*----------------------------------------------------------------------------*/
/* wToString: convert given integer into its char/str form                    */
/*----------------------------------------------------------------------------*/
char* String::wToString(i8 i, char* w)
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
  } while(i);
  
  if (bNegative)
    *p++ = '-';
  
  *p = '\0';
  while(--p > q) {
    char c = *p;
    *p = *q;
    *q++ = c;
  }
  
  return w;
}

/*----------------------------------------------------------------------------*/
/* zTerminator: get ptr to null terminator                                    */
/*----------------------------------------------------------------------------*/
char* String::zTerminator(char* s)
{
  i8 nLen  = iLength(s);
  
  char* z = s + nLen;
  
  return z;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
