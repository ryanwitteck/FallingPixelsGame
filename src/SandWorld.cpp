#include "../include/SandWorld.hpp"
#include <iostream>
// Some global variables to set a baseline for every element.
Cell _EMPTY = {
			CellType::EMPTY,
			sf::Color(0, 0, 0, 0) 
		};
	 
Cell _SAND = {
			CellType::SAND,
			sf::Color(235, 200, 175)
		};
	 
Cell _WATER = {
			CellType::WATER,
			sf::Color(175, 200, 235)
		};
	 
Cell _ROCK = {
			CellType::ROCK,
			sf::Color(100, 100, 100)
		};
Cell _SMOKE = {
			CellType::SMOKE,
			sf::Color(117, 117, 117)
		};
Cell _ACID = {
			CellType::ACID,
			sf::Color(94, 255, 115)
		};
Cell _GODBLOCK{
			CellType::GODBLOCK,
			sf::Color(255,255,255)
		};
Cell _LAVA{
			CellType::LAVA,
			sf::Color(255,60,0)
		};

// Made as a global variable to support toggling the element instead of holding down a button and clicking 
Cell placeMe = _EMPTY;

int PEN_SIZE = 11; //Default 10 size



SandWorld::SandWorld(size_t width, size_t height): m_width (width), m_height (height) {
    m_cells = new Cell[m_width * m_height];
}

SandWorld::~SandWorld(){
    delete[] m_cells;
}


void SandWorld::WipeScreen(){
    Cell _EMPTY = {
			CellType::EMPTY,
			sf::Color(0, 0, 0, 0) // 0 alpha allows for a background
		};
    for(int i = 0; i < m_width; i++){
        for(int j = 0; j < m_height; j++){
            SetCell(i, j, _EMPTY);
        }
    }
}


const Cell& SandWorld::GetCell(size_t index){
    return m_cells[index];
}

const Cell& SandWorld::GetCell(size_t x, size_t y){
    return GetCell(GetIndex(x,y));
}

size_t SandWorld::GetIndex(size_t x, size_t y){
    return x + y * m_width;
}

bool SandWorld::InBounds(size_t x, size_t y){
    return x < m_width && y < m_height;
}

bool SandWorld::IsEmpty(size_t x, size_t y){
    return InBounds(x, y) && GetCell(x, y).type == CellType::EMPTY;
}

bool SandWorld::IsType(size_t x, size_t y, CellType giveType){
    return InBounds(x, y) && GetCell(x, y).type == giveType;
}

void SandWorld::SetCell(size_t x, size_t y, const Cell& cell){
    m_cells[GetIndex(x, y)] = cell;
}

void SandWorld::MoveCell(size_t x, size_t y, size_t xTo, size_t yTo){
    m_changes.emplace_back(GetIndex(xTo, yTo), GetIndex(x, y));
    
}

void SandWorld::CommitCells(){
    for(size_t i = 0; i < m_changes.size(); i++){
        if (m_cells[m_changes[i].first].type != CellType::EMPTY){
            m_changes[i] = m_changes.back();
            m_changes.pop_back();
            i--;
		}
    }

    std::sort(m_changes.begin(), m_changes.end()), [](auto& a, auto& b){
        return a.first < b.first;
    };

    size_t iprev = 0;

    m_changes.emplace_back(-1, -1);

    for(size_t i = 0; i < m_changes.size(); i++){
        if(m_changes[i+1].first != m_changes[i].first){
            size_t rrand = iprev + (int)(std::rand() / RAND_MAX) * (i - iprev);
            
            size_t dst = m_changes[rrand].first;
            size_t src = m_changes[rrand].second;

            m_cells[dst] = m_cells[src];
            m_cells[src] = Cell();

            iprev = i + 1;
        }
    }
    m_changes.clear();
}
	 

