#include "game_of_life.hpp"

int main()
{   
    GameOfLifeApplication   app;

    try
    {
        app.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return (EXIT_FAILURE);
    }
    

    return EXIT_SUCCESS;
}