#include <GenericFrame.h>
#include <SPN/SPNNumeric.h>

#include "spnloader.h"

#define NUMBER_OF_SAMPLES       500

using namespace J1939;

SPNLoader::SPNLoader(SPNViewer* viewer, const FrameList* frameList) : mChart(viewer), mFrameList(frameList) {

    qRegisterMetaType< QList<QPointF> >();
    connect(this, SIGNAL(pointsLoaded(QList<QPointF>)), mChart, SLOT(appendPoints(QList<QPointF>)));
}

SPNLoader::~SPNLoader() {
}

void SPNLoader::run() {

    QList<QPointF> points;
    unsigned int size = mFrameList->size();
    unsigned int count = 0;
    unsigned int oldProgress = 0, progress = 0;

    //We only will display 5000 samples
    unsigned int sampleRate = size / NUMBER_OF_SAMPLES;

    for(auto iter = mFrameList->begin(); iter != mFrameList->end(); ++iter) {
        if(iter->second->isGenericFrame()) {
            GenericFrame* frame = static_cast<GenericFrame*>(iter->second);
            if(frame->hasSPN(mChart->getSPNNumber())) {
                const SPNNumeric* spn = static_cast<SPNNumeric*>(frame->getSPN(mChart->getSPNNumber()));
                double value = spn->getFormatedValue();

                if(count % sampleRate == 0) {
                    points.append(QPointF(iter->first, value));
                }

                progress = 100 * count++ / size;

                if(oldProgress != progress) {
//                    emit pointsLoaded(std::move(points));
//                    points.clear();
                    oldProgress = progress;
                }

            }
        }
    }
    emit pointsLoaded(std::move(points));
    points.clear();

}
