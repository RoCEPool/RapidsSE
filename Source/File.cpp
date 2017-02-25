/*============================================================================*/
/* File.cpp -- os file                                                        */
/*============================================================================*/

#include "Common.h"

#include "Pool.h"

extern Heap*  gpSmallHeap;

Atomic gzFileOpenCount;

/*----------------------------------------------------------------------------*/

// Opens for reading. If the file does not exist or cannot be found, the
// fopen call fails.
const char* C_OPEN_READ = "r";

// Opens an empty file for writing. If the given file exists, its contents
// are destroyed.
const char* C_OPEN_WRITE = "w";

// Opens for writing at the end of the file (appending) without removing
// the end-of-file (EOF) marker before new data is written to the file.
// Creates the file if it does not exist.
const char* C_OPEN_APPEND = "a";

// Opens for both reading and writing. The file must exist.
const char* C_OPEN_READ_WRITE_EXIST = "r+";

// Opens an empty file for both reading and writing. If the file exists, its
// contents are destroyed.
const char* C_OPEN_READ_WRITE_CREATE = "w+";

// Opens for reading and appending. The appending operation includes the
// removal of the EOF marker before new data is written to the file. The EOF
// marker is not restored after writing is complete. Creates the file if it
// does not exist.
const char* C_OPEN_READ_APPEND = "a+";

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
File* Common::_pFile(void* pAddr)
{
  File* pFile = reinterpret_cast<File*>(pAddr);
  
  RASSERT( !pFile || CN_FILE == pFile->eClass() );
  
  return pFile;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* File::operator new(size_t iSize)
{
  RASSERT(iSize);

  void* pObject = Heap::pAllocate(CN_FILE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* File::operator new(size_t iSize, File* pFile)
{
  RASSERT(iSize);
  return pFile;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void File::operator delete(void* pFileAddress)
{
  Object* pObject = reinterpret_cast<Object*>(pFileAddress);
  
  Heap::vFree(pObject);
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
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
File::~File(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  if (m_pFilePtr) {
    vWarn("File::~File file not closed: %s %s\n",m_sFilePath.a(),m_sOpenMode.a());

    vClose();
    
    if (m_xSysCall.bHadError())
      vWarn("File::~File: file could not be closed: %s",m_sFilePath.a());
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
    iResult = setvbuf(m_pFilePtr,NULL,_IONBF,(int)0);
  else
    iResult = setvbuf(m_pFilePtr,NULL,_IOFBF,(int)nBytes);
  
  if (iResult)
    vWarn("File::vBuffer: setvbuf failed: %s",m_sFilePath.a());
  
}

/*----------------------------------------------------------------------------*/
/* clear file arguments                                                       */
/*----------------------------------------------------------------------------*/
void File::vClear(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  m_xSysCall.vClear();
    
  m_sFilePath   = "";
  m_sOpenMode   = "";
  m_iTraceLevel = 0;
  m_pFilePtr    = nullptr;
}

/*----------------------------------------------------------------------------*/
/* close file                                                                 */
/*----------------------------------------------------------------------------*/
void File::vClose(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();

  // never close stdin
  ASSERT(! bIsStdin() );
  
  if (m_iTraceLevel)
    vTrace("File::vClose: closing file %s mode %s",m_sFilePath.a(),m_sOpenMode.a());
  
  if ( ! m_pFilePtr ) {
    m_xSysCall.vFailure(0);
    vWarn("File::vClose: file not open: %s",m_sFilePath.a());
    return;
  }
  
  i8 rCode = fclose(m_pFilePtr);
  
  if ((char)EOF == rCode) {
    m_xSysCall.vError(errno);
    vWarn("File::vClose: fclose(%p) error; errno=%d : %s %s",
          m_pFilePtr,errno,m_sFilePath.a(),m_sOpenMode.a());
    return;
  }
  
  --gzFileOpenCount;
  
  vReset();
  
  m_xSysCall.vSuccess();
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void File::vFileDump(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  vTrace("%sFile:\n", m_sFilePath.a());
  vTrace("%s  Open Mode %s\n", m_sOpenMode.a());
  vTrace("%s  Trace level %lld\n", m_iTraceLevel);
  vTrace("%s  File Pointer 0x%p\n", m_pFilePtr);
}

/*---------------------------------------------------------------------------*/
/* test if named file exists                                                 */
/*---------------------------------------------------------------------------*/
bool File::bExists(const char *psFileName)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  struct stat xStat;
  
  int rCode = stat(psFileName,&xStat);
  
  if (0 == rCode && (xStat.st_mode & S_IFREG)) {
    m_xSysCall.vSuccess();
    return true;
  }
  
  if (rCode && ENOENT == errno) {
    m_xSysCall.vFailure(0);
    return false;
  }
  
  m_xSysCall.vError(errno);
  vWarn("File::bExists: stat() error; errno=%d",errno);
  
  return false;
}

/*---------------------------------------------------------------------------*/
/* flush file                                                                */
/*---------------------------------------------------------------------------*/
void File::vFlush(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  i8 rCode = fflush(m_pFilePtr);

  if ((char)EOF == rCode) {
    m_xSysCall.vError(errno);
    vWarn("File::vFlush: fflush() error; errno=%d",errno);
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

  if ( ! m_pFilePtr )
    return 0;

  char cChar = getc(m_pFilePtr);
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
  m_pFilePtr    = nullptr;
  m_iTraceLevel = 0;
  
  m_pLimit    = nullptr;
  m_pNext     = nullptr;
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

  if ( ! m_pFilePtr )
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

  if (m_pFilePtr)
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

  if (stdin == m_pFilePtr)
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

  if (stdout == m_pFilePtr)
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
  
  if (m_pFilePtr) {
    vWarn("File::bOpen: previous file not closed: %s",m_sFilePath.a());
    vClose();
  }
  
  if (m_iTraceLevel)
    vTrace("File::bOpen: opening file %s mode %s",m_sFilePath.a(),m_sOpenMode.a());
  
  m_pFilePtr = fopen(m_sFilePath.a(),m_sOpenMode.a());
  
  if ( ! m_pFilePtr ) {
    vWarn("File::bOpen: fopen(%s,%s) error; errno=%d",
          m_sFilePath.a(),m_sOpenMode.a(),errno);
    return false;
  }
  
  ++gzFileOpenCount;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
bool File::bOpen(const char* aOpenMode, i8 iTraceLevel)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  if (m_pFilePtr) {
    vWarn("File::bOpen: previous file not closed: %s\n",m_sFilePath.a());
    vClose();
  }
  
  m_sOpenMode   = aOpenMode;
  m_iTraceLevel = iTraceLevel;
  
  if (m_iTraceLevel)
    vTrace("File::bOpen: opening file %s mode %s\n",m_sFilePath.a(),m_sOpenMode.a());
  
  m_pFilePtr = fopen(m_sFilePath.a(),m_sOpenMode.a());
  
  if ( ! m_pFilePtr ) {
    vWarn("File::bOpen: fopen(%s,%s) error; errno=%d\n",
          m_sFilePath.a(),m_sOpenMode.a(),errno);
    return false;
  }
  
  ++gzFileOpenCount;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
bool File::bOpen(const char* aFilePath, const char* aOpenMode, i8 iTraceLevel)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();

  if (m_pFilePtr) {
    vWarn("File::bOpen: previous file not closed: %s\n",m_sFilePath.a());
    vClose();
  }
  
  vSet(aFilePath,aOpenMode,iTraceLevel);
  
  if (m_iTraceLevel)
    vTrace("File::bOpen: opening file %s mode %s\n",m_sFilePath.a(),m_sOpenMode.a());
  
  m_pFilePtr = fopen(m_sFilePath.a(),m_sOpenMode.a());
  
  if ( ! m_pFilePtr ) {
    m_xSysCall.vError(errno);
    vWarn("File::bOpen: fopen(%s,%s) error; errno=%d\n",
          m_sFilePath.a(),m_sOpenMode.a(),errno);
    return false;
  }
  
  ++gzFileOpenCount;
  
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
void File::vPath(const char* aFilePath)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  if (m_pFilePtr) {
    vWarn("File::vPath: previous file not closed: %s\n",m_sFilePath.a());
    vClose();
  }
  
  m_sFilePath = aFilePath;
}

/*----------------------------------------------------------------------------*/
/* set path                                                                   */
/*----------------------------------------------------------------------------*/
void File::vPath(StrVal& sFilePath)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  if (m_pFilePtr) {
    vWarn("File::vPath: previous file not closed: %s\n",m_sFilePath.a());
    vClose();
  }
  
  m_sFilePath = sFilePath;
}

/*----------------------------------------------------------------------------*/
/* formatted print                                                            */
/*----------------------------------------------------------------------------*/
void File::vPrint(const char* sFormat, ...)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  va_list args;

  va_start(args,sFormat);
  i8 nPrinted = vfprintf(m_pFilePtr,sFormat,args);
  va_end(args);

  if (-1 == nPrinted) {
    m_xSysCall.vError(errno);
    vWarn("File::vPrint: vfprintf() error; errno=%d",errno);
    return;
  }
  
  m_xSysCall.vSuccess();
}

/*----------------------------------------------------------------------------*/
/* read chunk                                                                 */
/*----------------------------------------------------------------------------*/
i8 File::iReadChunk(void* pChunk, i8 nBytes)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  i8 iBytesRead = fread(pChunk,1,nBytes,m_pFilePtr);
  
  return iBytesRead;
}

/*----------------------------------------------------------------------------*/
/* read next line                                                             */
/*----------------------------------------------------------------------------*/
bool File::bReadLine(char* aBuffer)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  // nb: man fscanf does not indicate that errno gets set???
  
  //i8 nItems;
  
  //  if (stdin == m_pFilePtr)
  //    nItems = fscanf(m_pFilePtr,"%[^\n]%*c",s.a());
  //  else
  //    nItems = fscanf(m_pFilePtr,"%[^\n]\n",s.a());
  
  char *y = aBuffer;
  
  *y = '\0';
  for (;;) {
    char c = getc(m_pFilePtr);
    if ('\n' == c || (char)EOF ==c) {
      break;
    }
    *y++ = c;
  }
  
  *y = '\0';
  
  if (*aBuffer) {
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
//bool File::bReadLine(StrVal& s)
//{
//  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
//
//  m_xSysCall.vClear();
//  
//  // nb: man fscanf does not indicate that errno gets set???
//
//  //i8 nItems;
//  
////  if (stdin == m_pFilePtr)
////    nItems = fscanf(m_pFilePtr,"%[^\n]%*c",s.a());
////  else
////    nItems = fscanf(m_pFilePtr,"%[^\n]\n",s.a());
//
////  *s.a() = '\0';
////  for (;;) {
////     char c = getc(m_pFilePtr);
////     if ('\n' == c || (char)EOF ==c)
////       break;
////     else
////       String::vConcat(s.a(),c);
////  }
//
//  char* z = s.a();
//  
//  *z = '\0';
//  for (;;) {
//    char c = getc(m_pFilePtr);
//    if ('\n' == c || (char)EOF ==c)
//      break;
//    else
//      String::vAppendChar(&z,c);
//  }
//  
//  String::vAppendChar(&z,'\0');
//  
//  if (*s.a()) {
//    m_xSysCall.vSuccess();
//    return true;
//  }
//  else {
//    return false;
//  }
//}
//------------------------------------------------------------------------------
bool File::bReadLine(StrVal& s)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);
  
  m_xSysCall.vClear();
  
  if ( ! m_pScratch ) {
    m_pScratch = new Scratch;
    m_pNext  = m_pScratch->pWorkArea();
    m_pLimit = m_pScratch->pWorkArea();
  }
  
  char* z = s.a();
  *z = '\0';
  
  i8 nChars = 0;
  for (;;) {
    
    if (m_pNext == m_pLimit) {
      m_pNext = m_pScratch->pWorkArea();
      i8 iBytesRead = fread(m_pNext,1,C_WORKAREA,m_pFilePtr);
      
      if ( ! iBytesRead )
        break;
      
      m_pLimit = m_pNext + iBytesRead;
    }
    
    char c = *m_pNext++;
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
/* reset file arguments                                                       */
/*----------------------------------------------------------------------------*/
void File::vReset(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_pFilePtr = nullptr;
  vSet("","");
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
    m_pNext  = m_pScratch->pWorkArea();
    m_pLimit = m_pScratch->pWorkArea();
    
    vBuffer(C_256KB);
  }
  
  char* z = m_pRecord->pWorkArea();
  *z = '\0';
  m_iRecordLength = 0;
  
  for (;;) {
    
    if (m_pNext == m_pLimit) {
      m_pNext = m_pScratch->pWorkArea();
      i8 iBytesRead = fread(m_pNext,1,C_WORKAREA,m_pFilePtr);
      
      if ( ! iBytesRead )
        break;
      
      m_pLimit = m_pNext + iBytesRead;
    }
    
    char c = *m_pNext++;
    
    if ('\n' == c || (char)EOF == c)
      break;
    
    *z++ = c;
    m_iRecordLength++;
  }
  
  *z = '\0';
  m_iRecordLength++;
  
  if (*m_pRecord->pWorkArea()) {
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
  
  i8 rCode = fseeko(m_pFilePtr,iOffset,SEEK_SET);
  
  if (rCode) {
    m_xSysCall.vError(errno);
    vWarn("File::vSeek: fseeko() error; errno=%d",errno);
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
  
  i8 rCode = fseeko(m_pFilePtr,0,SEEK_END);
  
  if (rCode) {
    m_xSysCall.vError(errno);
    vWarn("File::vSeek: fseeko() error; errno=%d",errno);
    return;
  }
  
  m_xSysCall.vSuccess();
}

/*----------------------------------------------------------------------------*/
/* set file arguments                                                         */
/*----------------------------------------------------------------------------*/
void File::vSet(const char* aFilePath, const char* aOpenMode, i8 iTraceLevel)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  if (m_pFilePtr) {
    vWarn("File::vSet: previous file not closed: %s\n",m_sFilePath.a());
    vClose();
  }
  
  m_sFilePath   = aFilePath;
  m_sOpenMode   = aOpenMode;
  m_iTraceLevel = iTraceLevel;
}

/*----------------------------------------------------------------------------*/
/* set file arguments                                                         */
/*----------------------------------------------------------------------------*/
void File::vSet(FILE* pFilePtr, const char* aFilePath, const char* aOpenMode, i8 iTraceLevel)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  if (m_pFilePtr) {
    vWarn("File::vSet: previous file not closed: %s\n",m_sFilePath.a());
    vClose();
  }
  
  m_sFilePath   = aFilePath;
  m_sOpenMode   = aOpenMode;
  m_pFilePtr    = pFilePtr;
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
  
  i8 rCode = ftello(m_pFilePtr);
  
  if (-1 == rCode) {
    m_xSysCall.vError(errno);
    vWarn("File::iTell: ftello() error; errno=%d",errno);
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

  if ( ! m_pFilePtr )
    return;
  
  i8 iResult = ungetc(cChar,m_pFilePtr);
  
  if ((char)EOF == iResult)
    vWarn("File::vUngetChar: unget failed: '%c' %s\n",cChar,m_sFilePath.a());
}

/*----------------------------------------------------------------------------*/
/* zero this structure -- used only in special/nesting cases                  */
/*----------------------------------------------------------------------------*/
void File::vZero(void)
{
  _precondition_(_MAGIC_,CN_FILE,_FOOTER_);

  m_xSysCall.vClear();
  
  m_sFilePath   = "";
  m_sOpenMode   = "";
  m_iTraceLevel = 0;
  m_pFilePtr    = nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* File::pFooter(void)
{
  _precondition_(_MAGIC_,CN_FILE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
