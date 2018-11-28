/*============================================================================*/
/* File.cpp -- os file                                                        */
/*============================================================================*/

#include "System.h"

#include "StorageEngine.h"

extern ResourceTracker gxResourceTracker;

/*----------------------------------------------------------------------------*/

// Opens for reading. If the file does not exist or cannot be found, the
// fopen call fails.
Stz* C_OPEN_READ = "r"_Stz;

// Opens an empty file for writing. If the given file exists, its contents
// are destroyed.
Stz* C_OPEN_WRITE = "w"_Stz;

// Opens for writing at the end of the file (appending) without removing
// the end-of-file (EOF) marker before new data is written to the file.
// Creates the file if it does not exist.
Stz* C_OPEN_APPEND = "a"_Stz;

// Opens for both reading and writing. The file must exist.
Stz* C_OPEN_READ_WRITE_EXIST = "r+"_Stz;

// Opens an empty file for both reading and writing. If the file exists, its
// contents are destroyed.
Stz* C_OPEN_READ_WRITE_CREATE = "w+"_Stz;

// Opens for reading and appending. The appending operation includes the
// removal of the EOF marker before new data is written to the file. The EOF
// marker is not restored after writing is complete. Creates the file if it
// does not exist.
Stz* C_OPEN_READ_APPEND = "a+"_Stz;

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
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* File::operator new(size_t, File* pFile)
{
  return pFile;
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
  
  vInit();
 
  _checkInitialised_(CN_FILE);
  
  _postcondition_(_MAGIC_,CN_FILE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
File::File(Stz* aFilePath, i8 iTraceLevel)
: Object(CN_FILE)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  vInit();
  
  m_sFilePath = aFilePath;
  m_iTraceLevel = iTraceLevel;

  _checkInitialised_(CN_FILE);
  
  _postcondition_(_MAGIC_,CN_FILE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
File::File(Stz* aFilePath, Stz* aOpenMode, i8 iTraceLevel)
: Object(CN_FILE)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  vInit();
  
  m_sFilePath = aFilePath;
  m_sOpenMode = aOpenMode;
  m_iTraceLevel = iTraceLevel;
  
  _checkInitialised_(CN_FILE);
  
  _postcondition_(_MAGIC_,CN_FILE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
File::~File(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  if (m_aFilePtr) {
    vWarn("File::~File file not closed: %s %s\n"_Stz,m_sFilePath.aChar(),m_sOpenMode.aChar());

    vClose();
    
    if (m_xSysCall.bHadError())
      vWarn("File::~File: file could not be closed: %s"_Stz,m_sFilePath.aChar());
  }
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set file buffer                                                            */
/*----------------------------------------------------------------------------*/
void File::vBuffer(i8 nBytes)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  m_xSysCall.vClear();
  int iResult;
  
  if (nBytes == 0)
    iResult = setvbuf(m_aFilePtr,NULL,_IONBF,(int)0);
  else
    iResult = setvbuf(m_aFilePtr,NULL,_IOFBF,(int)nBytes);
  
  if (iResult)
    vWarn("File::vBuffer: setvbuf failed: %s"_Stz,m_sFilePath.aChar());
  
}

/*----------------------------------------------------------------------------*/
/* clear file arguments                                                       */
/*----------------------------------------------------------------------------*/
void File::vClear(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  m_xSysCall.vClear();
    
  m_sFilePath   = ""_Stz;
  m_sOpenMode   = ""_Stz;
  m_iTraceLevel = 0;
  m_aFilePtr    = nullptr;
}

/*----------------------------------------------------------------------------*/
/* close file                                                                 */
/*----------------------------------------------------------------------------*/
void File::vClose(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();

  // never close stdin
  if (bIsStdin()) {
    vReset();
    return;
  }
  
  if (m_iTraceLevel)
    vTrace("File::vClose: closing file %s mode %s"_Stz,m_sFilePath.aChar(),m_sOpenMode.aChar());
  
  if ( ! m_aFilePtr ) {
    m_xSysCall.vFailure(0);
    vWarn("File::vClose: file not open: %s"_Stz,m_sFilePath.aChar());
    return;
  }
  
  i8 rCode = fclose(m_aFilePtr);
  
  if ((char)EOF == rCode) {
    m_xSysCall.vError(errno);
    vWarn("File::vClose: fclose(%p) error; errno=%d : %s %s"_Stz,
          m_aFilePtr,errno,m_sFilePath.aChar(),m_sOpenMode.aChar());
    return;
  }
  
  gxResourceTracker.vDecFileOpen();

  vReset();
  
  m_xSysCall.vSuccess();
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void File::vFileDump(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  vTrace("%sFile:\n"_Stz, m_sFilePath.aChar());
  vTrace("%s  Open Mode %s\n"_Stz, m_sOpenMode.aChar());
  vTrace("%s  Trace level %lld\n"_Stz, m_iTraceLevel);
  vTrace("%s  File Pointer 0x%p\n"_Stz, m_aFilePtr);
}

/*---------------------------------------------------------------------------*/
/* test if named file exists                                                 */
/*---------------------------------------------------------------------------*/
bool File::bExists(Stz* psFileName)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  struct stat xStat;
  
  int rCode = stat(psFileName->aChar(),&xStat);
  
  if (0 == rCode && (xStat.st_mode & S_IFREG)) {
    m_xSysCall.vSuccess();
    return true;
  }
  
  if (rCode && ENOENT == errno) {
    m_xSysCall.vFailure(0);
    return false;
  }
  
  m_xSysCall.vError(errno);
  vWarn("File::bExists: %s stat() error; errno=%d"_Stz, psFileName->aChar(), errno);
  
  return false;
}

/*---------------------------------------------------------------------------*/
/* flush file                                                                */
/*---------------------------------------------------------------------------*/
void File::vFlush(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  i8 rCode = fflush(m_aFilePtr);

  if ((char)EOF == rCode) {
    m_xSysCall.vError(errno);
    vWarn("File::vFlush: fflush() error; errno=%d"_Stz,errno);
    return;
  }

  m_xSysCall.vSuccess();
}

/*----------------------------------------------------------------------------*/
/* get next character                                                         */
/*----------------------------------------------------------------------------*/
char File::cGetChar(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  if ( ! m_aFilePtr )
    return 0;

  char cChar = getc(m_aFilePtr);
  if ((char)EOF == cChar)
    cChar = 0;
  
  return cChar;
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void File::vInit(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vSetMagic();
  
  //m_sFilePath
  //m_sOpenMode
  m_aFilePtr    = nullptr;
  m_iTraceLevel = 0;
  
  m_aLimit    = nullptr;
  m_aNext     = nullptr;
  m_pScratch  = nullptr;
  m_pRecord   = nullptr;
  m_iRecordLength = 0;
  
  m_xSysCall.vClear();
}

/*----------------------------------------------------------------------------*/
/* is file close?                                                             */
/*----------------------------------------------------------------------------*/
bool File::bIsClosed(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  if ( ! m_aFilePtr )
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

  if (m_aFilePtr)
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

  if (stdin == m_aFilePtr)
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

  if (stdout == m_aFilePtr)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
bool File::bOpen(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  if (m_aFilePtr) {
    vWarn("File::bOpen: previous file not closed: %s"_Stz,m_sFilePath.aChar());
    vClose();
  }
  
  if (m_iTraceLevel)
    vTrace("File::bOpen: opening file %s mode %s"_Stz,m_sFilePath.aChar(),m_sOpenMode.aChar());
  
  m_aFilePtr = fopen(m_sFilePath.aChar(),m_sOpenMode.aChar());
  
  if ( ! m_aFilePtr ) {
    vWarn("File::bOpen: fopen(%s,%s) error; errno=%d"_Stz,
          m_sFilePath.aChar(),m_sOpenMode.aChar(),errno);
    return false;
  }
  
  gxResourceTracker.vIncFileOpen();

  return true;
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
bool File::bOpen(Stz* aOpenMode, i8 iTraceLevel)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  if (m_aFilePtr) {
    vWarn("File::bOpen: previous file not closed: %s\n"_Stz,m_sFilePath.aChar());
    vClose();
  }
  
  m_sOpenMode   = (Stz*)aOpenMode;
  m_iTraceLevel = iTraceLevel;
  
  if (m_iTraceLevel)
    vTrace("File::bOpen: opening file %s mode %s\n"_Stz,m_sFilePath.aChar(),m_sOpenMode.aChar());
  
  m_aFilePtr = fopen(m_sFilePath.aChar(),m_sOpenMode.aChar());
  
  if ( ! m_aFilePtr ) {
    vWarn("File::bOpen: fopen(%s,%s) error; errno=%d\n"_Stz,
          m_sFilePath.aChar(),m_sOpenMode.aChar(),errno);
    return false;
  }
  
  gxResourceTracker.vIncFileOpen();

  return true;
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
bool File::bOpen(Stz* aFilePath, Stz* aOpenMode, i8 iTraceLevel)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();

  if (m_aFilePtr) {
    vWarn("File::bOpen: previous file not closed: %s\n"_Stz,m_sFilePath.aChar());
    vClose();
  }
  
  vSet((Stz*)aFilePath,aOpenMode,iTraceLevel);
  
  if (m_iTraceLevel)
    vTrace("File::bOpen: opening file %s mode %s\n"_Stz,m_sFilePath.aChar(),m_sOpenMode.aChar());
  
  m_aFilePtr = fopen(m_sFilePath.aChar(),m_sOpenMode.aChar());
  
  if ( ! m_aFilePtr ) {
    m_xSysCall.vError(errno);
    vWarn("File::bOpen: fopen(%s,%s) error; errno=%d\n"_Stz,
          m_sFilePath.aChar(),m_sOpenMode.aChar(),errno);
    return false;
  }
  
  gxResourceTracker.vIncFileOpen();

  return true;
}

/*----------------------------------------------------------------------------*/
/* get path                                                                   */
/*----------------------------------------------------------------------------*/
s256& File::sPath(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  return m_sFilePath;
}

/*----------------------------------------------------------------------------*/
/* set path                                                                   */
/*----------------------------------------------------------------------------*/
void File::vPath(Stz* aFilePath)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  if (m_aFilePtr) {
    vWarn("File::vPath: previous file not closed: %s\n"_Stz,m_sFilePath.aChar());
    vClose();
  }
  
  m_sFilePath = (Stz*)aFilePath;
}

/*----------------------------------------------------------------------------*/
/* set path                                                                   */
/*----------------------------------------------------------------------------*/
void File::vPath(StrVal& sFilePath)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  if (m_aFilePtr) {
    vWarn("File::vPath: previous file not closed: %s\n"_Stz,m_sFilePath.aChar());
    vClose();
  }
  
  m_sFilePath = sFilePath;
}

/*----------------------------------------------------------------------------*/
/* formatted print                                                            */
/*----------------------------------------------------------------------------*/
void File::vPrint(Stz* aFormat, ...)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  va_list args;

  va_start(args,aFormat);
  i8 nPrinted = vfprintf(m_aFilePtr,aFormat->aChar(),args);
  va_end(args);

  if (-1 == nPrinted) {
    m_xSysCall.vError(errno);
    vWarn("File::vPrint: vfprintf() error; errno=%d"_Stz,errno);
    return;
  }
  
  m_xSysCall.vSuccess();
}

/*----------------------------------------------------------------------------*/
/* read chunk                                                                 */
/*----------------------------------------------------------------------------*/
i8 File::iReadChunk(void* aChunk, i8 nBytes)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  i8 iBytesRead = fread(aChunk,1,nBytes,m_aFilePtr);
  
  return iBytesRead;
}

/*----------------------------------------------------------------------------*/
/* read next line                                                             */
/*----------------------------------------------------------------------------*/
bool File::bReadLine(Stz* aBuffer)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  // nb: man fscanf does not indicate that errno gets set???
  
  //i8 nItems;
  
  //  if (stdin == m_aFilePtr)
  //    nItems = fscanf(m_aFilePtr,"%[^\n]%*c",s.aChar());
  //  else
  //    nItems = fscanf(m_aFilePtr,"%[^\n]\n",s.aChar());
  
  char *y = aBuffer->aChar();
  
  *y = '\0';
  for (;;) {
    char c = getc(m_aFilePtr);
    if ('\n' == c || (char)EOF ==c) {
      break;
    }
    *y++ = c;
  }
  
  *y = '\0';
  
  if (*aBuffer->aChar()) {
    m_xSysCall.vSuccess();
    return true;
  }
  else {
    return false;
  }
}

/*----------------------------------------------------------------------------*/
/* read next line                                                             */
/*----------------------------------------------------------------------------*/
bool File::bReadLine(StrVal& s)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  m_xSysCall.vClear();
  
  if ( ! m_pScratch ) {
    m_pScratch = new Scratch;
    m_aNext  = m_pScratch->aWspArea();
    m_aLimit = m_pScratch->aWspArea();
  }
  
  char* z = s.aChar();
  *z = '\0';
  
  i8 nChars = 0;
  for (;;) {
    
    if (m_aNext == m_aLimit) {
      m_aNext = m_pScratch->aWspArea();
      i8 iBytesRead = fread(m_aNext,1,m_pScratch->nWspLength(),m_aFilePtr);
      
      if ( ! iBytesRead )
        break;
      
      m_aLimit = m_aNext + iBytesRead;
    }
    
    char c = *m_aNext++;
    nChars++;
    
    if ('\n' == c || (char)EOF == c)
      break;
    
    *z++ = c;
  }
  
  *z = '\0';
  
  if (nChars) {
    m_xSysCall.vSuccess();
    return true;
  }
  else {
    return false;
  }
}

