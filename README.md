# Projet IngProt - TFTP

Based on wireshark-in-C-with-TCPdump - Projet TSR analyser of the network like wireshark- https://github.com/loic-hourdin/wireshark-in-C-with-TCPdump

Only work on ubuntu because of some library

to install:
    sudo apt-get install libpcap0.8-dev
    make

to use it:
    sudo ./analyse <file.pcap>
    sudo ./analyse -v <file.pcap> for content of DATA

