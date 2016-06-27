//
// Created by linqi on 16-4-5.
//

#ifndef MY_JNI_PERSPECTIVEADD_H
#define MY_JNI_PERSPECTIVEADD_H



#include "opencv2/opencv.hpp"
#include <android/log.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#include <stdio.h>
#include <stdlib.h>
#include <ui/GraphicBuffer.h>
#include <ui/PixelFormat.h>
#include "log.h"
#include "include/MyStruct.h"
#include "include/MutGetHomography.h"
#include <MutliFrameDenoise.h>
#include <hardware/gralloc.h>


using namespace android;
using namespace mystruct;
using namespace cv;

using android::GraphicBuffer;
using android::sp;


const char gPerspectiveVertexShader[] =
        "attribute vec4 a_position;\n"
                "uniform mat3 uMVPMatrix1;\n"
                "uniform mat3 uMVPMatrix2;\n"
                "uniform mat3 uMVPMatrix3;\n"
                "uniform mat3 uMVPMatrix4;\n"
                "uniform mat3 uMVPMatrix5;\n"
                "uniform mat3 uMVPMatrix6;\n"
                "uniform vec2 textureSize;\n"
                "attribute vec2 a_texCoord;\n"
                "varying vec2 v_texCoord1;\n"
                "varying vec2 v_texCoord2;\n"
                "varying vec2 v_texCoord3;\n"
                "varying vec2 v_texCoord4;\n"
                "varying vec2 v_texCoord5;\n"
                "varying vec2 v_texCoord6;\n"
                "varying vec2 texCoord;\n"
                "void main() {\n"
                "  texCoord = a_texCoord;\n"
                "  float width = textureSize.x;\n"
                "  float height = textureSize.y;\n"
                "  vec3 sPoint = vec3(a_texCoord.x*width,a_texCoord.y*height,1.0);\n"
                "  vec3 dPoint = uMVPMatrix1*sPoint;\n"
                "  float X = dPoint.x/(dPoint.z*width);\n"
                "  float Y = dPoint.y/(dPoint.z*height);\n"
                "  v_texCoord1 = vec2(X,Y);\n"
                "  dPoint = uMVPMatrix2*sPoint;\n"
                "  X = dPoint.x/(dPoint.z*width);\n"
                "  Y = dPoint.y/(dPoint.z*height);\n"
                "  v_texCoord2 = vec2(X,Y);\n"
                "  dPoint = uMVPMatrix3*sPoint;\n"
                "  X = dPoint.x/(dPoint.z*width);\n"
                "  Y = dPoint.y/(dPoint.z*height);\n"
                "  v_texCoord3 = vec2(X,Y);\n"
                "  dPoint = uMVPMatrix4*sPoint;\n"
                "  X = dPoint.x/(dPoint.z*width);\n"
                "  Y = dPoint.y/(dPoint.z*height);\n"
                "  v_texCoord4 = vec2(X,Y);\n"
                "  dPoint = uMVPMatrix5*sPoint;\n"
                "  X = dPoint.x/(dPoint.z*width);\n"
                "  Y = dPoint.y/(dPoint.z*height);\n"
                "  v_texCoord5 = vec2(X,Y);\n"
                "  dPoint = uMVPMatrix6*sPoint;\n"
                "  X = dPoint.x/(dPoint.z*width);\n"
                "  Y = dPoint.y/(dPoint.z*height);\n"
                "  v_texCoord6 = vec2(X,Y);\n"
                "  gl_Position = a_position;\n"
                "}\n";

const char gPerspectiveFragmentShader[] =
        "#extension GL_OES_EGL_image_external : require\n"
        "precision highp float;\n"
                "varying vec2 v_texCoord1;\n"
                "varying vec2 v_texCoord2;\n"
                "varying vec2 v_texCoord3;\n"
                "varying vec2 v_texCoord4;\n"
                "varying vec2 v_texCoord5;\n"
                "varying vec2 v_texCoord6;\n"
                "varying vec2 texCoord;\n"
                "uniform samplerExternalOES u_samplerTexture1;\n"
                "uniform samplerExternalOES u_samplerTexture2;\n"
                "uniform samplerExternalOES u_samplerTexture3;\n"
                "uniform samplerExternalOES u_samplerTexture4;\n"
                "uniform samplerExternalOES u_samplerTexture5;\n"
                "uniform samplerExternalOES u_samplerTexture6;\n"
                
                " float median(float r1, float r2, float r3, float r4, float r5, float r6) {\n"
                "   float max12 = max(r1, r2);\n"
                "   float max34 = max(r3, r4);\n"
                "   float max56 = max(r5, r6);\n"
                "   float min12 = min(r1, r2);\n"
                "   float min34 = min(r3, r4);\n"
                "   float min56 = min(r5, r6);\n"
                "   float maxmin = max(max(min12, min34), min56);\n"
                "   float minmax = min(min(max12, max34), max56);\n"
                "   return (maxmin+minmax) / 2.0; \n"
                " }\n"

                "void main() {\n"

                "  gl_FragColor = texture2D(u_samplerTexture1,v_texCoord1)*0.167;\n"
                "  gl_FragColor += texture2D(u_samplerTexture2,v_texCoord2)*0.167;\n"
                "  gl_FragColor += texture2D(u_samplerTexture3,v_texCoord3)*0.167;\n"
                "  gl_FragColor += texture2D(u_samplerTexture4,v_texCoord4)*0.167;\n"
                "  gl_FragColor += texture2D(u_samplerTexture5,v_texCoord5)*0.167;\n"
                "  gl_FragColor += texture2D(u_samplerTexture6,v_texCoord6)*0.167;\n"

