#ifndef __SDCARD_H__
#define __SDCARD_H__

#include <util/Util.h>

class SdCard
{
	class SdCardStatic
	{
		friend class SdCard;
		public:
			SdCardStatic();
		private:
	};

	public:

	private:
		SdCard(); //make constructor private so that no instance can be created other than the singleton instance.
		static SdCardStatic sdCardStatic;
};

#endif
