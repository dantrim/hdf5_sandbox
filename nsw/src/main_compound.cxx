// top level include for HDF5
#include "H5Cpp.h"
using namespace H5;

// std/stl
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <iomanip>
using namespace std;

int main(int argc, char* argv[])
{

    cout << "main_compound" << endl;

    const H5std_string FILE_NAME("compound_test.h5");
    const H5std_string DATASET_NAME("vmm_config");
    const H5std_string MEMBER_THRESHOLD("sdt");
    const H5std_string MEMBER_GAIN("gain");
    const H5std_string MEMBER_TRIMS("channel_trims");
    const H5std_string MEMBER_TAC("tac_slope");
    const H5std_string MEMBER_PEAKTIME("peak_time");

    const H5std_string ATTRIBUTE_TIMESTAMP("creation_time");

    typedef struct vmm_config_struct {
        int threshold;
        int tac_slope;
        int peak_time;
        float gain;
        int trims[64];
        
    } vmm_config_struct;


    //int N_VMMS = 5;
    hsize_t N_VMMS = 5;

    try {

        /////////////////////////////////////////////////
        // dummy data for VMMs
        /////////////////////////////////////////////////
        vmm_config_struct vmm_configs[N_VMMS];
        for(size_t i = 0; i < N_VMMS; i++) {
            vmm_configs[i].threshold = 230; // DAC
            vmm_configs[i].gain = 9.0; // mV/fC
            vmm_configs[i].tac_slope = 350; // ns
            vmm_configs[i].peak_time = 100; // ns
            for(size_t ichan = 0; ichan < 64; ichan++) {
                vmm_configs[i].trims[ichan] = ichan * i;
            }
        } // i


        ////////////////////////////////////////////////
        // write the data
        ////////////////////////////////////////////////
        H5File* file = new H5File( FILE_NAME, H5F_ACC_TRUNC );

        hsize_t dim[1] = {N_VMMS}; 
        DataSpace dataspace( 1, dim ); // first arg is RANK

        // setup map of the struct for HDF5
        hsize_t vmms[1] = {64};
        ArrayType trim_array( PredType::NATIVE_INT, 1, vmms ); 
        CompType vmm_config_type( sizeof(vmm_config_struct) );
        vmm_config_type.insertMember( MEMBER_THRESHOLD, HOFFSET(vmm_config_struct, threshold), PredType::NATIVE_INT );
        vmm_config_type.insertMember( MEMBER_GAIN, HOFFSET(vmm_config_struct, gain), PredType::NATIVE_FLOAT );
        vmm_config_type.insertMember( MEMBER_TRIMS, HOFFSET(vmm_config_struct, trims), trim_array );
        vmm_config_type.insertMember( MEMBER_TAC, HOFFSET(vmm_config_struct, tac_slope), PredType::NATIVE_INT );
        vmm_config_type.insertMember( MEMBER_PEAKTIME, HOFFSET(vmm_config_struct, peak_time), PredType::NATIVE_INT );

        DataSet* dataset;
        dataset = new DataSet( file->createDataSet(DATASET_NAME, vmm_config_type, dataspace) );
        dataset->write( vmm_configs, vmm_config_type );

        ////////////////////////////////////////////////
        // store timestamp as attribute
        ////////////////////////////////////////////////
        stringstream timestr; 
        std::time_t time_now = std::time(nullptr);
        timestr << std::put_time(std::localtime(&time_now), "%y-%m-%d %OH:%OM:%OS");
        cout << "TIMESTR = " << timestr.str() << endl;
        H5std_string attribute_timestamp_data ( timestr.str() );
        StrType timestamp_type(H5T_STRING, 256);
        Attribute attribute_timestamp = dataset->createAttribute( ATTRIBUTE_TIMESTAMP, timestamp_type, H5S_SCALAR);
        attribute_timestamp.write( timestamp_type, attribute_timestamp_data );

        delete dataset;
        delete file;

    } // try block
    catch(...) {
        cout << "Whoops there was an error" << endl;
        return 1;
    } // catch

    


    return 0;
}
