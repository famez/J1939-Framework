
#include <QDebug>

#include <TRCReader.h>

#include <J1939Factory.h>
#include <J1939Frame.h>

#include <Transport/TPDTFrame.h>
#include <Transport/TPCMFrame.h>
#include <Transport/BAM/BamConnectionManager.h>

#include "trcloader.h"

using namespace Can;
using namespace J1939;

TRCLoader::TRCLoader(const QString &file, QList< QPair< u32, J1939::J1939Frame* > >* frameList) : mFileName(file), mProgress(0), mFramesList(frameList)
{

}

void TRCLoader::run() {


    TRCReader reader(mFileName.toUtf8().constData());


    if(!reader.isFileLoaded()) {
        qDebug() << "Error reading file";
    }

    BamConnectionManager connManager;

    while (reader.getNumberOfFrames() > 0 && (reader.getCurrentPos() < reader.getNumberOfFrames() - 1)) {

        reader.readNextCanFrame();

        u32 timeStamp = reader.getLastCanFrame().first;

        CanFrame frame(reader.getLastCanFrame().second);

        try {

            std::unique_ptr<J1939Frame> decodedFrame = J1939Factory::getInstance().
                    getJ1939Frame(frame.getId(), (u8*)frame.getData().c_str(), frame.getData().size());


            if(decodedFrame.get()) {
                //qDebug() << "Frame PGN:" << decodedFrame->getPGN() << endl;

                //Check if frame belongs to transport layer
                //TODO: We are assuming that the only transport protocol used here is BAM, but it could be also Connection Mode
                switch (decodedFrame->getPGN()) {
                case TP_DT_PGN:
                case TP_CM_PGN:

                    //Delegate the work to the transport layer
                    connManager.consumeFrame(*decodedFrame);

                    //If transport layer has generated data...
                    if(connManager.getSessionStatus() == SESSION_STAT_OK && connManager.framesPending(LAYER_DATA)) {
                        //We must be aware of unique pointer, release() operation necessary to avoid the destruction of the pointer when getting out of the
                        //scope
                        mFramesList->append(qMakePair(timeStamp, connManager.dequeueFrame(LAYER_DATA).release()));
                    }
                    break;
                default:
                    mFramesList->append(qMakePair(timeStamp, decodedFrame.release()));
                    break;
                }

            } else {
                //qDebug() << "Unknown frame" << endl;
            }

        } catch (J1939DecodeException& e) {
            qDebug() << "Decode error: " << QString::fromStdString(e.getMessage());
        }

        int progress = 100 * reader.getCurrentPos() / reader.getNumberOfFrames();

        if(mProgress != progress) {
            emit progressUpdated(progress);
        }

        mProgress = progress;

    }

}
