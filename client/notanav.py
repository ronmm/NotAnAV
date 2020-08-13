COMMAND = 0


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


def main():
    # TODO: Call the hook C module (usermode one) and check that it started correctly

    # data_path = '/tmp/data.csv'
    data_path = './data.csv'  # TODO: Convert this to an environment variable

    with open(data_path, 'rb') as data_file:
        data = data_file.read()

    data_lines = data_to_lines(data)

    ip = convert_ip_address(data_lines[0])

    print(data_lines)


def convert_ip_address(line):

    if line[COMMAND] != CONNECT.NAME:
        return

    addr = strip_field_name(line[CONNECT.ADDRESS])
    addr = addr.rjust(8, b'0')

    octets = []
    i = 0

    for index in range(0,4):
        octets.append(int(f'0x{addr[i:i+2].decode()}', 16))
        i += 2

    return f'{octets[0]}.{octets[1]}.{octets[2]}.{octets[3]}'


def strip_data(data):
    return data.replace(b'"', b'').replace(b'\'', b'')


def strip_field_name(field):
    # TODO: Might create a problem when there's an equal sign in the EXECVE argv
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

        final_lines.append(split_line)

    return final_lines


def clear_data_file(data_path):
    # TODO: Fetch and clear every 5 minutes
    return data_path


if __name__ == "__main__":
    main()