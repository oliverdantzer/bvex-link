import os
from bvex_codec.sample import Sample, WhichPrimitive, PrimitiveData, FileData
from netCDF4 import Dataset
import numpy as np

SAMPLE_STORE_DIR = os.path.abspath("data")


def create_netcdf_file(file_path: str) -> None:
    """Create a new NetCDF file with the appropriate structure for storing primitive data."""
    with Dataset(file_path, 'w', format='NETCDF4') as nc:
        # Create dimensions
        nc.createDimension('time', None)  # Unlimited dimension for time

        # Create variables
        # Double precision float for timestamps
        nc.createVariable('timestamp', 'f8', ('time',))
        # Double precision float for values
        nc.createVariable('value', 'f8', ('time',))

        # Add attributes
        nc.timestamp.units = 'seconds since 1970-01-01 00:00:00'
        nc.timestamp.long_name = 'Unix timestamp'
        nc.value.long_name = 'Primitive value'


def write_to_netcdf(file_path: str, value: float | int | bool, timestamp: float) -> None:
    """Write a primitive value and its timestamp to the NetCDF file."""
    with Dataset(file_path, 'a') as nc:
        # Convert boolean to float if necessary
        if isinstance(value, bool):
            value = float(value)

        # Get the current size of the time dimension
        time_size = len(nc.dimensions['time'])

        # Write the new data
        nc.variables['timestamp'][time_size] = timestamp
        nc.variables['value'][time_size] = value


class SampleStore:
    def __init__(self, sample_store_dir_path: str) -> None:
        if not os.path.exists(sample_store_dir_path):
            os.makedirs(sample_store_dir_path)
        self.sample_store_dir_path = sample_store_dir_path
        self.primitive_files: dict[str, str] = {}

    def store_sample(self, sample: Sample):
        def store_file(sample: Sample):
            assert isinstance(sample.data, FileData)
            metric_dir = os.path.join(
                SAMPLE_STORE_DIR, f"metric-{sample.metadata.metric_id}")
            if not os.path.exists(metric_dir):
                os.makedirs(metric_dir)
            sample_file = os.path.join(
                metric_dir, f"{sample.data.filename}")
            with open(sample_file, "wb") as f:
                f.write(sample.data.data)

        def store_primitive(sample: Sample):
            assert isinstance(sample.data, PrimitiveData)
            assert sample.data.which_primitive in [
                WhichPrimitive.FLOAT, WhichPrimitive.BOOL, WhichPrimitive.INT]
            if sample.metadata.metric_id not in self.primitive_files:
                self.primitive_files[sample.metadata.metric_id] = os.path.join(
                    self.sample_store_dir_path, f"metric-{sample.metadata.metric_id}.nc")
                create_netcdf_file(
                    self.primitive_files[sample.metadata.metric_id])
            sample_file = self.primitive_files[sample.metadata.metric_id]
            value = sample.data.value
            assert isinstance(value, float) or isinstance(
                value, int) or isinstance(value, bool)
            write_to_netcdf(sample_file, value, sample.metadata.timestamp)

        if isinstance(sample.data, FileData):
            store_file(sample)
        elif isinstance(sample.data, PrimitiveData):
            store_primitive(sample)
        else:
            raise ValueError(f"Unknown data type: {type(sample.data)}")
