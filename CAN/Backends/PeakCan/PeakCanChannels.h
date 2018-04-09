/*
 * PeakCanChannels.h
 *
 *  Created on: Apr 4, 2018
 *      Author: famez
 */

#ifndef BACKENDS_PEAKCAN_PEAKCANCHANNELS_H_
#define BACKENDS_PEAKCAN_PEAKCANCHANNELS_H_

#include <string>
#include <map>

#include <Singleton.h>

#include "PeakCanSymbols.h"


#define REGISTER_CHANNEL(x, y)		mChannels[x] = Channel(x, y)

namespace Can {
namespace PeakCan {

//Class to represent available channels in PeakCan
	class Channel{
	private:
		std::string mName;
	    TPCANHandle mIndex;
	public:
	    Channel() : mIndex(0) {}
	    Channel(const std::string name, TPCANHandle index) : mName(name), mIndex(index) {}
	    ~Channel() {}

	    const std::string& getName() const { return mName; }
	    TPCANHandle getIndex() const { return mIndex; }
	};

class PeakCanChannels : public ISingleton<PeakCanChannels>{
	SINGLETON_ACCESS;
private:
	PeakCanChannels();
	std::map<std::string, Channel> mChannels;
public:
	virtual ~PeakCanChannels();

	const std::map<std::string, Channel>& getChannels() const { return mChannels; }

	Channel getChannel(const std::string& name) const;


};

} /* namespace PeakCan */
} /* namespace Can */

#endif /* BACKENDS_PEAKCAN_PEAKCANCHANNELS_H_ */
