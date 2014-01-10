/*
**
** Author Alexandre Grosjean
**
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>



struct sockaddr_in addr;
char rmt_host[1024];

int skan(int port);

int main(int argc, char **argv)
{
	int a, b, c, d, current_port, max_port;
	struct hostent *foo;
	struct servent *bar;
	
	if (argc != 4)
	{
		printf("\n\nUSAGE: ./portscanner start_port end_port IPaddr\n\n");
		return (1);
	}

	if (sscanf(argv[3], "%d.%d.%d.%d", &a,&b,&c,&d) != 4)
	{
		foo = gethostbyname(argv[3]);
		if (foo == NULL)
		   return (0);
		sprintf(rmt_host,"%d.%d.%d.%d",
				(unsigned char )foo->h_addr_list[0][0],
				(unsigned char ) foo->h_addr_list[0][1],
				(unsigned char ) foo->h_addr_list[0][2],
				(unsigned char ) foo->h_addr_list[0][3]);
	}
	
	else 
      	strncpy(rmt_host,argv[3],99);		
	
	if (argc == 4)
	{
		current_port = atoi(argv[1]);
		max_port = atoi(argv[2]);
		while (current_port < max_port)
		{
			if (skan((int)current_port))
			{
				bar = getservbyport(htons(current_port),"tcp");
				
				printf("%d (%s) is running.\n", current_port,
				(bar == NULL) ? "UNKNOWN" : bar->s_name);
			}
			current_port++;
		}
	}
	return 0;
}

/*
**
**	Function skan(port) scans the port passed in as a parameter
**	and return 1 (BOOL value) if port is open and returns 0 
**	for every other reason such as socket failure or closed port.
**
*/

int skan(int port)
{
	int s;
	int r;
    
	s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    
	if (s < 0)
	{
		printf("there was an error creating the socket");
		return (0);
	}
   	addr.sin_family = PF_INET;
   	addr.sin_port = htons(port);
   	addr.sin_addr.s_addr = inet_addr(rmt_host);

   	r = connect(s,(struct sockaddr *) &addr, sizeof(addr));

   	close(s);

   	if (r == -1)
	{
		//printf("port : %d is closed", port); //prints closed ports
		return (0);
	}
	else
   		return (1);
	
}
