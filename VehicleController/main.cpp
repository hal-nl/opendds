/*
 * File:   main.cpp
 *
 * Copyright (c) 2025 Haluk Ates
 * Licensed under the MIT License.
 *
 */


#include "helper.hpp"
#include "vehicle_controller_run.hpp"

using namespace std;
using namespace libconfig;

const string version_info = "1.0.0";

int main(int argc, char** argv)
{
   string         app_name    =  GetApplicationName(argv[0]);
   const string   default_cfg = app_name +".cfg";
   string         cfg_conf    = "";

   // Use hash and convert to string
   hash<thread::id> hasher;
   string mainThreadId = DecimalToCode64(hasher(this_thread::get_id()));

   try
   {
      // ------------------------------------------------------------------- //
      // -- Define and parse the program's command line options via boost -- //
      // ------------------------------------------------------------------- //
      namespace po = boost::program_options;

      po::options_description desc("Options");
      desc.add_options()
         ("help,h",
               app_name.c_str())
         ("configuration,c",
               po::value<string>(&cfg_conf)->default_value(default_cfg),
               "Configuration file")
         ("version,v",
               "Version info")
      ;

      po::variables_map vm;
      try
      {
         po::store(po::parse_command_line(argc, argv, desc), vm); // can throw
         if ( vm.count("help")  )
         {
            cout << desc << endl;
            return 0;
         }
         if ( vm.count("version")  )
         {
            cout << version_info << endl;
            return 0;
         }

         // Handle unrecognized options
         // throws on error, so do after help in case there are any problems
         po::notify(vm);
      }
      catch(po::error& e)
      {
         cerr << e.what() << endl << desc << endl;
         return -1;
      }

      // -------------------------------------------------- //
      // --------- application code comes here ------------ //
      // -------------------------------------------------- //
      // cout << "Using configuration file '" << cfg_conf << "' \n";

      // Read the configuration file.
      if (cfg_conf.empty())
      {
         cerr << "No configuration file given.\n" << desc << endl;
         return 0;
      }
      else if (!filesystem::exists(cfg_conf))
      {
         cerr << "Configuration file '" << cfg_conf << "' does not exist.\n";
         return EXIT_FAILURE;
      }

      try
      {
         appLogger("out  ","Starting application: " + app_name + ", version: " + version_info, mainThreadId);
         appLogger("out  ","--configuration=" + cfg_conf, mainThreadId);

         Config cfg;
         cfg.readFile(cfg_conf.c_str());
         run_controller(argc, argv, cfg);
      }
      catch (const FileIOException &fioex)
      {
         appLogger("error","I/O error while reading file.", mainThreadId);
         return EXIT_FAILURE;
      }
      catch (const ParseException &pex)
      {
         string ss;
         ss.append("Parse error at ").append(pex.getFile()).append(":").append(to_string(pex.getLine()))
            .append(" - ").append(pex.getError()).append("\n");
         appLogger("error",ss, mainThreadId);
         return EXIT_FAILURE;
      }
   }
   catch(exception& e)
   {
      appLogger("error","Unhandled Exception reached the top of main: " + string(e.what()) + ", application will now exit", mainThreadId);
      return EXIT_FAILURE;
   }

   return 0;
}
