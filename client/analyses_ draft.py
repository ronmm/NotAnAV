import sys
import re
import time
import subprocess

from client.analyses_utils import parse_wget, get_commands_within_time_frame, write_log
from client.notanav import get_all_events, CONNECT, EXECVE, COMMAND, TIME, clear_data_file

RECON_COMMANDS = [b'uname -a', b'netstat -noa', b'cat /etc/passwd', b'ps -fade', b'arp -a']
URL_IDENTIFIERS = [b'http://', b'https://', b'ftp://', b'smb://']
USER_MODE_MODULE = sys.argv[1]
DATA_PATH = sys.argv[2]

def get_events(event_list, event_commands):
    """
    Return a list of all event commands in raw events
    """
    events = []
    for row in event_list:
        for field in row:
            if any(word in field for word in event_commands):
                events.append(row)
    return events
    

def main():
    clear_data_file(DATA_PATH)
    subprocess.Popen(USER_MODE_MODULE)

    while True:
        try:
            event_list = get_all_events(DATA_PATH)

            identify_download_and_execute(event_list)

            recon_sequences = []
            recon_events = get_events(event_list, RECON_COMMANDS)

            for event in recon_events:
                result = get_commands_within_time_frame(recon_events, EXECVE.NAME, event[TIME])

                if len(result) < 3:
                    continue

                if result not in recon_sequences:
                    recon_sequences.append(result)

            if recon_sequences:
                write_log(str(recon_sequences) + "\r\n" + "Found {} reconnaissance sequences".format(len(recon_sequences)))
                print("Found {} reconnaissance sequences".format(len(recon_sequences)))

            domain_events = get_events(event_list, URL_IDENTIFIERS)
            accessed_urls = set()

            for event in domain_events:
                domain = re.search(r"(http|https|smb|ftp)://[^ ]+", str(event)).group(0)
                accessed_urls.add(domain)

            if accessed_urls:
                write_log(str(accessed_urls) + "\r\n" + "Found {} domains in command lines: {}".format(len(accessed_urls), accessed_urls))
                print("Found {} domains in command lines: {}".format(len(accessed_urls), accessed_urls))

            time.sleep(1 * 60)
        except Exception as e:
            continue



def identify_download_and_execute(all_events):
    wget_commands = get_events(all_events, [b'wget'])

    if wget_commands:
        for command in wget_commands:
            if command[COMMAND] != EXECVE.NAME:
                pass

            event_time = command[TIME]
            wget_address, wget_port, downloaded_file_name = parse_wget(command)

            if wget_address == b'localhost':
                wget_address = b'127.0.0.1'

            # Check the CONNECTs happened in the correct time frame
            connect_commands = get_commands_within_time_frame(all_events, CONNECT.NAME, event_time)

            # Check the CONNECTs are to the same address
            connect_commands = [command for command in connect_commands if command[CONNECT.ADDRESS] == wget_address]

            execve_commands = get_commands_within_time_frame(all_events, EXECVE.NAME, event_time)

            chmod_commands = [command for command in execve_commands
                              if b'chmod' in command[EXECVE.PATH] and downloaded_file_name in command[EXECVE.ARGV]]

            execution_commands = [command for command in execve_commands
                                  if downloaded_file_name in command[EXECVE.PATH]]

            if connect_commands and chmod_commands and execution_commands:
                log = "The pid {} downloaded '{}' from {}:{} and executed it".format(command[EXECVE.PID].decode(), downloaded_file_name.decode(), wget_address.decode(), wget_port.decode())
                write_log(log)
                print(log)
    

if __name__ == '__main__':
    main()
