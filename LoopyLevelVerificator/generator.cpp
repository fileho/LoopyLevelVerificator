#include "generator.h"

#include <random>
#include <string>
#include <vector>
#include <iostream>

generator::generator(const size_t width, const size_t height): width_{width}, height_{height} {}

void generator::generate()
{
	create_frame();
	
	char* c = empty_square();
	if (c)
		*c = 'S';

	c = empty_square();
	if (c)
		*c = 'G';

	add_walls(12);
	add_pushers(6);
	add_reset(2);
}

void generator::print()
{
	for (const auto& row : map_)
		std::cout << row << "\n";
}

std::vector<std::string> generator::get_map() const
{
	return map_;
}

std::unique_ptr<std::pair<size_t, size_t>> generator::empty_square_position()
{
	const size_t max_tries = 20;

	for (size_t i = 0; i < max_tries; i++)
	{
		const size_t r = random_number((width_ - 2) * (height_ - 2));

		const size_t w{ r % (width_ - 2) + 1 };
		const size_t h{ r % (height_ - 2) + 1 };

		if (map_[h][w] == empty_)
			return std::make_unique<std::pair<size_t, size_t>>(std::make_pair(h, w));
	}

	return nullptr;
}

char* generator::empty_square()
{
	if (const auto square = empty_square_position(); square)
		return &map_[square->first][square->second];

	return nullptr;
}

void generator::add_walls(const size_t count)
{
	const auto limit = random_number(count);
	
	for (size_t i = 0; i < limit; i++)
		if (char* c = empty_square(); c)
			*c = wall_;
}

void generator::add_pushers(const size_t count)
{
	const std::vector<pusher> pushers{ pusher(-1, 0, 'U'), pusher(1, 0, 'D'), pusher(0, -1, 'L'), pusher(0, 1, 'R')};

	const auto limit = random_number(count);
	
	for (size_t i = 0; i < limit; i++)
	{
		const auto pos = empty_square_position();
		
		if (!pos)
			continue;

		for (const auto& [x, y, c] : pushers)
			if (map_[pos->first + x][pos->second + y] == empty_)
			{
				map_[pos->first][pos->second] = c;
				break;
			}
	}
	
}

void generator::add_reset(const size_t count)
{
	const size_t limit = random_number(count);

	for (size_t i = 0; i < limit; i++)
	{
		if (char* c = empty_square(); c)
			*c = '0';
	}
}

size_t generator::random_number(const size_t max)
{
	const auto dist = std::uniform_int_distribution<size_t>(0, max);
	return dist(rd_);
}

void generator::create_frame()
{
	map_.clear();
	
	std::string full{};
	
	for (size_t i = 0; i < width_; i++)
		full += wall_;

	std::string empty{ wall_ };

	map_.emplace_back(full);
	
	for (size_t i = 0; i < width_ - 2UL; i++)
		empty += empty_;
	empty += wall_;

	for (size_t i = 0; i < height_ - 2; i++)
		map_.emplace_back(empty);
	map_.emplace_back(full);
}
