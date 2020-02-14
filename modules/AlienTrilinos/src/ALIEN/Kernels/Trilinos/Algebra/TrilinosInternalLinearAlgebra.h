// -*- C++ -*-
#ifndef ALIEN_KERNELS_TRILINOS_ALGEBRA_TRILINOSINTERNALLINEARALGEBRA_H
#define ALIEN_KERNELS_TRILINOS_ALGEBRA_TRILINOSINTERNALLINEARALGEBRA_H

#include <ALIEN/Alien-TrilinosPrecomp.h>
#include <ALIEN/Utils/Precomp.h>

#include <ALIEN/Kernels/Trilinos/TrilinosBackEnd.h>
#include <ALIEN/Core/Backend/IInternalLinearAlgebraT.h>

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Alien {

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef AlgebraTraits<BackEnd::tag::tpetraserial>::matrix_type TrilinosMatrixType;
typedef AlgebraTraits<BackEnd::tag::tpetraserial>::vector_type TrilinosVectorType;

class ALIEN_TRILINOS_EXPORT TrilinosInternalLinearAlgebra
        : public IInternalLinearAlgebra<TrilinosMatrixType, TrilinosVectorType> {
public:
    TrilinosInternalLinearAlgebra(Arccore::MessagePassing::IMessagePassingMng *pm = nullptr) {}

    virtual ~TrilinosInternalLinearAlgebra() {}

    // IInternalLinearAlgebra interface.
    Arccore::Real norm0(const Vector &x) const;

    Arccore::Real norm1(const Vector &x) const;

    Arccore::Real norm2(const Vector &x) const;

    void mult(const Matrix &a, const Vector &x, Vector &r) const;

    void axpy(const Arccore::Real &alpha, const Vector &x, Vector &r) const;

    void aypx(const Arccore::Real &alpha, Vector &y, const Vector &x) const;

    void copy(const Vector &x, Vector &r) const;

    Arccore::Real dot(const Vector &x, const Vector &y) const;

    void scal(const Arccore::Real &alpha, Vector &x) const;

    void diagonal(const Matrix &a, Vector &x) const;

    void reciprocal(Vector &x) const;

    void pointwiseMult(const Vector &x, const Vector &y, Vector &w) const;

    void dump(Matrix const &a, std::string const &filename) const;

    void dump(Vector const &x, std::string const &filename) const;

};

#ifdef KOKKOS_ENABLE_OPENMP
typedef AlgebraTraits<BackEnd::tag::tpetraomp>::matrix_type TpetraOmpMatrixType;
typedef AlgebraTraits<BackEnd::tag::tpetraomp>::vector_type TpetraOmpVectorType;

class ALIEN_TRILINOS_EXPORT TpetraOmpInternalLinearAlgebra
  : public IInternalLinearAlgebra<TpetraOmpMatrixType, TpetraOmpVectorType>
{
 public:
  TpetraOmpInternalLinearAlgebra(IParallelMng* pm = nullptr){}
  virtual ~TpetraOmpInternalLinearAlgebra() {}

  // IInternalLinearAlgebra interface.
  Real norm0(const Vector& x) const  { return 0. ; }
  Real norm1(const Vector& x) const  { return 0. ; }
  Real norm2(const Vector& x) const  { return 0. ; }
  void mult(const Matrix& a, const Vector& x, Vector& r) const {}
  void axpy(const Real& alpha, const Vector& x, Vector& r) const {}
  void aypx(const Real& alpha, Vector& y, const Vector& x) const {}
  void copy(const Vector& x, Vector& r) const {}
  Real dot(const Vector& x, const Vector& y) const { return 0. ; }
  void scal(const Real& alpha, Vector& x) const {}
  void diagonal(const Matrix& a, Vector& x) const {}
  void reciprocal(Vector& x) const {}
  void pointwiseMult(const Vector& x, const Vector& y, Vector& w) const {}
};
#endif

#ifdef KOKKOS_ENABLE_THREADS
typedef AlgebraTraits<BackEnd::tag::tpetrapth>::matrix_type TpetraPthMatrixType;
typedef AlgebraTraits<BackEnd::tag::tpetrapth>::vector_type TpetraPthVectorType;

class ALIEN_TRILINOS_EXPORT TpetraPthInternalLinearAlgebra
  : public IInternalLinearAlgebra<TpetraPthMatrixType, TpetraPthVectorType>
{
 public:
  TpetraPthInternalLinearAlgebra(IParallelMng* pm = nullptr){}
  virtual ~TpetraPthInternalLinearAlgebra() {}

  // IInternalLinearAlgebra interface.
  Real norm0(const Vector& x) const { return 0. ; }
  Real norm1(const Vector& x) const { return 0. ; }
  Real norm2(const Vector& x) const { return 0. ; }
  void mult(const Matrix& a, const Vector& x, Vector& r) const { }
  void axpy(const Real& alpha, const Vector& x, Vector& r) const { }
  void aypx(const Real& alpha, Vector& y, const Vector& x) const { }
  void copy(const Vector& x, Vector& r) const { }
  Real dot(const Vector& x, const Vector& y) const { return 0. ; }
  void scal(const Real& alpha, Vector& x) const { }
  void diagonal(const Matrix& a, Vector& x) const { }
  void reciprocal(Vector& x) const { }
  void pointwiseMult(const Vector& x, const Vector& y, Vector& w) const { }
};
#endif

/*---------------------------------------------------------------------------*/
#ifdef KOKKOS_ENABLE_CUDA
typedef AlgebraTraits<BackEnd::tag::tpetracuda>::matrix_type TpetraCudaMatrixType;
typedef AlgebraTraits<BackEnd::tag::tpetracuda>::vector_type TpetraCudaVectorType;

class ALIEN_TRILINOS_EXPORT TpetraCudaInternalLinearAlgebra
  : public IInternalLinearAlgebra<TpetraCudaMatrixType, TpetraCudaVectorType>
{
 public:
  TpetraCudaInternalLinearAlgebra(IParallelMng* pm = nullptr){}
  virtual ~TpetraCudaInternalLinearAlgebra() {}

  // IInternalLinearAlgebra interface.
  Real norm0(const Vector& x) const { return 0. ; }
  Real norm1(const Vector& x) const { return 0. ; }
  Real norm2(const Vector& x) const { return 0. ; }
  void mult(const Matrix& a, const Vector& x, Vector& r) const { }
  void axpy(const Real& alpha, const Vector& x, Vector& r) const { }
  void aypx(const Real& alpha, Vector& y, const Vector& x) const { }
  void copy(const Vector& x, Vector& r) const { }
  Real dot(const Vector& x, const Vector& y) const { return 0. ; }
  void scal(const Real& alpha, Vector& x) const { }
  void diagonal(const Matrix& a, Vector& x) const { }
  void reciprocal(Vector& x) const { }
  void pointwiseMult(const Vector& x, const Vector& y, Vector& w) const { }
};
#endif

/*---------------------------------------------------------------------------*/

} // namespace Alien

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif /* ALIEN_TRILINOSIMPL_TRILINOSLINEARALGEBRA_H */
