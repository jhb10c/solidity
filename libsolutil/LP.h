/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/
// SPDX-License-Identifier: GPL-3.0
#pragma once

#include <libsolutil/Common.h>

#include <boost/rational.hpp>

#include <vector>
#include <variant>

namespace solidity::util
{

/**
 * Constraint of the form
 *  - data[1] * x_1 + data[2] * x_2 + ... <= data[0]  (equality == false)
 *  - data[1] * x_1 + data[2] * x_2 + ...  = data[0]  (equality == true)
 * The set and order of variables is implied.
 */
struct Constraint
{
	std::vector<boost::rational<bigint>> data;
	bool equality = false;

	bool operator<(Constraint const& _other) const;
	bool operator==(Constraint const& _other) const;
};

/**
 * State used when solving an LP problem.
 */
struct SolvingState
{
	std::vector<std::string> variableNames;
	/// Lower and upper bounds for variables (in the sense of >= / <=).
	std::vector<std::array<std::optional<boost::rational<bigint>>, 2>> bounds;
	std::vector<Constraint> constraints;

	bool operator<(SolvingState const& _other) const;
	bool operator==(SolvingState const& _other) const;
	std::string toString() const;
};

enum class LPResult
{
	Unknown,
	Unbounded,
	Feasible,
	Infeasible
};

/**
 * LP solver.
 *
 * Tries to split a given problem into sub-problems and utilizes a cache to quickly solve
 * similar problems.
 */
class LPSolver
{
public:
	std::pair<smtutil::CheckResult, std::map<std::string, boost::rational<bigint>>> check(SolvingState _state);

private:
	// TODO do we need to cache the model?
	std::map<SolvingState, std::pair<LPResult, std::vector<boost::rational<bigint>>>> m_cache;
};

}