void SandWorld::MoveWater(size_t x, size_t y, const Cell& cell){
	if (IsType(x, y + 1, CellType::EMPTY)) {
		MoveCell(x, y, x, y + 1);
		return;
	}
	bool downLeft  = IsEmpty(x - 1, y + 1);
	bool downRight = IsEmpty(x + 1, y + 1);
	bool left  = IsEmpty(x - 1, y);
	bool right = IsEmpty(x + 1, y);

	if (downLeft && downRight) {
		downLeft  = (rand() % 2) != 0;
		downRight = !downLeft;
	}

	if (downLeft)  {
		MoveCell(x, y, x - 1, y);
	}
			
	else if (downRight) {
		MoveCell(x, y, x + 1, y );
	}
	
	if (left && right) {
		left  = (rand() % 2) != 0;
		right = !left;
	}
	
	if (right && (rand() % 2) != 0) {
		MoveCell(x, y, x + 1, y );
	}
			
	else if (left && (rand() % 2) != 0)  {
		MoveCell(x, y, x - 1, y);
	}
	
	else if (IsType(x, y + 1, CellType::SMOKE) && (rand() % 2) != 0){
		Cell water = GetCell(x, y +1);
		SetCell(x, y + 1, cell);
		SetCell(x, y, water);
	}
}


void SandWorld::MoveAcid(size_t x, size_t y, const Cell& cell){
	if (IsType(x, y + 1, CellType::EMPTY)) {
		MoveCell(x, y, x, y + 1);
	}

	else if(IsType(x, y + 1, CellType::ACID)){
		bool downLeft  = IsEmpty(x - 1, y + 1);
		bool downRight = IsEmpty(x + 1, y + 1);
		bool left  = IsEmpty(x - 1, y);
		bool right = IsEmpty(x + 1, y);
		
		if (downLeft && downRight) {
		downLeft  = (rand() % 2) != 0;
			downRight = !downLeft;
		}

		if (downLeft && (rand() % 2) != 0)  {
			MoveCell(x, y, x - 1, y + 1);
		}
		
		else if (downRight && (rand() % 2) != 0) {
			MoveCell(x, y, x + 1, y + 1);
		}
		
		if (left && right) {
			left  = (rand() % 2) != 0;
			right = !left;
		}
		
		if (right && (rand() % 2) != 0) {
			MoveCell(x, y, x + 1, y );
		}
		
		else if (left && (rand() % 2) != 0)  {
			MoveCell(x, y, x - 1, y);
		}

	}

	//Left
	if ((IsType(x-1, y, CellType::SMOKE) || IsType(x-1, y, CellType::WATER) || IsType(x-1, y, CellType::ROCK) || IsType(x-1, y, CellType::SAND) ||  IsType(x-1, y, CellType::LAVA)) && (rand() % 5) != 0){	
		SetCell(x-1, y, _EMPTY);
		if((rand() % 5) != 0){
			SetCell(x, y, _EMPTY);
		}
	}
	//Right
	if ((IsType(x+1, y, CellType::SMOKE) || IsType(x+1, y, CellType::WATER) || IsType(x+1, y, CellType::ROCK) || IsType(x+1, y, CellType::SAND) || IsType(x+1, y, CellType::LAVA)) && (rand() % 5) != 0){
		SetCell(x+1, y, _EMPTY);
		if((rand() % 5) != 0){
			SetCell(x, y, _EMPTY);
		}
	}
	//Below
	if ((IsType(x, y + 1, CellType::SMOKE) || IsType(x, y + 1, CellType::WATER) || IsType(x, y + 1, CellType::ROCK) || IsType(x, y + 1, CellType::SAND)|| IsType(x, y + 1, CellType::LAVA)) && (rand() % 2) != 0){
		SetCell(x, y + 1, _EMPTY);
		if((rand() % 5) != 0){
			SetCell(x, y, _EMPTY);
		}
	}
	//Above
	if ((IsType(x, y - 1, CellType::SMOKE) || IsType(x, y - 1, CellType::WATER) || IsType(x, y - 1, CellType::ROCK) || IsType(x, y - 1, CellType::SAND) || IsType(x, y - 1, CellType::LAVA)) && (rand() % 5) != 0){
		SetCell(x, y - 1, _EMPTY);
		if((rand() % 5) != 0){
			SetCell(x, y, _EMPTY);
		}
	}
}

void SandWorld::MoveSand(size_t x, size_t y, const Cell& cell){
		
	if (IsType(x, y + 1, CellType::EMPTY)) {
		MoveCell(x, y, x, y + 1);
	}

	else if(IsType(x, y + 1, CellType::SAND)){
		bool downLeft  = IsEmpty(x - 1, y + 1);
		bool downRight = IsEmpty(x + 1, y + 1);

		if (downLeft && downRight) {
			downLeft  = (rand() % 2) != 0;
			downRight = !downLeft;
		}

		if (downLeft && (rand() % 2) != 0)  {
			MoveCell(x, y, x - 1, y + 1);
		}
		
		else if (downRight && (rand() % 2) != 0) {
			MoveCell(x, y, x + 1, y + 1);
		}
	}

	else if (IsType(x, y + 1, CellType::WATER) && (rand() % 2) != 0){
		Cell water = GetCell(x, y +1);
		SetCell(x, y + 1, cell);
		SetCell(x, y, water);
	}

	else if (IsType(x, y + 1, CellType::SMOKE) && (rand() % 2) != 0){
		Cell water = GetCell(x, y +1);
		SetCell(x, y + 1, cell);
		SetCell(x, y, water);
	}
}


