/*============================================================================*/
/* s.hpp -- an s-string                                                       */
/*============================================================================*/
#pragma once

#include "Values.h"

namespace Values
{
  class s : public Object
  {
  private:
    i8   m_nDataBytes; // size of the data area (may not be length of the string)
    // ..dataArea..
    
  public:
    void* operator new(size_t);
    
    s(eCN eClass, i8 iSize);
   ~s(void);
    
    void operator = (const char* aArg);
    void operator = (char* aArg);
    void operator = (s& rArg);
    
    void  vAppend(const char c);
    void  vAppend(const char* aArg);
    void  vCopy(const char* aArg);
    char* pDataArea(void);
    i8    nDataBytes(void);
    void  vDataBytes(void);
    bool  bEnds(const char* aSuffix);
    void  vInit(i8 iSize);
    bool  bIsEqual(s& sArg, i8 iCase) ;
    i8    iLength(void);
    char* a(void);
    char* z(void);
    
    Footer* pFooter(void);
  };
  
  //};

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  
  template<int SIZE>
  class sAlt : public Object
  {
  private:
    i8     m_nDataBytes; // size of the data area (may not be length of the string)
    char   m_mDataArea[SIZE]; // string data area
    Footer m_xFooter;
  public:
    void* operator new(size_t);
    
    sAlt(eCN eClass, i8 iSize);
    ~sAlt(void);
    
    void operator = (const char* aArg);
    void operator = (char* aArg);
    void operator = (s& rArg);
    
    void  vAppend(const char c);
    void  vAppend(const char* aArg);
    void  vCopy(const char* aArg);
    char* mDataArea(void);
    i8    nDataBytes(void);
    void  vDataBytes(void);
    bool  bEnds(const char* aSuffix);
    void  vInit(i8 iSize);
    bool  bIsEqual(s& sArg, i8 iCase) ;
    i8    iLength(void);
    char* a(void);
    char* z(void);
    
    Footer* pFooter(void);
  };
  
}

using namespace Values;

/*----------------------------------------------------------------------------*/
/* operator new: class cannot be new'd                                        */
/*----------------------------------------------------------------------------*/
template<int SIZE>
void* sAlt<SIZE>::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  vFatal("fatal: s class cannot be new'd");
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template<int SIZE>
sAlt<SIZE>::sAlt(eCN eClassNo, i8 iSize)
: Object(eClassNo)
{
  _precondition_(_MAGIC_,eClassNo);
  
  m_nDataBytes = iSize;
  *mDataArea() = '\0';
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template<int SIZE>
sAlt<SIZE>::~sAlt(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template<int SIZE>
void sAlt<SIZE>::operator = (const char* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  String::vCopyLen(mDataArea(),aArg,m_nDataBytes-1);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template<int SIZE>
void sAlt<SIZE>::operator = (char* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  String::vCopyLen(mDataArea(),aArg,m_nDataBytes-1);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template<int SIZE>
void sAlt<SIZE>::operator = (s& rArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  vCopy(rArg.a());
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* append given character                                                     */
/*----------------------------------------------------------------------------*/
template<int SIZE>
void sAlt<SIZE>::vAppend(const char c)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  // limit???
  
  String::vConcat(mDataArea(),c);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* append argument                                                            */
/*----------------------------------------------------------------------------*/
template<int SIZE>
void sAlt<SIZE>::vAppend(const char* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  String::vConcat(mDataArea(),aArg);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* copy argument                                                              */
/*----------------------------------------------------------------------------*/
template<int SIZE>
void sAlt<SIZE>::vCopy(const char* aArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  String::vCopyLen(mDataArea(),aArg,m_nDataBytes-1);
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get address of data area                                                   */
/*----------------------------------------------------------------------------*/
template<int SIZE>
char* sAlt<SIZE>::mDataArea(void)
{
  //_precondition_(_MAGIC_,_FOOTER_);
  
  char* p = pAdd<char*>(this,sizeof(s));
  
  return p;
}

/*----------------------------------------------------------------------------*/
/* get size of data area                                                      */
/*----------------------------------------------------------------------------*/
template<int SIZE>
i8 sAlt<SIZE>::nDataBytes(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_nDataBytes;
}

/*----------------------------------------------------------------------------*/
/* set limit of data size (based on the string size)                          */
/*----------------------------------------------------------------------------*/
template<int SIZE>
void sAlt<SIZE>::vDataBytes(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_nDataBytes = String::iSize(mDataArea());
}

/*----------------------------------------------------------------------------*/
/* test if string ends with given suffix                                      */
/*----------------------------------------------------------------------------*/
template<int SIZE>
bool sAlt<SIZE>::bEnds(const char* aSuffix)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = String::bEnds(mDataArea(),aSuffix);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
template<int SIZE>
void sAlt<SIZE>::vInit(i8 iSize)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_nDataBytes = iSize;
  *mDataArea() = '\0';
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* is euqal to given argument?                                                */
/*----------------------------------------------------------------------------*/
template<int SIZE>
bool sAlt<SIZE>::bIsEqual(s& sArg, i8 iCase)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  bool b = String::bEqualTo(mDataArea(),sArg.pDataArea(),iCase);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get length of string in data area                                          */
/*----------------------------------------------------------------------------*/
template<int SIZE>
i8 sAlt<SIZE>::iLength(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return String::iLength(mDataArea());
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
template<int SIZE>
char* sAlt<SIZE>::a(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return mDataArea();
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
template<int SIZE>
char* sAlt<SIZE>::z(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return String::zTerminator(mDataArea());
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template<int SIZE>
Footer* sAlt<SIZE>::pFooter(void)
{
  _precondition_(_MAGIC_);
  
  Footer* pFooter = pAdd<Footer*>(this,sizeof(s)+m_nDataBytes);
  
  return pFooter;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
