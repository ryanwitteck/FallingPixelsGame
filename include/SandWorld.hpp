#ifndef SANDWORLD_HPP
#define SANDWORLD_HPP

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <cmath>
#include "CellProperties.hpp"
class SandWorld {
    public:

        const size_t m_width = 0;
        const size_t m_height = 0;

        // Constructor
        SandWorld(size_t width, size_t height);
        
        // Destructor
        ~SandWorld();

        // Gets teh cell at index
        const Cell& GetCell(size_t index);
        
        // Gets the cell at x y
        const Cell& GetCell(size_t x, size_t y);
        
        // Converts an x y coord to a 1D array index
        size_t GetIndex(size_t x, size_t y);
        
        // Checks if x y is within the bounds
        bool InBounds(size_t x, size_t y);
       
        // Checks if the cell at x y is empty
        bool IsEmpty(size_t x, size_t y);
        
        // Checks if the cell at x y is of type giveType
        bool IsType(size_t x, size_t y, CellType giveType);
        
        // Sets the cell at x y to the given cell
        void SetCell(size_t x, size_t y, const Cell& cell);
        
        // Moves a cell from x y to xTo yTo
        void MoveCell(size_t x, size_t y, size_t xTo, size_t yTo);
        
        // Stages the moves for this tick to be executed in a random order
        void CommitCells();
        
        // Renders the window and converts our cells to pixels.
        void Render(sf::RenderWindow& window);
        
        // Wipes the screen blank
        void WipeScreen();
        
        //Moves water in a water like behavior
        void MoveWater(size_t x, size_t y, const Cell& cell);
        
	    // Moves sand cells in a sand-like behavior.
        void MoveSand(size_t x, size_t y, const Cell& cell);
        
        // Moves rocks in a rock-like behavior
        void MoveRock(size_t x, size_t y, const Cell& cell);
        
        // Moves smoke in a smoke-like behavior.
        void MoveSmoke(size_t x, size_t y, const Cell& cell);
        
	    // Moves acid cells in a acid-like behavior.
	    // In other games, acid is not completely invunerable, 
        // so there is a decay aspect here when dissolving other substances, 
        // where the acid itself dissapears, as to not have infinite acid.
        void MoveAcid(size_t x, size_t y, const Cell& cell);
        
        // Does not move god cells in a god-like behavior.
        // May change this in a future implementation.
        void MoveGodBlock(size_t x, size_t y, const Cell& cell);

        // Moves lava cells in a lava-like behavior.
        // May melt or burn other elements.
        void MoveLava(size_t x, size_t y, const Cell& cell);

        // Handles our keyboard and mouse inputs, as well as update our world on each tick,
        // including placing tiles and determining movements for each cell.
        void HandleInputs(sf::RenderWindow& window);

    private:

        // Our array of cells
        Cell* m_cells = nullptr;

        // Our vector of staged moves, from index to index
        std::vector<std::pair<size_t, size_t>> m_changes; 
};

#endif
