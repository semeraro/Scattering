#include "Domain.h"

BOV_METADATA read_bov_metadata( string filename) {
    const static std::unordered_map<std::string,int> string_to_item{
    {"DATA_FILE:",1},
    {"DATA_SIZE:",2},
    {"DATA_FORMAT:",3},
    {"VARIABLE:",4},
    {"DATA_ENDIAN:",5},
    {"DIVIDE_BRICK:",6},
    {"DATA_BRICKLETS:",7},
    {"TIME:",8}
    };
    BOV_METADATA filedata;
    std::ifstream bovfile(filename);
    std::string line,tmp;
    std::vector<std::string> items;
    // read the header. thow if ther is a file problem
    if(!bovfile) {
        throw std::runtime_error("failed to open: "+filename);
        return filedata;
    }
    while(std::getline(bovfile,line)) {
        std::stringstream ss(line);
        // parse out the line
         while(std::getline(ss,tmp,' ')) {
            items.push_back(tmp);
        }
        // do stuff based on first item
        //std::cout << " items[0] " << items[0] << " " << string_to_item.at(items[0]) << std::endl;
        switch(string_to_item.at(items[0])) {
            case 1:
            filedata.rawfilename=std::string(items[1]);
            break;
            case 2:
            filedata.nx = stoi(items[1]);
            filedata.ny = stoi(items[2]);
            filedata.nz = stoi(items[3]);
            break;
            case 3:
            filedata.format = items[1];
            break;
            case 4:
            filedata.variable = items[1];
            break;
            case 5:
            filedata.endian = items[1];
            break;
            case 6:
            transform(items[1].begin(),items[1].end(),items[1].begin(),::toupper);
            items[1] == "TRUE" ? filedata.divide = TRUE : filedata.divide = FALSE;
            break;
            case 7:
            filedata.bx = stoi(items[1]);
            filedata.by = stoi(items[2]);
            filedata.bz = stoi(items[3]);
            break;
            case 8:
            filedata.time = stof(items[1]);
            break;
        }
        items.clear();
    }
    return filedata;
}
    // read the data itself
    // for now just read some floats 
    // adjust for datatype later
int read_raw_data(BOV_METADATA &filedata) {
//#define sphere 
#ifdef sphere
// do sphere

    int count = filedata.nx*filedata.ny*filedata.nz;
    float *dat = new float[count];
    vec3f center = {0.5,0.5,0.5};
    float one = 1.0;
    vec3f spc = {one/filedata.nx,one/filedata.ny,one/filedata.nz};
    for(int k=0;k<filedata.nz;k++)
        for(int j=0;j<filedata.ny;j++)
            for(int i=0;i<filedata.nx;i++) {
                vec3i idx = {i,j,k};
                vec3f dist = idx*spc - center;
                int index = i + j*filedata.nx + k*(filedata.nx*filedata.ny);
                dat[index] = length(dist);
            }
    filedata.thedata = (void *)dat;

#endif
#ifndef sphere
    unsigned long count = filedata.nx*filedata.ny*filedata.nz;
    unsigned long long bytes = count*sizeof(float);
    
    std::ifstream rawfile(filedata.rawfilename, ios::in | ios::binary);
    if(!rawfile) {
        throw std::runtime_error("failed to open: "+filedata.rawfilename);
        return 0;
    } else {
        float *rawdata = new float[count];
        streampos src = 0;
        rawfile.seekg(src,ios_base::beg);
        rawfile.read((char *)rawdata,bytes);
        if(rawfile) {
            filedata.thedata = (void *)rawdata;
            return 1;
        }
        else {
            throw std::runtime_error("error reading raw data...");
            return 0;
        }
    }
#endif
}
Domain::Domain(string filename, string fieldname, bool metadataonly) {
    LoadData(filename,fieldname,metadataonly);
    
}
int Domain::LoadData(string filename,string fieldname,bool metadataonly) {
    cout<<" loading data from file" << filename << endl;
    fnext = filename.substr(filename.find_last_of(".")+1);
    if(fnext == "bov") {// load a raw single var file. ignore input fieldname
        cout<< "bov file: " << filename << endl;
        BOV_METADATA filedata = read_bov_metadata(filename);
        // fill in the domain data
        coords = {filedata.nx,filedata.ny,filedata.nz};
        origin = {0.,0.,0.};
        // unit cube 
        spacing = {1.,1.,1.};
        //spacing = spacing/(coords-1);
        if(!metadataonly) {
            if(read_raw_data(filedata)) {
                variable = (float*)filedata.thedata;
                npts = coords.product();
                return 1;
            } else {
                throw std::runtime_error(" error reading raw file ");
                return 0;
            }
        }
    }
    return 1;
}