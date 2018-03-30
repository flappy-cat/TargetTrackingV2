#include "videoencodeh264.h"
#include <unistd.h>
#include <QDebug>
#include "globalsettings.h"
#include "datamanager.h"
#include "log4cat.h"
#include"comutils.h"

VideoEncodeH264::VideoEncodeH264(QObject* parent):QThread(parent)
{
#if 0
    filename_out = "/home/nvidia/applogs/ds.h264";
    fp_out = NULL;
#endif
    pUdpSendInstance = NULL;

    pCodec = NULL;
    pCodecCtx = NULL;

    framecnt = 0;
    capFrameCnt = 0;

    data_y = NULL;
    data_u = NULL;
    data_v = NULL;
    bufVideoSend.fill (0);
    bufVideoSend.resize (2048);
}


VideoEncodeH264::~VideoEncodeH264 ()
{

}

void VideoEncodeH264::run()
{
    StartEncodeLoop ();
}



void VideoEncodeH264::StartEncodeLoop ()
{
    GlobalSettings *pGlobalSetting = GlobalSettings::GetInstance ();
    DataManager *pDataManager = DataManager::GetInstance ();

    while(1)
    {



        pDataManager->bIsStartVideoCapture = true;
        while(!cap.isOpened ())
        {
#if FLAPPYDEBUG
            qDebug()<<"Attempt to open camera";
#endif
            if(pDataManager->videoSrc == 0x01)
            {

                cap.open (1);
                if(cap.isOpened ())
                {
                    qDebug()<<"OPEN CCD CAM SUCESS!";
                }
                else
                {
                    qDebug()<<"OPEN CCD CAM FAILED!";
                }

            }
            else if(pDataManager->videoSrc == 0x02)
            {
                cap.open (7);

                if(cap.isOpened ())
                {
                    qDebug()<<"OPEN INFRA CAM SUCESS!";
                }
                else
                {
                    qDebug()<<"OPEN INFRA CAM FAILED!";
                }
            }
        }

        GlobalInit();
        InitFFmpeg();
        int size = /*capFrame.cols * capFrame.rows*/640 * 480;
        data_y = (unsigned char * )malloc(sizeof(char) * size);
        data_u = (unsigned char * )malloc(sizeof(char) * size / 4);
        data_v = (unsigned char * )malloc(sizeof(char) * size / 4);

        //cvNamedWindow ("ssss");
        //namedWindow ("ssss");
        while(1)
        {

            //static int filenum = 0;
            //char ss[100];
            //memset(ss,0,100);
            //sprintf(ss,"/home/nvidia/pic%d.bmp",filenum++);
            //m_timeStart = m_timeEnd;
            //m_timeEnd = QTime::currentTime ();

           //qDebug()<<"One whole frame = "<<m_timeEnd.msecsTo (m_timeStart);

            if(!pDataManager->bIsStartVideoCapture)
            {


                cap.release ();

                capFrameCnt = 0;
#if 0
                if(fp_out != NULL)
                {
                    fflush(fp_out);
                    fclose(fp_out);
                    fp_out = NULL;
                }
#endif
                if(data_y != NULL)
                {
                    free(data_y);
                    data_y = NULL;
                }
                if(data_u != NULL)
                {
                    free(data_u);
                    data_u = NULL;
                }
                if(data_v != NULL)
                {
                    free(data_v);
                    data_v = NULL;
                }
                CloseFFmpeg ();
                //EASYLOG("CLOSE FFMPEG ENCODER");
                break;
            }

            cap >> capFrame;

            if(pDataManager->bIsStartTracking == true)
            {
                if(pDataManager->bIsResetTracking == true)
                {
                    //Visible reletive algorium enabled when visibleVideo is on and night vision is closed
                    if((pDataManager->videoSrc == 0x01) && (pDataManager->dayNightReverse == 0x55))
                    {
                            trackAlgorithm.MatchAndTrackInit (pDataManager->recvTargetRect,capFrame,pDataManager->featureVisible,50,1);

                            trackAlgorithm.ReadCalibrateFile (1, pDataManager->focusZoom);
                    }
                    else if((pDataManager->videoSrc == 0x01) && (pDataManager->dayNightReverse == 0xAA))
                    {
                        trackAlgorithm.MatchAndTrackInit (pDataManager->recvTargetRect,capFrame,pDataManager->featureInfrared,15,2);

                        trackAlgorithm.ReadCalibrateFile (1, pDataManager->focusZoom);
                    }
                    //and the others
                    else if(pDataManager->videoSrc == 0x02)
                    {
                        trackAlgorithm.MatchAndTrackInit (pDataManager->recvTargetRect,capFrame,pDataManager->featureInfrared,15,2);//2->1

                        trackAlgorithm.ReadCalibrateFile (2, pDataManager->electronicZoom);//2-->1
                    }
                    else
                    {

                    }
                    pDataManager->bIsResetTracking = false;
                }
                else
                {

                    Rect resultRect = trackAlgorithm.KeepTrack (capFrame);
                    rectangle (capFrame, resultRect, Scalar(255,0, 0, 0), 2, 8, 0);

                    Point pointIn = Point(resultRect.x+resultRect.width/2,
                                          resultRect.y+resultRect.height/2);

                    if((pDataManager->videoSrc == 0x01))// && (pDataManager->dayNightReverse == 0x55))
                    {

                        trackAlgorithm.Point2Angle (pointIn, pDataManager->yawAngle_target,
                                                    pDataManager->pitchAngle_target,1);
                    }
                    else// if(pDataManager->videoSrc == 0x02)
                    {

                        trackAlgorithm.Point2Angle (pointIn, pDataManager->yawAngle_target,
                                                    pDataManager->pitchAngle_target,2);
                    }



                    pDataManager->yawAngle_target = pDataManager->yawAngle_target*180/PI;
                    pDataManager->pitchAngle_target = pDataManager->pitchAngle_target*180/PI;

                    if(pDataManager->assistFlag == 0x01)//open
                    {

                        float yaw_pre,pitch_pre;
                        pDataManager->mutex_predictdata.lock ();
                        yaw_pre = pDataManager->yawAngle_predict;
                        pitch_pre = pDataManager->pitchAngle_predict;
                        pDataManager->mutex_predictdata.unlock ();

                        Point pointCross;
                        if((pDataManager->videoSrc == 0x01) && (pDataManager->dayNightReverse == 0x55))//pDataManager->videoSrc == 0x01)
                        {
                            trackAlgorithm.Angle2Point (yaw_pre,pitch_pre, pointCross, 1);
                        }
                        else// if(pDataManager->videoSrc == 0x02)
                        {
                            trackAlgorithm.Angle2Point (yaw_pre,pitch_pre, pointCross, 2);
                        }

                        //========Draw predict point ==============
                        //     |
                        //   --O--
                        //     |
                        //=========================================
                        static Scalar colorTargetTag(250,250,250);
                        {
                            int x = pointCross.x;
                            int y = pointCross.y;

                            line(capFrame,Point(x-40,y) , Point(x-10,y) ,colorTargetTag , 2 ,8 ,0);
                            line(capFrame,Point(x+10,y) , Point(x+40,y) ,colorTargetTag , 2 ,8 ,0);

                            line(capFrame,Point(x,y-40) , Point(x,y-10) ,colorTargetTag , 2 ,8 ,0);
                            line(capFrame,Point(x,y+10) , Point(x,y+40) ,colorTargetTag , 2 ,8 ,0);

                            circle (capFrame,Point(x,y) , 10 ,colorTargetTag , 1 ,LINE_AA ,0);


                        }

                    }

                }
            }
              if(pDataManager->assistFlag == 0x01)
              {
                //Set flag on image center
                int imgCenterX;
                int imgCenterY;

                if(pDataManager->focusZoom>=1 && pDataManager->focusZoom<=30)
                {
                    imgCenterX = (int)pDataManager->cameraMatrix[pDataManager->focusZoom -1].at<double>(0,2);//capFrame.cols / 2;
                    imgCenterY = (int)pDataManager->cameraMatrix[pDataManager->focusZoom -1].at<double>(1,2);//capFrame.rows / 2;
                }
                line(capFrame,Point(imgCenterX-20,imgCenterY) , Point(imgCenterX+20,imgCenterY) ,Scalar(250,250,250), 1 ,8 ,0);
                line(capFrame,Point(imgCenterX,imgCenterY-20) , Point(imgCenterX,imgCenterY+20) ,Scalar(250,250,250) , 1 ,8 ,0);
             }
            //-----------end----------

            resize (capFrame, capFrame, Size(640, 480));

            av_init_packet(&pkt);
            pkt.data = NULL;    // packet data will be allocated by the encoder
            pkt.size = 0;

            convertBGR2YUV420(capFrame,data_y,data_u,data_v);

            pFrame->data[0] = data_y;
            pFrame->data[1] = data_u;
            pFrame->data[2] = data_v;

            pFrame->pts = capFrameCnt;

            ret = avcodec_encode_video2(pCodecCtx, &pkt, pFrame, &got_output); // 1ms
            if (ret < 0) return;
            if (got_output)
            {
    //            qDebug()<<"size:"<<pkt.size<<"\n";
    //            printf("size:%d\n", pkt.size);
    //            printf("Succeed to encode frame:%5d\tsize:%5d\n",framecnt, pkt.size);
    //            framecnt++;
#if 0
                fwrite(pkt.data, 1, pkt.size, fp_out);
#endif
                //                pUdpSendInstance->udpSocket->writeDatagram((char*)pkt.data, pkt.size,
//                                                           QHostAddress(pGlobalSetting->m_str_RemoteAddr_IP),
//                                                           pGlobalSetting->m_uint_RemoteAddr_Port);
/*
                quint32 packetSize = pkt.size;
                quint32 packetNum = packetSize / 2048 + 1;
                QByteArray videoBuf;
                bufVideoSend.resize (9);
                for (quint32 i = 0 ; i < packetNum ; i++)
                {

                    quint32 onePckSize;
                    if(i + 1 <  packetNum) onePckSize = 2048;
                    if(i + 1 == packetNum) onePckSize = packetSize % 2048;

                    //unsigned char* pFrameData = new unsigned char[9+pkt.size];
                    videoBuf.resize (onePckSize);
                    bufVideoSend[0] = 0xEB;
                    bufVideoSend[1] = 0x90;
                    bufVideoSend[2] = 0x22;
                    if(pDataManager->videoSrc == 0x01)
                    {
                        bufVideoSend[3] = 0x01;//0x01:visible  0x02:infrared
                    }
                    else if(pDataManager->videoSrc == 0x02)
                    {
                        bufVideoSend[3] = 0x02;//0x01:visible  0x02:infrared
                    }
                    bufVideoSend[4] = (onePckSize & 0x0000FF00)>>8;
                    bufVideoSend[5] = onePckSize & 0x000000FF;
                    bufVideoSend[6] = packetNum;//0x01;
                    if(capFrameCnt%10 == 0)
                        bufVideoSend[7] = 0x00;
                    else
                        bufVideoSend[7] = 0x01;
                    bufVideoSend[8] = 0x19;//Current frame rate

                    videoBuf.fromRawData ((char*)(pkt.data + i*2048) , onePckSize);

                    bufVideoSend.append (videoBuf);

                    pUdpSendInstance->udpSocket->writeDatagram(bufVideoSend,
                                                               QHostAddress(pGlobalSetting->m_str_RemoteAddr_IP),
                                                               pGlobalSetting->m_uint_RemoteAddr_Port);

                    videoBuf.clear ();
                }
*/

//===========================================================================================
                unsigned char* pFrameData = new unsigned char[9+pkt.size];
                pFrameData[0] = 0xEB;
                pFrameData[1] = 0x90;
                pFrameData[2] = 0x22;
                if(pDataManager->videoSrc == 0x01)
                {
                    pFrameData[3] = 0x01;//0x01:visible  0x02:infrared
                }
                else if(pDataManager->videoSrc == 0x02)
                {
                    pFrameData[3] = 0x02;//0x01:visible  0x02:infrared
                }
                pFrameData[4] = (pkt.size & 0x0000FF00)>>8;
                pFrameData[5] = pkt.size & 0x000000FF;
                pFrameData[6] = 0x01;
                if(capFrameCnt%10 == 0)
                    pFrameData[7] = 0x00;
                else
                    pFrameData[7] = 0x01;
                pFrameData[8] = 0x19;
                memcpy (pFrameData+9, pkt.data, pkt.size);

                pUdpSendInstance->udpSocket->writeDatagram((char*)/*pkt.data*/pFrameData, pkt.size+9,
                                                           QHostAddress(pGlobalSetting->m_str_RemoteAddr_IP),
                                                           pGlobalSetting->m_uint_RemoteAddr_Port);

                if(pFrameData != NULL)
                {
                    delete pFrameData;
                    pFrameData = NULL;
                }

//===============================================================================================
                av_free_packet(&pkt);
            }

            //usleep (5000);

            capFrameCnt++;
        }
        //destroyAllWindows ();

    }


}



