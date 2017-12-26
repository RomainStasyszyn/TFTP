#include "tftp.h"

void print_udp(){
	switch (verb) {
        case 1:
            printf("|%6u",ntohs(udp->source));
            printf("|%6u",ntohs(udp->dest));
            break;
        case 2:
            couleur("1;34");
            printf("TFTP:: ");
            couleur("0");
            printf("port source :%u\t",ntohs(udp->source));
            printf("port destination :%u\n",ntohs(udp->dest));
            break;
        case 3:
        	printf("--------------");
            couleur("1;34");
            printf("TFTP");
            couleur("0");
            printf("--------------\n");
        	printf("port source :%u\n",ntohs(udp->source));
        	printf("port destination :%u\n",ntohs(udp->dest));
        	printf("taille :%u\n",ntohs(udp->len));
        	printf("checksum :%u\n",ntohs(udp->check));
            break;
        default:
            printf("::error::verb superieur a 3\n");
            break;
    }
}

void print_tftp_rw(){
	printf("Name of file : ");
	int i = 2, c = 0;
	while(tftp[i] != 0){
		printf("%c", tftp[i]);
		i++;
	}
	i++;
	printf(" in mode : ");
	while(tftp[i] != 0){
		printf("%c", tftp[i]);
		i++;
	}
	i++;
	printf(" with option : ");
	couleur("33");
	while(i < 512 && c < 2){
		if(tftp[i] == 0){
			printf(" ");
			c++;
		} else if(tftp[i] > 47 && tftp[i] < 126) {
			printf("%c", tftp[i]);
			c = 0;
		}
		i++;
	}
	couleur("0");
}

void print_tftp_block(){
	couleur("35");
	printf("Block n°%d%d", tftp[2], tftp[3]);
	couleur("0");
}

void print_tftp_error(){
	couleur("0");
	int e = tftp[3];
	printf("Error Code %d :", e);
	switch (e) {
		case 0:
			printf("Not defined, see error message (if any).\n");
			break;
		case 1:
			printf("File not found.\n");
			break;
		case 2:
			printf("Access violation.\n");
			break;
		case 3:
			printf("Disk full or allocation exceeded.\n");
			break;
		case 4:
			printf("Illegal TFTP operation.\n");
			break;
		case 5:
			printf("Unknown transfer ID.\n");
			break;
		case 6:
			printf("File already exists.\n");
			break;
		case 7:
			printf("No such user.\n");
			break;
		default:
			break;
	}
	int i = 4;
	printf("Message : ");
	while(tftp[i] != 0){
		printf("%c", tftp[i]);
		i++;
	}
}

void print_tftp_oack(){
	int i = 2, c = 0;
	couleur("33");
	while(i < 512 && c < 2){
		if(tftp[i] == 0){
			printf(" ");
			c++;
		} else if(tftp[i] > 47 && tftp[i] < 126) {
			printf("%c", tftp[i]);
			c = 0;
		}
		i++;
	}
	couleur("0");
}

void print_tftp_apli(){
	int i;
    switch (verb) {
        case 1:
            printf("| ");
            couleur("34");
            printf("TFTP ");

			couleur("36");
			printf("de taille %d ", udp->len);

			couleur("0");
			printf("de type ");

			couleur("1;32");
			switch(tftp[1]){
				case 1:
					printf("RRQ\n");
					couleur("0");
					print_tftp_rw();
					break;
				case 2:
					printf("WRQ\n");
					couleur("0");
					print_tftp_rw();
					break;
				case 3:
					printf("DATA ");
					print_tftp_block();
					if(pr){
						printf("\n");
						for(i=4; i<udp->len;i++){
							printf("%c", tftp[i]);
						}
					}
					break;
				case 4:
					printf("ACK ");
					print_tftp_block();
					break;
				case 5:
					printf("ERROR\n");
					print_tftp_error();
					break;
				case 6:
					printf("OACK\n");
					print_tftp_oack();
					break;
				default :
					printf("%d\n", tftp[1]);
					couleur("0");
					break;
			}
            break;
        case 2:
            break;
        case 3:
            break;
        default:
            printf("::error::verb superieur a 3\n");
            break;
    }
}