/*----------------------------------------------------------------------------*/
/* remove specified file                                                      */
/*----------------------------------------------------------------------------*/
bool File::bRemove(s256& rPath)
{
  char* aPath = rPath.aChar();
  
  int rCode = unlink(aPath);
  
  if (-1 == rCode) {
    vWarn("File::bRemove: rmdir() error; errno=%d"_Stz,errno);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* reset file arguments                                                       */
/*----------------------------------------------------------------------------*/
void File::vReset(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_aFilePtr = nullptr;
  vSet(""_Stz,""_Stz);
}

/*----------------------------------------------------------------------------*/
/* read next record                                                           */
/*----------------------------------------------------------------------------*/
bool File::bReadRecord(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  m_xSysCall.vClear();
  
  if ( ! m_pScratch ) {
    m_pScratch = new Scratch;
    m_pRecord  = new Scratch;
    m_aNext  = m_pScratch->aWspArea();
    m_aLimit = m_pScratch->aWspArea();
    
    vBuffer(C_256KB);
  }
  
  char* z = m_pRecord->aWspArea();
  *z = '\0';
  m_iRecordLength = 0;
  
  for (;;) {
    
    if (m_aNext == m_aLimit) {
      m_aNext = m_pScratch->aWspArea();
      i8 iBytesRead = fread(m_aNext,1,m_pScratch->nWspLength(),m_aFilePtr);
      
      if ( ! iBytesRead )
        break;
      
      m_aLimit = m_aNext + iBytesRead;
    }
    
    char c = *m_aNext++;
    
    if ('\n' == c || (char)EOF == c)
      break;
    
    *z++ = c;
    m_iRecordLength++;
  }
  
  *z = '\0';
  m_iRecordLength++;
  
  if (*m_pRecord->aWspArea()) {
    m_xSysCall.vSuccess();
    return true;
  }
  else {
    return false;
  }
}

/*----------------------------------------------------------------------------*/
/* get scratch record                                                         */
/*----------------------------------------------------------------------------*/
Scratch* File::pRecord(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  return m_pRecord;
}

/*----------------------------------------------------------------------------*/
/* get scratch record length                                                  */
/*----------------------------------------------------------------------------*/
i8 File::iRecordLength(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  return m_iRecordLength;
}

/*----------------------------------------------------------------------------*/
/* seek to offset                                                             */
/*----------------------------------------------------------------------------*/
void File::vSeek(i8 iOffset)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  i8 rCode = fseeko(m_aFilePtr,iOffset,SEEK_SET);
  
  if (rCode) {
    m_xSysCall.vError(errno);
    vWarn("File::vSeek: fseeko() error; errno=%d"_Stz,errno);
    return;
  }
  
  m_xSysCall.vSuccess();
}

/*----------------------------------------------------------------------------*/
/* seek to end                                                                */
/*----------------------------------------------------------------------------*/
void File::vSeekEnd(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  i8 rCode = fseeko(m_aFilePtr,0,SEEK_END);
  
  if (rCode) {
    m_xSysCall.vError(errno);
    vWarn("File::vSeek: fseeko() error; errno=%d"_Stz,errno);
    return;
  }
  
  m_xSysCall.vSuccess();
}

/*----------------------------------------------------------------------------*/
/* set file arguments                                                         */
/*----------------------------------------------------------------------------*/
void File::vSet(Stz* aFilePath, Stz* aOpenMode, i8 iTraceLevel)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  if (m_aFilePtr) {
    vWarn("File::vSet: previous file not closed: %s\n"_Stz,m_sFilePath.aChar());
    vClose();
  }
  
  m_sFilePath   = (Stz*)aFilePath;
  m_sOpenMode   = (Stz*)aOpenMode;
  m_iTraceLevel = iTraceLevel;
}

