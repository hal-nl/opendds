/*
 * File:   ObjectVisibility.hpp
 * Author: Haluk Ates
 *
 * C++ visibility support: It will hide most of the ELF symbols which would
 * have previously (and unnecessarily) been public.
 * See also http://gcc.gnu.org/wiki/Visibility
 *
 * Linux:
 * Use the following command via terminal to show the external symbols:
 *
 * $ nm -C -D <nameof.so>
 *
* Created on July 2025
 */

#ifndef OBJECTVISIBILITY_HPP
#define OBJECTVISIBILITY_HPP

   #if __GNUC__ >= 4
      #define DLL_PUBLIC __attribute__ ((visibility ("default")))
      #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
   #else
      #define DLL_PUBLIC
      #define DLL_LOCAL
   #endif

   #define EXTERN_DLL_FUNC extern "C" DLL_PUBLIC

#endif /* OBJECTVISIBILITY_HPP */

