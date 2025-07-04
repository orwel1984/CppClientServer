# Boost Configuration Helper for CMake
# This file provides functions to configure boost

function(configure_boost version components)
    string(REPLACE "." "_" BoostDir "${version}")
    set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/external/boost_${BoostDir}" CACHE STRING "Boost root directory")
    set(BOOST_INCLUDEDIR "${BOOST_ROOT}" CACHE STRING "Boost include directory")
    set(BOOST_USE_STATIC_LIBS ON CACHE STRING   "Use static Boost libraries")
    set(BOOST_NO_SYSTEM_PATHS ON CACHE STRING "Do not use system paths for Boost")
    set(BOOST_STAGE_DIR "${BOOST_ROOT}/stage" CACHE STRING "Boost stage directory")
    set(BOOST_LIBRARYDIR "${BOOST_ROOT}/stage/lib" CACHE STRING "Boost library directory")   

    # Build Boost's non header-only modules if needed
    find_missing_components("${components}" missing_libs)
    if(missing_libs)
        run_bootstrap()
        build_missing_libs("${missing_libs}")
    else()
        message(STATUS "All Boost libraries already built.")
    endif()

    message(STATUS "components: ${components}")
    find_package(Boost ${version} REQUIRED COMPONENTS ${components})

    # Export variables to parent scope
    set(Boost_LIBRARIES "${Boost_LIBRARIES}" PARENT_SCOPE)
    set(Boost_LIBRARY_DIRS "${Boost_LIBRARY_DIRS}" PARENT_SCOPE)
    set(Boost_INCLUDE_DIRS "${Boost_INCLUDE_DIRS}" PARENT_SCOPE)

endfunction()

function(print_boost_libraries BOOST_LIBS)
    message(STATUS "Boost static libraries:")
    foreach(lib ${BOOST_LIBS})
        message(STATUS "  ✓ ${lib}")
    endforeach()
endfunction()

function(find_missing_components COMPONENTS OUT_VAR)
    set(missing "")
    foreach(component IN LISTS COMPONENTS)
        string(REPLACE "-" "_" libname "${component}")
        if(NOT EXISTS "${BOOST_STAGE_DIR}/lib/libboost_${libname}.a")
            list(APPEND missing "${component}")
        endif()
    endforeach()
    set(${OUT_VAR} "${missing}" PARENT_SCOPE)
endfunction()

function(run_bootstrap)
    string(JOIN "," libs_list ${BOOST_COMPONENTS})
    message(STATUS "Running Boost bootstrap with libraries: ${libs_list}")

    # run command:
    #   ./bootstrap.sh --with-libraries=filesystem,program_options    
    execute_process(
        COMMAND ./bootstrap.sh 
            --with-libraries=${libs_list}
        WORKING_DIRECTORY ${BOOST_ROOT}
        RESULT_VARIABLE _bootstrap_result
    )
    if(NOT _bootstrap_result EQUAL 0)
        message(FATAL_ERROR "Boost bootstrap failed.")
    endif()
endfunction()

function(get_b2_args_from_components COMPONENTS OUT_VAR)
    set(_withComponents "")
    foreach(component IN LISTS COMPONENTS)
        list(APPEND _withComponents "--with-${component}")
    endforeach()
    set(${OUT_VAR} "${_withComponents}" PARENT_SCOPE)
endfunction()

function(build_missing_libs COMPONENTS)
    # Form --with- arguments
    get_b2_args_from_components("${COMPONENTS}" BOOST_B2_ARGS)

    # Build command arguments
    set(B2_COMMAND_ARGS
        link=static
        threading=multi
        variant=release
        cxxstd=${CMAKE_CXX_STANDARD}
        --debug-configuration
        --stagedir=${BOOST_STAGE_DIR}
        ${BOOST_B2_ARGS}
    )

    message(STATUS "Running Boost build command: ./b2 ${B2_COMMAND_ARGS}")

    # run B2:
    #   ./b2 <args> --with-<lib1>;--with-<lib2>    
    execute_process(
        COMMAND ./b2 ${B2_COMMAND_ARGS}
        WORKING_DIRECTORY ${BOOST_ROOT}
        RESULT_VARIABLE _b2_result
        OUTPUT_VARIABLE _b2_output
        ERROR_VARIABLE _b2_error
    )

    if(NOT _b2_result EQUAL 0)
        message(FATAL_ERROR "Boost b2 build failed.\nOutput:\n${_b2_output}\nError:\n${_b2_error}")
    endif()

endfunction()
