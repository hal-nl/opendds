/*
 * File:   DDSInit.hpp
 * Author: haluk ates
 *
* Created on July 2025
 */

#ifndef DDSINIT_HPP
#define DDSINIT_HPP


#include "ObjectVisibility.hpp"

#ifdef LINUX
  DLL_LOCAL void OnDLLInit(void) __attribute__((constructor));
  DLL_LOCAL void OnDLLTerm(void) __attribute__((destructor));
#else

#endif

#endif /* DDSINIT_HPP */