void SandWorld::MoveRock(size_t x, size_t y, const Cell& cell){
		
	if (IsType(x, y + 1, CellType::EMPTY)) {
		MoveCell(x, y, x, y + 1);
	}
		
	else if (IsType(x, y + 1, CellType::WATER) && (rand() % 2) != 0){
		Cell water = GetCell(x, y +1);
		SetCell(x, y + 1, cell);
		SetCell(x, y, water);
	}

	else if (IsType(x, y + 1, CellType::SMOKE) && (rand() % 2) != 0){
		Cell water = GetCell(x, y +1);
		SetCell(x, y + 1, cell);
		SetCell(x, y, water);
	}
}


void SandWorld::MoveSmoke(size_t x, size_t y, const Cell& cell){
		
	if (IsType(x, y - 1, CellType::EMPTY)) {
		MoveCell(x, y, x, y - 1);
		return;
	}
		
	bool downLeft  = IsEmpty(x - 1, y + 1);
	bool downRight = IsEmpty(x + 1, y + 1);
		
	bool left  = IsEmpty(x - 1, y  );
	bool right = IsEmpty(x + 1, y);
	
	if (downLeft && downRight) {
		downLeft  = (rand() % 2) != 0;
		downRight = !downLeft;
	}
		
	if (downLeft)  {
		MoveCell(x, y, x - 1, y);
	}
		
	else if (downRight) {
		MoveCell(x, y, x + 1, y );
	}
		
	if (left && right) {
		left  = (rand() % 2) != 0;
		right = !left;
	}
		
	if (right && (rand() % 2) != 0) {
		MoveCell(x, y, x + 1, y );
	}
		
	else if (left && (rand() % 2) != 0)  {
		MoveCell(x, y, x - 1, y);
	}
}


void SandWorld::MoveGodBlock(size_t x, size_t y, const Cell& cell){
	// Currently does nothing and does not react to any other cell.

}
void SandWorld::MoveLava(size_t x, size_t y, const Cell& cell){
	if (IsType(x, y + 1, CellType::EMPTY)) {
		MoveCell(x, y, x, y + 1);
	}

	else if(IsType(x, y + 1, CellType::LAVA)){
		bool downLeft  = IsEmpty(x - 1, y + 1);
		bool downRight = IsEmpty(x + 1, y + 1);
		bool left  = IsEmpty(x - 1, y);
		bool right = IsEmpty(x + 1, y);
		
		if (downLeft && downRight) {
		downLeft  = (rand() % 2) != 0;
			downRight = !downLeft;
		}

		if (downLeft && (rand() % 2) != 0)  {
			MoveCell(x, y, x - 1, y + 1);
		}
		
		else if (downRight && (rand() % 2) != 0) {
			MoveCell(x, y, x + 1, y + 1);
		}
		
		if (left && right) {
			left  = (rand() % 2) != 0;
			right = !left;
		}
		
		if (right && (rand() % 2) != 0) {
			MoveCell(x, y, x + 1, y );
		}
		
		else if (left && (rand() % 2) != 0)  {
			MoveCell(x, y, x - 1, y);
		}

	}

	//Left
	if ((IsType(x-1, y, CellType::SMOKE) || IsType(x-1, y, CellType::WATER) || IsType(x-1, y, CellType::ROCK) || IsType(x-1, y, CellType::SAND)) && (rand() % 5) != 0){	
		if(IsType(x-1, y, CellType::WATER) && (rand() % 7) != 0){
			SetCell(x -1, y, _ROCK);
		}
		else{
			SetCell(x-1, y, _EMPTY);
		}

		if((rand() % 9) != 0){
			SetCell(x, y, _EMPTY);
		}
	}
	//Right
	if ((IsType(x+1, y, CellType::SMOKE) || IsType(x+1, y, CellType::WATER) || IsType(x+1, y, CellType::ROCK) || IsType(x+1, y, CellType::SAND)) && (rand() % 5) != 0){
		if(IsType(x+1, y, CellType::WATER) && (rand() % 7) != 0){
			SetCell(x +1, y, _ROCK);
		}
		else{
			SetCell(x+1, y, _EMPTY);
		}
		
		if((rand() % 9) != 0){
			SetCell(x, y, _EMPTY);
		}
	}
	//Below
	if ((IsType(x, y + 1, CellType::SMOKE) || IsType(x, y + 1, CellType::WATER) || IsType(x, y + 1, CellType::ROCK) || IsType(x, y + 1, CellType::SAND)) && (rand() % 2) != 0){
		if(IsType(x, y+1, CellType::WATER) && (rand() % 7) != 0){
			SetCell(x, y+1, _ROCK);
		}
		else{
			SetCell(x, y+1, _EMPTY);
		}
		if((rand() % 9) != 0){
			SetCell(x, y, _EMPTY);
		}
	}
	//Above
	if ((IsType(x, y - 1, CellType::SMOKE) || IsType(x, y - 1, CellType::WATER) || IsType(x, y - 1, CellType::ROCK) || IsType(x, y - 1, CellType::SAND)) && (rand() % 5) != 0){
		if(IsType(x, y-1, CellType::WATER) && (rand() % 7) != 0){
			SetCell(x, y-1, _ROCK);
		}
		else{
			SetCell(x, y-1, _EMPTY);
		}
		if((rand() % 9) != 0){
			SetCell(x, y, _EMPTY);
		}
	}
}


