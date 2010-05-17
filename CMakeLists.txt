CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

project(as2java)

set(Boost_ADDITIONAL_VERSIONS "1.37.0" "1.37" "1.38.0" "1.38" "1.39.0" "1.39" "1.40.0" "1.40" "1.42")
SET( Boost_USE_MULTITHREAD On )
SET( Boost_USE_STATIC_LIBS Off )
#SET( Boost_DEBUG On)

# Components that need linking (NB does not include header-only components like bind)
set(AS2JAVA_BOOST_COMPONENTS program_options filesystem system)
find_package(Boost COMPONENTS ${AS2JAVA_BOOST_COMPONENTS} QUIET)
if (NOT Boost_FOUND)
	# Try again with the other type of libs
	set(Boost_USE_STATIC_LIBS NOT ${Boost_USE_STATIC_LIBS})
	find_package(Boost COMPONENTS ${AS2JAVA_BOOST_COMPONENTS} )
endif()

set ( AS2JAVA_SRC
    Class.cpp
    ClassCollection.cpp
    ClassCollection.hpp
    Class.hpp
    Grammar.cpp
    Grammar.hpp
    Grammar_impl.hpp
    Method.cpp
    Method.hpp
    Parser.cpp
    Parser.hpp
    ParserState.cpp
    ParserState.hpp
    Symbol.cpp
    Symbol.hpp
    SymbolTable.cpp
    SymbolTable.hpp
    main.cpp
    )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${DEV_BIN}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${DEV_LIB}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${DEV_BIN}")

#add_definitions( ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} )

link_directories ( ${Boost_LIBRARY_DIRS} )

add_executable ( as2java ${AS2JAVA_SRC} )

#add_dependencies(as2java xxx)

#message("Boost libraries: ${Boost_LIBRARIES}" )

target_link_libraries( as2java ${Boost_LIBRARIES} )

set_target_properties ( as2java
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)


