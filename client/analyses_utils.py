from datetime import datetime

from client.notanav import EXECVE, COMMAND, TIME

STRP_FORMAT = "%a %b %d %H:%M:%S %Y"


def parse_wget(wget_event):
    if wget_event[COMMAND] == EXECVE.NAME:
        url = wget_event[EXECVE.ARGV].split(b' ')[2]
        url = url.split(b'/')
        address = url[-2]
        return address.split(b':')[0], address.split(b':')[1], url[-1]


def get_commands_within_time_frame(all_events, command_type, event_time, time_frame=5):
    """
    Returns all the commands with the same command_type in the specified time_frame.

    :param all_events:
    :param command_type:
    :param event_time:
    :param time_frame: The time_frame in minutes
    :return:
    """
    events = []

    for event in all_events:
        temp_event_time = event[TIME]

        try:
            temp_time_obj = datetime.strptime(temp_event_time.decode(), STRP_FORMAT)
            real_time_obj = datetime.strptime(event_time.decode(), STRP_FORMAT)
        except Exception:
            continue

        diff = temp_time_obj - real_time_obj
        diff_in_minutes = divmod(diff.days * (24 * 60 * 60) + diff.seconds, 60)

        event_within_time_frame = False

        if -time_frame <= diff_in_minutes[0] <= time_frame:
            event_within_time_frame = True

        if event_within_time_frame:
            if event[COMMAND] == command_type:
                events.append(event)

    return events


def write_log(log_line):
    log_path = '/tmp/notAnAV.log'

    with open(log_path, 'a+') as log:
        log.write("[!] SUSPICIOUS ACTIVITY:\r\n")
        log.write(log_line)
        log.write('\r\n')
        log.write("[!] END OF SUSPICIOUS ACTIVITY\r\n")
