#include "MutliFrameDenoise.h"
#include <android/log.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include "include/format.h"
#include "log.h"
#include "include/PerspectiveAdd.h"

using namespace std;
using namespace android;

static double work_begin = 0;
static double work_end = 0;
static double gTime = 0;

static void workBegin()
{
    work_begin = getTickCount();
}

static void workEnd()
{
    work_end = getTickCount() - work_begin;
    gTime = work_end /((double)getTickFrequency() )* 1000.0;
    LOGE("TIME = %lf ms \n",gTime);
}

static PerspectiveAdd* g_APUnit;
static vector <Mat> g_picVec;
static vector <Mat> g_grayVec;

MutliFrameDenoise::MutliFrameDenoise():initialized(false)
{
}

MutliFrameDenoise::~MutliFrameDenoise()
{
	initialized = false;

}


void MutliFrameDenoise::getImageUnderDir(char *path, char *suffix)
{
    Mat outRGB,outYUV;
    struct dirent* ent = NULL;
    DIR *pDir;
    char dir[512];
    char tSuffix[8];
    struct stat statbuf;

    g_picVec.clear();
    g_grayVec.clear();

    if((pDir = opendir(path)) == NULL )
    {
        LOGE("getFileList:Cannot open directory:%s\n", path);
        return;
    }
    while((ent = readdir(pDir)) != NULL )
    {
        snprintf(dir, 512,"%s/%s", path, ent->d_name );
        lstat(dir, &statbuf);

        if(S_ISDIR(statbuf.st_mode) )
        {
            if(strcmp(".",ent->d_name) == 0 || strcmp("..",ent->d_name) == 0)
                continue;
        } else {
            if(strcmp(suffix,ent->d_name + strlen(ent->d_name) - strlen(suffix)) != 0)
                continue;
            Mat bayer,yuv,rgb,yv12;
            bayer = imread(dir,0);
            cvtColor(bayer, rgb, CV_BayerBG2BGR);
            cvtColor(rgb, yv12, COLOR_RGB2YUV_YV12);
            cvtColor(rgb, yuv, COLOR_BGR2YUV);
            //LOGE("yuv.width = %d,yuv.height = %d",yuv.size().width,yuv.size().height);
            //LOGE("yuv.channels = %d",yuv.channels());
            g_picVec.push_back(yv12);

            vector<Mat> YUVchanel;
            YUVchanel.clear();
            split(yuv, YUVchanel);
            g_grayVec.push_back(YUVchanel[0]);
            LOGE("YUVchanel[0].width = %d,YUVchanel[0].heigh = %d",YUVchanel[0].cols,YUVchanel[0].rows);
            //LOGE("YUVchanel[0].channels = %d",YUVchanel[0].channels());
        }
    }
    closedir(pDir);
}

void  MutliFrameDenoise::initOpenGLES(alloc_device_t *m_alloc_dev, int width, int height)
{
    //getImageUnderDir("/data/isptune","pgm");
    g_APUnit = new PerspectiveAdd();
    g_APUnit->initOpenGLES(m_alloc_dev, width, height);

	initialized = true;
}

void  MutliFrameDenoise::updateImageData(struct cv_fimc_buffer *m_buffers_capture) {
	g_APUnit->updateImageData(m_buffers_capture);
}


long  MutliFrameDenoise::processing(int* targetAddr, int mode)
{
	LOGD("MutliFrameDenoise::processing...");
	if (!initialized) {
		LOGE("processing failed - check init first.");
		return NULL;
	}

	workBegin();
    Mat outMat;
    int HomMethod = RANSAC; //RHO RANSAC LMEDS

    g_APUnit->setMode(HomMethod);
    g_APUnit->Progress(outMat, targetAddr, mode);

    workEnd();
    //return (long)imgData;
    return 0;

}

int MutliFrameDenoise::getResult(int targetAddr) {
	g_APUnit->getResult(targetAddr);
}

void MutliFrameDenoise::destroy() {
	delete g_APUnit;
	initialized = false;
}
