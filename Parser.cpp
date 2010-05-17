//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// as2java - ActionScript to Java Converter
//
// Copyright (C) 2001 - 2010 Tony Richards
//
// This file is licensed under the terms of the MIT license, which can be found
// at this address: http://en.wikipedia.org/wiki/MIT_License
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 1
#define BOOST_SPIRIT_DEBUG
#endif

#include "Parser.hpp"
#include "SymbolTable.hpp"

#include "Grammar_impl.hpp"

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Parser::Parser(Class* _pClass)
:   m_pClass(_pClass)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Parser::~Parser()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Parser::parse(const boost::filesystem::path& _path)
{
    iterator_t file_first(_path.string());

    if (!file_first)
    {
        std::cout << "Unable to open file!" << std::endl;

        // TODO Throw exception?  This is a bad error and shouldn't ever happen.
        return false;
    }

    ParserState parserState(m_pClass);
    Grammar grammar(parserState);

    iterator_t file_last = file_first.make_end();

    boost::spirit::parse_info<iterator_t> info =
        boost::spirit::parse(file_first, file_last, grammar,
            // Skip Parser
            (
                    boost::spirit::blank_p
                |   boost::spirit::eol_p
                |   boost::spirit::comment_nest_p("/*", "*/") 
                |   boost::spirit::comment_p("//")
            )
        );

    if (!info.full)
    {
        SymbolTable::getInstance().parseError(m_pClass);

#ifdef BOOST_SPIRIT_DEBUG
        int nLine = 0;

        iterator_t erroriter;
        for(erroriter = info.stop; erroriter != file_first; --erroriter)
        {
            if (*erroriter == '\n')
            {
                --nLine;
            }

            if (nLine > 4)
                break;
        }

        bool bHerePrinted = false;

        for(; erroriter != file_last; ++erroriter)
        {
            if (nLine == 0 && !bHerePrinted)
            {
                std::cout << "Here --> ";
                bHerePrinted = true;
            }

            if (*erroriter == '\n')
            {
                ++nLine;
            }

            std::cout << (*erroriter);

            if (nLine > 4)
                break;
        }
#endif // 

        return false;
    }

    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