void SandWorld::Render(sf::RenderWindow& window){
   
    sf::Uint8* pixels = new sf::Uint8[m_width * m_height * 4];
	
	sf::Texture texture;
	texture.create(m_width, m_height);
	
	for(int y = 0; y < m_height; y++){
		for(int x = 0; x < m_width; x++){
			pixels[4 * (y * m_width + x)] = GetCell(x,y).color.r;
			pixels[4 * (y * m_width + x) + 1] = GetCell(x,y).color.g;
			pixels[4 * (y * m_width + x) + 2] = GetCell(x,y).color.b;
			pixels[4 * (y * m_width + x) + 3] = GetCell(x,y).color.a;
		}
	}
	
	texture.update(pixels);
	
	sf::Sprite sprite;
	sprite.setTexture(texture);
	
	window.draw(sprite);
}


void SandWorld::HandleInputs(sf::RenderWindow& window){

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
		WipeScreen();
	}
	sf::Vector2i globalPosition = sf::Mouse::getPosition(window);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) placeMe = _SAND;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) placeMe = _WATER;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) placeMe = _ROCK;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) placeMe = _SMOKE;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) placeMe = _ACID;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)) placeMe = _GODBLOCK;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7)) placeMe = _EMPTY;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8)) placeMe = _LAVA;

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LBracket) && PEN_SIZE > 3) PEN_SIZE -=3;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::RBracket) && PEN_SIZE < 31) PEN_SIZE +=3;



	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		for (size_t x = globalPosition.x - (PEN_SIZE /2); x < globalPosition.x + (PEN_SIZE/2); x++){
			for (size_t y = globalPosition.y - (PEN_SIZE /2); y < globalPosition.y + (PEN_SIZE/2); y++) {
				if (!InBounds(x, y)) continue;
				if(placeMe.type == CellType::EMPTY){
					SetCell(x, y, placeMe);
				}
				else if(IsEmpty(x, y)){
            		SetCell(x, y, placeMe);
				}
			}
		}
	}
	
	for (size_t x = 0; x < m_width;  x++) {
		for (size_t y = 0; y < m_height; y++) {
			const Cell& cell = GetCell(x, y);
			switch (cell.type)  {
    			case CellType::SAND:
       				MoveSand(x, y, cell);
        			break;
    			case CellType::WATER:
       				MoveWater(x, y, cell);
        			break;
				case CellType::ROCK:
       				MoveRock(x, y, cell);
        			break;
				case CellType::SMOKE:
       				MoveSmoke(x, y, cell);
        			break;
				case CellType::ACID:
       				MoveAcid(x, y, cell);
        			break;
				case CellType::GODBLOCK:
       				MoveGodBlock(x, y, cell);
        			break;
				case CellType::LAVA:
       				MoveLava(x, y, cell);
        			break;
				default:
					break;
			}
		}
	}

	CommitCells();

}




