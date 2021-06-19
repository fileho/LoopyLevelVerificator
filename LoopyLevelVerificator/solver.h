#ifndef SOLVER_H
#define SOLVER_H

#include <string>
#include <vector>


class solver
{
public:
	void solve(const std::vector<std::string>& level, size_t min_moves = 3, size_t max_moves = 6);
	void print() const;
	[[nodiscard]] bool has_solution() const;
	[[nodiscard]] std::string get_code() const;
	[[nodiscard]] size_t solution_length() const;
private:
	std::vector<std::string> map_{};
	std::string code_{};	
	size_t moves_{};


	[[nodiscard]] static size_t make_hash(std::pair<size_t, size_t> robot, size_t ins, size_t checkpoints) noexcept;
	[[nodiscard]] static std::pair<size_t, size_t> get_robot(const std::vector<std::string>& map) noexcept;
	[[nodiscard]] static std::vector<std::pair<size_t, size_t>> get_teleports(const std::vector<std::string>& map);
	static bool movepads(std::pair<size_t, size_t>& robot, char current) noexcept;
	[[nodiscard]] static bool is_wall(char current) noexcept;
	[[nodiscard]] static int checkpoints(const std::vector<std::string>& map) noexcept;
	[[nodiscard]] static bool valid_code(const std::string& code) noexcept;
	[[nodiscard]] static std::string next_code(std::string code) noexcept;
	static void execute_instruction(std::pair<size_t, size_t>& robot, std::string code, std::vector<std::string>& map, size_t current_move);
};

#endif // SOLVER_H

