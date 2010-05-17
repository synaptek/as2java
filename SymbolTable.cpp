//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// as2java - ActionScript to Java Converter
//
// Copyright (C) 2001 - 2010 Tony Richards
//
// This file is licensed under the terms of the MIT license, which can be found
// at this address: http://en.wikipedia.org/wiki/MIT_License
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "SymbolTable.hpp"
#include "Class.hpp"
#include "Parser.hpp"
#include "Symbol.hpp"
#include "Method.hpp"
#include "ClassCollection.hpp"

#include <boost/filesystem.hpp>

#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/replace.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SymbolTable::SymbolTable()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SymbolTable::~SymbolTable()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SymbolTable&
SymbolTable::getInstance()
{
    static SymbolTable sm_instance;
    return sm_instance;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SymbolTable::push_back(Symbol* _pSymbol)
{
    m_mapNameToSymbol.insert(map_name_to_symbol_pair_type(_pSymbol->getName(), _pSymbol));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SymbolTable::parseError(Class* _pClass)
{
    m_parseErrorList[_pClass] = "";
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SymbolTable::findSymbols(const std::string& _symbol, bool _method, std::list<Symbol*>& _symbols)
{
    map_name_to_symbol_type::iterator iter = m_mapNameToSymbol.find(_symbol);

    if (iter == m_mapNameToSymbol.end())
        return;


    map_name_to_symbol_type::iterator lastElement = m_mapNameToSymbol.upper_bound(_symbol);

    for(; iter != lastElement; iter++)
    {
        if (_method)
        {
            if (iter->second->getSymbolType() == Symbol::METHOD && iter->second->isStatic())
            {
                _symbols.push_back(iter->second);
            }
        }
        else
        {
            _symbols.push_back(iter->second);
        }
    }

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SymbolTable::deferCategoryHandler(const Class* const _pBaseClass, const Class* const _pClass)
{
    m_handlersToGenerate[_pBaseClass].push_back(_pClass);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SymbolTable::generateCategoryHandlers(const boost::filesystem::path& _stubPath)
{
    for(handler_class_map::iterator iter = m_handlersToGenerate.begin(); iter != m_handlersToGenerate.end(); iter++)
    {
        /// Base category class
        const Class* const pBaseClass = iter->first;

        // A list (possibly some duplicates!) of all of the methods
        Class::method_list_type allMethods;

        // Copy all of the methods from the base class to the allMethods list
        const Class::method_list_type& baseMethods = pBaseClass->getMethods();
        for(Class::method_list_type::const_iterator methodIter = baseMethods.begin(); methodIter != baseMethods.end(); methodIter++)
        {
            allMethods.push_back(*methodIter);
        }

        // Get all of the methods from all of the category classes
        for(category_class_list::iterator pClassList = iter->second.begin(); pClassList != iter->second.end(); pClassList++)
        {
            /// Class that needs the handler 
            const Class* const pClass = *pClassList;

            const Class::method_list_type& categoryMethods = pClass->getMethods();
            for(Class::method_list_type::const_iterator methodIter = categoryMethods.begin(); methodIter != categoryMethods.end(); methodIter++)
            {
                allMethods.push_back(*methodIter);
            }

            const Class* pSuperClass = pClass->getSuperClass();
            while(pSuperClass != NULL)
            {
                if (strcmp(pSuperClass->getClassName().c_str(), "EOGenericRecord") == 0)
                {
                    break;
                }

                const Class::method_list_type& superMethods = pSuperClass->getMethods();
                for(Class::method_list_type::const_iterator methodIter = superMethods.begin(); methodIter != superMethods.end(); methodIter++)
                {
                    allMethods.push_back(*methodIter);
                }

                // Traverse up the class heirarchy
                pSuperClass = pSuperClass->getSuperClass();
            }
        }

        // The new package is the same as the base class package plus ".stubs"
        std::string newPackageName = pBaseClass->getPackageName() + ".stubs";

        // The new directory name is the "javaport" root path plus "stubs" the package name, 
        // except replace the "." with "/" then normalize it.
        std::string newDirectoryName = _stubPath.string() + std::string("/") + newPackageName;
        boost::algorithm::replace_all(newDirectoryName, "//", "/");
        boost::algorithm::replace_all(newDirectoryName, ".", "/");
        boost::filesystem::path newDirectory(newDirectoryName);
        newDirectory.normalize();

        // Create the directories
        boost::filesystem::create_directories(newDirectory);

        std::string baseClassName(pBaseClass->getClassName());

        std::string missingClass(pBaseClass->getClassName());
        missingClass += "Delegate";

        // Create the filename
        boost::filesystem::path newFileName = newDirectory / (missingClass + ".java");
        newFileName.normalize();

        // Generate a Delegate class
        std::ofstream output(newFileName.string().c_str(), std::ios::trunc);

        // Standard header
        output << "/*~ START HEADER ~*/" << std::endl;
        output << "// todo: insert std copyright/disclaimer goes here" << std::endl;
        output << std::endl;

        // Package
        output << "// Generated delegate stub" << std::endl;
        output << "package " << newPackageName << ";" << std::endl;
        output << std::endl;

        // Standard imports
        output << "import java.util.*;" << std::endl;
        output << "import java.io.*;" << std::endl;
        output << "import java.math.*;" << std::endl;
        output << std::endl;
        output << "import com.webobjects.foundation.*;" << std::endl;
        output << "import com.webobjects.eocontrol.*;" << std::endl;
        output << "import com.webobjects.eoaccess.*;" << std::endl;
        output << "import com.webobjects.appserver.*;" << std::endl;
        output << std::endl;
        output << "import com.tc.migration.*;" << std::endl;
        output << "import com.tc.stubs.*;" << std::endl;
        output << "import static com.tc.migration.DBPROCESS.*;" << std::endl;
        output << "import static com.tc.migration.JCGlobals.*;" << std::endl;
        output << std::endl;
        output << "/*~ END HEADER ~*/" << std::endl;
        output << "import " << pBaseClass->getFullName() << ";" << std::endl;

        // Get all of the methods from all of the category classes
        for(category_class_list::iterator pClassList = iter->second.begin(); pClassList != iter->second.end(); pClassList++)
        {
            /// Class that needs the handler 
            const Class* const pClass = *pClassList;
            output << "import " << pClass->getFullName() << ";" << std::endl;
        }

        output << std::endl << std::endl << std::endl;


        output << "public class " << missingClass << " extends EOGenericRecord {" << std::endl;

        // Member variables
        output << "    private " << baseClassName << " delegate;" << std::endl << std::endl;

        // Output the constructor
        output << "    public " << missingClass << "(" << baseClassName << " _delegate) {" << std::endl;
        output << "        delegate = _delegate;" << std::endl;
        output << "    }" << std::endl << std::endl;

        // Output the default constructor
        output << "    public " << missingClass << "() {" << std::endl;
        output << "        delegate = null;" << std::endl;
        output << "    }" << std::endl << std::endl;

        // Output the delegate setter
        output << "    public void setDelegate(" << baseClassName << " _delegate) {" << std::endl;
        output << "        delegate = _delegate;" << std::endl;
        output << "    }" << std::endl << std::endl;

        // Output all of the methods
        for(Class::method_list_type::const_iterator methodIter = allMethods.begin(); methodIter != allMethods.end(); methodIter++)
        {
            // TODO Skip non-public methods
            if (!(*methodIter)->isPublic())
            {
                // Not skipping for now; 
                // Public methods aren't being marked public correctly right now
                //continue;
            }

            // Skip static methods
            if ((*methodIter)->isStatic())
            {
                continue;
            }

            const Method::parm_list_type& parms = (*methodIter)->getParameters();

            // Output the method signature
            output << "    public " << (*methodIter)->getType() << " " << (*methodIter)->getName() << "(";

            // Iterate through the parameters and output them
            bool firstParm = true;
            for(Method::parm_list_type::const_iterator parmIter = parms.begin(); parmIter != parms.end(); parmIter++)
            {
                if (!firstParm)
                {
                    output << ", ";
                }
                else
                {
                    firstParm = false;
                }

                output << (*parmIter).first << " " << (*parmIter).second;
            }

            output << ") {" << std::endl;

            // Output calling of the delegate
            output << "        throw new RuntimeException();" << std::endl;

#if 0
            output << "        ";
            if (strcmp((*methodIter)->getType().c_str(), "void") != 0)
            {
                output << "return ";
            }
            output << "delegate." << (*methodIter)->getName() << "(";
            firstParm = true;
            for(Method::parm_list_type::const_iterator parmIter = parms.begin(); parmIter != parms.end(); parmIter++)
            {
                if (!firstParm)
                {
                    output << ", ";
                }
                else
                {
                    firstParm = false;
                }

                output << (*parmIter).second;
            }
            output << ");" << std::endl;
#endif 
            // End of the method
            output << "    }" << std::endl << std::endl;

        }

        output << "}" << std::endl << std::endl;

        output.close();

        // Get the fully qualified path name
        std::stringstream fullName;
        fullName << newPackageName << "." << missingClass;

#if 0 // No need to do this because it's already too late
        // Create a new class and parse it so it will be added to the symbol table.
        ClassCollection::getInstance().push_back(missingClass, fullName.str(), newPackageName, newFileName.string(), false);

        // Now, if we're lucky, we'll find the class and we can add it to the required imports.
        SymbolTable::getInstance().findSymbols(missingClass, symbols);

        if (symbols.size() == 1)
        {
            _pClass->addSymbolImport(symbols.front());
        }
        else
        {
            stats.otherErrors(missingClass);
        }
#endif
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SymbolTable::dump(const std::string& _basePath)
{
    {
        boost::filesystem::path path(_basePath);
        path /= "symbols.txt";
        path.normalize();

        std::ofstream out(path.string().c_str(), std::ios::trunc);

        out << "className,symbolName,public (1/0),static (1/0),type (v = variable, m = method)" << std::endl;

        for(map_name_to_symbol_type::iterator iter = m_mapNameToSymbol.begin(); iter != m_mapNameToSymbol.end(); iter++)
        {
            out << iter->second->getClass()->getFullName() << "," << iter->second->getName() << ","
                << (iter->second->isPublic() ? "1" : "0") << "," << (iter->second->isStatic() ? "1" : "0") << ",";

            Symbol::SymbolType symbolType = iter->second->getSymbolType();
            if (symbolType == Symbol::VARIABLE)
            {
                out << "v";
            }
            else
            {
                out << "m";
            }

            out << std::endl;
        }

        out.close();
    }

    {
        boost::filesystem::path path(_basePath);
        path /= "fr4linkage_ParseErrors.txt";
        path.normalize();

        std::ofstream out(path.string().c_str(), std::ios::trunc);

        for(parse_error_list::iterator iter = m_parseErrorList.begin(); iter != m_parseErrorList.end(); iter++)
        {
            out << iter->first->getFullName() <<  std::endl;
        }

        out.close();
    }
}

