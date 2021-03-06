#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <sstream>
#include <Windows.h>
using namespace std;
#pragma comment(lib, "ws2_32.lib")

//-------- Define variables --------


int main(void) {

	//---- Create Server with multiple clients (2) ----

	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0) {
		cerr << "Can't Initialize winsock! Quitting" << endl;
		return 0;
	}

	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Quitting" << endl;
		return 0;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(listening, (sockaddr*)&hint, sizeof(hint));

	//---- Start listening for client ----
	listen(listening, SOMAXCONN);
	cout << "start listenning for new clients" << endl;

	fd_set master;
	FD_ZERO(&master);
	FD_SET(listening, &master);

	bool running = true;
	int t = 0;
	bool listening_socket_open = true;
	fd_set copy = master;
	bool first = true;
	while (true) {
		cout << "test#2" << endl;
		int i = 0;
		

		//int socketCount = select(0, &copy, nullptr, nullptr, nullptr);
		/*
		//-------- Verify if the are enough players and if then close the listening socket --------

		int socketCounter = select(0, &copy, nullptr, nullptr, nullptr);

		//---- enough players ----
		if (socketCounter > 2 && listening_socket_open) {
			closesocket(listening);
			listening_socket_open = false;
			cout << "Enough clients connected! Closing listening socket!" << endl;
		}

		//---- not enough players ----

		if (socketCounter < 2 && !listening_socket_open) {
			sockaddr_in hint;
			hint.sin_family = AF_INET;
			hint.sin_port = htons(54000);
			hint.sin_addr.S_un.S_addr = INADDR_ANY;
			bind(listening, (sockaddr*)&hint, sizeof(hint));

			listen(listening, SOMAXCONN);
			cout << "reopening listening socket (not enough clients!)" << endl;
			listening_socket_open = true;
		}


		*/
		//-------- Run this for every client --------
		cout << "test# l85" << endl;
		for (i; i < master.fd_count; i++) {
			
				SOCKET sock = master.fd_array[i];
				cout << "test l89" << endl;

				//-------- if its a new client => give its 'playing id' --------
				if (sock == listening) {
					if (0 >= master.fd_count && first == true) {

						cout << "test inside sock == listening l93" << endl;

						//---- autoriser la connection ----
						SOCKET client = accept(listening, nullptr, nullptr);
						FD_SET(client, &master);

						//---- transform t from int to string ----
						stringstream temp;
						temp << t;
						string id = temp.str();

						//---- send the t value ----
						send(client, id.c_str(), id.size() + 1, 0);

						//---- set t value to 0 or 1 ----
						t++;
						if (t > 1) {
							t = 0;
						}
					}
				} 

				//-------- if the connection isn't a new one --------
				else {
					
					first = false;
					char buf[4096];
					ZeroMemory(buf, 4096);

					int bytesIn = recv(sock, buf, 4096, 0);
					
					//---- check for disconnection ----
					if (bytesIn <= 0) {
						
						cout << "MIAOooooo" << endl;
						closesocket(sock);
						FD_CLR(sock, &master);
						t = t - 1;
						
					}
					
					//---- If there is a message then ----
					else  {
						
						//-------- data recieved isn't empty => process it --------

						cout << "MIAuuuuuuu" << endl;

						//---- Define variables needed ----
						int cases[64];
						int lastcase[64] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 56, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63 };
						int g = 0;
						printf("test111");

						//-------- receive the 64 sensors --------
						for (g; g < 64; g++) {

							//---- send recieved message to client ----
							string str = "recieved";
							int sendresult = send(sock, str.c_str(), str.size() + 1, 0);
							if (sendresult) {
								cout << "recieve sent" << endl;
							}

							//---- recieving the data ----
							ZeroMemory(buf, 4096);
							int bytes_recieved = recv(sock, buf, 4096, 0);
							
							//---- store it into an array for comparision ----
							string cas = string(buf, 0, bytes_recieved);
							int casi = atoi(cas.c_str());
							cases[g] = casi;
							
						}

						//---- print recieved data into console (will remove, its just for tests)----
						cout << "start of list" << endl;
						for (int j = 0; j < 64; j++) { cout << cases[j] << endl; }
						cout << "end of list" << endl;

						//---- compare lists ----
						for (int count = 0; count < 64; count++) {

							//---- if they are almost the same => don't do anything special ----
							int verif = cases[count] - lastcase[count];
							if (verif < 20) {

							}

							//---- if the value aren't almost the same ----
							else {
							 
							}

						}
					
					}
				}
			}
		
	}

	
	WSACleanup();
	system("PAUSE");
	return 0;
}