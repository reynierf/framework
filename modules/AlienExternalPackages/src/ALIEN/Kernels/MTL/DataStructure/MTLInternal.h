#ifndef ALIEN_MTLIMPL_MTLINTERNAL_H
#define ALIEN_MTLIMPL_MTLINTERNAL_H
/* Author : havep at Fri Jun  6 17:56:23 2008
 * Generated by createNew
 */

//! Internal struct for MTL implementation
/*! Separate data from header;
 *  can be only included by LinearSystem and LinearSolver
 */
#include <ALIEN/Kernels/MTL/MTLPrecomp.h>
#include <boost/numeric/mtl/mtl.hpp>
#include <boost/numeric/itl/itl.hpp>

#include <ALIEN/Distribution/MatrixDistribution.h>

/*---------------------------------------------------------------------------*/

namespace Alien::MTL4Internal {

/*---------------------------------------------------------------------------*/

//! Check parallel feature for MTL
struct Features
{
  static void checkParallel(const MatrixDistribution& dist)
  {
    // This behaviour may be changed when Parallel MTL will be plugged
#ifndef USE_PMTL4
    if (dist.isParallel())
      throw Arccore::FatalErrorException(
          A_FUNCINFO, "MTL does not support parallel feature");
#endif
  }
};

/*---------------------------------------------------------------------------*/

class MatrixInternal
{
 private:
#ifdef MTL_4_0_9555
  typedef mtl::mat::parameters<mtl::row_major, mtl::index::c_index,
      mtl::non_fixed::dimensions, false, unsigned>
      row_para;
  typedef mtl::mat::parameters<mtl::tag::col_major> col_para;
  typedef mtl::compressed2D<double, col_para> col_matrix_type;
  typedef mtl::compressed2D<double, row_para> row_matrix_type;
  typedef mtl::compressed2D<double, row_para> matrix_type;
#else
  typedef mtl::matrix::parameters<mtl::row_major, mtl::index::c_index,
      mtl::non_fixed::dimensions, false, unsigned>
      row_para;
  typedef mtl::matrix::parameters<mtl::tag::col_major> col_para;
  typedef mtl::compressed2D<double, col_para> col_matrix_type;
  typedef mtl::compressed2D<double, row_para> row_matrix_type;
  typedef mtl::compressed2D<double, row_para> matrix_type;
#endif

 public:
#ifdef USE_PMTL4
  typedef mtl::matrix::distributed<matrix_type> MTLMatrixType;
  MatrixInternal(int nrow, int ncol, mtl::par::block_distribution& row_dist)
  : m_internal(nrow, ncol, row_dist)
  {}
#else /* USE_PMTL4 */
  typedef row_matrix_type MTLMatrixType;
  MatrixInternal(int nrow, int ncol)
  : m_internal(nrow, ncol)
  {}
#endif /* USE_PMTL4 */

 public:
  MTLMatrixType m_internal;
};

/*---------------------------------------------------------------------------*/

class VectorInternal
{
 private:
  typedef mtl::dense_vector<double> vector_type;

 public:
#ifdef USE_PMTL4
  typedef mtl::vector::distributed<vector_type> MTLVectorType;
  VectorInternal(int nrow, mtl::par::block_distribution& row_dist)
  : m_internal(nrow, row_dist)
  {}
#else /* USE_PMTL4 */
  typedef vector_type MTLVectorType;
  VectorInternal(int nrow)
  : m_internal(nrow)
  {}
#endif /* USE_PMTL4 */

 public:
  MTLVectorType m_internal;
};

/*---------------------------------------------------------------------------*/

} // namespace Alien::MTL4Internal

/*---------------------------------------------------------------------------*/
#endif /* ALIEN_MTLIMPL_MTLINTERNAL_H */
