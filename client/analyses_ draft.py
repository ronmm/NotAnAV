# Constants
from client.analyses_utils import parse_wget, get_commands_within_time_frame
from client.notanav import get_all_events, CONNECT, EXECVE, COMMAND

DATA_PATH = r"C:\Users\Ron\Desktop\Final Project\data.csv"
RECON_COMMANDS = [b'uname -a', b'netstat -noa', b'cat /etc/passwd', b'ps -fade', b'arp -a']
URL_IDENTIFIERS = [b'http://', b'https://', b'ftp://', b'smb://']


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

    event_list = get_all_events()
    recon_events = get_events(event_list, RECON_COMMANDS)
    for event in recon_events:
        print(event)

    domain_events = get_events(event_list, URL_IDENTIFIERS)
    for event in domain_events:
        print(event)

    identify_download_and_execute()


def identify_download_and_execute():
    all_events = get_all_events()
    wget_commands = get_events(all_events, [b'wget'])

    if wget_commands:
        for command in wget_commands:
            if command[COMMAND] != EXECVE.NAME:
                pass

            event_time = None
            wget_address, wget_port, downloaded_file_name = parse_wget(command)

            # TODO: Remove
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
                # TODO: Document an alert to the user
                print(f"The pid {command[EXECVE.PID].decode()} downloaded '{downloaded_file_name.decode()}' "
                      f"from {wget_address.decode()}:{wget_port.decode()} and executed it")
    

if __name__ == '__main__':
    main()
