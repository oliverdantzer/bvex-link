export interface Sample {
  metadata: {
    timestamp: number;
    metric_id: string;
  };
  data: {
    value: any;
  };
}

export interface TelemetryPoint {
  timestamp: number;
  value: any;
  id: string;
}
