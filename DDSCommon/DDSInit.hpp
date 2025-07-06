/*
 * File: DDSInit.hpp
 *
 * Copyright (c) 2025 Haluk Ates
 * Licensed under the MIT License.
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

