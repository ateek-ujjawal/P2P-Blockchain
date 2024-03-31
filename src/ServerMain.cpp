#include <iostream>
#include <thread>

#include "SSocket.h"
#include "ServerThread.h"

int main(int argc, char *argv[]) {
	int port;
	int peer_cnt;
	SSocket socket;
	ServerThread serverThread;
	std::unique_ptr<SSocket> new_socket;
	std::vector<std::thread> thread_vector;

	if (argc < 3) {
		std::cerr << "usage: ./server [port] [peers] (list all peer [ip] [port])" << std::endl;
		exit(1);
	}
	port = atoi(argv[1]);
	peer_cnt = atoi(argv[2]);

	// check the peers
	if (argc - 3 != peer_cnt * 2) {
		std::cout << "wrong arg" << std::endl;
		return 0;
	}

	serverThread.SetPeers(peer_cnt, &argv[3]);

	if (!socket.Init(port)) {
		std::cout << "Socket initialization failed" << std::endl;
		return 0;
	}

	std::thread block_th(&ServerThread::ServerGenerateBlock, &serverThread);
	thread_vector.push_back(std::move(block_th));

	while ((new_socket = socket.Accept())) {
		std::thread th(&ServerThread::ServerCommunicate, &serverThread, std::move(new_socket));
		thread_vector.push_back(std::move(th));
	}

	return 0;
}
