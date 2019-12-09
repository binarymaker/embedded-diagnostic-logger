#!/usr/bin/env python

#*****************************************************************************
# ______  _                             ___  ___        _               
# | ___ \(_)                            |  \/  |       | |              
# | |_/ / _  _ __    __ _  _ __  _   _  | .  . |  __ _ | | __ ___  _ __ 
# | ___ \| || '_ \  / _` || '__|| | | | | |\/| | / _` || |/ // _ \| '__|
# | |_/ /| || | | || (_| || |   | |_| | | |  | || (_| ||   <|  __/| |   
# \____/ |_||_| |_| \__,_||_|    \__, | \_|  |_/ \__,_||_|\_\\___||_|   
#                                 __/ |                                 
#                                |___/                                  
#                                                                       
# Copyright (C) 2019 Binary Maker - All Rights Reserved
#
# This program and the accompanying materials are made available
# under the terms described in the LICENSE file which accompanies
# this distribution.
# Written by Binary Maker <https://github.com/binarymaker>
#*****************************************************************************

import datetime
import json
import re
import sys

from optparse import OptionParser
from os import path, walk

LOG_MODULE_PATTERN = "LOG_MODULE_DEFINE\((.*)\);"
LOG_STRING_PATTERN = "^\s*LOG_(?:Debug|Trace|Error|Warning|Info|Fatal){1}\((.*)_EVENT"

LOG_MODULES_HEADER = """/* DO NOT MODIFY - AUTO-GENERATED */
#ifndef LOG_MODULES_H
#define LOG_MODULES_H

/* Log module definitions */
"""

LOG_MODULES_FOOTER = """
#endif
"""

no_of_scan_file = 0
no_of_module_file = 0


def parser_error(filename, line, msg):
    print ("Error: In '%s' line %d" % (filename, line))
    print ("       %s" % (msg))
    sys.exit(1)


def parse_module(filename):

    global no_of_scan_file
    global no_of_module_file

    module = {
        'name': '',
        'fmt_strs': []
    }

    if not path.isfile(filename):
        return None

    root, ext = path.splitext(filename)
    if ext != '.c':
        return None
 
    no_of_scan_file += 1

    expected_name = path.basename(root)

    line_num = 1
    with open(filename, 'r') as f:
        for line in f:
            # Look for module definitions
            m = re.match(LOG_MODULE_PATTERN, line)
            if m is not None:
                if module['name'] != '':
                    parser_error(filename, line_num, "Found multiple module definitions in '%s'" % (filename))

                if len(m.groups()) != 1:
                    parser_error(filename, line_num, "Module regex error")

                module['name'] = m.groups()[0].strip()
                module['name'] = module['name'].replace('_','-')
                if module['name'] != expected_name:
                    parser_error(filename, line_num, "Expected module '%s' but got '%s'" % (expected_name, module['name'], filename))

            # Look for log strings
            m = re.match(LOG_STRING_PATTERN, line)
            if m is not None:
                if len(m.groups()) != 1:
                    parser_error(filename, line_num, "Format string regex error")

                else:
                    fmt_str = m.groups()[0]
                    if "\\\"" in line:
                        parser_error(filename, line_num, "Format string cannot contain escaped double-quotes")

                module['fmt_strs'].append((line_num, fmt_str))

            line_num += 1

    if module['name'] == '':
        return None
    else:
        no_of_module_file += 1
        print(filename)
        return module


def walker(modules, dirname, names):
    for name in names:
        filename = path.join(dirname, name)
        
        module = parse_module(filename)
        if module is not None:
            name = module['name']
            fmt_strs = module['fmt_strs']

            if len(fmt_strs) == 0:
                    print ("Error: No format strings found in '%s'" % (filename))
                    sys.exit(1)

            for m in modules:
                if name == m['name']:
                    print ("Error: Module name '%s' was found more than once" % (name))
                    sys.exit(1)

            modules.append(module)


def main(parser):
    # Parse options and arguments
    options, args = parser.parse_args()

    if len(args) != 1:
        print ("Must specify search path for C source files.\n")
        parser.print_help()
        sys.exit(1)

    # Gather the C source file modules
    modules = []
    for directory, dirnames, filenames in walk(args[0]):
        walker(modules, directory, filenames)

    modules = sorted(modules, key=lambda k:k['name'])
    # If no modules/source files were found then exit with error
    if len(modules) == 0:
        print ("Error: No C source files found.")
        sys.exit(1)

    # Create log modules header and log string CSV filesimport csv
    jsonFile   = sys.stdout if options.jsonFilename   == '-' else open(options.jsonFilename,   'w+')
    headerFile = sys.stdout if options.headerFilename == '-' else open(options.headerFilename, 'w+')

    jsonDict = {
        'date': datetime.datetime.utcnow().isoformat(),
        'firmware_version': options.fwVersion,
        'device_model': int(options.deviceModel) if options.deviceModel is not None else options.deviceModel,
        'device_submodel': int(options.deviceSubmodel) if options.deviceSubmodel is not None else options.deviceSubmodel,
        'log_strings': []
    }

    # Create header file and JSON dictionary
    headerFile.write(LOG_MODULES_HEADER)
    n = 0
    
    for module in modules:
        name = module['name']
        fmt_strs = module['fmt_strs']

        headerFile.write("#define LOG_MODULE_%-32s  (%d)\n\n" % (name.replace('-','_'), n))

        event_n = 0
        for fmt_str in fmt_strs:
            fmt_str_clean = fmt_str[1].replace(' ','')
            fmt_str_consol = fmt_str_clean.replace('_',' ')
            jsonDict['log_strings'].append({
                'module_name': name,
                'module_id': n,
                'line': fmt_str[0],
                'fmt_str': fmt_str_consol,
                'fmt_id': event_n
            })
            headerFile.write("#define %-43s  (%d)\n" % (fmt_str_clean +"_EVENT", event_n))
            event_n += 1

        n += 1
        headerFile.write("/*********************************************************/\n")
    headerFile.write(LOG_MODULES_FOOTER)

    # Write JSON to file
    jsonFile.write(json.dumps(jsonDict, sort_keys=True, indent=4, separators=(',', ': ')))

    # Close files if not stdout
    if headerFile is not sys.stdout:
        headerFile.close()
    if jsonFile is not sys.stdout:
        jsonFile.close()


if __name__ == "__main__":
    parser = OptionParser("usage: %prog [OPTIONS] <search path>")
    parser.add_option("--header",    dest="headerFilename", help="write generated log modules header to FILE", metavar="FILE", default="log_modules.h")
    parser.add_option("--fwVersion", dest="fwVersion",      help="firmware version string")
    parser.add_option("--json",      dest="jsonFilename",   help="write generated log strings JSON to FILE", metavar="FILE", default="log_strings.json")
    parser.add_option("--model",     dest="deviceModel",    help="device model")
    parser.add_option("--submodel",  dest="deviceSubmodel", help="device sub-model")

    main(parser)
    print ("SCANNED FILES : ", no_of_scan_file)
    print ("MODULE FILES  : ", no_of_module_file)
