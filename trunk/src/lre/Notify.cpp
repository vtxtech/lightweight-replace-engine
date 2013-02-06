/*
 * Lightweight Replace Engine
 * Copyright (C) 2012-2013 Johannes Scholz. All rights reserved.
 *
 * This file is licensed under the GNU Lesser General Public License 3 (LGPLv3),
 * but distributed WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * @brief Inspired by OpenSceneGraph's Notify class
 *
 */

#include "Notify.h"

//-- STL --//
#include <sstream>

namespace lre {

	class NoOutputStreamBuffer : public std::streambuf
	{
	private:
		std::streamsize xsputn(const std::streambuf::char_type *str, std::streamsize n)
		{
			return n;
		}
	};

	struct NoOutputStream : public std::ostream
	{
	public:
		NoOutputStream(): std::ostream(new NoOutputStreamBuffer) {
			mybuffer_ = dynamic_cast<NoOutputStreamBuffer *>(rdbuf());
		}

		~NoOutputStream() {
			rdbuf(0);
			delete mybuffer_;
		}

	protected:
		NoOutputStreamBuffer* mybuffer_;
	};

	struct NotifySettings
	{
		NotifySettings()
		{
			notifyLevel_ = lre::NOTICE;
		}

		lre::NotifySeverity notifyLevel_;
		
		// dummy stream for zero-output
		std::stringstream noOutputStream_;
	};

	static NotifySettings& getNotifySettings()
	{
		static NotifySettings s_NotifySettings;
		return s_NotifySettings;
	};

	void lre::setNotifyLevel(lre::NotifySeverity severity)
	{
		getNotifySettings().notifyLevel_ = severity;
	}

	lre::NotifySeverity lre::getNotifyLevel()
	{
		return getNotifySettings().notifyLevel_;
	}

	std::ostream& notify(const lre::NotifySeverity severity)
	{
		if (severity == lre::ERROR) { 
			return std::cerr;
		} else {
			if (severity < getNotifyLevel()) {
				return getNotifySettings().noOutputStream_;
			} else {
				return std::cout;
			}
		}
	}

} // namespace lre
