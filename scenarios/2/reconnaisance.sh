echo 'Linux & Kernel versions:' >> ./output.txt
uname -a >> ./output.txt

echo 'Listening ports:' >> ./output.txt
netstat -noa | grep LISTEN >> ./output.txt

echo 'List of users:' >> ./output.txt
cat /etc/passwd >> ./output.txt

echo 'List of running processes:' >> ./output.txt
ps -fade >> ./output.txt

echo 'Arp table:' >> ./output.txt
arp -a >> ./output.txt


