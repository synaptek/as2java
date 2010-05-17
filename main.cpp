//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// as2java - ActionScript to Java Converter
//
// Copyright (C) 2001 - 2010 Tony Richards
//
// This file is licensed under the terms of the MIT license, which can be found
// at this address: http://en.wikipedia.org/wiki/MIT_License
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "ClassCollection.hpp"
#include "SymbolTable.hpp"
#include "Class.hpp"

#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/replace.hpp>

#include <boost/filesystem.hpp>

#include <boost/program_options.hpp>

#include <string>
#include <list>
#include <set>
#include <iostream>
#include <sstream>
#include <fstream>

namespace po = boost::program_options;

typedef boost::program_options::variables_map   variables_map_type;
static variables_map_type          sm_variables;

static std::string                  sm_path;
static std::string                  sm_list;

static boost::filesystem::path sm_root;

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void parseCode(const std::string& _path, const std::string& _packageName, bool _isTarget)
{
	std::cout << "Parsing: " << _path << std::endl;

    std::string leaf = _path;

    boost::iterator_range<std::string::iterator> result = boost::algorithm::find_last(leaf, ".");

    std::string extension(result.end(), leaf.end());

    // If the extension is "as", create a new class
    if (extension == "as")
    {
        boost::iterator_range<std::string::iterator> classStart = boost::algorithm::find_last(leaf, "/");

        std::string className(classStart.begin() + 1, result.begin());

        //std::string::iterator end2 = result.end();
        //end2--;

        //std::string className(leaf.begin(), end2);

        std::stringstream fullName;
        fullName << _packageName << "." << className;

        // Add the class to the collection.  This method also parses the class.
		ClassCollection::getInstance().push_back(className, fullName.str(), _packageName, _path, _isTarget);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void findCode(const boost::filesystem::path& _path, bool _isTarget)
{

    ClassCollection& classes = ClassCollection::getInstance();

    if (!exists(_path))
    {
        std::cout << "Error finding path " << _path.string() << std::endl;
        return;
    }

    if (is_directory(_path))
    {
        // Default constructor is the same as end()
        boost::filesystem::directory_iterator end_iter;

        for(boost::filesystem::directory_iterator iter(_path); iter != end_iter; iter++)
        {
            findCode(*iter, _isTarget);
        }
    }
    else
    {
        // Determine the namespace for this path by stripping out sm_root and replacing "/" with "."
        std::string path = _path.string();
        std::string root = sm_root.string();

        boost::iterator_range<std::string::const_iterator> pathStart = boost::algorithm::find_first(path, root);
        boost::iterator_range<std::string::const_iterator> pathEnd = boost::algorithm::find_last(path, "/");
        std::string packageName(pathStart.end(), pathEnd.begin());

        boost::algorithm::replace_all(packageName, "/", ".");
        boost::algorithm::replace_first(packageName, ".com.", "com.");

        parseCode(_path.string(), packageName, _isTarget);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

extern bool parseErrors(const std::string& _errors, const std::string& _dstub);

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int main(int _argc, const char* _argv[])
{
    // Parse the command line
    po::options_description description("Allowed options");

    typedef std::vector<std::string> list_type;

    description.add_options()
        ("help,h", "Produce this help message")
        ("source,s", po::value< list_type >(), "ActionScript source folder to convert to Java")
        ("dest,d", po::value<std::string>(), "Target Java output folder")
        ("logdir,o", po::value<std::string>(), "Directory to place the output logs and stats.")
        //("use", po::value<std::string>(), "[PACKAGE] Fully qualified Java class name that is required")
        //("dest", po::value<std::string>(), "[PACKAGE] Destination directory to export uses.")
    ;

    try
    {
        po::store(po::parse_command_line(_argc, (char**)_argv, description), sm_variables);
        po::notify(sm_variables);
    }
    catch(...)
    {
        std::cout << "Error parsing command line" << std::endl;
        std::cout << std::endl << description << std::endl;
        return 1;
    }

    if (sm_variables.count("help") > 0 || sm_variables.size() == 0)
    {
        std::cout << std::endl << description << std::endl;
        return 1;
    }

    // Uncomment this next line if you want to only parse files in a list (helpful in debugging grammar)
    //sm_list = "C:\\dev\\as2java\\source\\list.txt";

    if (sm_list.length())
    {
        std::ifstream input(sm_list.c_str());

        char szLine[4096];
        std::string lineData;

        while(input)
        {
            input.getline(szLine, sizeof(szLine) - 1);
            lineData = szLine;

            if (!input)
            {
                if (!input.eof())
                {
                    std::cout << "Error reading " << sm_list << std::endl;
                    return 1;
                }
            }
            else
            {
                sm_root = boost::filesystem::system_complete(boost::filesystem::path(sm_path, boost::filesystem::native)).normalize()
                    / "target" / "as_src";

                boost::filesystem::path filePath(lineData);

                findCode(filePath, false);
            }
        }

        input.close();
    }
    else
    {
        std::string dest;
        std::string logdir;

        if (sm_variables.count("dest"))
        {
            dest = sm_variables["dest"].as<std::string>();
        }

        if (sm_variables.count("logdir"))
        {
            logdir = sm_variables["logdir"].as<std::string>();
        }

        if (sm_variables.count("source"))
        {
            const list_type& sources = sm_variables["source"].as<list_type>();
            for(list_type::const_iterator iter = sources.begin(); iter != sources.end(); iter++)
            {
                sm_root = boost::filesystem::system_complete(boost::filesystem::path(*iter, boost::filesystem::native)).normalize();

                std::cout << "Processing " << sm_root.string() << "..." << std::endl;

                findCode(sm_root, false);
            }
        }

        if (logdir.length() > 0)
        {
            std::cout << "Logging to " << logdir << std::endl;
            ClassCollection::getInstance().dumpParseErrors(logdir);
            SymbolTable::getInstance().dump(logdir);
        }
        else
        {
            std::cout << "No log files written." << std::endl;
        }
    }

    std::cout << "Done!" << std::endl;

    return 0;
}
