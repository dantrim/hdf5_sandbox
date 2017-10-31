// top level include for HDF5
#include "H5Cpp.h"
using namespace H5;

// std/stl
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main(int argc, char* argv[])
{
    cout << "main_inspect" << endl;
    if(argc != 2) {
        cout << "error, must provide an argument specifying the file to inspect" << endl;
        return 1;
    }

    string filename = argv[1];

    const H5std_string FILE_NAME(filename.c_str());
    const H5std_string DATASET_NAME("vmm_config");

    typedef struct vmm_config_struct {
        int threshold;
        int tac_slope;
        int peak_time;
        float gain;
        int trims[64];
    } vmm_config_struct;

    try {

        H5File file(FILE_NAME, H5F_ACC_RDWR);
        DataSet dataset = file.openDataSet(DATASET_NAME);

        DataType dset_type = dataset.getDataType();
        DataType compound_type(H5T_COMPOUND);

        cout << "dest idt type = " << dset_type.getId() << endl;
        cout << "compoudn tyep = " << compound_type.getId() << endl;

        if(dset_type.getId() == compound_type.getId()) {
            cout << "FOUND COMPOUND DATATYPE IN FILE: " << filename << endl;
        }
        

    } // try block
    catch(...) {
        cout << "woops, something went wrong" << endl;
        return 1;
    }

    return 0;
}
