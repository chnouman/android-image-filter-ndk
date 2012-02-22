#include <jni.h>
#include <time.h>
#include <android/log.h>
#include <android/bitmap.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define  LOG_TAG    "libimageprocessing"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static int rgb_clamp(int value) {
  if(value > 255) {
    return 255;
  }
  if(value < 0) {
    return 0;
  }
  return value;
}

static void brightness(AndroidBitmapInfo* info, void* pixels){
	int xx, yy, red, green, blue, L;
	uint32_t* line;
	float brightness = 1.6;

	for(yy = 0; yy < info->height; yy++){
			line = (uint32_t*)pixels;
			for(xx =0; xx < info->width; xx++){

			  //extract the RGB values from the pixel
				red = (int) ((line[xx] & 0x00FF0000) >> 16);
				green = (int)((line[xx] & 0x0000FF00) >> 8);
				blue = (int) (line[xx] & 0x00000FF );

        //manipulate each value
        red = rgb_clamp((int)(red * brightness));
        green = rgb_clamp((int)(green * brightness));
        blue = rgb_clamp((int)(blue * brightness));

        // set the new pixel back in
        line[xx] =
          ((red << 16) & 0x00FF0000) |
          ((green << 8) & 0x0000FF00) |
          (blue & 0x000000FF);
			}

			pixels = (char*)pixels + info->stride;
		}
}

JNIEXPORT void JNICALL Java_com_example_ImageActivity_brightness(JNIEnv * env, jobject  obj, jobject bitmap)
{

    AndroidBitmapInfo  info;
    int ret;
    void* pixels;

    if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0) {
            LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
            return;
        }
    if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap format is not RGBA_8888 !");
        return;
    }

    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }

    brightness(&info,pixels);

    AndroidBitmap_unlockPixels(env, bitmap);
}