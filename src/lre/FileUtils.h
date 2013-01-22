/*
 * Lightweight Replace Engine
 * Copyright (C) 2012-2013 Johannes Scholz. All rights reserved.
 *
 * This file is licensed under the GNU Lesser General Public License 3 (LGPLv3),
 * but distributed WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifndef lre_FileUtil_H
#define lre_FileUtil_H 1

#include "Export.h"

//-- STL --//
#include <string>
#include <vector>

namespace lre {

	class LRE_EXPORT FileUtils {
		public:
			// Filename related utils
			static std::string removeExtension(const std::string& filename);
			static std::string extractFilename(const std::string& filename);
			static std::string extractDirectory(const std::string& filename);
			static std::string excludeTrailingSeparator(const std::string& filename);
			static std::string includeTrailingSeparator(const std::string& filename);
			static std::string getNativeEndline();
			static bool makeDirectory(const std::string& path);
			static std::string separator();

			// File related utils
			static std::vector<std::string> findFiles(const std::string& path, const std::string& extension, bool recursive);
			static bool fileExists(const std::string& filename);
			static std::string getFile(const std::string& filename);
			static bool putFile(const std::string& filename, const std::string& text);

		private:
			static std::vector<std::string> getDirectoryContent(const std::string& path);
	};

} // namespace lre

#endif // lre_ReplaceEngine_H
