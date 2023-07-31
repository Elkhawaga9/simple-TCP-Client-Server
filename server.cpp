#include<iostream>
#include<string>
#include<WS2tcpip.h>
#pragma comment (lib,"ws2_32.lib")
using namespace std;
int main()
{

	string ipAddress = "127.0.0.1";
	int port = 54000;
	cout << "enter IP Address <space> port number: ";
	cin >> ipAddress >> port;

	// initialize winsock
	WSADATA wsaData;
	int err;
	err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0)
	{
		cout << "Can't start winsock :" << WSAGetLastError() << "\n";
		system("pause");
		exit(1);
	}
	//create a socket
	int serverSock = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSock != -1)
	{
		cout << "Server Socket ID: " << serverSock<<"\n";
	}
	else
	{
		printf("failed to create a socket ");
		printf("%d", WSAGetLastError());
		exit(1);
	}

	//bind socket to ip and port
	struct sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	serverAddress.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(serverSock, (sockaddr*)&serverAddress, sizeof(serverAddress)) == 0)
	{
		cout << "server is binded to port no. " << port << "\n";
	}
	else
	{
		cout << "binding failed\n";
		exit(1);
	}
	//socket for listening
	if (listen(serverSock, 5) == 0)
	{
		cout << "listening . . .\n";
	}
	else
	{
		cout << "failed to listen\n";
		exit(1);
	}
	//wait for connection
	struct sockaddr_in connectedClientAddress;
	memset(&connectedClientAddress, 0, sizeof(connectedClientAddress));

	int clientAdressLength = 0;

	int clinetSocket = accept(serverSock,NULL, NULL);

	if(clinetSocket ==-1)
	{
		cout << "failed to accept a connection request\n";
		exit(1);
	}
	else
	{
		cout << "Accepted a request at socket ID: " << clinetSocket << "\n";
	}

	
	//chat between client and server
	cin.ignore();
	char buf[4096];
	string serverMessage;
	while (true)
	{
		// clear buffer
		memset(buf, 0, 4096);
		// wait for client to send data
		int bytesRecieved = recv(clinetSocket, buf, 4096, 0);
		if (bytesRecieved == -1)
		{
			cout << "There was a connection issue." << endl;
			break;
		}
		if (bytesRecieved == 0)
		{
			cout << "The client disconnected" << endl;
			break;
		}
		// display message
		cout << "Client: " << string(buf, 0, bytesRecieved)<<"\n";

		// the server sends
		cout << "server: ";
		getline(cin, serverMessage);
		if(serverMessage.size()>0)
			send(clinetSocket, serverMessage.c_str(), bytesRecieved + 1, 0);
	}
	//close listening socket
	closesocket(serverSock);

	//close socket
	closesocket(clinetSocket);

	//shutdown winsock
	WSACleanup();
}