#include "solver.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

void solver::execute_instruction(std::pair<size_t, size_t>& robot, std::string code, std::vector<std::string>& map, const size_t current_move) {
	switch (const auto move = code[current_move % code.size()]; move) {
	case 'L':
		if (!is_wall(map[robot.first][robot.second - 1]))
			--robot.second;
		break;
	case 'R':
		if (!is_wall(map[robot.first][robot.second + 1]))
			++robot.second;
		break;
	case 'U':
		if (!is_wall(map[robot.first - 1][robot.second]))
			--robot.first;
		break;
	case 'D':
		if (!is_wall(map[robot.first + 1][robot.second]))
			++robot.first;
		break;
	case 'N':
		break;
	case 'E':
		if ((map[robot.first + 1][robot.second] == 'x'))
			map[robot.first + 1][robot.second] = '.';
		if ((map[robot.first - 1][robot.second] == 'x'))
			map[robot.first - 1][robot.second] = '.';
		if ((map[robot.first][robot.second + 1] == 'x'))
			map[robot.first][robot.second + 1] = '.';
		if ((map[robot.first][robot.second - 1] == 'x'))
			map[robot.first][robot.second - 1] = '.';
		break;
	default:
		break;
	}
}

void solver::solve(const std::vector<std::string>& level, const size_t min_moves, size_t max_moves) {
    map_ = level;
    moves_ = 0;
    code_ = "";

    auto robot = get_robot(level);

    const auto start = robot;
    const int checkpoint = checkpoints(level);
    auto teleports = get_teleports(level);


    for (size_t i = min_moves; i <= max_moves; ++i)
    {
        std::string code{};

        for (size_t j = 0; j < i; ++j)
            code += 'R';

        std::pair<std::string, size_t> solution = std::make_pair("", SIZE_MAX);

        while (valid_code(code)) {
            robot = start;
            std::vector<std::string> map = level;
            size_t checkpts = checkpoint;

            std::unordered_set<size_t> explored{};

            size_t current_move{};
            size_t offset{};

            auto hash = make_hash(robot, current_move % code.size(), checkpts);
        	
            while (explored.find(hash) == explored.end()) {
                explored.insert(hash);

                char current = map[robot.first][robot.second];

                while (movepads(robot, current))
                    current = map[robot.first][robot.second];


                if (current == 'G' && !checkpts) {
                    if (current_move - offset < solution.second)
                        solution = std::make_pair(code, static_cast<int>(current_move - offset));
                    break;
                }

                switch (current) {
                case 'C':
                    map[robot.first][robot.second] = 'c';
                    --checkpts;
                    break;
                case '0':
                    offset += code.size() - (current_move % code.size());
                    current_move += code.size() - (current_move % code.size());
                	break;
                case 'T':
                    if (robot == teleports[0])
	                    robot = teleports[1];
                    else
	                    robot = teleports[0];
                    break;
                default:;
                	break;
                }

                execute_instruction(robot, code, map, current_move);

                ++current_move;
                hash = make_hash(robot, current_move % code.size(), checkpts);
            }

            code = next_code(code);
        }

        if (solution.second < SIZE_MAX) {
            code_ = solution.first;
            moves_ = solution.second;
            return;
        }
    }
}

void solver::print() const {
    if (has_solution())
        std::cout << "Victory " << code_ << " " << moves_ << '\n';
    else
        std::cout << "No solution found\n";
}

bool solver::has_solution() const
{
    return moves_ != 0;
}

std::string solver::get_code() const
{
    return code_;
}

size_t solver::solution_length() const
{
    return moves_;
}

size_t solver::make_hash(std::pair<size_t, size_t> robot, size_t ins, size_t checkpoints) noexcept {
    return robot.first | robot.second << 4 | ins << 8 | checkpoints << 12;
}

std::pair<size_t, size_t> solver::get_robot(const std::vector<std::string>& map) noexcept {
    for (std::size_t i{}; i < map.size(); ++i) {
        for (size_t j{}; j < map[i].size(); ++j) {
            if (map[i][j] == 'S')
                return std::make_pair(i, j);
        }
    }

    return std::make_pair(0, 0);
}

std::vector<std::pair<size_t, size_t>> solver::get_teleports(const std::vector<std::string>& map) {
    std::vector<std::pair<size_t, size_t>> ret{};

    for (size_t i = 0; i < map.size(); ++i)
	    for (size_t j = 0; j < map[0].size(); ++j)
		    if (map[i][j] == 'T')
			    ret.emplace_back(i, j);

    return ret;
}

bool solver::movepads(std::pair<size_t, size_t>& robot, const char current) noexcept {
    switch (current) {
    case 'L':
        --robot.second;
        return true;
    case 'R':
        ++robot.second;
        return true;
    case 'U':
        --robot.first;
        return true;
    case 'D':
        ++robot.first;
        return true;
    default:
        return false;
    }
}

bool solver::is_wall(const char current) noexcept {
    return current == 'X' || current == 'x';
}

int solver::checkpoints(const std::vector<std::string>& map) noexcept {
    int ret{};

    for (auto&& x : map)
    {
        for (auto&& y : x)
        {
            if (y == 'C')
                ++ret;
        }
    }

    return ret;
}

bool solver::valid_code(const std::string& code) noexcept {
    return std::ranges::any_of(code, [](const char c) { return c != 'N'; });
}

std::string solver::next_code(std::string code) noexcept {
    for (std::size_t i{ 0 }; i < code.size(); ++i)
    {
        if (code[i] == 'N')
            continue;

        switch (code[i]) {
        case 'R':
            code[i] = 'U';
            break;
        case 'U':
            code[i] = 'L';
            break;
        case 'L':
            code[i] = 'D';
            break;
        case 'D':
            code[i] = 'E';
            break;
        case 'E':
            code[i] = 'N';
            break;
        default:
            break;
        }

        for (std::size_t j{}; j < i; ++j)
            code[j] = 'R';

        return code;
    }

    return code;
}