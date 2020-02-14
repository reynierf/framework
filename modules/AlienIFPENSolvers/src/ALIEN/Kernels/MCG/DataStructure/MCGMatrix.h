// -*- C++ -*-
#ifndef ALIEN_MCGIMPL_MCGMATRIX_H
#define ALIEN_MCGIMPL_MCGMATRIX_H
/* Author : mesriy at Tue Jul 24 15:56:45 2012
 * Generated by createNew
 */

#include <ALIEN/Kernels/MCG/MCGPrecomp.h>
#include <ALIEN/Kernels/MCG/MCGBackEnd.h>
#include <ALIEN/Core/Impl/IMatrixImpl.h>
#include <ALIEN/Core/Block/Block.h>
#include <ALIEN/Data/Space.h>

/*---------------------------------------------------------------------------*/

BEGIN_MCGINTERNAL_NAMESPACE

template<int N>
struct CSRStruct ;

class MatrixInternal ;

END_MCGINTERNAL_NAMESPACE

/*---------------------------------------------------------------------------*/

namespace Alien {

/*---------------------------------------------------------------------------*/

class MCGMatrix : public IMatrixImpl
{
 public:
  typedef MCGInternal::MatrixInternal MatrixInternal;

 public :

  MCGMatrix(const MultiMatrixImpl * multi_impl);
  virtual ~MCGMatrix() ;

public:
  void init(const ISpace& row_space,
            const ISpace& col_space,
            const MatrixDistribution & dist)
  {
    std::cout<<"init MCGMatrix with m_domain_offset = dist.rowOffset()/m_equations_num "<<std::endl ;
    //m_unknowns_num = space.structInfo().size() ;
    if(this->block()!=nullptr)
      m_unknowns_num = this->block()->size();
    else
      m_unknowns_num = 1;
    m_equations_num = m_unknowns_num ;
    m_block_size = m_equations_num*m_unknowns_num ;
    m_domain_offset = dist.rowOffset()/m_equations_num ;
  }

  void initSpace0(const Space & space)
  {
    m_space0 = &space ;
  }

  void initSpace1(const Space & space)
  {
    m_space1 = &space ;
  }

  const ISpace & space() const {
    if(m_space0)
      return *m_space0 ;
    else
      return IMatrixImpl::rowSpace();
  }

  const Space & space0() const {
    return *m_space0 ;
  }

  const Space & space1() const {
    return *m_space1 ;
  }

  void clear() { }

public:

  void setBlockSize(Integer nequations,Integer nunknowns)
  {
    m_equations_num = nequations ;
    m_unknowns_num = nunknowns ;
    m_block_size = m_equations_num*m_unknowns_num ;
  }

  Integer getEquationsNum() const {
    return m_equations_num ;
  }

  Integer getUnknownsNum() const {
    return m_unknowns_num ;
  }


  bool initMatrix(int nrow,
                  int const* row_offset,
                  int const* cols) ;

  bool initSubMatrix01(int nrow,
                       int nb_extra_eq,
                       int const* row_offset,
                       int const* cols) ;

  bool initSubMatrix11(int nb_extra_eq,
                       int const* row_offset,
                       int const* cols) ;


  bool addMatrixValues(const int nrow, const int * rows,
                       const int * ncols, const int * cols,
                       const double * values) ;

  bool initMatrixValues(int nrow,
                       int const* row_offset,
                       int const* cols,
                       Real const* values) ;

  bool setInitValues(const int nrow, const int * rows,
                     const double * values) ;

  bool initSubMatrix11Values( Real const* ExtraDiagValues) ;

  bool initSubMatrix10Values( Real const* ExtraRowValues) ;

  bool initSubMatrix01Values( Real const* ExtraColValues) ;

  Space m_row_space1;
  Space m_col_space1;
  
  int const* m_row_offset1;
  int const* m_row_offset2;
  int const* m_row_offset3;  
  int const* m_row_offset4;

public:
  MatrixInternal * internal() { return m_internal ; }
  const MatrixInternal * internal() const { return m_internal ; }

  int const * get_row_offset1() const;
  int const * get_row_offset2() const;

  template<int N>
  MCGInternal::CSRStruct<N>& getCSRStruct() ;

  template<int N>
  MCGInternal::CSRStruct<N> const& getCSRStruct() const;

private :
  Integer ijk(Integer i, Integer j, Integer k, Integer block_size, Integer unknowns_num) const {
    return k*block_size+i*unknowns_num+j ;
  }

  MatrixInternal* m_internal ;
  Integer m_equations_num ;
  Integer m_unknowns_num ;
  Integer m_block_size ;
  Integer m_domain_offset ;
  Space const* m_space0 ;
  Space const* m_space1 ;

  //const MatrixDistribution* m_distribution;
};

/*---------------------------------------------------------------------------*/

} // namespace Alien

/*---------------------------------------------------------------------------*/

#endif /* ALIEN_MCGIMPL_MCGMATRIX_H */
