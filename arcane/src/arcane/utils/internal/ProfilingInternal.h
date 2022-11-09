﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* ProfilingInternal.h                                         (C) 2000-2022 */
/*                                                                           */
/* Classes internes pour gérer le profilage.                                 */
/*---------------------------------------------------------------------------*/
#ifndef ARCANE_UTILS_INTERNAL_PROFILINGINTERNAL_H
#define ARCANE_UTILS_INTERNAL_PROFILINGINTERNAL_H
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

// Note: ce fichier n'est pas disponible pour les utilisateurs de Arcane.
// Il ne faut donc pas l'inclure dans un fichier d'en-tête public.

#include "arcane/utils/String.h"

#include <map>

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Arcane::impl
{

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*!
 * \brief Statistiques d'exécution d'une boucle.
 */
struct ARCANE_UTILS_EXPORT ForLoopProfilingStat
{
 public:

  //! Ajoute les infos de l'exécution \a s
  void add(const ForLoopOneExecStat& s);

  Int64 nbCall() const { return m_nb_call; }
  Int64 nbChunk() const { return m_nb_chunk; }
  Int64 execTime() const { return m_exec_time; }

 private:

  Int64 m_nb_call = 0;
  Int64 m_nb_chunk = 0;
  Int64 m_exec_time = 0;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

class ForLoopStatInfoListImpl
{
 public:

  void print(std::ostream& o);

 public:

  // TODO Utiliser un hash pour le map plutôt qu'une String pour accélérer les comparaisons
  std::map<String, ForLoopProfilingStat> m_stat_map;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // namespace Arcane::impl

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif
