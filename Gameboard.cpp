#include "Gameboard.h"



Gameboard::Gameboard() : players(nullptr) { }

Gameboard::Gameboard(std::vector<Player>* playerList) {
    this->players = playerList;
}

Gameboard::~Gameboard() { }


void Gameboard::init() {
    this->randomize();
    this->initPlayers();
}

sf::Clock Gameboard::elapsed;

void Gameboard::reset() {
    this->cells.clear();
}

const uint16_t Gameboard::NUMBER = 40;
const float Gameboard::MIN_DIST = 20.f;
const float Gameboard::BORDER = 5.f;
const uint16_t Gameboard::MIN_CAPACITY = 25;
const uint16_t Gameboard::MAX_CAPACITY = 1000;
const float Gameboard::WIDTH = 800.f;
const float Gameboard::HEIGHT = 600.f;

void Gameboard::randomize() {
    bool ok;
    uint64_t security = 0;
    float radius, x, y, dx, dy, spredRadius, freqRot;
    int capacity;
    for (uint32_t i = 0; i < this->NUMBER; ++i) {
        ok = true;
        // (float)rand() / (float)RAND_MAX  ----- between 0 and 1
        radius = (float)rand() / (float)RAND_MAX * 20.f + 20.f;
        x = (float)rand() / (float)RAND_MAX * (this->WIDTH - 2.f * (radius + this->BORDER)) + radius + this->BORDER;
        y = (float)rand() / (float)RAND_MAX * (this->HEIGHT - 2.f * (radius + this->BORDER)) + radius + this->BORDER;
        capacity = rand() % (this->MAX_CAPACITY - this->MIN_CAPACITY - (int)(radius / 2.5f)) + this->MIN_CAPACITY;
        for (uint32_t j = 0; j < this->cells.size(); ++j) {
            dx = this->cells[j].pos.x - x;
            dy = this->cells[j].pos.y - y;
            spredRadius = this->cells[j].radius + radius + this->MIN_DIST;
            if (dx * dx + dy * dy < spredRadius * spredRadius) {
                --i;
                ok = false;
                ++security;
                break;
            }
        }
        if (ok) {
            this->cells.push_back(Cell(radius, x, y, capacity));
            freqRot = ((float)rand() / RAND_MAX - 0.5f);
            this->frequenceRotation.push_back((freqRot != 0.f ? freqRot : 0.01f));
        }
        if (security > this->NUMBER * 10.f) {
            break;
        }
    }
}

void Gameboard::initPlayers() {
    for (uint64_t i = 0; i < this->cells.size(); ++i) {
        this->cells[i].idOwner = 0;
    }
    int c;
    for (uint64_t i = 1; i < (*this->players).size() + 1; ++i) {
        c = rand() % this->cells.size();
        if (this->cells[c].idOwner == 0) {
            this->cells[c].idOwner = i;
            this->cells[c].capacity += 100;
            this->cells[c].units += 100;
        }
        else // for loop with randomize search of condition 
            --i;
    }
}

void Gameboard::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::Texture texture;
    texture.loadFromFile("Textures/bg.png");
    sf::Sprite sprite;
    sprite.setTexture(texture);
    target.draw(sprite);

    sf::Vector2f ratio(this->dot(
        target.getDefaultView().getSize(),
        sf::Vector2f(1.f / this->WIDTH, 1.f / this->HEIGHT)
    ));
    states.transform.scale(ratio);
    for (std::deque<Link>::const_iterator it = this->links.begin(); it != this->links.end(); ++it) {
        sf::Vertex line[] = {
            sf::Vertex(it->getOrgPos()),
            sf::Vertex(it->getDestPos())
        };
        line[0].color = (*this->players)[(uint64_t)it->org->getOwner() - 1].getColor();
        target.draw(line, 2, sf::Lines, states);
        target.draw(*it, states);
    }

    for (uint64_t i = 0; i < this->cells.size(); ++i) {
        sf::CircleShape cell(this->cells[i].radius, 20);
        cell.setOrigin(this->cells[i].radius, cells[i].radius);
        cell.setPosition(this->cells[i].pos);
        cell.setOutlineColor(sf::Color(255, 255, 255));
        cell.setOutlineThickness(2);
        if (this->cells[i].idOwner == 0) {
            cell.setFillColor(Player::DEFAULT_COLOR[0]);
        }
        else {
            cell.setFillColor((*this->players)[this->cells[i].idOwner - 1].getColor());
        }
        cell.setRotation(
            this->elapsed.getElapsedTime().asSeconds() * 6.28f * 
            RAD_TO_DEG * this->frequenceRotation[i]
        );

        target.draw(cell, states);

        sf::Text text;
        text.setFillColor(sf::Color::Black);

        text.setFont(*FontManager::getFont("Textures/JetBreins.ttf"));
        text.setCharacterSize(6 + this->cells[i].radius / 8);
        text.setString("" + ttos(this->cells[i].units) + "/\n/" + ttos(this->cells[i].capacity));
        text.setOrigin(
            text.getLocalBounds().left + text.getGlobalBounds().width / 2.f,
            text.getGlobalBounds().height / 2.f
        );
        text.setPosition(this->cells[i].pos);

        target.draw(text, states);
    }
}

void Gameboard::link(int idOrg, int idDest, float strength) {
    if (idOrg < 0 || idDest < 0 || 
        idOrg >= (int)this->cells.size() ||
        idDest >= (int)this->cells.size() ||
        idOrg == idDest)
        return;

    for (std::deque<Link>::iterator it = this->links.begin(); it < this->links.end(); ++it) {
        // delete trash arrows
        if (&this->cells[idOrg] == it->dest && &this->cells[idDest] == it->org) {
            it->invert();
            return;
        }
        else if (&this->cells[idOrg] == it->org && &this->cells[idDest] == it->dest) {
            this->links.erase(it);
            return;
        }
    }

    this->links.push_back(Link(&this->cells[idOrg], &this->cells[idDest], strength));
}

void Gameboard::update(float dt) {
    for (std::vector<Cell>::iterator it = this->cells.begin(); it != this->cells.end(); ++it) {
        it->update(dt);
    }
    for (std::deque<Link>::iterator it = this->links.begin(); it < this->links.end(); ++it) {
        it->update(dt);
    }
}

// search closer by self radius
int Gameboard::getCloserCell(float x, float y) {
    int id = 0;
    float dx, dy, sr;
    for (std::vector<Cell>::iterator it = this->cells.begin(); it != this->cells.end(); it++) {
        dx = it->pos.x - x;
        dy = it->pos.y - y;
        sr = it->radius;
        if (dx * dx + dy * dy < sr * sr)
            return id;
        ++id;
    }
    return -1;
}

int Gameboard::getCloserCell(sf::Vector2f coord) {
    return this->getCloserCell(coord.x, coord.y);
}

void Gameboard::free(int idPlayer, int idCell) {
    if (idCell < 0 || this->cells[idCell].getOwner() != idPlayer)
        return;

    for (std::deque<Link>::iterator it = this->links.begin(); it < this->links.end();) {
        if (it->org == &this->cells[idCell] || it->dest == &this->cells[idCell]) {
            it = this->links.erase(it);
        }
        else {
            ++it;
        }
    }
}

void Gameboard::changeLinkState(int idOrg, int idDest) {
    for (std::deque<Link>::iterator it = this->links.begin(); it < this->links.end(); ++it) {
        if (it->org == &this->cells[idOrg] && it->dest == &this->cells[idDest]) {
            it->nextState();
            break;
        }
    }
}

int Gameboard::getOwner(int idCell) {
    return this->cells.at(idCell).idOwner;
}

sf::Packet& operator>> (sf::Packet& packet, Gameboard& gboard) {
    sf::Uint8 new_size = 0;
    packet >> new_size;
    gboard.cells.resize(new_size, Cell());
    for (uint64_t i = 0; i < gboard.cells.size(); ++i) {
        packet >> 
            gboard.cells[i].radius >> 
            gboard.cells[i].pos.x >> 
            gboard.cells[i].pos.y >> 
            gboard.cells[i].idOwner >> 
            gboard.cells[i].capacity >> 
            gboard.cells[i].units >> 
            gboard.cells[i].time;
    }
    return packet;
}

sf::Packet& operator<< (sf::Packet& packet, const Gameboard& gboard) {
    sf::Uint8 new_size = gboard.cells.size();
    packet << new_size;
    for (uint64_t i = 0; i < new_size; ++i) {
        packet << 
            gboard.cells[i].radius << 
            gboard.cells[i].pos.x << 
            gboard.cells[i].pos.y << 
            gboard.cells[i].idOwner << 
            gboard.cells[i].capacity << 
            gboard.cells[i].units << 
            gboard.cells[i].time;
    }
    return packet;
}



