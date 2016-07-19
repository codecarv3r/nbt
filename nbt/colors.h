/*
 *   ___    __ __  ____ ________
 *  |   \  |  |  |/ _  \        |
 *  |    \ |  |    (_) /__    __|
 *  |  |\ \|  |     _  \  |  |
 *  |  | \    |    (_) |  |  |
 *  |__|  \___|__|\____/  |__|
 *
 *  colors.h
 *  This file is part of nbt.
 *
 *  Created by Silas Schwarz on 7/14/16.
 *  Copyright (c) 2016 ZCodeMT LLC.
 *
 *  nbt is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  nbt is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with nbt.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef colors_h
#define colors_h

#ifndef NBT_USE_COLORS
#define NBT_USE_COLORS 1
#endif /* NBT_USE_COLORS */

#if NBT_USE_COLORS

#define RESET			"\x1b[00m"

#define BLACK			"\x1b[30m"
#define RED				"\x1b[31m"
#define GREEN			"\x1b[32m"
#define YELLOW			"\x1b[33m"
#define BLUE			"\x1b[34m"
#define MAGENTA			"\x1b[35m"
#define CYAN			"\x1b[36m"
#define LIGHT_BLACK		"\x1b[390m"
#define LIGHT_RED		"\x1b[91m"
#define LIGHT_GREEN		"\x1b[92m"
#define LIGHT_YELLOW	"\x1b[92m"
#define LIGHT_BLUE		"\x1b[94m"
#define LIGHT_MAGENTA	"\x1b[95m"
#define LIGHT_CYAN		"\x1b[96m"
#define LIGHT_WHITE		"\x1b[97m"

#define XBLACK		"\x1b[38;5;000m"
#define XRED		"\x1b[38;5;001m"
#define XGREEN		"\x1b[38;5;002m"
#define XYELLOW		"\x1b[38;5;003m"
#define XBLUE		"\x1b[38;5;004m"
#define XMAGENTA	"\x1b[38;5;005m"
#define XCYAN		"\x1b[38;5;006m"
#define XWHITE		"\x1b[38;5;007m"
#define XLBLACK		"\x1b[38;5;008m"
#define XLRED		"\x1b[38;5;009m"
#define XLGREEN		"\x1b[38;5;010m"
#define XLYELLOW	"\x1b[38;5;011m"
#define XLBLUE		"\x1b[38;5;012m"
#define XLMAGENTA	"\x1b[38;5;013m"
#define XLCYAN		"\x1b[38;5;014m"
#define XLWHITE		"\x1b[38;5;015m"

#define XRRED		"\x1b[38;5;001m"
#define XRORANGE	"\x1b[38;5;166m"
#define XRYELLOW	"\x1b[38;5;003m"
#define XRGREEN		"\x1b[38;5;002m"
#define XRBLUE		"\x1b[38;5;020m"
#define XRPURPLE	"\x1b[38;5;093m"

#else

#define RESET			""

#define BLACK			""
#define RED				""
#define GREEN			""
#define YELLOW			""
#define BLUE			""
#define MAGENTA			""
#define CYAN			""
#define LIGHT_BLACK		""
#define LIGHT_RED		""
#define LIGHT_GREEN		""
#define LIGHT_YELLOW	""
#define LIGHT_BLUE		""
#define LIGHT_MAGENTA	""
#define LIGHT_CYAN		""
#define LIGHT_WHITE		""

#define XBLACK		""
#define XRED		""
#define XGREEN		""
#define XYELLOW		""
#define XBLUE		""
#define XMAGENTA	""
#define XCYAN		""
#define XWHITE		""
#define XLBLACK		""
#define XLRED		""
#define XLGREEN		""
#define XLYELLOW	""
#define XLBLUE		""
#define XLMAGENTA	""
#define XLCYAN		""
#define XLWHITE		""

#define XRRED		""
#define XRORANGE	""
#define XRYELLOW	""
#define XRGREEN		""
#define XRBLUE		""
#define XRPURPLE	""

#endif /* NBT_USE_COLORS */

#endif /* colors_h */
