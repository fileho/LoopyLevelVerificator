#ifndef GENERATOR_H
#define GENERATOR_H

#include <vector>
#include <string>
#include <random>
#include <memory>

class generator
{
public:
	generator(size_t width, size_t height);

	void generate();
	[[nodiscard]] std::vector<std::string> get_map() const;
	void print();
private:
	size_t width_;
	size_t height_;

	const char wall_{ 'X' };
	const char empty_{ '.' };

	std::random_device rd_{};

	std::vector<std::string> map_{};

	std::unique_ptr<std::pair<size_t, size_t>> empty_square_position();
	char* empty_square();
	void add_walls(size_t count);
	void add_pushers(size_t count);
	void add_reset(size_t count);
	[[nodiscard]] size_t random_number(size_t max);
	


	void create_frame();

	struct pusher
	{
		int x;
		int y;
		char dir;
	};

};

#endif