void VideoEncodeH264::GlobalInit()
{
    pUdpSendInstance = UdpSendService::GetInstance();

#if 0
    fp_out = fopen(filename_out, "wb");
    if (!fp_out)
    {
        //printf("Could not open %s\n", filename_out);
        return;
    }
#endif
}

int VideoEncodeH264::InitFFmpeg()
{
//    int w = capFrame.cols;
//    int h = capFrame.rows;
    int w = 640;
    int h = 480;

    avcodec_register_all();

    pCodec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!pCodec)
    {
        //printf("Codec not found\n");
        return -1;
    }
    pCodecCtx = avcodec_alloc_context3(pCodec);
    if (!pCodecCtx)
    {
        //printf("Could not allocate video codec context\n");
        return -1;
    }
    pCodecCtx->bit_rate = 400000;
    pCodecCtx->width = w;
    pCodecCtx->height = h;
    pCodecCtx->time_base.num = 1;
    pCodecCtx->time_base.den = 25;
    pCodecCtx->gop_size = 10;
    pCodecCtx->max_b_frames = 1;
    pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;

    av_opt_set(pCodecCtx->priv_data, "preset", "slow", 0);

    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
    {
        //printf("Could not open codec\n");
        return -1;
    }

    pFrame = av_frame_alloc();
    if (!pFrame)
    {
       // printf("Could not allocate video frame\n");
        return -1;
    }
    pFrame->format = pCodecCtx->pix_fmt;
    pFrame->width  = pCodecCtx->width;
    pFrame->height = pCodecCtx->height;

    int ret;
    ret = av_image_alloc(pFrame->data, pFrame->linesize, pCodecCtx->width, pCodecCtx->height,
                         pCodecCtx->pix_fmt, 16);
    if (ret < 0)
    {
        return -1;
    }
