#include "Server.h"



Server::Server(sf::RenderWindow& win) : Client(win) {
	// window is client 
	this->window.setTitle("server");
	this->idPlayer = 1;
}

Server::~Server() { }


void Server::launch() {
	this->start = false;
	this->connected = false;
	this->exitCurrentGame = false;

	this->gboard.init();

	sf::Thread thread(&Server::waitClient, &(*this));
	thread.launch();

	this->socket_udp.setBlocking(false);
	this->menuWaitingRoom();

	thread.terminate();

	if (!this->window.isOpen())
		return;
	
	if (this->connected) {
		this->serverStatus = this->SERVER_START_GAME;
		
		sf::Packet packet;

		for (std::vector<sf::TcpSocket*>::iterator it = this->sockets.begin(); it != this->sockets.end(); ++it) {
			sf::TcpSocket& client = **it;
			
			packet << this->playerConnected << this->playerMax << this->serverStatus;

			if (client.send(packet) == sf::Socket::Done)
				std::cout << "Send push sequence to " << client.getRemoteAddress().toString() << std::endl;
			
		}

		packet << this->gboard;

		uint64_t id = 0;
		for (std::vector<sf::TcpSocket*>::iterator it = this->sockets.begin(); it != this->sockets.end(); ++it) {
			sf::TcpSocket& client = **it;
			if (client.send(packet) == sf::Socket::Done)
				std::cout << "Carte be send " << int(id + 2) << std::endl;
			client.setBlocking(false);
			++id;
		}

		this->timeElapsed.restart();
		this->run();
	}
	else {
		std::cout << "Solo game" << std::endl;
		this->timeElapsed.restart();
		this->run();
	}
}

void Server::actionLink(int orgX, int orgY, int destX, int destY) {
	int idOrg = this->gboard.getCloserCell(orgX, orgY);
	if (idOrg >= 0 && this->gboard.getOwner(idOrg) == this->idPlayer) {
		int idDest = this->gboard.getCloserCell(destX, destY);
		this->gboard.link(idOrg, idDest);
		sf::Packet packet;
		sf::Uint8 code = this->LINK;
		sf::Uint16 org = idOrg;
		sf::Uint16 dest = idDest;
		packet << code << org << dest;
		for (std::vector<sf::TcpSocket*>::iterator it = this->sockets.begin(); it != this->sockets.end(); ++it)
			(**it).send(packet);
	}
}

void Server::actionFree(int mouseX, int mouseY) {
	sf::Uint16 idCell = this->gboard.getCloserCell(mouseX, mouseY);
	gboard.free(this->idPlayer, idCell);
	sf::Packet packet;
	sf::Uint8 code = this->FREE;
	sf::Uint16 org = idCell;
	packet << code << this->idPlayer << org;
	for (std::vector<sf::TcpSocket*>::iterator it = this->sockets.begin(); it != this->sockets.end(); ++it)
		(**it).send(packet);
}

void Server::actionFree(sf::Uint8 nidPlayer, int mouseX, int mouseY) {
	sf::Uint16 idCell = this->gboard.getCloserCell(mouseX, mouseY);
	this->gboard.free(nidPlayer, idCell);
	sf::Packet packet;
	sf::Uint8 code = this->FREE;
	sf::Uint16 org = idCell;
	packet << code << nidPlayer << org;
	for (std::vector<sf::TcpSocket*>::iterator it = this->sockets.begin(); it != this->sockets.end(); ++it)
		(**it).send(packet);
}

void Server::actionChangeLink(int orgX, int orgY, int destX, int destY) {
	int idOrg = this->gboard.getCloserCell(orgX, orgY);
	if (idOrg >= 0 && this->gboard.getOwner(idOrg) == this->idPlayer) {
		int idDest = this->gboard.getCloserCell(destX, destY);
		if (idDest >= 0) {
			this->gboard.changeLinkState(idOrg, idDest);
			sf::Packet packet;
			sf::Uint8 code = this->CHANGE;
			sf::Uint16 org = idOrg;
			sf::Uint16 dest = idDest;
			packet << code << org << dest;
			for (std::vector<sf::TcpSocket*>::iterator it = this->sockets.begin(); it != this->sockets.end(); ++it)
				(**it).send(packet);
		}
	}
}

