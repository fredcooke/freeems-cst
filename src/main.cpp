/**
 * @file       main.cpp
 * @headerfile main.h
 * @author     sean
 * @brief      freeems-cst entry point
 *
 * freeems-cst: freeems 'comms smoke test'
 *
 * Copyright (C) 2010 Sean Stasiak sstasiak at gmail
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "main.h"

/* boost::program_options */
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
namespace po = boost::program_options;

/* std:: */
#include <string>
#include <iostream>

int
main( int argc,
      char *argv[] )
{
    const std::string caption( "freeems-cst" );
    po::options_description od( caption, 80 );
    od.add_options()
        ( "help", "show help" )
    ;

    po::variables_map vm;
    po::store( po::parse_command_line( argc, argv, od ) , vm );
    po::notify( vm );

    const po::variable_value help = vm["help"];
    if( !help.empty() )     /**< someone would like to see help */
    {
        std::cout << od;
        return 0;
    }

    return 0;   /**< TODO: implement !0 on failure for future automation plans */
}

