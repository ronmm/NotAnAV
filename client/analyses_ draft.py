# Constants
DATA_PATH = r"C:\Users\Ron\Desktop\Final Project\data.csv"
RECON_COMMANDS = ['uname -a', 'netstat -noa', 'cat /etc/passwd', 'ps -fade', 'arp -a']
URL_IDENTIFIERS = ['http://', 'https://', 'ftp://', 'smb://']

def get_recon_commands(event_list):
    """
    Return a list of all recon commands in raw events
    """
    recon_commands_executions = []
    for row in event_list:
        if any(word in row for word in RECON_COMMANDS):
            recon_commands_executions.append(row)
    return recon_commands_executions
    
def get_domains(event_list):
    """
    Return a list of events that contain domains in execution arguments
    """
    domain_events = []
    for row in event_list:
        if any(word in row for word in URL_IDENTIFIERS):
            domain_events.append(row)
    return domain_events
    
def main():
    
    event_list = open(DATA_PATH, 'r').readlines()
    recon_events = get_recon_commands(event_list)
    for event in recon_events:
        print(event)
        
    domain_events = get_domains(event_list)
    for event in domain_events:
        print(event)
    

if __name__ == '__main__':
    main()
