import { DomainObject, OpenMCT } from "openmct";
import { Sample, TelemetryPoint } from "./sample";

interface RequestOptions {
  filters?: any; // Adjust type if filters have a specific structure
  order: "asc" | "desc";
  size: number;
  enforceSize: boolean;
  timeContext?: any; // Adjust type if timeContext has a specific structure
  domain: string;
  start: number;
  end: number;
  onPartialResponse?: Function; // Adjust type if this is a specific function signature
  signal?: AbortSignal;
}

export function HistoricalTelemetryPlugin() {
  return function install(openmct: OpenMCT) {
    var provider = {
      supportsRequest: function (domainObject: DomainObject): boolean {
        return domainObject.type === "example.telemetry";
      },
      request: async function (
        domainObject: any,
        options: RequestOptions
      ): Promise<TelemetryPoint[]> {
        const start_date = new Date(options.start).toISOString();
        const end_date = new Date(options.end).toISOString();
        var url = "history/samples/" +
          domainObject.identifier.key +
          "?start=" +
          start_date +
          "&end=" +
          end_date;
        let response = await fetch(url);
        let data: Sample[] = await response.json();
        let points: TelemetryPoint[] = data.map(
          function (sample) {
            const secondsSinceEpoch = sample.metadata.timestamp;
            const millisecondsSinceEpoch = secondsSinceEpoch * 1000;
            return {
              timestamp: millisecondsSinceEpoch,
              value: sample.data.value,
              id: sample.metadata.metric_id,
            };
          }
        );
        return points;
      },
    };

    openmct.telemetry.addProvider(provider);
  };
}
