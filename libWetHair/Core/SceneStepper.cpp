//
// This file is part of the libWetHair open source project
//
// The code is licensed solely for academic and non-commercial use under the
// terms of the Clear BSD License. The terms of the Clear BSD License are
// provided below. Other licenses may be obtained by contacting the faculty
// of the Columbia Computer Graphics Group or a Columbia University licensing officer.
//
// The Clear BSD License
//
// Copyright 2017 Yun (Raymond) Fei, Henrique Teles Maia, Christopher Batty,
// Changxi Zheng, and Eitan Grinspun
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted (subject to the limitations in the disclaimer
// below) provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//  list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation
//  and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its contributors may be used
//  to endorse or promote products derived from this software without specific
//  prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS
// LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
// OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.

#include "SceneStepper.h"

template<int DIM>
SceneStepper<DIM>::~SceneStepper()
{}

template<int DIM>
const VectorXs& SceneStepper<DIM>::getAcceleration() const
{
  return m_a;
}

template<int DIM>
void SceneStepper<DIM>::accept( TwoDScene<DIM> & scene, scalar dt )
{
  VectorXs& x = scene.getX();
  VectorXs& v = scene.getV();
  
  v = (m_next_x - x) / dt;
  x = m_next_x;
  if( scene.getParameter().no_fictitious ) {
    SceneStepper<DIM>::m_a.setZero();
  } else {
    SceneStepper<DIM>::m_a = (v - m_old_v) / dt;
  }
  m_old_v = v;
    
  mathutils::check_isnan("SIM: v", v);
  mathutils::check_isnan("SIM: x", x);
}

template<int DIM>
void SceneStepper<DIM>::setNextX( const VectorXs& nextx )
{
  m_next_x = nextx;
}

template<int DIM>
const VectorXs& SceneStepper<DIM>::getNextX() const
{
  return m_next_x;
}

template<int DIM>
void SceneStepper<DIM>::PostStepScene( TwoDScene<DIM> & scene, scalar dt )
{
  scene.postCompute(dt);
}

template<int DIM>
void SceneStepper<DIM>::init( TwoDScene<DIM> & scene )
{
  m_old_v = scene.getV();
  m_a.resize(m_old_v.size());
  m_a.setZero();
}

template<int DIM>
const std::vector<scalar>& SceneStepper<DIM>::getTimingStatistics() const
{
  return m_timing_statistics;
}

template<int DIM>
void SceneStepper<DIM>::write(std::vector<scalar>&) const {};

template<int DIM>
void SceneStepper<DIM>::read(const scalar* data) {};

template<int DIM>
size_t SceneStepper<DIM>::size() {return 0;};

// explicit instantiations at bottom
template class SceneStepper<2>;
template class SceneStepper<3>;
