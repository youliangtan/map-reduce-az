
#ifndef DNSLOOKUP_H
#define DNSLOOKUP_H

// Ref to this:
// http://www.zedwood.com/article/cpp-dns-lookup-ipv4-and-ipv6

#include <cstring> //memset
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
 
#include <iostream>
#include <string>
#include <vector>
 
using std::vector;
using std::string;
using std::cout;
using std::endl;
 
vector<string> dns_lookup(const string &host_name, int ipv)
{
    vector<string> output;
 
    struct addrinfo hints, *res, *p;
    int status, ai_family;
    char ip_address[INET6_ADDRSTRLEN];
 
    ai_family = ipv==6 ? AF_INET6 : AF_INET; //v4 vs v6?
    ai_family = ipv==0 ? AF_UNSPEC : ai_family; // AF_UNSPEC (any), or chosen
    memset(&hints, 0, sizeof hints);
    hints.ai_family = ai_family; 
    hints.ai_socktype = SOCK_STREAM;
 
    if ((status = getaddrinfo(host_name.c_str(), NULL, &hints, &res)) != 0) {
        //cerr << "getaddrinfo: "<< gai_strerror(status) << endl;
        return output;
    }
 
    //cout << "DNS Lookup: " << host_name << " ipv:" << ipv << endl;
 
    for(p = res;p != NULL; p = p->ai_next) {
        void *addr;
        if (p->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
        } else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
        }
 
        // convert the IP to a string
        inet_ntop(p->ai_family, addr, ip_address, sizeof ip_address);
        output.push_back(ip_address);
    }
 
    freeaddrinfo(res); // free the linked list
 
    return output;
}


#endif // DNSLOOKUP_H
