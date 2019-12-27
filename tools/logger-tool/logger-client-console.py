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

colorama.init()

log_ids = ["TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL", "RESTART"]

timestamp = datetime.datetime

try:
    ser = serial.Serial(
        port='COM2',\
        baudrate=9600,\
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

print("connected to: " + ser.portstr)

with open('log_tokens.json') as f:
    module_record = json.load(f)

def json_record(module_id, event_id):
    for entry in module_record['log_strings']:
        if module_id == entry['module_id'] and event_id == entry['fmt_id']:
            return entry['module_name'], entry['line'], entry['fmt_str'] 

def unsign_to_sign(value, byte_size):
    max_val = (1 << (byte_size * 8))
    if (value > (max_val - 1) / 2):
        return (max_val - value) * -1
    return value

lasttik = time.perf_counter()
start_log_time = time.perf_counter()

data_recevied = False
data_collect = []
data_frame = None
logfile = open(timestamp.now().strftime('./logfiles/logfile_%H_%M_%S_%d_%m_%Y.log'), "w+") 

fmt = '%-9s%8.3f %-8s%-4s%-6s%-32s'
data_fmt = '%s%-1s'

def frame_length(byte):
    log_id = byte & 7
    
    if (log_id == log_ids.index("RESTART")):
        return 3

    if (log_id == log_ids.index("TRACE") or log_id == log_ids.index("INFO") or
        log_id == log_ids.index("WARN") or log_id == log_ids.index("FATAL")):
        return 3
    
    if (log_id == log_ids.index("DEBUG") or log_id == log_ids.index("ERROR")):
        return 4

def frame_extra_length(byte):
    data_byte_size = byte & 7
    return data_byte_size

def frame_min_trunk_time(baudrate):
    delay = 1000/baudrate
    return (delay * 3.5)

trunk = False
req_length = 40
while True:
    
    for line in ser.read():
        if (data_recevied == False):
          req_length = frame_length(line)
          
        data_recevied = True
        data_collect.append(line)
        
        if (len(data_collect) >= req_length):
            
            if req_length > 4:
                trunk = True
                break
            if req_length == 3:
                trunk = True
                break
            if req_length == 4:
                req_length += frame_extra_length(line)

        lasttik = time.perf_counter()
        
    if (((time.perf_counter() - lasttik) > (frame_min_trunk_time(ser.baudrate)) and (data_recevied == True)) or (trunk == True)):
        data_recevied = False
        trunk = False
        data_frame = data_collect
        data_collect = []
        
    if data_frame is not None:
    
        mcu_log_time = time.perf_counter() - start_log_time

        log_id = data_frame[0] & 7
        log_id_str = log_ids[log_id]

        log_time = timestamp.now().strftime("%H:%M:%S")
        
        if(log_id == log_ids.index("RESTART")):
            start_log_time = time.perf_counter()
        else:
            module_str, line_no, event_str = json_record(data_frame[1],data_frame[2])
            log_consol = (log_time, mcu_log_time, module_str, line_no, log_id_str,'--> ' + event_str )

        if (log_id == log_ids.index("TRACE") or log_id == log_ids.index("INFO") or
            log_id == log_ids.index("WARN") or log_id == log_ids.index("FATAL")):
            
            print (fmt % log_consol)
            logfile.write((fmt % log_consol) + "\n")
        
        if (log_id == log_ids.index("DEBUG") or log_id == log_ids.index("ERROR")):
            data_length = ((data_frame[0] >> 3) & 31 ) + 1
            data_bytes = (data_frame[3] & 7)
            data_sign  = (data_frame[3] >> 4) & 1
            data_hex   = (data_frame[3] >> 7) & 1
            data_char  = (data_frame[3] >> 6) & 1
            
            dataArray = []
            for data_num in range(0, data_length):
                data = 0
                for byte_num in range(0, data_bytes):
                    data = data | data_frame[byte_num + (byte_num * data_num) + 4] << (byte_num * 8)
                if data_sign:
                    data = unsign_to_sign(data, data_bytes)
                if data_hex:
                    data = hex(data)
                if data_char:
                    data = chr(data)
                dataArray.append(data)
            print (fmt % (log_consol), dataArray)
            logfile.write(data_fmt % (fmt % (log_consol) ,dataArray) + '\n')

        data_frame = None
        
logfile.close()
ser.close()