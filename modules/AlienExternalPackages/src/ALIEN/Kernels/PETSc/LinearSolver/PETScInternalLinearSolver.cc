/* Author : havep at Fri Jun  6 11:02:38 2008
 * Generated by createNew
 */

#include "PETScInternalLinearSolver.h"

#include <memory>
#include <sstream>

#ifdef ALIEN_USE_ARCANE
#include <arcane/MathUtils.h>
#endif

#include <ALIEN/Utils/ParameterManager/BaseParameterManager.h>

#include <ALIEN/Core/Backend/LinearSolverT.h>
#include <ALIEN/Expression/Solver/SolverStats/SolverStater.h>

#include <arccore/base/NotImplementedException.h>

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include <petscksp.h>
#include <petscmat.h>
#include <petscvec.h>
#include <petscversion.h>

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include "ALIEN/Utils/Precomp.h"
#include <ALIEN/Data/Space.h>
#include <ALIEN/Kernels/PETSc/Algebra/PETScInternalLinearAlgebra.h>
#include <ALIEN/Kernels/PETSc/Algebra/PETScLinearAlgebra.h>
#include <ALIEN/Kernels/PETSc/DataStructure/PETScInternal.h>
#include <ALIEN/Kernels/PETSc/DataStructure/PETScMatrix.h>
#include <ALIEN/Kernels/PETSc/DataStructure/PETScVector.h>
#include <ALIEN/Kernels/PETSc/LinearSolver/IPETScKSP.h>
#include <ALIEN/Kernels/PETSc/LinearSolver/IPETScPC.h>
#include <ALIEN/Kernels/PETSc/PETScBackEnd.h>
#include <ALIEN/axl/PETScLinearSolver_IOptions.h>

