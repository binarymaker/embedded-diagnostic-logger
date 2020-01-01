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



import sys
import serial
import serial.tools.list_ports
import time
import datetime
import json
import colorama
import crc8
from optparse import OptionParser

log_ids = ["TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL", "START"]
crc = crc8.crc8()

class frame:
    LOG_ID = 1
    MODULE_ID = 2
    EVENT_ID = 3
    DATATYPE = 4
    DATA_LENGTH = 5
    DATA = 6
    CRC = 6
    
    list_of_frames = []
    
    def __init__(self):
        with open('log_tokens.json') as f:
            self.module_record = json.load(f)
            self.is_decoded = True

    def isFrameBegin(self, raw_frame):
        if (raw_frame[0] == ord('#')):
            return True
        else:
            return False

    def findFrameLength(self, raw_frame):
        if (raw_frame[1] == log_ids.index("TRACE") or
            raw_frame[1] == log_ids.index("INFO")  or
            raw_frame[1] == log_ids.index("WARN")  or
            raw_frame[1] == log_ids.index("FATAL") or
            raw_frame[1] == log_ids.index("START")):
            return 6
        if (raw_frame[1] == log_ids.index("DEBUG") or
            raw_frame[1] == log_ids.index("ERROR") ):
            return 8
    
    def findDataLength(self, raw_frame):
        return ((raw_frame[self.DATATYPE] & 0x07) * raw_frame[self.DATA_LENGTH])

    def catchFrame(self, line):
        trunk_length = self.findFrameLength(line)
        trunk_line = line[ : trunk_length]
        del line[:trunk_length]
        self.list_of_frames.append(trunk_line)

    def moduleRecords(self, module_id, event_id):
        for entry in self.module_record['log_strings']:
            if module_id == entry['module_id'] and event_id == entry['fmt_id']:
                self.event_module = entry['module_name']
                self.event_line = entry['line']
                self.event_string = entry['fmt_str']

    def decode(self, frame):
        if(self.crcVerify(frame)):
            self.event_log_level = log_ids[frame[1]]
            self.moduleRecords(frame[self.MODULE_ID], frame[self.EVENT_ID])
            
            if (frame[1] == log_ids.index("DEBUG") or
                frame[1] == log_ids.index("ERROR") ):
                self.debug_data = self.debug_data_extract(frame)
            else:
                self.debug_data = []
        else:
            print("CRC ERROR")

    def debug_data_extract(self,frame):
        data_bytes = frame[self.DATATYPE] & 0x07
        data_length = frame[self.DATA_LENGTH]
        data_array = []
        for data_num in range(0, data_length):
            data = 0
            for byte_num in range(0, data_bytes):
                data = data | frame[byte_num + (byte_num * data_num) + self.DATA] << (byte_num * 8)
        data_array.append(data)
        return (data_array)

    def crcVerify(self,frame):
        data = frame[1:-1]
        crc.calculate(data)
        if(crc.result() == 0):
            return True
        else:
            return False

colorama.init()

def frame_gap_time(baudrate):
    delay = 1000/baudrate
    return (delay * 3.5)

def main(options):
    global log_ids

    try:
        ser = serial.Serial(
            port=options.port,\
            baudrate=options.baudrate,\
            parity=serial.PARITY_NONE,\
            stopbits=serial.STOPBITS_ONE,\
            bytesize=serial.EIGHTBITS,\
                timeout=0)
    except:
        print ("Please check COM port ...")
        ports = list(serial.tools.list_ports.comports())
        for com in ports:
            print (com)
        
        sys.exit(1)

    print("connected to: " + ser.portstr, "Log level: " + options.loglevel)

    timestamp = datetime.datetime 
    logfile = open(timestamp.now().strftime('./logfiles/logfile_%Y_%m_%d_%H_%M_%S_.log'), "w+")
    
    logfile.write("Log level: " + options.loglevel + "\n")
    serial_raw_data = []
    serial_data = None
    data_count = 0

    last_tick = time.perf_counter()
    start_log_time = time.perf_counter()
    frame_length = 255
    
    start_log_time = time.perf_counter() 

    fmt = '%-9s%8.3f %-8s%-4s%-6s%-32s'
    data_fmt = '%s%-1s'

    decoder = frame()
    
    try:
        while True:
            for data in ser.read():
                serial_raw_data.append(data)

                if (decoder.isFrameBegin(serial_raw_data) and len(serial_raw_data) == 2):
                    frame_length = decoder.findFrameLength(serial_raw_data)
                
                if(frame_length == 8 and len(serial_raw_data) == 6):
                    frame_length += decoder.findDataLength(serial_raw_data)

                last_tick = time.perf_counter()

            if((((time.perf_counter() - last_tick) > 0.5) and 
                len(serial_raw_data) > 0) 
                or
                ((frame_length == len(serial_raw_data)))):

                last_tick = time.perf_counter()
                serial_data = serial_raw_data
                serial_raw_data = []
                new_data = False
            
            if(serial_data is not None):
                mcu_log_time = time.perf_counter() - start_log_time

                log_time = timestamp.now().strftime("%H:%M:%S")
                decoder.decode(serial_data)
                
                if(decoder.event_log_level == "START"):
                    start_log_time = time.perf_counter()
                else:
                    if (log_ids.index(decoder.event_log_level) >= log_ids.index(options.loglevel)):
                        log_consol = (log_time, 
                                        mcu_log_time, 
                                        decoder.event_module, 
                                        decoder.event_line, 
                                        decoder.event_log_level,
                                        '--> ' + decoder.event_string)

                        if(len(decoder.debug_data) == 0):
                            print (fmt % log_consol)
                            logfile.write((fmt % log_consol) + "\n")
                        else:
                            print (fmt % (log_consol), decoder.debug_data)
                            logfile.write(data_fmt % (fmt % (log_consol) ,decoder.debug_data) + '\n')

                serial_data = None

    except KeyboardInterrupt:
        ser.close()
        logfile.close()

if __name__ == "__main__":
    parser = OptionParser("usage: --com <COM_ID> --baud <9600> --level <TRACE>")
    parser.add_option("-c", "--com",   dest="port",     help="serial port")
    parser.add_option("-b", "--baud",  dest="baudrate", help="baudrate", default="9600")
    parser.add_option("-l", "--level",  dest="loglevel", help="loglevel", default="TRACE")

     # Parse options and arguments
    options, args = parser.parse_args()

    if not options.port:
        parser.error("com port required")
    
    if options.loglevel not in log_ids:
        print("log levels tags:", log_ids)
        parser.error("correct log level tags required")
        
    main(options)
    print("*"*20)
    print("log application closed")
