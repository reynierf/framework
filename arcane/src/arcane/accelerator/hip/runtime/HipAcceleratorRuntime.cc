﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* HipAcceleratorRuntime.cc                                    (C) 2000-2021 */
/*                                                                           */
/* Runtime pour 'HIP'.                                                       */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include "arcane/accelerator/hip/HipAccelerator.h"

#include "arcane/utils/PlatformUtils.h"
#include "arcane/utils/Array.h"
#include "arcane/utils/TraceInfo.h"
#include "arcane/utils/NotSupportedException.h"
#include "arcane/utils/FatalErrorException.h"

#include "arcane/accelerator/core/RunQueueBuildInfo.h"

#include "arcane/accelerator/AcceleratorGlobal.h"
#include "arcane/accelerator/IRunQueueRuntime.h"
#include "arcane/accelerator/IRunQueueStream.h"
#include "arcane/accelerator/RunCommand.h"

#include <iostream>

using namespace Arccore;

namespace Arcane::Accelerator::Hip
{

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

void checkDevices()
{
  int nb_device = 0;
  ARCANE_CHECK_HIP(hipGetDeviceCount(&nb_device));
  std::ostream& o = std::cout;
  o << "Initialize Arcane CUDA runtime\n";
  o << "Available device = " << nb_device << "\n";
  for( int i=0; i<nb_device; ++i ){
    hipDeviceProp_t dp;
    ARCANE_CHECK_HIP(hipGetDeviceProperties(&dp, i));

    o << "\nDevice " << i << " name=" << dp.name << "\n";
    o << " computeCapability = " << dp.major << "." << dp.minor << "\n";
    o << " totalGlobalMem = " << dp.totalGlobalMem << "\n";
    o << " sharedMemPerBlock = " << dp.sharedMemPerBlock << "\n";
    o << " regsPerBlock = " << dp.regsPerBlock << "\n";
    o << " warpSize = " << dp.warpSize<< "\n";
    o << " memPitch = " << dp.memPitch<< "\n";
    o << " maxThreadsPerBlock = " << dp.maxThreadsPerBlock<< "\n";
    o << " totalConstMem = " << dp.totalConstMem<< "\n";
    o << " clockRate = " << dp.clockRate<< "\n";
    //o << " deviceOverlap = " << dp.deviceOverlap<< "\n";
    o << " multiProcessorCount = " << dp.multiProcessorCount<< "\n";
    o << " kernelExecTimeoutEnabled = " << dp.kernelExecTimeoutEnabled<< "\n";
    o << " integrated = " << dp.integrated<< "\n";
    o << " canMapHostMemory = " << dp.canMapHostMemory<< "\n";
    o << " computeMode = " << dp.computeMode<< "\n";
    o << " maxThreadsDim = "<< dp.maxThreadsDim[0] << " " << dp.maxThreadsDim[1]
      << " " << dp.maxThreadsDim[2] << "\n";
    o << " maxGridSize = "<< dp.maxGridSize[0] << " " << dp.maxGridSize[1]
      << " " << dp.maxGridSize[2] << "\n";
  }
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

class HipRunQueueStream
: public impl::IRunQueueStream
{
 public:
  HipRunQueueStream(impl::IRunQueueRuntime* runtime,const RunQueueBuildInfo& bi)
  : m_runtime(runtime)
  {
    if (bi.isDefault())
      ARCANE_CHECK_HIP(hipStreamCreate(&m_hip_stream));
    else{
      int priority = bi.priority();
      ARCANE_CHECK_HIP(hipStreamCreateWithPriority(&m_hip_stream,hipStreamDefault,priority));
    }
  }
  ~HipRunQueueStream() noexcept(false) override
  {
    ARCANE_CHECK_HIP(hipStreamDestroy(m_hip_stream));
  }
 public:
  void notifyBeginKernel([[maybe_unused]] RunCommand& c) override
  {
    return m_runtime->notifyBeginKernel();
  }
  void notifyEndKernel(RunCommand&) override
  {
    return m_runtime->notifyEndKernel();
  }
  void barrier() override
  {
    ARCANE_CHECK_HIP(hipStreamSynchronize(m_hip_stream));
  }
  void* _internalImpl() override { return &m_hip_stream; }
 private:
  impl::IRunQueueRuntime* m_runtime;
  hipStream_t m_hip_stream;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

class HipRunQueueRuntime
: public impl::IRunQueueRuntime
{
 public:
  ~HipRunQueueRuntime() override = default;
 public:
  void notifyBeginKernel() override
  {
    ++m_nb_kernel_launched;
    if (m_is_verbose)
      std::cout << "BEGIN HIP KERNEL!\n";
  }
  void notifyEndKernel() override
  {
    ARCANE_CHECK_HIP(hipGetLastError());
    if (m_is_verbose)
      std::cout << "END HIP KERNEL!\n";
  }
  void barrier() override
  {
    ARCANE_CHECK_HIP(hipDeviceSynchronize());
  }
  eExecutionPolicy executionPolicy() const override
  {
    return eExecutionPolicy::HIP;
  }
  impl::IRunQueueStream* createStream(const RunQueueBuildInfo& bi) override
  {
    return new HipRunQueueStream(this,bi);
  }
 private:
  Int64 m_nb_kernel_launched = 0;
  bool m_is_verbose = false;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // End namespace Arcane::Accelerator::Hip

namespace 
{
Arcane::Accelerator::Hip::HipRunQueueRuntime global_hip_runtime;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

// Cette fonction est le point d'entrée utilisé lors du chargement
// dynamique de cette bibliothèque
extern "C" ARCANE_EXPORT void
arcaneRegisterAcceleratorRuntimehip()
{
  using namespace Arcane::Accelerator::Hip;
  Arcane::Accelerator::impl::setUsingHIPRuntime(true);
  Arcane::Accelerator::impl::setHIPRunQueueRuntime(&global_hip_runtime);
  Arcane::platform::setAcceleratorHostMemoryAllocator(getHipMemoryAllocator());
  checkDevices();
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