/*
                "  vec4 color1, color2, color3, color4, color5, color6;"
                "  color1 = texture2D(u_samplerTexture1,v_texCoord1);\n"
                "  color2 = texture2D(u_samplerTexture2,v_texCoord2);\n"
                "  color3 = texture2D(u_samplerTexture3,v_texCoord3);\n"
                "  color4 = texture2D(u_samplerTexture4,v_texCoord4);\n"
                "  color5 = texture2D(u_samplerTexture5,v_texCoord5);\n"
                "  color6 = texture2D(u_samplerTexture6,v_texCoord6);\n"
                
                //"  float R, G, B;\n"
                //"  R = median(color1.r, color2.r, color3.r, color4.r, color5.r, color6.r);"
                //"  G = median(color1.g, color2.g, color3.g, color4.g, color5.g, color6.g);"
                //"  B = median(color1.b, color2.b, color3.b, color4.b, color5.b, color6.b);"
                "  float cof = 1.0 / 2.0;\n"
                "  gl_FragColor = color4*cof + color3*cof;\n"
                //"  gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
*/

                "}\n";

class PerspectiveAdd
{
public:
    PerspectiveAdd();
    ~PerspectiveAdd();

public:
    int initOpenGLES(alloc_device_t *m_alloc_dev, int width, int height);
    int Progress(Mat & _outMat, int* targetAddr, int mode);
    bool setMode(int homoMethod = RANSAC);
	int updateImageData(struct cv_fimc_buffer *m_buffers_capture);
    int perspectiveAndAdd(const vector <fHomography> & HomographyVec, Mat &dstImage, int* targetAddr, int mode);
	int getResult(int targetAddr);
private:
	int mCurrentId;
    vector <Mat> m_images;
    vector <Mat> m_grays;
    double work_begin;
    double work_end;
    unsigned char *gData;
    float mWidth;
    float mHeight;
    int HomoMethod;
    bool checkInitOpenGLES;

    GLuint vPositionHandle;
    GLuint vTexCoordHandle;
    GLuint vHomograyHandle1;
    GLuint vHomograyHandle2;
    GLuint vHomograyHandle3;
    GLuint vHomograyHandle4;
    GLuint vHomograyHandle5;
    GLuint vHomograyHandle6;

    GLuint vSizeHandle;

    GLuint programObject;
    // texture
    GLuint textureID1;
    GLuint textureID2;
    GLuint textureID3;
    GLuint textureID4;
    GLuint textureID5;
    GLuint textureID6;

    EGLContext context;
    EGLDisplay display;
	EGLSurface surface;

    GLuint targetTexId;
    GLuint fboTargetHandle;
    EGLImageKHR mTargetEGLImage;
    sp <GraphicBuffer> mTargetGraphicBuffer;
    char* mTargetGraphicBufferAddr;
	buffer_handle_t mTargetBufferHandle;

	class CameraGLTexImage {
	public:
		int mId;
		int mShareFd;
		EGLDisplay eglDisplay;
		char* mBufferAddr;
		buffer_handle_t mHandle;
	    EGLImageKHR mEGLImage;
	    sp <GraphicBuffer> mGraphicBuffer;

		CameraGLTexImage(int id, EGLDisplay display = NULL, char* bufAddr = NULL, buffer_handle_t handle = NULL, int share_fd = 0);
		~CameraGLTexImage();

		int createTexImage(int width, int height, int format);
		int updateTexImage(int width, int height, int format);
		void clear();
		buffer_handle_t bufferHandleAlloc(uint32_t w, uint32_t h, PixelFormat format, uint32_t usage);
		void bufferHandleFree(buffer_handle_t handle);
	};
	CameraGLTexImage* mCameraGLTexImage[6];// = {NULL};
	GLuint mTextureIds[6];

private:
    int InitEGL();
    int DestroyEGL();
    void checkFBO();
    void workBegin();
    void workEnd(char* module_name = "null");
    GLuint LoadShader( GLenum type, const char *shaderSrc );
    GLuint LoadProgram( const char *vShaderStr, const char *fShaderStr );

    void initializeTmpResEGLImage(alloc_device_t *m_alloc_dev, int fboWidth, int fboHeight, GLuint *tex,
                                  GLuint * fbo, GLuint texGroup);
	int rebindFrameBufferObject(int fboTex, int fboId, long address);
};







#endif //MY_JNI_PERSPECTIVEADD_H
