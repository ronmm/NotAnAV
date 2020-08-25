import os
import subprocess
import threading
import time

COMMAND = 0
TIME = -1


class CONNECT:
    NAME = b"CONNECT"
    ADDRESS = 1
    PORT = 2
    PID = 3


class EXECVE:
    NAME = b"EXECVE"
    PATH = 1
    ARGV = 2
    PID = 3


class CHMOD:
    NAME = b"CHMOD"
    PATH = 1
    MODE = 2
    PID = 3


def get_all_events(user_mode_module, data_path):
    # TODO: Call the hook C module (usermode one) and check that it started correctly
    subprocess.Popen(user_mode_module)

    time.sleep(30)

    with open(data_path, 'rb') as data_file:
        data = data_file.read()

    data_lines = data_to_lines(data)

    return data_lines


def convert_ip_address(line):

    if line[COMMAND] != CONNECT.NAME:
        return

    addr = strip_field_name(line[CONNECT.ADDRESS])
    addr = addr.rjust(8, b'0')

    octets = []
    i = 0

    for index in range(0,4):
        octets.append(int('0x{}'.format(addr[i:i+2].decode()), 16))
        i += 2

    return '{}.{}.{}.{}'.format(octets[0], octets[1], octets[2], octets[3]).encode()


def strip_data(data):
    return data.replace(b'"', b'').replace(b'\'', b'')


def strip_field_name(field):
    if b'=' in field:
        return field.split(b'=')[1]

    return field


def data_to_lines(data):
    lines = data.split(b'\n')
    final_lines = []

    for line in lines:
        split_line = []

        for field in strip_data(line).split(b','):
            split_line.append(strip_field_name(field))

        if split_line[COMMAND] == CONNECT.NAME:
            split_line[CONNECT.ADDRESS] = convert_ip_address(split_line)

        final_lines.append(split_line)

    return final_lines


def clear_data_file(data_path):
    t = threading.Thread(target=clear_data, args=(data_path,))
    t.start()


def clear_data(data_path):
    while True:
        time.sleep(5*60)

        if os.path.exists(data_path):
            os.remove(data_path)
