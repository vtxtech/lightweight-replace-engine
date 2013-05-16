/*
 * Lightweight Replace Engine
 * Copyright (C) 2012-2013 Johannes Scholz. All rights reserved.
 *
 * This file is licensed under the GNU Lesser General Public License 3 (LGPLv3),
 * but distributed WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * @brief Convenience functions for setting std::cout color.
 *
 */

#ifndef lre_ConsoleColor_H
#define lre_ConsoleColor_H 1

#include <iostream>

std::ostream& blue(std::ostream &s);
std::ostream& red(std::ostream &s);
std::ostream& green(std::ostream &s);
std::ostream& yellow(std::ostream &s);
std::ostream& white(std::ostream &s);

#endif // lre_ConsoleColor_H
