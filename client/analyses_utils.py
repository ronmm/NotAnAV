from client.notanav import EXECVE, COMMAND


def parse_wget(wget_event):
    # TODO: Parse using regex

    if wget_event[COMMAND] == EXECVE.NAME:
        url = wget_event[EXECVE.ARGV].split(b' ')[2]
        url = url.split(b'/')
        address = url[-2]
        return address.split(b':')[0], address.split(b':')[1], url[-1]


def get_commands_within_time_frame(all_events, command_type, event_time, time_frame=5):
    """
    Returns all the commands with the same command_type in the specified time_frame.
    # TODO: Add the times for events and really use it

    :param all_events:
    :param command_type:
    :param event_time:
    :param time_frame: The time_frame in minutes
    :return:
    """
    events = []

    for event in all_events:
        # TODO: Implement
        event_within_time_frame = True

        if event_within_time_frame:
            if event[COMMAND] == command_type:
                events.append(event)

    return events
