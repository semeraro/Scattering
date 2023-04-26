#include "Domain.h"
struct BOV {
    int nx,ny,nz;
    string rawfilename;
    string variable;
    string endian;
};
BOV read_bov( string filename) {
    
}
Domain::Domain(string filename, string fieldname) {
    LoadData(filename,fieldname);
    
}
int Domain::LoadData(string filename,string fieldname) {
    cout<<" loading data from file" << filename << endl;
    fnext = filename.substr(filename.find_last_of(".")+1);
    if(fnext == "bov") cout<< "bov file" << endl;

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