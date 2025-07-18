/*
 * File:   vehicle_controller.cpp
 *
 * Copyright (c) 2025 Haluk Ates
 * Licensed under the MIT License.
 *
 */


#include "vehicle_controller.hpp"

const string version_info = "1.0.0";

/**
 * @brief Use a real logger for this. This serves as an example only
 */
void appLogger(const string& typ, const string& msg, const string& threadId)
{
   std::lock_guard<std::mutex> lock(std::mutex);
   string logMessage = fmt::format("[{:s}][{:s}] {:s}", typ, threadId, msg);
   cout <<  logMessage << endl;
}

/**
 * @brief Get the Application Name of the current application
 *
 * @param name
 * @return string
 */
string GetApplicationName(const char *name)
{
   std::lock_guard<std::mutex> lock(std::mutex);
   std::filesystem::path p(name);
   string application_name = p.stem().string();
   return application_name;
}

/**
 * @brief Decimal to hex code
 *
 * @param decimal
 * @return string
 */
string DecimalToCode64(size_t decimal)
{
   std::lock_guard<std::mutex> lock(std::mutex);
   const char *hexDigits = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ@$";
   string hexValue;

   do
   {
      hexValue = hexDigits[decimal % 64] + hexValue;
      decimal /= 64;
   } while (decimal > 0);

   return hexValue;
}

/**
 * @brief Get the file contents
 *
 * @param fullFileName
 * @return string
 */
string GetContents(const string &fullFileName)
{
   std::ifstream f(fullFileName.c_str());
   if (f)
   {
      string data;
      f.seekg(0, std::ios::end);
      data.resize(f.tellg());
      f.seekg(0);
      f.read(data.data(), data.size());
      return data;
   }
   return "";
}

/**
 * @brief Check if directories exist, else create
 *
 * @param dirlist : list of directories to check
 * @return int
 */
int check_directories(const vector<string>& dirlist, const string& threadId)
{
   // -----------------------------------------------------------------
   // Check input dir/ output dir and create if not exists
   // -----------------------------------------------------------------
   for(auto d : dirlist )
   {
      string fullPath = filesystem::path(d);
      if (!filesystem::exists(fullPath))
      {
         // Folder does not exist, create it
         if (filesystem::create_directories(fullPath))
         {
            appLogger("info ","Folder created successfully: " + fullPath, threadId);
         }
         else
         {
            appLogger("error","Failed to create folder: " + fullPath, threadId);
            return -1;
         }
      }
      else
      {
         appLogger("info ","Folder already exists: " + fullPath, threadId);
      }
   }
   return 0;
}


int main(int argc, char** argv)
{
   string         app_name    =  GetApplicationName(argv[0]);
   const string   default_cfg = app_name +".cfg";
   string         cfg_conf    = "";

   // Use hash and convert to string
   std::hash<std::thread::id> hasher;
   string mainThreadId = DecimalToCode64(hasher(std::this_thread::get_id()));

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
      else if (!std::filesystem::exists(cfg_conf))
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
   catch(std::exception& e)
   {
      appLogger("error","Unhandled Exception reached the top of main: " + string(e.what()) + ", application will now exit", mainThreadId);
      return EXIT_FAILURE;
   }

   return 0;
}
