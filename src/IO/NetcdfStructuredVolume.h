/*
 * A volume object suitable for use by the OpenVKL
 * testing/examples framework. 
 */
#pragma once
// from OpenVKL distro
#include "TestingStructuredVolume.h"
// from Scattering/IO (this directory)
#include "Domain.h"
// std
#include <algorithm>

namespace openvkl {
    namespace testing {
        struct NetcdfStructuredVolume final : public TestingStructuredVolume {
            NetcdfStructuredVolume(const std::string &filename,
                                   const std::string &field);
            void generateVoxels(std::vector<unsigned char> &voxels,
                             std::vector<float> &time,
                             std::vector<uint32_t> &tuvIndex) const override final;
            private:
            std::string filename;
            std::string fieldname;
        };
        // the constructor needs to call the base class constructor
        // and pass data it doesnt have until after the data file is read.
        // so we pass fake data now and reset it later.
        inline NetcdfStructuredVolume::NetcdfStructuredVolume(
                const std::string &filename, 
                const std::string &field) : filename(filename),fieldname(field),
                TestingStructuredVolume(std::string("foo") /* grid type */,
                        vec3i(0) /* dimensions */,
                        vec3f(0.) /* origin */,
                        vec3f(0.1) /* spacing */,
                        TemporalConfig(),
                        VKL_FLOAT)
        {
            Domain dom(filename,fieldname);
            this->getGridOrigin()  = vec3f(dom.origin);
            this->getGridSpacing() = vec3f(dom.spacing);
            this->getGridType()    = std::string("structuredRegular");
            this->dimensions       = vec3i(dom.coords);
        }
        /* this method reads the voxels. It combines the generateVoxels
         * and readStaticFile methods from the RawHFileStructuredVolume.
         */
        inline void NetcdfStructuredVolume::generateVoxels(
                std::vector<unsigned char> &voxels,
                std::vector<float> &time,
                std::vector<uint32_t> &tuvIndex) const 
        {
            const size_t numValues = dimensions.long_product();
            const size_t numBytes  = numValues*sizeOfVKLDataType(voxelType);
            voxels.resize(numBytes);
            time.clear();
            tuvIndex.clear();

        }
    }
}
