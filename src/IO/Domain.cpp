#include "Domain.h"
struct BOV {
    int nx,ny,nz;
    int bx,by,bz;
    string rawfilename;
    string variable;
    string endian;
    string format;
    bool divide;
    float time;
    void* thedata;
    
};

BOV read_bov( string filename) {
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
    BOV filedata;
    std::ifstream bovfile(filename);
    std::string line,tmp;
    std::vector<std::string> items;
    // read the header. 
    while(std::getline(bovfile,line)) {
        std::stringstream ss(line);
        // parse out the line
         while(std::getline(ss,tmp,' ')) {
            items.push_back(tmp);
        }
        // do stuff based on first item
        std::cout << " items[0] " << items[0] << " " << items.capacity() << std::endl;
        switch(string_to_item.at(items[0])) {
            case 1:
            filedata.rawfilename=items[1];
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
    // read the data itself
    // for now just read some floats 
    // adjust for datatype later
    std::cout << "filedata " << filedata.nx << " " << filedata.ny << std::endl;
    int count = filedata.nx*filedata.ny*filedata.nz;
    int bytes = count*sizeof(float);
    std::cout << "allocating " << count << " floats" << std::endl;
    float *rawdata = new float[count];
    string filetoopen = "C:\\Users\\Dave Semeraro\\Documents\\VolumeRendering\\Data\\1000.float256";
    std::cout << "open " << filetoopen << std::endl;
    std::ifstream rawfile(filetoopen, ios::in | ios::binary);
    std::ifstream input;
    input.open(filetoopen,ios::in|ios::binary);
    std::cout << " input good is " << input.good() << std::endl;
    if(input.good())
        input.read((char*)rawdata,sizeof(float)*count);
    input.close();
    std::cout << "sample " << rawdata[300] << std::endl;
    //streampos src = 0;
    //rawfile.seekg(src,ios_base::beg);
    //rawfile.read((char *)rawdata,bytes);
    //filedata.thedata = (void *)rawdata;
    return filedata;
}
Domain::Domain(string filename, string fieldname) {
    LoadData(filename,fieldname);
    
}
int Domain::LoadData(string filename,string fieldname) {
    cout<<" loading data from file" << filename << endl;
    fnext = filename.substr(filename.find_last_of(".")+1);
    if(fnext == "bov") {// load a raw single var file. ignore input fieldname
        cout<< "bov file: " << filename << endl;
        BOV filedata = read_bov(filename);
        }
    
#ifdef WITH_NETCDF
    NcFile dataFile(filename.c_str(),NcFile::ReadOnly);
    if(!dataFile.is_valid()) {
        cout<< " bad file open " << filename << endl;
        exit(0);
    }
    NcDim *XH = dataFile.get_dim("xh");
    NcDim *YH = dataFile.get_dim("yh");
    NcDim *ZH = dataFile.get_dim("zh");
    long nx = XH->size();
    long ny = YH->size();
    long nz = ZH->size();
    long nt = 1;
    cout << "Read the data" << endl;
    float xh[nx];
    float yh[ny];
    float zh[nz];
    NcVar *x = dataFile.get_var("xh");
    x->get(xh,nx);
    NcVar *y = dataFile.get_var("yh");
    y->get(yh,ny);
    NcVar *z = dataFile.get_var("zh");
    z->get(zh,nz);
    origin = vec3f(xh[0],yh[0],zh[0]);
    // asume constant spacing
    float spx = xh[1]-xh[0];
    float spy = yh[1]-yh[0];
    float spz = zh[1]-zh[0];
    npts = nx*ny*nz;
    spacing = vec3f(spx,spy,spz);
    coords = vec3i(nx,ny,nz);
    variable = new float[npts];
    cerr << " reading field " << fieldname.c_str() << endl;
    NcVar *data = dataFile.get_var(fieldname.c_str());
    long numvals = data->num_vals();
    int numdims = data->num_dims();
    cerr << "numvals " << numvals << " numdims " << numdims << endl;
    data->get(variable,1,nz,ny,nx);
    return npts;
#endif
    return 0;
}