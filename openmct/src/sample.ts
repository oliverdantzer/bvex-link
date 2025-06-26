export interface Sample {
  metadata: {
    timestamp: number;
    metric_id: string;
    which_data_type: "primitive" | "file";
  };
  data: any;
}
export interface TelemetryPoint {
  timestamp: number;
  id: string;
}

export interface PrimitivePoint extends TelemetryPoint {
  value: any;
}

export interface ImagePoint extends TelemetryPoint {
  url: string;
}

export function sampleToTelemetryPoint(sample: Sample): TelemetryPoint {
  const secondsSinceEpoch = sample.metadata.timestamp;
  const millisecondsSinceEpoch = secondsSinceEpoch * 1000;
  const timestamp = millisecondsSinceEpoch;
  const id = sample.metadata.metric_id;

  if (sample.metadata.which_data_type === "primitive") {
    const point: PrimitivePoint = {
      timestamp,
      value: sample.data.value,
      id,
    };
    return point;
  } else if (sample.metadata.which_data_type === "file") {
    const filename: string = sample.data.filename;
    const extension = filename.split(".").pop();
    const point: ImagePoint = {
      timestamp,
      url: `${timestamp}.${extension}`,
      id,
    };
    return point;
  } else {
    throw new Error(
      `Unexpected sample data type: ${sample.metadata.which_data_type}`
    );
  }
}