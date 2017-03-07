/*============================================================================*/
/* StrVar.hpp -- fixed-length string variable (template)                      */
/*============================================================================*/
#pragma once

namespace Values
{
  template <int SIZE>
  class StrVar : public StrVal
  {
  private:
    char m_cArea[SIZE]; // Str byte area
    
    // must be last
    Footer m_xFooter;
    
  public:    
    StrVar(void);
    StrVar(const char* aArg);
    StrVar(StrVar* pThis);
   ~StrVar(void);
    
    void operator =(Str* aArg);
    void operator =(char* aArg);
    void operator =(StrVal& xArg);
  
//    operator Str  (void);
//    operator Str& (void);
//    operator Str* (void);
  
    void vTest(void);
  
    Footer* pFooter(void);
  };
  
  void* _pStrVar(void* aAddr);
}

using namespace Values;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <int SIZE>
StrVar<SIZE>::StrVar(void)
: StrVal(CN_STRVAR,SIZE)
{
  _precondition_(_MAGIC_,CN_STRVAR,_FOOTER_);
  
  _checkInitialisedSize_(SIZE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <int SIZE>
StrVar<SIZE>::StrVar(const char* aArg)
: StrVal(CN_STRVAR,SIZE)
{
  _precondition_(_MAGIC_,CN_STRVAR,_FOOTER_);
  
  StrVal::vCopy(aArg);
  
  _checkInitialisedSize_(SIZE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <int SIZE>
StrVar<SIZE>::StrVar(StrVar* pThis)
: StrVal(CN_STRVAR,SIZE)
{
  _precondition_(_MAGIC_,CN_STRVAR,_FOOTER_);
  
  _checkInitialisedSize_(SIZE);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <int SIZE>
StrVar<SIZE>::~StrVar(void)
{
  _precondition_(_MAGIC_,CN_STRVAR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void StrVar<SIZE>::operator =(Str* aArg)
{
  _precondition_(_MAGIC_,CN_STRVAR,_FOOTER_);
  
  StrVal::vCopy(aArg->a());
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void StrVar<SIZE>::operator =(char* aArg)
{
  _precondition_(_MAGIC_,CN_STRVAR,_FOOTER_);
  
  StrVal::vCopy(aArg);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void StrVar<SIZE>::operator =(StrVal& rArg)
{
  _precondition_(_MAGIC_,CN_STRVAR,_FOOTER_);
  
  vCopy(rArg.aStr());
}

/*----------------------------------------------------------------------------*/
/* get reference to Str area                                                  */
/*----------------------------------------------------------------------------*/
//template <int SIZE>
//StrVar<SIZE>::operator Str&(void)
//{
//  _precondition_(_MAGIC_);
//  
//  // return reference to the RowId inherited class
//  char* aAddr = pAdd<char*>(this,sizeof(StrVal));
//  
//  return reinterpret_cast<Str&>(*aAddr);
//}

/*----------------------------------------------------------------------------*/
/* get address of Str area                                                    */
/*----------------------------------------------------------------------------*/
//template <int SIZE>
//StrVar<SIZE>::operator Str(void)
//{
//  _precondition_(_MAGIC_);
//  
//  // return reference to the RowId inherited class
//  return reinterpret_cast<Str>(*pAdd<char*>(this,sizeof(StrVal)));
//}
//
/*----------------------------------------------------------------------------*/
/* get address of Str area                                                    */
/*----------------------------------------------------------------------------*/
//template <int SIZE>
//StrVar<SIZE>::operator Str*(void)
//{
//  _precondition_(_MAGIC_);
//  
//  // return reference to the RowId inherited class
//  return reinterpret_cast<Str*>(pAdd<char*>(this,sizeof(StrVal)));
//}
//
/*----------------------------------------------------------------------------*/
/* test Str                                                                   */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void StrVar<SIZE>::vTest(void)
{
  _precondition_(_MAGIC_,CN_STRVAR);

  aStrArea()->vPrint();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <int SIZE>
Footer* StrVar<SIZE>::pFooter(void)
{
  _precondition_(_MAGIC_,CN_STRVAR);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
