#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#include "dns.h"
#include "hashtable.h"

#define SIZE      20000
#define STRSIZE   256

DNSHandle InitDNS()
{
    DNSHandle hDNS = (DNSHandle)InitHashTable(SIZE);

    if (hDNS != INVALID_DNS_HANDLE)
    {
        return hDNS;
    }

    return INVALID_DNS_HANDLE;
}


void LoadHostsFile(DNSHandle hDNS, const char* hostsFilePath)
{
    FILE* inputFile = fopen(hostsFilePath, "r");
    char string[STRSIZE];
    unsigned int ipNums[4];

    if (inputFile == NULL)
    {
        return;
    }

    while (fscanf(inputFile, "%d.%d.%d.%d %s", ipNums, ipNums + 1, ipNums + 2, ipNums + 3, string) != EOF)
    {
        IPADDRESS ip = ipNums[0] << 24 | ipNums[1] << 16 | ipNums[2] << 8 | ipNums[3];
        AddInHashTable(hDNS, string, ip);
    }

    fclose(inputFile);
}


IPADDRESS DnsLookUp(DNSHandle hDNS, const char* hostName)
{
    IPADDRESS ip = GetFromHashTable(hDNS, hostName);

    if (ip != INVALID_IP_ADDRESS)
    {
        return (IPADDRESS)ip;
    }

    return INVALID_IP_ADDRESS;
}


void ShutdownDNS(DNSHandle hDNS)
{
    DestroyHashTable(&hDNS);
}
