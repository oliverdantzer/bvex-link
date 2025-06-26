import { DomainObject, OpenMCT } from "openmct";
import { PrimitivePoint, Sample, TelemetryPoint, sampleToTelemetryPoint } from "./sample.js";
import { time } from "console";

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
        let start_timestamp = options.start || 0; // posix timestamp
        const end_timestamp = options.end || Date.now(); // posix timestamp
        const start_date = new Date(start_timestamp).toISOString();
        const end_date = new Date(end_timestamp).toISOString();
        var url =
          "history/samples/" +
          domainObject.identifier.key +
          "?start=" +
          start_date +
          "&end=" +
          end_date;
        let response = await fetch(url);
        let data: Sample[] = await response.json();
        let points: TelemetryPoint[] = data.map(sampleToTelemetryPoint);
        console.log("hello", points);

        points.forEach((point) => {
          if (
            point.timestamp < options.start ||
            point.timestamp > options.end
          ) {
            throw new Error(
              `Telemetry point out of range: ${point.timestamp} not in [${options.start}, ${options.end}]`
            );
          }
        });

        return points;
      },
    };

    openmct.telemetry.addProvider(provider);
  };
}
