/*
 *
 */
#include "Domain.h"
#include "openvkl/VKLFilter.h"
#include "rkcommon/utility/SaveImage.h"
#include "rkcommon/containers/AlignedVector.h"
#include "rkcommon/math/box.h"
#ifdef WITH_NETCDF
#include "NetcdfStructuredVolume.h"
#endif
#include <openvkl/openvkl.h>
#if defined(_MSC_VER)
#include <malloc.h>
#include <windows.h>
#endif
using FrameBuffer = rkcommon::containers::AlignedVector<vec3f>;
using ColorChannel = rkcommon::containers::AlignedVector<float>;
extern VKLVolume DomainToVolume(const Domain&,VKLDevice device);
struct Ray
{
	vec3f org;
	vec3f dir;
	range1f t;
};
VKLDevice init_vkl() {
    vklLoadModule("cpu_device");
    VKLDevice device = vklNewDevice("cpu");
    vklDeviceSetInt(device,"logLevel",VKL_LOG_INFO);
    vklDeviceSetString(device,"logOutput","cout");
    vklCommitDevice(device);
    return device;
}
//
// A little test code to validate the IO bits. 
// Open a particular file and create a Domain object.
//
int main(int argc, char **argv) {
    VKLDevice dev = init_vkl();
    // the file spec
    std::string directory("C:\\Users\\Dave Semeraro\\Documents\\VolumeRendering\\Data\\");
    std::string filename("cloud.bov");
    std::string filespec = directory+filename;
    std::string fieldname("qi");
    std::cout << "Opening " + filespec + " " << std::endl;
    Domain Storm(filespec,fieldname);
    VKLVolume StormVolume = DomainToVolume(Storm,dev);
#ifdef WITH_NETCDF
    openvkl::testing::NetcdfStructuredVolume volume(filespec,fieldname);
#endif
    VKLSampler sampler = vklNewSampler(StormVolume);
    vklSetInt(sampler,"filter",VKL_FILTER_TRICUBIC);
    vklSetInt(sampler,"gradientFilter",VKL_FILTER_TRICUBIC);
    vklCommit(sampler);
    vkl_box3f bbox = vklGetBoundingBox(StormVolume);
    std::cout << "\tBoundingBox\n" << std::endl;
    std::cout << "\t\tlower = " << bbox.lower.x << " " << bbox.lower.y << std::endl;
    std::cout << "\t\tupper = " << bbox.upper.x << " " << bbox.upper.y << std::endl;
    unsigned int numatts = vklGetNumAttributes(StormVolume);
    std::cout << "\tnum attributes = " << numatts << std::endl;
    vkl_range1f valueRange;
    for(int i = 0;i<numatts;i++) {
        valueRange = vklGetValueRange(StormVolume,i);
        std::cout << i << " " << valueRange.lower << " " << valueRange.upper << std::endl;
    }
    int attributeIndex = 0;
    int numValues = 1;
    float values[1];
    //values[1] = 65000.0;
    values[0] = 0.5;
    //values[0] = 0.4;
    VKLData valuesData = vklNewData(dev,
				numValues,
				VKL_FLOAT,
				values,
				VKL_DATA_DEFAULT,0);

    VKLHitIteratorContext hitContext = vklNewHitIteratorContext(sampler);
    vklSetInt(hitContext,"attributeIndex",attributeIndex);
    vklSetData(hitContext, "values", valuesData);
    vklRelease(valuesData);
    vklCommit(hitContext);
    // framebuffer and friends
    vec2i imagesize={2000,2000};
    auto numPixels = imagesize.x*imagesize.y;
    FrameBuffer framebuffer;
    framebuffer.resize(numPixels);
    ColorChannel accum_r;
    ColorChannel accum_g;
    ColorChannel accum_b;
    accum_r.resize(numPixels);
    accum_g.resize(numPixels);
    accum_b.resize(numPixels);
    std::fill(accum_r.begin(),accum_r.end(),0.f);
    std::fill(accum_g.begin(),accum_g.end(),0.f);
    std::fill(accum_b.begin(),accum_b.end(),0.f);
    std::vector<Ray> rayvector(numPixels);
    float tanfov = tan(40.0*M_PI/180.0);
    // load rays starting with the top left corner, moving right and down
    for(int rayindex = 0;rayindex < numPixels;rayindex++){
	    Ray ray;
	    ray.t = range1f(2000.f,4500.f);
	    ray.org = vec3f(1250.0,1500.0,3400.0);
	    auto pixel = vec2i(rayindex % imagesize.x,rayindex/imagesize.y);
	    float u = pixel.x/float(imagesize.x-1); // 0 < u <1
	    float v = pixel.y/float(imagesize.y-1);
	    float x = tanfov*(u - 0.5);
	    float y = tanfov*(v - 0.5);
	    float z = -1.f;
	    ray.dir = normalize(vec3f(x,y,z));
        //std::cout << pixel << " " << vec2f(u,v) << " " << vec3f(x,y,z) << " " << ray.org << std::endl;
        //std::cout << ray.dir.x << ' ' << ray.dir.y << ' ' << ray.dir.z << std::endl;
	    rayvector[rayindex] = ray;
        //std::cout <<rayvector[rayindex].dir.x << std::endl;
    }
    {
#if defined(_MSC_VER)
        char *buffer = (char*)_malloca(vklGetHitIteratorSize(hitContext));
        char *sbuffer = (char*)_malloca(vklGetHitIteratorSize(hitContext));
#else
        char buffer[vklGetHitIteratorSize(hitContext)];
	    char sbuffer[vklGetHitIteratorSize(hitContext)];
#endif
	    vkl_range1f rayTRange;
        // direction light is pointing
	    const vec3f lightDir[] = {normalize(vec3f(1.f,0.f,1.f)),
	    			normalize(vec3f(10.f,0.f, 10.f))};
	    VKLHit hit;
	    const float surfaceAlpha = 0.99f;
	    const float emission[] = {1.0f,0.0f}; // light emission from light sources
	    auto ray = rayvector.begin();
        int pixelindex = 0;
        while (ray != rayvector.end()){
		    vkl_vec3f *rayOrigin = (vkl_vec3f*)&(*ray).org;
		    vkl_vec3f *rayDirection = (vkl_vec3f *)&(*ray).dir;
		    rayTRange.lower = (*ray).t.lower;
		    rayTRange.upper = (*ray).t.upper;
            //std::cout <<rayDirection->x << ' ' <<rayDirection->y<<' '<<rayDirection->z<<std::endl;
		    VKLHitIterator hitIterator = vklInitHitIterator(hitContext,
			        rayOrigin, rayDirection, &rayTRange,0.f,buffer);
	        vec3f color(0.f);
	        float alpha = 0.f;
            //while(vklIterateHit(hitIterator,&hit) && alpha < 0.99f) {
            int numhits = 0;
            //std::cout << "pixel " << pixelindex << std::endl;
            if(vklIterateHit(hitIterator,&hit)){// if I get a single hit along this ray
                numhits++;
	    	    const vec3f c = (*ray).org + hit.t * (*ray).dir;
	    	    const vkl_vec3f grad = vklComputeGradient(
	    	    	sampler,(vkl_vec3f *)&c, 0,0);
	    	    vec3f N = normalize(vec3f(grad.x,grad.y,grad.z));
	    	    if (std::isnan(N.x) || std::isnan(N.y) || std::isnan(N.z))
	    	    	N = vec3f(0.f);
	    	    const vec3f wi = normalize(-1.f*(*ray).dir); // direction from hit to eye point
	    	    const float ci = dot(N,wi); // cos angle between surface normal and eye direction
	    	    float illum = 0.f;
	    	    if(length(N) > 0) {
	    	    	//illum=0.1f;
                    for(int i=0;i<1;i++) {
	    			    const vec3f wo = lightDir[i]; // light direction
	    			    const float co = dot(N,wo); // cos angle between surface normal and light direction
	    			    illum += abs(co)*emission[i];
                        //if((co>0)==(ci>0)){ // test for shadow
	    			    //	//std::cout << "shadow test" << std::endl;
	    			    //	VKLHit shadowHit;
	    			    //	vkl_range1f tShadowRange;
	    			    //	tShadowRange.lower = hit.epsilon;
	    			    //	tShadowRange.upper = inf;
	    			    //	VKLHitIterator shadowIterator =
	    			    //	vklInitHitIterator(hitContext,
	    			    //	    (vkl_vec3f *)&c,(vkl_vec3f *)&wo,
	    			    //	    &tShadowRange,0,sbuffer);
	    			    //	if(vklIterateHit(shadowIterator,&shadowHit)) { 
	    			    //	    illum += 0;
	    			    //	}
	    			    //}
	    		    }
                } else {// no normal for some reason
	    	    	illum = 1.f;
	    	    }
	    	    vec4f surfaceColorAndOpacity = {0.5f,0.5f,0.5f,1.0f};
	    	    const vec3f albedo = vec3f(surfaceColorAndOpacity);
	    	    color = color +(1-alpha)*illum*albedo;
	    	    //alpha = alpha + (1.f - alpha)*surfaceAlpha;
                //std::cout << "Hit: " << numhits << " " << hit.sample << " " << hit.t << " " <<
                // hit.epsilon << " " << illum << std::endl;
	    	    //std::cout << color.x << " " << color.y << " " << color.z << " " << alpha << std::endl;
	        }    
            float &ar = accum_r[pixelindex];
            float &ag = accum_g[pixelindex];
            float &ab = accum_b[pixelindex];
            ar += color.x;
            ag += color.y;
            ab += color.z;
            //framebuffer[pixelindex] = vec3f(pow(ar,1.f/2.2f),
            //                            pow(ag,1.f/2.2f),
            //                            pow(ab,1.f/2.2f));
            //framebuffer[pixelindex] = vec3f(ar,ag,ab);
            framebuffer[pixelindex] = color;
            pixelindex++;
            ray++;
        }
    }
    std::string outfile = "vklout.pfm";
    rkcommon::utility::writePFM(outfile,imagesize.x,imagesize.y,framebuffer.data());
    std::cout << "Done" << std::endl;
}   
