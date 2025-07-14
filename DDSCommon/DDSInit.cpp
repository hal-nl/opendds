/*
 * File: DDSInit.cpp
 *
 * Copyright (c) 2025 Haluk Ates
 * Licensed under the MIT License.
 */

#include "DDSInit.hpp"

#ifdef LINUX
  /**
   * Function called when DLL is loaded
   */
  void OnDLLInit(void)
  {
    // <TODO: Do something on DLL initialize>
  }

  /**
  * Function called when DLL is terminated
  */
  void OnDLLTerm(void)
  {
    // <TODO: Do something on DLL tderminate>
  }
#else

 // dllmain.cpp : Defines the entry point for the DLL application.
  #include "stdafx.h"

  BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
  {
    switch (ul_reason_for_call)
    {
      case DLL_PROCESS_ATTACH:
      case DLL_THREAD_ATTACH:
      case DLL_THREAD_DETACH:
      case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
  }

#endif




