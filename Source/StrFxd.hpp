/*============================================================================*/
/* StrFxd.hpp -- fixed-length string value template                           */
/*============================================================================*/
#pragma once

namespace Values
{
  template <int SIZE>
  class StrFxd : public StrVbl
  {
  private:
    char m_cArea[SIZE]; // Str byte area
    
    // must be last
    Footer m_xFooter;
    
  public:    
    StrFxd(void);
    StrFxd(const char* aArg);
    StrFxd(StrFxd* pThis);
   ~StrFxd(void);
    
    void operator =(const char* aArg);
    void operator =(char* aArg);
    void operator =(StrVbl& xArg);
  
//    operator Str  (void);
//    operator Str& (void);
//    operator Str* (void);
  
    void vTest(void);
  
    Footer* pFooter(void);
  };
  
  void* _pStrFxd(void* aAddr);
}

using namespace Values;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <int SIZE>
StrFxd<SIZE>::StrFxd(void)
: StrVbl(CN_STRFXD,SIZE)
{
  _precondition_(_MAGIC_,CN_STRFXD,_FOOTER_);
  
  _checkInitialisedSize_(SIZE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <int SIZE>
StrFxd<SIZE>::StrFxd(const char* aArg)
: StrVbl(CN_STRFXD,SIZE)
{
  _precondition_(_MAGIC_,CN_STRFXD,_FOOTER_);
  
  StrVbl::vCopy(aArg);
  
  _checkInitialisedSize_(SIZE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <int SIZE>
StrFxd<SIZE>::StrFxd(StrFxd* pThis)
: StrVbl(CN_STRFXD,SIZE)
{
  _precondition_(_MAGIC_,CN_STRFXD,_FOOTER_);
  
  _checkInitialisedSize_(SIZE);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <int SIZE>
StrFxd<SIZE>::~StrFxd(void)
{
  _precondition_(_MAGIC_,CN_STRFXD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void StrFxd<SIZE>::operator =(const char* aArg)
{
  _precondition_(_MAGIC_,CN_STRFXD,_FOOTER_);
  
  StrVbl::vCopy(aArg);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void StrFxd<SIZE>::operator =(char* aArg)
{
  _precondition_(_MAGIC_,CN_STRFXD,_FOOTER_);
  
  StrVbl::vCopy(aArg);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void StrFxd<SIZE>::operator =(StrVbl& rArg)
{
  _precondition_(_MAGIC_,CN_STRFXD,_FOOTER_);
  
  vCopy(rArg.aStr());
}

/*----------------------------------------------------------------------------*/
/* get reference to Str area                                                  */
/*----------------------------------------------------------------------------*/
//template <int SIZE>
//StrFxd<SIZE>::operator Str&(void)
//{
//  _precondition_(_MAGIC_);
//  
//  // return reference to the RowId inherited class
//  char* aAddr = pAdd<char*>(this,sizeof(StrVbl));
//  
//  return reinterpret_cast<Str&>(*aAddr);
//}

/*----------------------------------------------------------------------------*/
/* get address of Str area                                                    */
/*----------------------------------------------------------------------------*/
//template <int SIZE>
//StrFxd<SIZE>::operator Str(void)
//{
//  _precondition_(_MAGIC_);
//  
//  // return reference to the RowId inherited class
//  return reinterpret_cast<Str>(*pAdd<char*>(this,sizeof(StrVbl)));
//}
//
/*----------------------------------------------------------------------------*/
/* get address of Str area                                                    */
/*----------------------------------------------------------------------------*/
//template <int SIZE>
//StrFxd<SIZE>::operator Str*(void)
//{
//  _precondition_(_MAGIC_);
//  
//  // return reference to the RowId inherited class
//  return reinterpret_cast<Str*>(pAdd<char*>(this,sizeof(StrVbl)));
//}
//
/*----------------------------------------------------------------------------*/
/* test Str                                                                   */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void StrFxd<SIZE>::vTest(void)
{
  _precondition_(_MAGIC_,CN_STRFXD);

  aStrArea()->vPrint();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <int SIZE>
Footer* StrFxd<SIZE>::pFooter(void)
{
  _precondition_(_MAGIC_,CN_STRFXD);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
