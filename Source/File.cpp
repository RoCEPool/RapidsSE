/*============================================================================*/
/* File.cpp -- os file                                                        */
/*============================================================================*/

#include "System.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
File* System::_pFile(void* aAddr)
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
  void* pObject = HeapMem::pAllocate(CN_FILE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void File::operator delete(void* pFileAddress)
{
  Object* pObject = reinterpret_cast<Object*>(pFileAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
File::File(void)
: Object(CN_FILE)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  //m_sFilePath
  //m_sOpenMode
  m_aFile    = nullptr;
  
  m_oSysCall.vClear();

  _checkInitialised_(CN_FILE);
  
  _postcondition_(_MAGIC_,CN_FILE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
File::~File(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  if (m_aFile) {
    vWarn("File::~File file not closed: %s %s\n"_Stz,m_sFilePath.aChar(),m_sOpenMode.aChar());

    vClose();
    
    if (m_oSysCall.bHadError())
      vWarn("File::~File: file could not be closed: %s"_Stz,m_sFilePath.aChar());
  }
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* test if given file exists in file system                                  */
/*---------------------------------------------------------------------------*/
bool File::bExists(Stz* aFilePath)
{
  struct stat xStat;
  
  int iResult = stat(aFilePath->aChar(),&xStat);
  
  if (0 == iResult && (xStat.st_mode & S_IFREG))
    return true;
  
  if (iResult && ENOENT == errno)
    return false;
  
  vWarn("File::bExists: %s stat() error; errno=%d"_Stz, aFilePath->aChar(), errno);
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* monitor profile                                                            */
/*----------------------------------------------------------------------------*/
void File::vMonitorPortrait(s48K& sPortrait)
{
  Statistics* pStats = &gpAttachMap->m_oStatistics;
  
  s32 sReadMag;  Magnitudes::vByteMag(pStats->File.m_nReadBytes,  sReadMag);
  s32 sWriteMag; Magnitudes::vByteMag(pStats->File.m_nWriteBytes, sWriteMag);
  s32 sPrintMag; Magnitudes::vByteMag(pStats->File.m_nPrintBytes, sPrintMag);

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
                      sReadMag.aStz(),
                    pStats->File.m_nWrites,
                      sWriteMag.aStz(),
                    pStats->File.m_nPrints,
                      sPrintMag.aStz(),

                    pStats->File.m_nGetcs,
                    pStats->File.m_nUngetcs
                   );
  
  
}

/*----------------------------------------------------------------------------*/
/* remove given file from file system                                         */
/*----------------------------------------------------------------------------*/
bool File::bRemove(Stz* aFilePath)
{
  int iResult = unlink(aFilePath->aChar());
  
  if (-1 == iResult) {
    vWarn("File::bRemove: unlink() error; errno=%d"_Stz,errno);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* print file statistics                                                      */
/*----------------------------------------------------------------------------*/
void File::vStatistics(void)
{
  Statistics* pStats = &gpAttachMap->m_oStatistics;

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

  Magnitudes::vByteMag(pStats->File.m_nReadBytes,  sByteMag);
  vTrace("  nFileReadBytes  %s\n"_Stz,sByteMag.aStz());
  Magnitudes::vByteMag(pStats->File.m_nPrintBytes, sByteMag);
  vTrace("  nFilePrintBytes %s\n"_Stz,sByteMag.aStz());
  Magnitudes::vByteMag(pStats->File.m_nWriteBytes, sByteMag);
  vTrace("  nFileWriteBytes %s\n"_Stz,sByteMag.aStz());
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

  m_oSysCall.vClear();

  // never close stdin or stdout
  if (bIsStdin() || bIsStdout()) {
    vReset();
    return;
  }
  
  if ( ! m_aFile ) {
    m_oSysCall.vFailure(0);
    vWarn("File::vClose: file not open: %s"_Stz,m_sFilePath.aChar());
    return;
  }
  
  if (gpAttachMap) gpAttachMap->m_oStatistics.File.m_nCloses++;
  i8 iResult = fclose(m_aFile);
  
  if ((char)EOF == iResult) {
    m_oSysCall.vError(errno);
    vWarn("File::vClose: fclose(%p) error; errno=%d : %s %s"_Stz,
          m_aFile,errno,m_sFilePath.aChar(),m_sOpenMode.aChar());
    return;
  }
  
  if (gpAttachMap) gpAttachMap->m_oResourceTracker.vDecFileOpen();
  if (gpAttachMap &&
      (gpAttachMap->m_bMarkResourceAll || gpAttachMap->m_bMarkResource[SR_FILE]))
    ActivityMap::vMarkRes(SR_FILE,"SysRes: File closed %s"_Stz,m_sFilePath.aStz());

  m_aFile = nullptr;

  vReset();
  
  m_oSysCall.vSuccess();
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void File::vFileDump(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  vTrace("%sFile:\n"_Stz, m_sFilePath.aChar());
  vTrace("%s  Open Mode %s\n"_Stz, m_sOpenMode.aChar());
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
  
  gpAttachMap->m_oStatistics.File.m_nFlushes++;
  i8 iResult = fflush(m_aFile);

  if ((char)EOF == iResult) {
    m_oSysCall.vError(errno);
    vWarn("File::vFlush: fflush() error; errno=%d"_Stz,errno);
    return;
  }

  m_oSysCall.vSuccess();
}

/*----------------------------------------------------------------------------*/
/* get next character                                                         */
/*----------------------------------------------------------------------------*/
char File::cGetChar(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  if ( ! m_aFile )
    return 0;

  gpAttachMap->m_oStatistics.File.m_nGetcs++;
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
/* open file for append                                                       */
/*----------------------------------------------------------------------------*/
bool File::bOpenAppend(Stz* aFilePath)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  bool bOpened = bOpenMode(aFilePath,C_OPEN_APPEND);
  
  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
bool File::bOpenMode(Stz* aFilePath, Stz* aOpenMode)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  if (aFilePath->bSame("stdin"_Stz)) {
    vUseStdin();
    return true;
  }

  if (aFilePath->bSame("stdout"_Stz)) {
    vUseStdout();
    return true;
  }
  
  if (m_aFile) {
    vWarn("File::bOpen: previous file not closed: %s\n"_Stz,m_sFilePath.aChar());
    vClose();
  }
  
  m_oSysCall.vClear();
  
  m_sFilePath   = aFilePath;
  m_sOpenMode   = aOpenMode;

  if (gpAttachMap) gpAttachMap->m_oStatistics.File.m_nOpens++;
  m_aFile = fopen(m_sFilePath.aChar(),m_sOpenMode.aChar());
  
  if ( ! m_aFile ) {
    m_oSysCall.vError(errno);
    vWarn("File::bOpen: fopen(%s,%s) error; errno=%d\n"_Stz,
          m_sFilePath.aChar(),m_sOpenMode.aChar(),errno);
    return false;
  }
  
  if (gpAttachMap) gpAttachMap->m_oResourceTracker.vIncFileOpen();
  if (gpAttachMap &&
      (gpAttachMap->m_bMarkResourceAll || gpAttachMap->m_bMarkResource[SR_FILE]))
    ActivityMap::vMarkRes(SR_FILE,"SysRes: File opened %s %s"_Stz,aFilePath,aOpenMode);

  return true;
}

/*----------------------------------------------------------------------------*/
/* open file for read                                                         */
/*----------------------------------------------------------------------------*/
bool File::bOpenRead(Stz* aFilePath)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  bool bOpened = bOpenMode(aFilePath,C_OPEN_READ);
  
  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* open file for write                                                        */
/*----------------------------------------------------------------------------*/
bool File::bOpenWrite(Stz* aFilePath)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  bool bOpened = bOpenMode(aFilePath,C_OPEN_WRITE);
  
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
  if (gpAttachMap) gpAttachMap->m_oStatistics.File.m_nPrints++;
  i8 nPrinted = vfprintf(m_aFile,aFormat->aChar(),args);
  if (gpAttachMap) gpAttachMap->m_oStatistics.File.m_nPrintBytes += nPrinted;
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
  
  gpAttachMap->m_oStatistics.File.m_nReads++;
  i8 nBytesRead = fread(aDst,1,nBytes,m_aFile);
  gpAttachMap->m_oStatistics.File.m_nReadBytes += nBytesRead;

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
  char *y = aDst->aChar();
  
  // collect chars till \n or EOF
  for (;;) {
    
    // get next char
    char cNextChar = getc(m_aFile);
    gpAttachMap->m_oStatistics.File.m_nGetcs++;

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
      if (*aDst->aChar()) {
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
  //  vWarn("File::vReset: previous file not closed: %s\n"_Stz,m_sFilePath.aChar());

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
  
  gpAttachMap->m_oStatistics.File.m_nSeeks++;
  i8 iResult = fseeko(m_aFile,iOffset,SEEK_SET);
  
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
  
  gpAttachMap->m_oStatistics.File.m_nSeeks++;
  i8 iResult = fseeko(m_aFile,0,SEEK_SET);
  
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
  
  gpAttachMap->m_oStatistics.File.m_nSeeks++;
  i8 iResult = fseeko(m_aFile,0,SEEK_END);
  
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
    vWarn("File::vBuffer: setvbuf failed: %s"_Stz,m_sFilePath.aChar());
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
  
  gpAttachMap->m_oStatistics.File.m_nTells++;
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
  
  gpAttachMap->m_oStatistics.File.m_nUngetcs++;
  i8 iResult = ungetc(cChar,m_aFile);
  
  if ((char)EOF == iResult)
    vWarn("File::vUngetChar: unget failed: '%c' %s\n"_Stz,cChar,m_sFilePath.aChar());
}

/*----------------------------------------------------------------------------*/
/* use stdin                                                                  */
/*----------------------------------------------------------------------------*/
void File::vUseStdin(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  if (m_aFile) {
    vWarn("File::vUseStdin: previous file not closed: %s\n"_Stz,m_sFilePath.aChar());
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
    vWarn("File::vUseStdin: previous file not closed: %s\n"_Stz,m_sFilePath.aChar());
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
  
  gpAttachMap->m_oStatistics.File.m_nWrites++;
  i8 nBytesWritten = fwrite(pBuffer, 1, nBytes, m_aFile);
  gpAttachMap->m_oStatistics.File.m_nWriteBytes += nBytesWritten;

  if (nBytesWritten != nBytes) {
    vWarn("File::vWriteBytes: fwrite(%s) error; errno=%d\n"_Stz,
          m_sFilePath.aChar(),errno);
    m_oSysCall.vError(errno);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* File::aFooter(void)
{
  _precondition_(_MAGIC_,CN_FILE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