void Server::menuWaitingRoom() {
	this->serverStatus = SERVER_READY;
	sf::Packet info_server;

	info_server << serverStatus;
	//info_server << serverStatus << sf::IpAddress::getLocalAddress().toInteger();

	sf::RectangleShape rect(window.getDefaultView().getSize());
	rect.setFillColor(sf::Color(0, 0, 0, 180));

	sf::Text info;
	sf::Font font;
	font.loadFromFile("Textures/JetBreins.ttf");
	
	info.setFont(font);
	info.setCharacterSize(24);
	info.setString("Waiting for players ... ");
	info.setOrigin(info.getLocalBounds().left + info.getGlobalBounds().width / 2.f,
		info.getGlobalBounds().height / 2.f);
	info.setPosition(window.getDefaultView().getSize() / 2.f);
	info.setFillColor(sf::Color::White);

	sf::Text info2 = info;
	info2.setString("");
	info2.setCharacterSize(12);
	info2.setPosition(200.f, 20.f);

	sf::Text buttonAdd = info;
	buttonAdd.setString("Add player");
	buttonAdd.setPosition(500.f, 20.f);

	sf::Text buttonRemove = info;
	buttonRemove.setString("Remove player");
	buttonRemove.setPosition(500.f, 40.f);

	sf::Text button_t;
	button_t.setFont(font);
	button_t.setCharacterSize(24);
	button_t.setString("Start !");
	button_t.setOrigin(button_t.getLocalBounds().left + button_t.getGlobalBounds().width / 2.f,
		button_t.getGlobalBounds().height / 2.f);
	button_t.setPosition(window.getDefaultView().getSize() / 2.f);
	button_t.move(0, 2.f * 24.f);
	button_t.setFillColor(sf::Color::White);

	sf::Event event;
	while (!this->start && !this->connected && this->window.isOpen() && !this->exitCurrentGame) {
		while (this->window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				this->window.close();
				this->start = true;
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					this->exitCurrentGame = true;
					this->start = true;
				}
			}
			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					if (button_t.getGlobalBounds().contains(
						this->window.mapPixelToCoords(
							sf::Mouse::getPosition(window)
						)
					)) {
						this->start = true;
						this->connected = true;
					}
					else if (buttonAdd.getGlobalBounds().contains(
						this->window.mapPixelToCoords(
							sf::Mouse::getPosition(this->window)
						)
					)) {
						if (this->playerMax < 8) {
							this->addPlayer();
							this->gboard.initPlayers();
						}
					}
					else if (buttonRemove.getGlobalBounds().contains(
						 this->window.mapPixelToCoords(
							sf::Mouse::getPosition(this->window)
						))) {
						 if (this->playerMax >= 2) {
							this->playerMax--;
							this->players.pop_back();
							this->gboard.initPlayers();
						 }

					}
				}
			}
		}

		// send information on all user of the local network
		if (this->socket_udp.send(info_server, sf::IpAddress::Broadcast, this->PORT_GAME_B) != sf::Socket::Done) {
			this->start = true;
		}
		if (button_t.getGlobalBounds().contains(
			window.mapPixelToCoords(sf::Mouse::getPosition(window))
		)) {
			button_t.setFillColor(sf::Color(200, 200, 255, 200));
		}
		else {
			button_t.setFillColor(sf::Color(255, 255, 255, 255));
		}
		if (buttonAdd.getGlobalBounds().contains(
			window.mapPixelToCoords(sf::Mouse::getPosition(window))
		)) {
			buttonAdd.setFillColor(sf::Color(200, 200, 255, 200));
		}
		else {
			buttonAdd.setFillColor(sf::Color(255, 255, 255, 255));
		}
		if (buttonRemove.getGlobalBounds().contains(
			window.mapPixelToCoords(sf::Mouse::getPosition(window))
		)) {
			buttonRemove.setFillColor(sf::Color(200, 200, 255, 200));
		}
		else {
			buttonRemove.setFillColor(sf::Color(255, 255, 255, 255));
		}
		info2.setString("Waiting : " + std::to_string(this->playerConnected)
			+ " / " + std::to_string(this->playerMax));

		this->window.clear();
		this->window.draw(this->gboard);
		this->window.draw(rect);
		this->window.draw(info);

		this->window.draw(button_t);
		this->window.draw(buttonAdd);
		this->window.draw(buttonRemove);
		this->window.draw(info2);

		this->window.display();
	}
}

void Server::waitClient() {
	this->serverStatus = this->SERVER_READY;
	if (this->listener.listen(this->PORT_GAME_A) != sf::Socket::Done)
		return;

	this->selector.add(this->listener);

	while (!this->start) {
		if(this->selector.wait(sf::seconds(0.1f))){
			if (this->selector.isReady(this->listener)) {
				sf::TcpSocket* socketTmp = new sf::TcpSocket();
				if (this->listener.accept(*socketTmp) == sf::TcpSocket::Done) {
					this->sockets.push_back(socketTmp);
					this->selector.add(*socketTmp);
					socketTmp->setBlocking(false);
					++this->playerConnected;
					std::cout << "new client: " << (int)this->playerConnected << " / " << (int)this->playerMax << std::endl;
				}
				else {
					delete socketTmp;
				}
			}
		}
		for (std::vector<sf::TcpSocket*>::iterator it = sockets.begin(); it != sockets.end(); ++it) {
			sf::TcpSocket& client = **it;
			if (this->selector.isReady(client)) {
				sf::Packet packet;
				packet << this->playerConnected << this->playerMax << this->serverStatus;
				if (client.send(packet) == sf::Socket::Done) 
					std::cout << "send to " << client.getRemoteAddress().toString() << std::endl;
			}
		}
	}
}

void Server::manageNetwork() {
	if (!this->connected)
		return;

	uint64_t i = 0;
	for (std::vector<sf::TcpSocket*>::iterator it = this->sockets.begin(); it != this->sockets.end(); ++it) {
		sf::TcpSocket& client = **it;
		if (this->selector.isReady(client)) {
			sf::Packet packet;
			if (client.receive(packet) == sf::Socket::Disconnected) {
				std::cout << "Disconnected client # " << (i + 2) << " ! " << std::endl;
				it = this->sockets.erase(it);
				break;
			}
			else {
				sf::Uint8 code;
				if (packet >> code) {
					this->networkActions(packet, code);
					for (uint64_t j = 0; j < this->sockets.size(); ++j) 
						if (j != i) 
							this->sockets[j]->send(packet);
				}
			}
		}
		++i;
	}

	// resynchronize clients with server each TIME_RESYNC sec
	if (this->timeElapsed.getElapsedTime().asMilliseconds() > this->TIME_RESYNC) {
		this->resyncClients();
		this->timeElapsed.restart();
	}
}

void Server::addPlayer() {
	++this->playerMax;
	this->players.push_back(Player(this->playerMax));
}

void Server::resyncClients() {
	sf::Packet packet;
	packet << (sf::Uint8)Client::SYNCHRONIZE_GBOARD << this->gboard;
	for (std::vector<sf::TcpSocket*>::iterator it = this->sockets.begin();
		it != this->sockets.end();
		++it) {
		sf::TcpSocket& client = **it;
		client.send(packet);
	}
}