#ifdef FLAPPYDEBUG
    qDebug()<<"Init decoder sccessfull";
#endif
    return 0;
}

void VideoEncodeH264::CloseFFmpeg ()
{
    if(pFrame != NULL)
    {
//        av_freep(&pFrame->data[0]);
        av_frame_free (&pFrame);
        pFrame = NULL;
    }
    if(pCodecCtx != NULL)
    {
        avcodec_close(pCodecCtx);
        av_free(pCodecCtx);
        pCodecCtx = NULL;
    }
//    if(pCodec != NULL)
//    {
//        av_free(pCodec);
//        pCodec = NULL;
//    }
}


void VideoEncodeH264::convertBGR2YUV420(Mat in, unsigned char* out_y, unsigned char* out_u, unsigned char* out_v)
{
    int idx_in = 0;
    int idx_out_y = 0;
    int idx_out_u = 0;
    int idx_out_v = 0;

    Mat tmp;
    // first, convert the input image into YCbCr
    tmp = Mat(Size(in.cols, in.rows), in.type());
    cvtColor(in, tmp, CV_RGB2YCrCb);

    quint32 i = 0;
    qint32 j = 0;
    /*  * widthStep = channel number * width
    * if width%4 == 0  * for example, width = 352, width%4 == 0, widthStep = 3 * 352 = 1056 */ ;
    for(j = 0; j < in.rows; j+=1)
    {
        idx_in = j * in.step;
        for(i = 0; i < in.step; i+=12)
        {
            // We use the chroma sample here, and put it into the out buffer
            // take the luminance sample
            out_y[idx_out_y] = tmp.data[idx_in + i + 0]; // Y
            idx_out_y++;
            out_y[idx_out_y] = tmp.data[idx_in + i + 3]; // Y
            idx_out_y++;
            out_y[idx_out_y] = tmp.data[idx_in + i + 6]; // Y
            idx_out_y++;
            out_y[idx_out_y] = tmp.data[idx_in + i + 9]; // Y
            idx_out_y++;
            if((j % 2) == 0)
            {
                // take the blue-difference and red-difference chroma components sample
                out_u[idx_out_u++] = tmp.data[idx_in + i + 1]; // Cr U
                out_u[idx_out_u++] = tmp.data[idx_in + i + 7]; // Cr U
                out_v[idx_out_v++] = tmp.data[idx_in + i + 2]; // Cb V
                out_v[idx_out_v++] = tmp.data[idx_in + i + 8]; // Cb V
            }
        }
    }

}