/*----------------------------------------------------------------------------*/
/* set file arguments                                                         */
/*----------------------------------------------------------------------------*/
void File::vSet(FILE* pFilePtr, Stz* aFilePath, Stz* aOpenMode, i8 iTraceLevel)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  if (m_aFilePtr) {
    vWarn("File::vSet: previous file not closed: %s\n"_Stz,m_sFilePath.aChar());
    vClose();
  }
  
  m_sFilePath   = (Stz*)aFilePath;
  m_sOpenMode   = (Stz*)aOpenMode;
  m_aFilePtr    = pFilePtr;
  m_iTraceLevel = iTraceLevel;
}

/*----------------------------------------------------------------------------*/
/* get SysCall                                                                */
/*----------------------------------------------------------------------------*/
SysCall& File::rSysCall(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  return m_xSysCall;
}

/*----------------------------------------------------------------------------*/
/* where are we?                                                              */
/*----------------------------------------------------------------------------*/
i8 File::iTell(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  m_xSysCall.vClear();
  
  i8 rCode = ftello(m_aFilePtr);
  
  if (-1 == rCode) {
    m_xSysCall.vError(errno);
    vWarn("File::iTell: ftello() error; errno=%d"_Stz,errno);
    return -1;
  }
  
  m_xSysCall.vSuccess();
  
  return rCode;
}

