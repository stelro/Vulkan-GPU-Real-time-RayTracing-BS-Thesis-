/* =======================================================================
   $File: Utils.cc
   $Date: 16/11/2018	
   $Revision: 
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for 
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */

#include "Utils.hh"
#include "core.hh"

#include <random>
#include <chrono>
#include <cmath>

namespace ost {

  float Utils::getElapsedTime() noexcept {

  	static std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
  	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

  	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

  	return static_cast<float>(ms) / 1000.0f;

  }

}