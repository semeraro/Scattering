#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char* argv[])
{
    ifstream input;
    ofstream output;
    const int elements = 256*256*256;
    float *infield = new float[elements];
    //float infield[elements];
    input.open("c:\\Users\\Dave Semeraro\\Documents\\VolumeRendering\\Data\\1000.float256",ios::in|ios::binary);
    if(input.good())
        input.read((char *)infield,sizeof(float)*elements);
        cout << " sample  " << infield[300] << endl;
    input.close();
    return 0;
}