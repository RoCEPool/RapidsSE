/*============================================================================*/
/* File.cpp -- os file                                                        */
/*============================================================================*/

#include "Resources.h"


Atomic gixFile;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
File* Resources::_pFile(void* aAddr)
{
  File* pFile = reinterpret_cast<File*>(aAddr);
  
  RASSERT_TYPE(pFile,CN_FILE);
  
  return pFile;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* File::operator new(size_t)
{
  File* pFile = (File*)HeapMem::pAllocate(CN_FILE);
  
  pFile->m_iViaMalloc = 0;
  
  return pFile;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* File::operator new(size_t, i8 iViaMalloc)
{
  File* pFile;
  
  if (iViaMalloc)
    pFile = (File*)MemLib::aMallocBlock(sizeof(File));
  else
    pFile = (File*)HeapMem::pAllocate(CN_FILE);

  pFile->m_iViaMalloc = iViaMalloc;
  
  return pFile;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void File::operator delete(void* pFileAddress)
{
  File* pFile = (File*)pFileAddress;
  
  if (pFile->m_iViaMalloc)
    MemLib::vFreeBlock(pFile,sizeof(File));
  else
    HeapMem::vFree(pFile);

}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
File::File(Focal* bIsFor)
: Resource(CN_FILE,bIsFor)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  if ( ! (0 == m_iViaMalloc || 1 == m_iViaMalloc) )
    m_iViaMalloc = 0;
  
  //m_sFilePath
  //m_sOpenMode
  m_aFile    = nullptr;
  
  m_oSysCall.vClear();

  _checkInitialised_(CN_FILE);
  
  ++ gixFile;
  AM::vLogRes(SL_INF,this,RA_FIL_CONSTRUCT,
              "File construct |gixFile=%lld|"_Stz, //!format-LogRes
              * gixFile);
  
  _postcondition_(_MAGIC_,CN_FILE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
File::~File(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  if (m_aFile) {
    vWarn("File::~File file not closed: %s %s\n"_Stz,m_sFilePath.azChar(),m_sOpenMode.azChar());

    vClose();
    
    if (m_oSysCall.bHadError())
      vWarn("File::~File: file could not be closed: %s"_Stz,m_sFilePath.azChar());
  }
  
  -- gixFile;
  AM::vLogRes(SL_INF,this,RA_FIL_DESTRUCT,
              "File destruct |gixFile=%lld|"_Stz, //!format-LogRes
              * gixFile);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* test if given file has access permissions                                 */
/*---------------------------------------------------------------------------*/
bool File::bAccess(Stz* szFilePath, i8 iPermission)
{
  int iResult = access(szFilePath->azChar(),(int)iPermission);

  if (0 == iResult)
    return true;
  else
    return false;
}

/*---------------------------------------------------------------------------*/
/* test if given file exists in file system                                  */
/*---------------------------------------------------------------------------*/
bool File::bExists(Stz* szFilePath, bool bWarning)
{
  struct stat xStat;
  
  int iResult = stat(szFilePath->azChar(),&xStat);
  
  if (0 == iResult && (xStat.st_mode & S_IFREG))
    return true;
  
  if (iResult && ENOENT == errno)
    return false;
  
  if (bWarning)
    vWarn("File::bExists: %s stat() error; errno=%d"_Stz, szFilePath->azChar(), errno);
  
  return false;
}

/*---------------------------------------------------------------------------*/
/* get the modification time of the file                                     */
/*---------------------------------------------------------------------------*/
bool File::bModTime(Stz* szFilePath, time_t& tTime)
{
  struct stat xStat;
  
  int iResult = stat(szFilePath->azChar(),&xStat);
  
  if (0 == iResult) {
    tTime = xStat.st_mtime;
    return true;
  }
  
  if (iResult && ENOENT == errno)
    return false;
  
  vWarn("File::bModTime: %s stat() error; errno=%d"_Stz, szFilePath->azChar(), errno);
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* monitor profile                                                            */
/*----------------------------------------------------------------------------*/
void File::vMonitorPortrait(s48K& sPortrait)
{
  Statistics* pStats = AM::pStatistics();

  s32 sReadMag;  MagLib::vByteMag(pStats->File.m_nReadBytes,  sReadMag);
  s32 sWriteMag; MagLib::vByteMag(pStats->File.m_nWriteBytes, sWriteMag);
  s32 sPrintMag; MagLib::vByteMag(pStats->File.m_nPrintBytes, sPrintMag);

  sPortrait.vSprint("File: "
                    "opens %lld "
                    "closes %lld "
                    "seeks %lld "
                    "tells %lld "
                    "flushes %lld\n"
                    "      "
                    "reads %lld (%s) "
                    "writes %lld (%s) "
                    "prints %lld (%s)\n"
                    "      "
                    "getcs %lld "
                    "ungetcs %lld "
                    "\n"_Stz,
                   
                    pStats->File.m_nOpens,
                    pStats->File.m_nCloses,
                    pStats->File.m_nSeeks,
                    pStats->File.m_nTells,
                    pStats->File.m_nFlushes,
                    
                    pStats->File.m_nReads,
                      sReadMag.szStz(),
                    pStats->File.m_nWrites,
                      sWriteMag.szStz(),
                    pStats->File.m_nPrints,
                      sPrintMag.szStz(),

                    pStats->File.m_nGetcs,
                    pStats->File.m_nUngetcs
                   );
  
  
}

/*----------------------------------------------------------------------------*/
/* remove given file from file system                                         */
/*----------------------------------------------------------------------------*/
bool File::bRemove(Stz* szFilePath)
{
  int iResult = unlink(szFilePath->azChar());
  
  if (-1 == iResult) {
    vWarn("File::bRemove: unlink() error; errno=%d"_Stz,errno);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get size of named file                                                     */
/*----------------------------------------------------------------------------*/
i8 File::nSize(Stz* szFilePath)
{
  struct stat xStat;
  
  int iResult = stat(szFilePath->azChar(),&xStat);
  
  if (0 == iResult)
    return xStat.st_size;
  
  vWarn("File::iSize: %s stat() error; errno=%d"_Stz, szFilePath->azChar(), errno);
  
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* print file statistics                                                      */
/*----------------------------------------------------------------------------*/
void File::vStatistics(void)
{
  Statistics* pStats = AM::pStatistics();

  s32 sByteMag;
  
  vTrace("  nFileCloses  %lld\n"_Stz,pStats->File.m_nCloses);
  vTrace("  nFileFlushes %lld\n"_Stz,pStats->File.m_nFlushes);
  vTrace("  nFileGetcs   %lld\n"_Stz,pStats->File.m_nGetcs);
  vTrace("  nFileOpens   %lld\n"_Stz,pStats->File.m_nOpens);
  vTrace("  nFilePrints  %lld\n"_Stz,pStats->File.m_nPrints);
  vTrace("  nFileReads   %lld\n"_Stz,pStats->File.m_nReads);
  vTrace("  nFileWrites  %lld\n"_Stz,pStats->File.m_nWrites);
  vTrace("  nFileSeeks   %lld\n"_Stz,pStats->File.m_nSeeks);
  vTrace("  nFileTells   %lld\n"_Stz,pStats->File.m_nTells);
  vTrace("  nFileUngetcs %lld\n"_Stz,pStats->File.m_nUngetcs);
  vTrace("\n"_Stz);

  MagLib::vByteMag(pStats->File.m_nReadBytes,  sByteMag);
  vTrace("  nFileReadBytes  %s\n"_Stz,sByteMag.szStz());
  MagLib::vByteMag(pStats->File.m_nPrintBytes, sByteMag);
  vTrace("  nFilePrintBytes %s\n"_Stz,sByteMag.szStz());
  MagLib::vByteMag(pStats->File.m_nWriteBytes, sByteMag);
  vTrace("  nFileWriteBytes %s\n"_Stz,sByteMag.szStz());
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* close file                                                                 */
/*----------------------------------------------------------------------------*/
void File::vClose(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

#ifdef WITH_STATISTICS_COUNTERS
  if (gpAttachMap) gpAttachMap->m_oStatistics.File.m_nCloses++;
#endif
  
  m_oSysCall.vClear();

  // never close stdin or stdout
  if (bIsStdin() || bIsStdout()) {
    vReset();
    return;
  }
  
  if (! m_aFile) {
    m_oSysCall.vFailure(0);
    vWarn("File::vClose: file not open: %s"_Stz,m_sFilePath.azChar());
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 
  i8 iResult = fclose(m_aFile);
  m_aFile = nullptr;

  if ((char)EOF == iResult) {
    m_oSysCall.vError(errno);
    vWarn("File::vClose: fclose(%p) error; errno=%d : %s %s"_Stz,
          m_aFile,errno,m_sFilePath.azChar(),m_sOpenMode.azChar());
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (! m_iViaMalloc && AM::bAttached()) AM::pResourceTracker()->vDecFileOpen();
  
  AM::vLogRes(SL_INF,this,RA_FIL_CLOSED,
              "File closed: path=%s"_Stz, //!format-LogRes
              m_sFilePath.szStz());

  vReset();
  
  m_oSysCall.vSuccess();
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void File::vFileDump(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  vTrace("%sFile:\n"_Stz, m_sFilePath.azChar());
  vTrace("%s  Open Mode %s\n"_Stz, m_sOpenMode.azChar());
  vTrace("%s  File Pointer 0x%p\n"_Stz, m_aFile);
}

/*----------------------------------------------------------------------------*/
/* get file path                                                              */
/*----------------------------------------------------------------------------*/
s256& File::sFilePath(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  return m_sFilePath;
}

/*---------------------------------------------------------------------------*/
/* flush file                                                                */
/*---------------------------------------------------------------------------*/
void File::vFlush(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_oSysCall.vClear();

#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.File.m_nFlushes++;
#endif
  
  i8 iResult = fflush(m_aFile);

  if ((char)EOF == iResult) {
    m_oSysCall.vError(errno);
    vWarn("File::vFlush: fflush() error; errno=%d"_Stz,errno);
    return;
  }

  m_oSysCall.vSuccess();
}

/*----------------------------------------------------------------------------*/
/* focal outline                                                              */
/*----------------------------------------------------------------------------*/
void File::vFocalOutline(s4K& sOutline)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  sOutline.vSprint("<%s focal>"_Stz,aCN[eClass()]);
}

/*----------------------------------------------------------------------------*/
/* get next character                                                         */
/*----------------------------------------------------------------------------*/
char File::cGetChar(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  if ( ! m_aFile )
    return 0;

#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.File.m_nGetcs++;
#endif
  
  char cChar = getc(m_aFile);
  if ((char)EOF == cChar)
    cChar = 0;
  
  return cChar;
}

/*----------------------------------------------------------------------------*/
/* is file close?                                                             */
/*----------------------------------------------------------------------------*/
bool File::bIsClosed(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  if ( ! m_aFile )
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is file open?                                                              */
/*----------------------------------------------------------------------------*/
bool File::bIsOpen(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  if (m_aFile)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is stdin?                                                                  */
/*----------------------------------------------------------------------------*/
bool File::bIsStdin(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  if (stdin == m_aFile)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is stdout?                                                                 */
/*----------------------------------------------------------------------------*/
bool File::bIsStdout(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  if (stdout == m_aFile)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get next lite/simple token                                                 */
/*----------------------------------------------------------------------------*/
bool File::bNextLiteTok(Stz *szDst)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  m_oSysCall.vClear();

  char *az = szDst->azChar();

  char cNextChar = getc(m_aFile);
  
  // skip spaces
  if (' ' == cNextChar) {
    while (' ' == cNextChar) {
      *az++ = cNextChar;
      cNextChar = getc(m_aFile);
    }
    ungetc(cNextChar,m_aFile);
    *az = '\0';
    return true;
  }
  
  if ('\n' == cNextChar) {
    *az++ = '\n';
    *az = '\0';
    return true;
  }
    
  if (' ' == cNextChar) {
    *az = '\0';
    ungetc(cNextChar,m_aFile);
    return true;
  }
  
  if ('\n' == cNextChar) {
    *az = '\0';
    ungetc(cNextChar,m_aFile);
    return true;
  }
  
  if ((char)EOF == cNextChar) {
    if (az == szDst->azChar()) {
      *az = '\0';
      return false;
    }
    else {
      ungetc(EOF,m_aFile);
      *az = '\0';
      return true;
    }
  }
  
  if ('/' == cNextChar) {
    cNextChar = getc(m_aFile);
    if ('/' == cNextChar) {
      *az++ = '/';
      *az++ = '/';
      for (;;) {
        cNextChar = getc(m_aFile);
        if ('\n' == cNextChar) {
          ungetc(cNextChar,m_aFile);
          break;
        }
        *az++ = cNextChar;
      }
      *az = '\0';
      return true;
    }

    ungetc(cNextChar,m_aFile);
    cNextChar = '/';
  }
  
  if ('/' == cNextChar) {
    cNextChar = getc(m_aFile);
    if ('*' == cNextChar) {
      char cPrevChar;
      do {
        cPrevChar = cNextChar;
        cNextChar = getc(m_aFile);
        *az++ = cNextChar;
      } while (! ('*' == cPrevChar && '/' == cNextChar) );

      *az = '\0';
      return true;
    }
    ungetc(cNextChar,m_aFile);
    cNextChar = '/';
  }

  if ('"' == cNextChar) {
    
    for (;;) {
      *az++ = cNextChar;
      cNextChar = getc(m_aFile);
      if ('\\' == cNextChar) {
        *az++ = cNextChar;
        cNextChar = getc(m_aFile);
        *az++ = cNextChar;
        continue;
      }
      if ('"' == cNextChar) {
        *az++ = cNextChar;
        *az = '\0';
        return true;
      }
    }
    
  }
  
  if ('\'' == cNextChar) {
    for (;;) {
      *az++ = cNextChar;
      cNextChar = getc(m_aFile);
      if ('\\' == cNextChar) {
        *az++ = cNextChar;
        cNextChar = getc(m_aFile);
        *az++ = cNextChar;
        continue;
      }
      if ('\\' == cNextChar) {
        *az++ = cNextChar;
        *az = '\0';
        return true;
      }
    }
  }
       
  do {
    *az++ = cNextChar;
    cNextChar = getc(m_aFile);
  } while (' ' != cNextChar && '\n' != cNextChar && EOF != cNextChar);
  ungetc(cNextChar,m_aFile);

  *az = '\0';
  return true;
}

/*----------------------------------------------------------------------------*/
/* open file for append                                                       */
/*----------------------------------------------------------------------------*/
bool File::bOpenAppend(Stz* szFilePath)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  bool bOpened = bOpenMode(szFilePath,C_OPEN_APPEND);
  
  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
bool File::bOpenMode(Stz* szFilePath, Stz* aOpenMode)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
#ifdef WITH_STATISTICS_COUNTERS
  if (gpAttachMap) gpAttachMap->m_oStatistics.File.m_nOpens++;
#endif
  
  if (szFilePath->bSame("stdin"_Stz)) {
    vUseStdin();
    return true;
  }

  if (szFilePath->bSame("stdout"_Stz)) {
    vUseStdout();
    return true;
  }
  
  if (m_aFile) {
    vWarn("File::bOpen: previous file not closed: %s\n"_Stz,m_sFilePath.azChar());
    vClose();
  }
  
  m_oSysCall.vClear();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_sFilePath   = szFilePath;
  m_sOpenMode   = aOpenMode;

  m_aFile = fopen(m_sFilePath.azChar(),m_sOpenMode.azChar());
  
  if (! m_aFile) {
    m_oSysCall.vError(errno);
    fprintf(stderr, "OpenMode failed\n");
    vTrace("File::bOpen: fopen(%s,%s) error; errno=%d\n"_Stz,
          m_sFilePath.azChar(),m_sOpenMode.azChar(),errno);
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (! m_iViaMalloc && AM::bAttached()) AM::pResourceTracker()->vIncFileOpen();
  
  AM::vLogRes(SL_INF,this,RA_FIL_OPENED,
              "File opened: path=%s; mode=%s"_Stz, //!format-LogRes
              szFilePath,aOpenMode);

  return true;
}

/*----------------------------------------------------------------------------*/
/* open file for read                                                         */
/*----------------------------------------------------------------------------*/
bool File::bOpenRead(Stz* szFilePath)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  bool bOpened = bOpenMode(szFilePath,C_OPEN_READ);
  
  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* open file for write                                                        */
/*----------------------------------------------------------------------------*/
bool File::bOpenWrite(Stz* szFilePath)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  bool bOpened = bOpenMode(szFilePath,C_OPEN_WRITE);
  
  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* formatted print                                                            */
/*----------------------------------------------------------------------------*/
void File::vPrint(Stz* aFormat, ...)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_oSysCall.vClear();
  
  va_list args;

  va_start(args,aFormat);
  
#ifdef WITH_STATISTICS_COUNTERS
  if (gpAttachMap) gpAttachMap->m_oStatistics.File.m_nPrints++;
#endif
  
  i8 nPrinted = vfprintf(m_aFile,aFormat->azChar(),args);
  
#ifdef WITH_STATISTICS_COUNTERS
  if (gpAttachMap) gpAttachMap->m_oStatistics.File.m_nPrintBytes += nPrinted;
#endif
  
  va_end(args);

  if (-1 == nPrinted) {
    m_oSysCall.vError(errno);
    vWarn("File::vPrint: vfprintf() error; errno=%d"_Stz,errno);
    return;
  }
  
  m_oSysCall.vSuccess();
}

/*----------------------------------------------------------------------------*/
/* formatted print                                                            */
/*----------------------------------------------------------------------------*/
void File::vPrint(va_list args, Stz* aFormat)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

#ifdef WITH_STATISTICS_COUNTERS
  if (gpAttachMap) gpAttachMap->m_oStatistics.File.m_nPrints++;
#endif
  
  i8 nPrinted = vfprintf(m_aFile,aFormat->azChar(),args);
  
#ifdef WITH_STATISTICS_COUNTERS
  if (gpAttachMap) gpAttachMap->m_oStatistics.File.m_nPrintBytes += nPrinted;
#endif
  
  va_end(args);
  
  if (-1 == nPrinted) {
    m_oSysCall.vError(errno);
    vWarn("File::vPrint: vfprintf() error; errno=%d"_Stz,errno);
    return;
  }
  
  m_oSysCall.vSuccess();
}

/*----------------------------------------------------------------------------*/
/* read bytes                                                                 */
/*----------------------------------------------------------------------------*/
i8 File::nReadBytes(void* aDst, i8 nBytes)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_oSysCall.vClear();

#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.File.m_nReads++;
#endif
  
  i8 nBytesRead = fread(aDst,1,nBytes,m_aFile);
  
#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.File.m_nReadBytes += nBytesRead;
#endif

  return nBytesRead;
}

/*----------------------------------------------------------------------------*/
/* read next line (terminated by \n or EOF)                                   */
/*----------------------------------------------------------------------------*/
bool File::bReadLine(Stz* aDst)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_oSysCall.vClear();
  
  // collect by y
  char *y = aDst->azChar();
  
  // collect chars till \n or EOF
  for (;;) {
    
    // get next char
    char cNextChar = getc(m_aFile);
  #ifdef WITH_STATISTICS_COUNTERS
    gpAttachMap->m_oStatistics.File.m_nGetcs++;
#endif

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // test for \n
    if ('\n' == cNextChar) {
      
      *y = '\0';
      
      m_oSysCall.vSuccess();
      return true;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // test for EOF
    if ((char)EOF == cNextChar) {
      
      *y = '\0';
      
      // did we collect any chars?
      if (*aDst->azChar()) {
        m_oSysCall.vSuccess();
        return true;
      }
      else {
        return false;
      }
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // collect any other char
    *y++ = cNextChar;

  }
  
}

/*----------------------------------------------------------------------------*/
/* reset file arguments                                                       */
/*----------------------------------------------------------------------------*/
void File::vReset(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  //if (m_aFile && ! bIsStdin() && ! bIsStdout())
  //  vWarn("File::vReset: previous file not closed: %s\n"_Stz,m_sFilePath.azChar());

  m_oSysCall.vClear();
  
  m_sFilePath   = ""_Stz;
  m_sOpenMode   = ""_Stz;
  m_aFile       = nullptr;
}

/*----------------------------------------------------------------------------*/
/* seek to offset                                                             */
/*----------------------------------------------------------------------------*/
void File::vSeek(i8 iOffset)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_oSysCall.vClear();
  
#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.File.m_nSeeks++;
#endif
  
  i8 iResult = fseeko(m_aFile,iOffset,SEEK_SET);
  
  if (iResult) {
    m_oSysCall.vError(errno);
    vWarn("File::vSeek: fseeko() error; errno=%d"_Stz,errno);
    return;
  }
  
  m_oSysCall.vSuccess();
}

/*----------------------------------------------------------------------------*/
/* seek backwards from current position                                       */
/*----------------------------------------------------------------------------*/
void File::vSeekBackwards(i8 nBytes)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_oSysCall.vClear();
  
#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.File.m_nSeeks++;
#endif
  
  i8 iResult = fseeko(m_aFile,(- nBytes),SEEK_CUR); // observe minus sign
  
  if (iResult) {
    m_oSysCall.vError(errno);
    vWarn("File::vSeek: fseeko() error; errno=%d"_Stz,errno);
    return;
  }
  
  m_oSysCall.vSuccess();
}

/*----------------------------------------------------------------------------*/
/* seek to end of file                                                        */
/*----------------------------------------------------------------------------*/
void File::vSeekEnd(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_oSysCall.vClear();

#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.File.m_nSeeks++;
#endif
  
  i8 iResult = fseeko(m_aFile,0,SEEK_END);
  
  if (iResult) {
    m_oSysCall.vError(errno);
    vWarn("File::vSeek: fseeko() error; errno=%d"_Stz,errno);
    return;
  }
  
  m_oSysCall.vSuccess();
}

/*----------------------------------------------------------------------------*/
/* seek to start                                                              */
/*----------------------------------------------------------------------------*/
void File::vSeekStart(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  m_oSysCall.vClear();
  
#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.File.m_nSeeks++;
#endif
  
  i8 iResult = fseeko(m_aFile,0,SEEK_SET);
  
  if (iResult) {
    m_oSysCall.vError(errno);
    vWarn("File::vSeek: fseeko() error; errno=%d"_Stz,errno);
    return;
  }
  
  m_oSysCall.vSuccess();
}

/*----------------------------------------------------------------------------*/
/* set file buffering                                                         */
/*----------------------------------------------------------------------------*/
void File::vSetBuffering(i8 nBytes)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  m_oSysCall.vClear();
  int iResult;
  
  if (C_NO_BUFFERING == nBytes)
    // set non-buffered file
    iResult = setvbuf(m_aFile,NULL,_IONBF,(int)0);
  else
    // set fully buffered file, with buffer created/managed along with file
    iResult = setvbuf(m_aFile,NULL,_IOFBF,(int)nBytes);
  
  if (iResult)
    vWarn("File::vBuffer: setvbuf failed: %s"_Stz,m_sFilePath.azChar());
}

/*----------------------------------------------------------------------------*/
/* get file size                                                              */
/*----------------------------------------------------------------------------*/
i8 File::iSize(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  i8 iSizeBytes = this->nSize(m_sFilePath);
  
  return iSizeBytes;
}

/*----------------------------------------------------------------------------*/
/* get SysCall                                                                */
/*----------------------------------------------------------------------------*/
SysCall& File::oSysCall(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  return m_oSysCall;
}

/*----------------------------------------------------------------------------*/
/* where are we?                                                              */
/*----------------------------------------------------------------------------*/
i8 File::iTell(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  m_oSysCall.vClear();

#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.File.m_nTells++;
#endif
  
  i8 iResult = ftello(m_aFile);
  
  if (-1 == iResult) {
    m_oSysCall.vError(errno);
    vWarn("File::iTell: ftello() error; errno=%d"_Stz,errno);
    return -1;
  }
  
  m_oSysCall.vSuccess();
  
  return iResult;
}

/*----------------------------------------------------------------------------*/
/* unget character                                                            */
/*----------------------------------------------------------------------------*/
void File::vUngetChar(char cChar)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  if ( ! m_aFile )
    return;

#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.File.m_nUngetcs++;
#endif
  
  i8 iResult = ungetc(cChar,m_aFile);
  
  if ((char)EOF == iResult)
    vWarn("File::vUngetChar: unget failed: '%c' %s\n"_Stz,cChar,m_sFilePath.azChar());
}

/*----------------------------------------------------------------------------*/
/* use stdin                                                                  */
/*----------------------------------------------------------------------------*/
void File::vUseStdin(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  if (m_aFile) {
    vWarn("File::vUseStdin: previous file not closed: %s\n"_Stz,m_sFilePath.azChar());
    vClose();
  }
  
  m_oSysCall.vClear();
  
  m_sFilePath   = "stdin"_Stz;
  m_sOpenMode   = C_OPEN_READ;
  m_aFile       = stdin;
}

/*----------------------------------------------------------------------------*/
/* use stdout                                                                 */
/*----------------------------------------------------------------------------*/
void File::vUseStdout(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  if (m_aFile) {
    vWarn("File::vUseStdin: previous file not closed: %s\n"_Stz,m_sFilePath.azChar());
    vClose();
  }
  
  m_oSysCall.vClear();
  
  m_sFilePath   = "stdout"_Stz;
  m_sOpenMode   = C_OPEN_WRITE;
  m_aFile       = stdout;
}

/*----------------------------------------------------------------------------*/
/* write bytes to file                                                        */
/*----------------------------------------------------------------------------*/
bool File::bWriteBytes(void* pBuffer, i8 nBytes)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  m_oSysCall.vClear();
 
#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.File.m_nWrites++;
#endif
  
  i8 nBytesWritten = fwrite(pBuffer, 1, nBytes, m_aFile);
  
#ifdef WITH_STATISTICS_COUNTERS
  gpAttachMap->m_oStatistics.File.m_nWriteBytes += nBytesWritten;
#endif

  if (nBytesWritten != nBytes) {
    vWarn("File::vWriteBytes: fwrite(%s) error; errno=%d\n"_Stz,
          m_sFilePath.azChar(),errno);
    m_oSysCall.vError(errno);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(File)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
