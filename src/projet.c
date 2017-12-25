#include "projet.h"

void got_packet(u_char *args, const struct pcap_pkthdr *hdr, const u_char *truepacket) {
    parser(truepacket);
    aff(hdr);
}

void parser(const u_char* truepacket){

    ethernet = (struct ether_header*)truepacket;
    switch (ntohs(ethernet->ether_type))
    {
        case 0x0800:
            //IP
            ip = (struct ip*)(truepacket+14);
            switch (ip->ip_p) {
                case 6:
                    //TCP
                    break;

                case 17:
                    //UDP
                    udp = (struct udphdr*)(truepacket+(4*ip->ip_hl)+14);

                    switch ((unsigned short int)ntohs(udp->dest)) {
                        case 69:
                            //bootp server
                            bootp = (struct bootp*)(truepacket+(4*ip->ip_hl)+14+8);
                            tftp = (unsigned char*)(truepacket+(4*ip->ip_hl)+14+8);
                            break;
                        default:
                            break;
                    }
                    break;

                default:
                    break;
            }
            break;

        case 0x0806:
            //arp
            break;

        default:
            break;
    }
}

void aff(const struct pcap_pkthdr *hdr){
    printf("\n -------------------------------------------- \n");
    numero++;
    if(verb==3){
        printf("-----------------------------------------------------------\n");
        couleur("31");
        couleur("1");
        printf("\t\t Packet n°%i\n",numero);
        couleur("0");
        printf("-----------------------------------------------------------\n");
        print_paquet((u_char*) ethernet,hdr);
    }
    if(verb==2){
        couleur("31");
        printf("Packet n°%i\n",numero);
        couleur("0");
    }
    if(verb==1){
            printf("%4i",numero);
    }
    char not_print=0;

    print_ethernet();
    switch (ntohs(ethernet->ether_type))
    {
        case 0x0800:
            //IP
            print_ip();
            switch (ip->ip_p)
            {
                case 6:
                    //TCP
                    break;
                case 17:
                    //UDP
                    print_udp();

                    switch (ntohs(udp->dest))
                    {
                        case 67:
                            //bootp server
                            print_bootp();
                            not_print=1;
                            break;
                        case 53:
                            if(verb==1){
                                printf("| ");
                                couleur("33");
                                printf("DNS ");
                                couleur("0");
                                not_print=1;
                            }
                            break;
                        default:
                            break;
                    }

                    if(!not_print){
                        switch (ntohs(udp->source))
                        {
                            case 67:
                                //bootp server
                                print_bootp();
                                break;
                            case 53:
                                if(verb==1){
                                    printf("| ");
                                    couleur("33");
                                    printf("DNS ");
                                    couleur("0");
                                    not_print=1;
                                }
                                break;
                            default:
                                print_tftp_apli();
                                break;
                        }
                    }
                break;
                default:
                    print_ip_apli();
                    break;
            }
            break;
        case 0x0806:
            //arp
            break;
        case 0x86dd:
            if(verb==1){
                printf("|---------------|---------------|------|------| ");
                couleur("35");
                couleur("1");
                printf("IPV6");
                couleur("0");
            }
            break;
        case 0x8035:
            if(verb==1){
                printf("|---------------|---------------|------|------| ");
                couleur("35");
                couleur("1");
                printf("RARP");
                couleur("0");
            }
            break;
        case 0x8100:
            if(verb==1){
                printf("|---------------|---------------|------|------| ");
                couleur("35");
                couleur("1");
                printf("IPV6");
                couleur("0");
            }
            break;
        default:
            if(verb==1){
                printf("|---------------|---------------|------|------| ");

                printf("protocole réseau inconue");
                couleur("0");
            }
            break;
    }
    printf("\n");
}

int main (int argc, char *argv[]){

    if (!(argc==1||argc==3||argc==5||argc==7||argc==9)){
        fprintf(stderr,"::erreur:: nbr d'arguments %d\ncommande de type: annalyseur -o <fichier>\n",argc);
        return 2;
    }

    pcap_t *capture;
    char* interface=NULL;
    char* fichier=NULL;
    char* errbuf=NULL;
    verb=1;
    numero=0;

    //pensez check argument
    if(strcmp(argv[1],"-o")==0){
        fichier=argv[2];
    } else{
        fprintf(stderr,"::erreur:: mauvais argument %s\ncommande de type: annalyseur -o <fichier>\n",argv[1]);
        return 2;
    }

    FILE* f = NULL;
    f = fopen(fichier, "r");
    if(f==NULL){
        perror("::erreur:: imposible de lire le fichier");
        return 2;
    }
    capture= pcap_fopen_offline(f, errbuf);
    if(capture==NULL){
        fprintf(stderr, "::erreur:: impossible d'ouvrir l'interface %s: %s\n", interface, errbuf);
        return 2;
    }

    if(verb==1){
        printf("n°  | IP source     | IP destination|port s|port d| information\n");
        printf("---------------------------------------------------------------------\n");
    }
    pcap_loop(capture, 0, got_packet,NULL);

    return 0;
}
