﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2024 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* MeshTimeHistoryAdder.cc                                     (C) 2000-2024 */
/*                                                                           */
/* Classe permettant d'ajouter un historique de valeur lié à un maillage.    */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include "arcane/core/MeshTimeHistoryAdder.h"
#include "arcane/core/internal/ITimeHistoryMngInternal.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Arcane
{

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

MeshTimeHistoryAdder::
MeshTimeHistoryAdder(ITimeHistoryMng* thm, IParallelMng* pm, const MeshHandle& mesh_handle)
: m_thm(thm)
, m_pm(pm)
, m_mesh_handle(mesh_handle)
{}

void MeshTimeHistoryAdder::
addValue(const TimeHistoryAddValueArg& thp, Real value)
{
  if(!thp.isLocal() || thp.localProcId() == m_pm->commRank()) {
    m_thm->_internalApi()->addValue(TimeHistoryAddValueArgInternal(thp, m_mesh_handle), value);
  }
}

void MeshTimeHistoryAdder::
addValue(const TimeHistoryAddValueArg& thp, Int64 value)
{
  if(!thp.isLocal() || thp.localProcId() == m_pm->commRank()) {
    m_thm->_internalApi()->addValue(TimeHistoryAddValueArgInternal(thp, m_mesh_handle), value);
  }
}

void MeshTimeHistoryAdder::
addValue(const TimeHistoryAddValueArg& thp, Int32 value)
{
  if(!thp.isLocal() || thp.localProcId() == m_pm->commRank()) {
    m_thm->_internalApi()->addValue(TimeHistoryAddValueArgInternal(thp, m_mesh_handle), value);
  }
}

void MeshTimeHistoryAdder::
addValue(const TimeHistoryAddValueArg& thp, RealConstArrayView values)
{
  if(!thp.isLocal() || thp.localProcId() == m_pm->commRank()) {
    m_thm->_internalApi()->addValue(TimeHistoryAddValueArgInternal(thp, m_mesh_handle), values);
  }
}

void MeshTimeHistoryAdder::
addValue(const TimeHistoryAddValueArg& thp, Int32ConstArrayView values)
{
  if(!thp.isLocal() || thp.localProcId() == m_pm->commRank()) {
    m_thm->_internalApi()->addValue(TimeHistoryAddValueArgInternal(thp, m_mesh_handle), values);
  }
}

void MeshTimeHistoryAdder::
addValue(const TimeHistoryAddValueArg& thp, Int64ConstArrayView values)
{
  if(!thp.isLocal() || thp.localProcId() == m_pm->commRank()) {
    m_thm->_internalApi()->addValue(TimeHistoryAddValueArgInternal(thp, m_mesh_handle), values);
  }
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // End namespace Arcane

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