/*----------------------------------------------------------------------------*/
/* get trace level                                                            */
/*----------------------------------------------------------------------------*/
i8 File::iTraceLevel(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  return m_iTraceLevel;
}

/*----------------------------------------------------------------------------*/
/* set trace level                                                            */
/*----------------------------------------------------------------------------*/
void File::vTraceLevel(i8 iTraceLevel)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_iTraceLevel = iTraceLevel;
}

/*----------------------------------------------------------------------------*/
/* unget character                                                            */
/*----------------------------------------------------------------------------*/
void File::vUngetChar(char cChar)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  if ( ! m_aFilePtr )
    return;
  
  i8 iResult = ungetc(cChar,m_aFilePtr);
  
  if ((char)EOF == iResult)
    vWarn("File::vUngetChar: unget failed: '%c' %s\n"_Stz,cChar,m_sFilePath.aChar());
}

/*----------------------------------------------------------------------------*/
/* set trace level                                                            */
/*----------------------------------------------------------------------------*/
bool File::bWriteBytes(void* pBuffer, i8 nSize)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  m_xSysCall.vClear();
  
  i8 iResult = fwrite(pBuffer , 1, nSize, m_aFilePtr);
  if (iResult != nSize) {
    vWarn("File::vWriteBytes: fwrite(%p) error; errno=%d\n"_Stz,
          m_sFilePath.aChar(),errno);
    m_xSysCall.vError(errno);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* zero this structure -- used only in special/nesting cases                  */
/*----------------------------------------------------------------------------*/
void File::vZero(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  m_sFilePath   = ""_Stz;
  m_sOpenMode   = ""_Stz;
  m_iTraceLevel = 0;
  m_aFilePtr    = nullptr;
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