#include <arccore/base/NotSupportedException.h>
#include <arccore/message_passing_mpi/MpiMessagePassingMng.h>

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Alien {

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

// Compile PETScLinearSolver.
template class ALIEN_EXTERNALPACKAGES_EXPORT LinearSolver<BackEnd::tag::petsc>;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // namespace Alien

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include <ALIEN/Kernels/PETSc/LinearSolver/PETScConfig.h>

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Alien {

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

bool PETScInternalLinearSolver::m_global_initialized = false;
bool PETScInternalLinearSolver::m_global_want_trace = false;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

PETScInternalLinearSolver::PETScInternalLinearSolver(
    Arccore::MessagePassing::IMessagePassingMng* parallel_mng,
    IOptionsPETScLinearSolver* options)
: m_verbose(VerboseTypes::none)
, m_null_space_constant_opt(false)
, m_parallel_mng(parallel_mng)
, m_options(options)
{}

/*---------------------------------------------------------------------------*/

PETScInternalLinearSolver::
~PETScInternalLinearSolver()
{
}

/*---------------------------------------------------------------------------*/

void
PETScInternalLinearSolver::
init(int argc, char** argv)
{
  if(m_parallel_mng == nullptr)
    return;
  //m_stater.reset();
  //m_stater.startInitializationMeasure();

  if (not m_global_initialized) 
  {
    std::vector<std::string> petsc_options;
    if (m_options->traceInfo())
    {
      // See PetscInitialize for details
      // http://www-unix.mcs.anl.gov/petsc/petsc-as/snapshots/petsc-current/docs/manualpages/Sys/PetscInitialize.html
      petsc_options.push_back("-info");
      petsc_options.push_back("-log_trace");
      petsc_options.push_back("petsc.log");
      m_global_want_trace = true;
    }
    if(argc>0)
    {
      for(int i=0;i<argc;++i)
      {
        std::stringstream stream;
        stream << argv[i];
        petsc_options.push_back(stream.str());
      }
    }
    if (not petsc_options.empty())
    {
      alien_info([&] {
        std::stringstream petsc_options_str("PETSC CMD : ");
        for(std::size_t i=0;i<petsc_options.size();++i)
          petsc_options_str<<petsc_options[i]<<" ";
        cout()<<petsc_options_str.str();
      });
    }

    // Emulate argc,argv with dynamic options
    // (since PetscOptionsInsertString cannot insert info options)
//    int petsc_argc = petsc_options.size()+1;
    const char ** petsc_c_options = new const char*[argc];
    petsc_c_options[0] = NULL;
    for(std::size_t i=0;i<petsc_options.size();++i)
      petsc_c_options[i+1] = petsc_options[i].c_str();
    char ** argv2 = (char**)petsc_c_options;

    alien_debug([&] {
      cout() << "PETSc Initialisation";
    });
    PetscInitialize(&argc,&argv2,NULL,"PETSc Initialisation");
    delete[] petsc_c_options;

    // Reduce memory due to log for graphical viewer
    PetscLogActions(PETSC_FALSE);
    PetscLogObjects(PETSC_FALSE);

#if ((PETSC_VERSION_MAJOR <= 3 && PETSC_VERSION_MINOR < 3) || (PETSC_VERSION_MAJOR < 3))
    if (m_options->traceInfo())
    {
      alien_info([&] {
        cout() << "PETSc options:";
      });
      PetscOptionsPrint(stdout);
    }
#endif /* PETSC_VERSION */

    m_global_initialized = true;
  }
  else
  {
	if (m_options->traceInfo() != m_global_want_trace) {
      alien_warning([&] {
        cout() << "PETSc trace-info option is global and given by the first initialisation; current state is " << ((m_global_want_trace)?"on":"off");
      });
    }
  }

  //m_stater.stopInitializationMeasure();
}

/*---------------------------------------------------------------------------*/

void
PETScInternalLinearSolver::
init()
{
  if(m_parallel_mng == nullptr)
    return;
  //m_stater.reset();
  //m_stater.startInitializationMeasure();

  if (not m_global_initialized)
  {
    std::vector<Arccore::String> petsc_options;
    if (m_options->traceInfo())
    {
      // See PetscInitialize for details
      // http://www-unix.mcs.anl.gov/petsc/petsc-as/snapshots/petsc-current/docs/manualpages/Sys/PetscInitialize.html
      petsc_options.push_back("-info");
      petsc_options.push_back("-log_trace");
      petsc_options.push_back("petsc.log");
      m_global_want_trace = true;
    }
    if(m_options->cmdLineParam().size()>0)
    {
    	petsc_options.push_back(" "); // separator
      Arccore::String command = m_options->cmdLineParam()[0];
      petsc_options.push_back(command);
    }
    if (not petsc_options.empty())
    {
      alien_info([&] {
        std::stringstream petsc_options_str("PETSC CMD : ");
        for(std::size_t i=0;i<petsc_options.size();++i)
          petsc_options_str<<petsc_options[i]<<" ";
        cout()<<petsc_options_str.str();
      });
    }

    // Emulate argc,argv with dynamic options
    // (since PetscOptionsInsertString cannot insert info options)
    int petsc_argc = petsc_options.size()+1;
    const char ** petsc_c_options = new const char*[petsc_argc];
    petsc_c_options[0] = NULL;
    for(std::size_t i=0;i<petsc_options.size();++i)
      petsc_c_options[i + 1] = petsc_options[i].localstr();
    char ** argv = (char**)petsc_c_options;

    alien_debug([&] {
      cout() << "PETSc Initialisation";
    });
    PetscInitialize(&petsc_argc,&argv,NULL,"PETSc Initialisation");
    delete[] petsc_c_options;

    // Reduce memory due to log for graphical viewer
    PetscLogActions(PETSC_FALSE);
    PetscLogObjects(PETSC_FALSE);
#if ((PETSC_VERSION_MAJOR <= 3 && PETSC_VERSION_MINOR < 3) || (PETSC_VERSION_MAJOR < 3))
    if (m_options->traceInfo())
    {
      alien_info([&] {
        cout() << "PETSc options:";
      });
      PetscOptionsPrint(stdout);
    }
#endif /* PETSC_VERSION */

    m_global_initialized = true;
  }
  else
  {
	if (m_options->traceInfo() != m_global_want_trace) {
      alien_warning([&] {
        cout() << "PETSc trace-info option is global and given by the first initialisation; current state is " << ((m_global_want_trace)?"on":"off");
      });
    }
  }

  m_verbose = m_options->verbose();
  //m_stater.stopInitializationMeasure();
}

/*---------------------------------------------------------------------------*/

void
PETScInternalLinearSolver::updateParallelMng(
    Arccore::MessagePassing::IMessagePassingMng* pm)
{
  m_parallel_mng = pm;
  if (m_parallel_mng != nullptr) {
    auto mpi_mng = dynamic_cast<Arccore::MessagePassing::Mpi::MpiMessagePassingMng*>(pm);
    if (mpi_mng)
      PETSC_COMM_WORLD = *static_cast<const MPI_Comm*>(mpi_mng->getMPIComm());
  }
}

/*---------------------------------------------------------------------------*/

const Alien::SolverStatus&
PETScInternalLinearSolver::
getStatus() const
{
  return m_status;
}

/*---------------------------------------------------------------------------*/

std::shared_ptr<ILinearAlgebra>
PETScInternalLinearSolver::
algebra() const
{
  return std::shared_ptr<ILinearAlgebra>(new PETScLinearAlgebra());
}

/*---------------------------------------------------------------------------*/

bool
PETScInternalLinearSolver::
solve(const PETScMatrix& matrix, const PETScVector& rhs, PETScVector& sol)
{
  // Find zero second member
  if (_isNull(rhs)) return _solveNullRHS(sol);

  const Mat & A = matrix.internal()->m_internal;
  const Vec & b = rhs.internal()->m_internal;
  Vec & x = sol.internal()->m_internal;

  if (m_verbose==VerboseTypes::high)
  {
    alien_info([&] {
      cout()<<"|---------------------------------------------|";
      cout()<<"| Start Linear Solver #"<< m_stater.solveCount();
    });
  }

  //m_stater.startSolveMeasure();
  KSP ksp;

  checkError("Solver create",KSPCreate(PETSC_COMM_WORLD,&ksp));
  checkError("Set solver operators", KSPSetOperators(ksp,
                                                     A,
                                                     A
#if ((PETSC_VERSION_MAJOR <= 3 && PETSC_VERSION_MINOR <= 3) || (PETSC_VERSION_MAJOR < 3))
                                                     ,DIFFERENT_NONZERO_PATTERN
#endif // PETSC_VERSION
						     ));

  // On utilise row space pour le moment
  // N'est utilisé que dans FieldSplit
  // Voir avec refactoring PETSc
  m_options->solver()->configure(ksp, matrix.rowSpace(), matrix.distribution());

  // Impact local : à réactiver en tant qu'option spécifique et probablement dans l'interface ILinearSolver
  // L'implémentation doit être prise en charge par les solveurs (faut il factoriser ce mini bout de code ?)
  // L'API devrait permettre de demander au solveur sa capacité de traiter les pbs à une constante près, si oui
  // le prend en charge, sinon demande à l'utilisateur de le prendre en charge
#ifdef PETSC_GETPCTYPE_NEW
  PCType pctype;
#else
  const PCType pctype;
#endif
  PC pc;
  KSPGetPC(ksp,&pc);
  PCGetType(pc, &pctype);
  if(m_null_space_constant_opt)
  {
    std::cout << "NULL SPACE\n";
    if(!std::string(pctype).compare(PCLU))
    {
      alien_info([&] {
        cout() << "[PETScSolver::NullSpaceHasConstants] Matrix factorization accounting for zero pivot\n";
      });
      PCFactorSetShiftAmount(pc, PETSC_DECIDE);
      PCFactorReorderForNonzeroDiagonal(pc, 1e-10);
    }
    else
    {
      MatNullSpace nsp;
      MatNullSpaceCreate(PETSC_COMM_WORLD, PETSC_TRUE, 0, NULL, &nsp);
#ifdef PETSC_HAVE_KSPSETNULLSPACE
      KSPSetNullSpace(ksp, nsp);
#endif
#ifndef NO_USER_WARNING
#ifndef WIN32 // Visual doesn't support #warning directive
#warning "TODO: Ne gère pas la fin de vie de MatNullSpace via MatNullSpaceDestroy"
#endif
#endif /* NO_USER_WARNING */
    }
  }

  if (m_verbose==VerboseTypes::high) {
#ifdef PETSC_KSPMONITORTRUERESIDUALNORM_OLD
    KSPMonitorSet(ksp,KSPMonitorTrueResidualNorm,PETSC_NULL,PETSC_NULL);
#else
    PetscViewerAndFormat *vf;
    PetscViewerAndFormatCreate(PETSC_VIEWER_STDOUT_WORLD,PETSC_VIEWER_DEFAULT,&vf);
    KSPMonitorSet(ksp,(PetscErrorCode (*)(KSP,PetscInt,PetscReal,void*))KSPMonitorTrueResidualNorm,
                  vf,(PetscErrorCode (*)(void**))PetscViewerAndFormatDestroy);
#endif
  }

  checkError("Solver solve",KSPSolve(ksp,
                                     b,
                                     x));

  KSPConvergedReason ksp_reason;
  KSPGetConvergedReason(ksp,&ksp_reason);

  KSPGetIterationNumber(ksp,&m_status.iteration_count);
  KSPGetResidualNorm(ksp,&m_status.residual);

  m_status.succeeded = (ksp_reason >= 0);

  if (m_verbose==VerboseTypes::high) {
    alien_info([&] {
      cout() << "| PETScSolver final state : " << convergedReasonString(ksp_reason);
    });
  } else if (not m_status.succeeded) {
    alien_info([&] {
      cout() << "PETScSolver final state : " << convergedReasonString(ksp_reason) << " after " <<m_status.iteration_count<<" iteration(s)" << " with residual " << m_status.residual;
    });
  }

#if ((PETSC_VERSION_MAJOR <= 3 && PETSC_VERSION_MINOR < 3) || (PETSC_VERSION_MAJOR < 3))
  checkError("Solver destroy",KSPDestroy(ksp));
#else /* PETSC_VERSION */
  checkError("Solver destroy",KSPDestroy(&ksp));
#endif /* PETSC_VERSION */

  //m_stater.stopSolveMeasure(m_status);

  if (m_verbose==VerboseTypes::high)
  {
    alien_info([&] {
      cout()<<"| End Linear Solver";
      cout()<<"|---------------------------------------------|";
    });
  }

  if (m_verbose!=VerboseTypes::low)
  {
    internalPrintInfo();
  }
  return m_status.succeeded;
}

/*---------------------------------------------------------------------------*/

void
PETScInternalLinearSolver::
end()
{
  if (m_verbose!=VerboseTypes::low)
  {
    internalPrintInfo();
  }
}

/*---------------------------------------------------------------------------*/

void
PETScInternalLinearSolver::checkError(const Arccore::String& msg, int ierr)
{
  if (ierr != 0)
  {
    const char * text;
    char * specific;
    PetscErrorMessage(ierr,&text,&specific);
    alien_fatal([&] {
      cout() << msg << " failed : " << text << " / " << specific << "[code=" << ierr << "]";
    });
  }
}

/*---------------------------------------------------------------------------*/

Arccore::String
PETScInternalLinearSolver::convergedReasonString(const Arccore::Integer reason) const
{
  switch (reason) {
  case KSP_CONVERGED_RTOL:
    return "converged RTOL";
  case KSP_CONVERGED_ATOL:
    return "converged ATOL";
  case KSP_CONVERGED_ITS:
    return "converged ITS";
  case KSP_CONVERGED_CG_NEG_CURVE:
    return "converged CG_NEG_CURVE";
  case KSP_CONVERGED_CG_CONSTRAINED:
    return "converged CG_CONSTRAINED";
  case KSP_CONVERGED_STEP_LENGTH:
    return "converged STEP_LENGTH";
  case KSP_CONVERGED_HAPPY_BREAKDOWN:
    return "converged HAPPY_BREAKDOWN";
  case KSP_DIVERGED_NULL:
    return "diverged NULL";
  case KSP_DIVERGED_ITS:
    return "diverged ITS";
  case KSP_DIVERGED_DTOL:
    return "diverged DTOL";
  case KSP_DIVERGED_BREAKDOWN:
    return "diverged BREAKDOWN";
  case KSP_DIVERGED_BREAKDOWN_BICG:
    return "diverged BREAKDOWN_BICG";
  case KSP_DIVERGED_NONSYMMETRIC:
    return "diverged NONSYMMETRIC";
  case KSP_DIVERGED_INDEFINITE_PC:
    return "diverged indefinite preconditioner";
#if ((PETSC_VERSION_MAJOR <= 3 && PETSC_VERSION_MINOR <= 3) || (PETSC_VERSION_MAJOR < 3))
  case KSP_DIVERGED_NAN:
    return "diverged NAN";
#endif
  case KSP_DIVERGED_INDEFINITE_MAT:
    return "diverged indefinite matrix";
  case KSP_CONVERGED_ITERATING:
    return "converged ITERATING";    
  default: {
    //String msg = String("Undefined PETSc reason [") + reason + String("]");
    std::stringstream msg("Undefined PETSc reason [");
    msg << reason << "]";
    throw Arccore::NotSupportedException(A_FUNCINFO, msg.str());
  }
  }
}

/*---------------------------------------------------------------------------*/

void
PETScInternalLinearSolver::
internalPrintInfo() const
{
  if(traceMng())
    m_stater.print(const_cast<Arccore::ITraceMng*>(traceMng()), m_status,
        "Linear Solver : PETScLinearSolver");
}

/*---------------------------------------------------------------------------*/

bool
PETScInternalLinearSolver::
_isNull(const PETScVector& b)
{
  bool is_zero_second_member = false;
  PETScInternalLinearAlgebra my_algebra(m_parallel_mng);
#ifdef ALIEN_USE_ARCANE
  if (Arcane::math::isNearlyZero(my_algebra.norm0(b))) is_zero_second_member = true;
#else
  if (my_algebra.norm0(b) < std::numeric_limits<Arccore::Real>::epsilon())
    is_zero_second_member = true;
#endif
  return is_zero_second_member;
}

/*---------------------------------------------------------------------------*/

bool
PETScInternalLinearSolver::
_solveNullRHS(PETScVector& x)
{
  if (m_verbose==VerboseTypes::high)
      {
	alien_info([&] {
	    cout()<<"|---------------------------------------------|";
	    cout()<<"| Start Linear Solver #"<< m_stater.solveCount();
	    cout()<<"| -- Zero second member -- ";
	  });
      }
  // x = 0;
  PETScInternalLinearAlgebra my_algebra(m_parallel_mng);
  my_algebra.scal(0, x);

  //m_stater.startSolveMeasure();
  m_status.succeeded = true;
  if (m_verbose==VerboseTypes::high)
  {
    alien_info([&] {
	cout()<< "| PETScSolver final state : converged " ;
      });
  }
  //m_stater.stopSolveMeasure(m_status);

  if (m_verbose==VerboseTypes::high)
    {
      alien_info([&] {
	cout()<<"| End Linear Solver";
	cout()<<"|---------------------------------------------|";
	});
    }

  return m_status.succeeded;
}

/*---------------------------------------------------------------------------*/

IInternalLinearSolver<PETScMatrix, PETScVector>*
PETScInternalLinearSolverFactory(Arccore::MessagePassing::IMessagePassingMng* p_mng,
    IOptionsPETScLinearSolver* options)
{
  return new PETScInternalLinearSolver(p_mng, options);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // namespace Alien

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/


