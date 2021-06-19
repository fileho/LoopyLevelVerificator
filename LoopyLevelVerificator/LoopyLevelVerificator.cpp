#include <iostream>
#include <vector>
#include <string>

#include "solver.h"
#include "generator.h"



int main()
{
    std::vector<std::string> map{};

    // X is a hard wall, G is a Goal, LRDU are pusher to the next square - don't place pusher facing wall it may cause endless loop
    // x is a destructible wall
    // 0 is reset to 0 instruction, . is an empty square
    // C is a checkpoint
    // S is a position of the robot

    map.emplace_back("XXXXXXXXXXX");
    map.emplace_back("X....S....X"); 
    map.emplace_back("X...LXXXX.X");
    map.emplace_back("X....D....X");
    map.emplace_back("X...LLLG..X");
    map.emplace_back("X.........X");
    map.emplace_back("XXXXXXXXXXX");

    generator generator{11, 7};
    solver solver{};

    size_t tries{};
	
    while (true)
    {
        ++tries;
        generator.generate();
        solver.solve(generator.get_map());

    	if (const size_t code_length = solver.get_code().size(); solver.solution_length() > 3 * code_length && code_length > 3)
    	{
            generator.print();
            solver.print();

            std::cout << "Tries " << tries << "\n";
            return 0;
    	}
    }
}


