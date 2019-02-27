/*
 * SPNHistory.h
 *
 *  Created on: 12 Feb 2019
 *      Author: fernando
 *
 *      For the moment, it only supports history for SPN status and SPN numeric
 */

#ifndef J1939_HISTORIC_SPNHISTORY_H_
#define J1939_HISTORIC_SPNHISTORY_H_

#include <memory>
#include <vector>

#include <Utils.h>
#include <Types.h>

#include <SPN/SPN.h>
#include <SPN/SPNSpec/SPNSpec.h>
#include <SPN/SPNSpec/SPNNumericSpec.h>
#include <SPN/SPNSpec/SPNStatusSpec.h>


namespace J1939 {

class SPNHistory {
public:
	std::shared_ptr<const SPNSpec> mGeneralSpec;
	struct {
		std::shared_ptr<const SPNNumericSpec> numeric;
		std::shared_ptr<const SPNStatusSpec> status;
	} mSpecificSpec;

	class Sample {
	private:
		Utils::TimeStamp mTimeStamp;
		union {
			double numeric;
			u8 status;
		} mValue;
	public:
		Sample() = default;
		Sample(const Utils::TimeStamp timeStamp, double value) : mTimeStamp(timeStamp) {
			mValue.numeric = value;
		}
		Sample(const Utils::TimeStamp timeStamp, u8 status) : mTimeStamp(timeStamp) {
			mValue.status = status;
		}
		
		void setTimeStamp(const Utils::TimeStamp& timestamp) { mTimeStamp = timestamp; }

		const Utils::TimeStamp& getTimeStamp() const {
			return mTimeStamp;
		}

		double getNumeric() const { return mValue.numeric; }

		u8 getStatus() const { return mValue.status; }
		
		bool operator<(const Sample& other) const { return mTimeStamp < other.mTimeStamp; }

	};

private:
	std::vector<Sample> mSamples;				//All the samples that have been stored
public:
	SPNHistory();
	virtual ~SPNHistory();

	void addSample(const Utils::TimeStamp& timeStamp, const SPN& spn);

	/*
	 * Gets the window of stored samples.
	 * [in] milliseconds indicates the duration of the window in millis
	 * [in] samples indicates the number of samples contained in the window
	 * [in] timeStamp indicates the final timestamp of the window
	 */
	std::vector<Sample> getWindow(const Utils::TimeStamp& timeStamp, u32 milliseconds, u32 samples) const;


};

} /* namespace J1939 */

#endif /* J1939_HISTORIC_SPNHISTORY_H_ */
