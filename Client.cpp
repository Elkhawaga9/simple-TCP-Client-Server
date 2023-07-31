#pragma comment(lib,"ws2_32.lib")
#include<iostream>
//#include<WinSock2.h>
#include<WS2tcpip.h>
#include<stdio.h>
#include<string>
using namespace std;
int main()
{
	string ipAddress = "127.0.0.1";
	int port = 54000;
	cout << "enter IP Address <space> port number: ";
	cin >> ipAddress >> port;
	//wstring wideIpAddress(ipAddress.begin(), ipAddress.end());
	
	//initialize winsock 
	WSADATA wsaData;
	int err;
	err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0)
	{
		cout << "Can't start winsock :" << WSAGetLastError()<<"\n";
		system("pause");
		exit(1);
	}
	//create a socket for the client
	int clientSock = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSock != -1)
	{
		printf("Client Socket ID: %d \n", clientSock);
	}
	else
	{
		printf("failed to create a socket ");
		printf("%d",WSAGetLastError());
		exit(1);
	}

	//connect the client to specific server
	struct sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(ipAddress.c_str());
	serverAddress.sin_port = htons(port);
	if (connect(clientSock, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == 0)
	{
		printf("connected successfully.\n");
	}
	else
	{
		printf("failed to connect %d \n",WSAGetLastError());
		closesocket(clientSock);
		WSACleanup();
		exit(1);
	}
	//chat to server
	char buf[4096];
	string userinput;
	do
	{
		// prompt the user for some text
		printf("Client: ");
		cin.ignore();
		getline(cin, userinput);
		if (userinput.size() > 0)
		{
			//send data to server
			int clientSend = send(clientSock, userinput.c_str(), userinput.size() + 1, 0);
			if (clientSend != SOCKET_ERROR)
			{
				//wait for response
				int clientRecieved = recv(clientSock, buf, 4096, 0);
				if (clientRecieved > 0)
				{
					cout << "Server: " << string(buf, 0, clientRecieved) << "\n";
				}
			}
		}
	} while (userinput.size() > 0);

	//close every thing
	closesocket(clientSock);
	WSACleanup();
}