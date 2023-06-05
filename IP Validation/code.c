#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <math.h> 
#include <ctype.h>
int ipVerify(char* ipAddr, unsigned char* octetArray) { 
// Function verifies a valid IP has been entered
char* token;
int i = 0;
int j = 0;
int periodCount = 0;
token = strtok(ipAddr, "."); 
while (token != NULL) {
for (j=0; j<strlen(token); j++) {
if (isdigit(token[j]) == 0) {
break; 
}
}
if (strlen(token) > 0 && strlen(token) < 4 && j == strlen(token)
&& atoi(token) < 256 && atoi(token) >= 0) { 
periodCount++;
octetArray[i] = atoi(token); 
} else {
break; 
}
i++;
token = strtok(NULL, "."); 
}
if (periodCount != 4) { 
return 0;
} else {
return 1; 
}
}
void printSubnetInfo(u_int32_t* addressOctets, int* CIDR, int* subnetBits) { 
// Prints infomation about the given subnet.u_int32_t netAddress; 
u_int32_t netMask;
netMask = (0xFFFFFFFF << (32 - (*CIDR + *subnetBits)) & 0xFFFFFFFF); 
netAddress = *addressOctets & netMask;
// display the network address
printf("\nThe Network address: %d.%d.%d.%d/%d\n", (netAddress >> 24) & 0xFF, 
(netAddress >> 16) & 0xFF,
(netAddress >> 8) & 0xFF, (netAddress) & 0xFF, *CIDR +
*subnetBits);
printf("\n");
// Subtract the network address from the broadcast address
printf("Total number of usable hosts: %d\n", ((netAddress | ~netMask) - netAddress) 
- 1);
printf("\n");
// Display the first host address
printf("First host address / the network address: %d.%d.%d.%d\n", ((netAddress + 1) 
>> 24) & 0xFF, ((netAddress + 1) >> 16) & 0xFF,
((netAddress + 1) >> 8) & 0xFF, (netAddress + 1) & 0xFF);
printf("\n");
netAddress = netAddress | ~netMask;
printf("Last host address / : %d.%d.%d.%d\n", ((netAddress - 1) >> 24) & 0xFF, 
((netAddress - 1) >> 16) & 0xFF,
((netAddress - 1) >> 8) & 0xFF, (netAddress - 1) & 0xFF);
printf("\n");
// display the broadcast address
printf("The Broadcast address : %d.%d.%d.%d\n", (netAddress >> 24) & 0xFF, 
(netAddress >> 16) & 0xFF,
(netAddress >> 8) & 0xFF, (netAddress) & 0xFF);
printf("\n"); 
}
int main() {
char ipAddr[18];char buff[4]; 
int CIDR;
unsigned char* octetArray; 
octetArray = calloc(4, sizeof(char)); 
u_int32_t addressOctets;
int subnetNumber;
int subnetBits = 0;
int totalSubnets = 0; 
u_int32_t currentSubnet; 
int i;
// Get address 
while (1) {
printf("Enter IP address: "); 
fgets(ipAddr, 17, stdin); 
ipAddr[strlen(ipAddr)-1] = '\0';
printf("\n\nVerifying:\n\n %s... ", ipAddr); 
// Verify
if (ipVerify(ipAddr, octetArray) == 0) {
printf("*********------Invalid IP entered!--------************\n"); 
} else {
printf("\n*********------Address verified!--------************\n"); 
break;
} 
}
// Get Mask 
while (1) {
printf("\n\n\nEnter the subnet mask: "); 
fgets(buff, 4, stdin);
CIDR = atoi(buff);
if (CIDR > 0 && CIDR < 32) { 
break;
} else {
printf("Invalid mask. Input again:\n"); 
}
}
printf("\n%d.%d.%d.%d/%d ", octetArray[0], octetArray[1], octetArray[2], 
octetArray[3], CIDR);
if (octetArray[0] > 239) { 
printf("(Class E)\n");
} else if (octetArray[0] > 223) { 
printf("(Class D)\n");
} else  if (octetArray[0] > 191) { 
printf("(Class C)\n");
} else if (octetArray[0] > 127) { 
printf("(Class B)\n");
} else {
printf("(Class A)\n"); 
}
// IP address into an integer
addressOctets = (octetArray[0] << 24) | (octetArray[1] << 16) | (octetArray[2] << 
8) | (octetArray[3]);
printSubnetInfo(&addressOctets, &CIDR, &subnetBits); 
do {printf("Enter no. of required networks, \nor q to quit: "); 
fgets(buff, 4, stdin);
subnetNumber = atoi(buff); 
if (subnetNumber == 0) {
printf("Exit .\n"); 
exit(0);
}
while (subnetNumber > totalSubnets) { 
subnetBits++;
totalSubnets = pow(2, subnetBits); 
}
if ((CIDR + subnetBits) > 31) {
printf("No. of networks too large .\n"); 
}
} while ((CIDR + subnetBits) > 31); 
printf("\nTotal subnets created:
%d\n\t\t\t\t\t\t**********************************************\n", totalSubnets);
// print the information of each subnet 
for (i=0; i<totalSubnets; i++) {
currentSubnet = (addressOctets & ((0xFFFFFFFF << (32 - CIDR)) & 0xFFFFFFFF))
| i << (32 - (CIDR + subnetBits));
printSubnetInfo(&currentSubnet, &CIDR, &subnetBits); 
}
free(octetArray); 
return 0;
}