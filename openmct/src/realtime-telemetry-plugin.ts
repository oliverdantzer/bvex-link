import { Sample, TelemetryPoint } from "./sample";
import { OpenMCT } from "openmct";

type ListenerCallback = (point: TelemetryPoint) => void;

/**
 * Basic Realtime telemetry plugin using websockets.
 */
export function RealtimeTelemetryPlugin() {
  return function (openmct: OpenMCT) {
    var socket = new WebSocket("/realtime");
    // mapping of metric id to array of callbacks for
    // listeners to the metric
    var listener: Record<string, ListenerCallback[]> = {};

    socket.onmessage = function (event) {
      const sample: Sample = JSON.parse(event.data);
      const key = sample.metadata.metric_id;
      const point: TelemetryPoint = {
        timestamp: sample.metadata.timestamp * 1000,
        value: sample.data.value,
        id: key,
      };
      listener[key].forEach((callback) => {
        callback(point);
      });
    };

    var provider = {
      supportsSubscribe: function (domainObject: any) {
        console.log(
          "supportsSubscribe",
          domainObject.type === "example.telemetry"
        );
        return domainObject.type === "example.telemetry";
      },
      subscribe: function (domainObject: any, callback: (point: any) => void) {
        var key = domainObject.identifier.key;
        // initialize callback array if doesn't exist
        listener[key] = listener[key] || [];

        // add callback to array
        listener[key].push(callback);

        // request subscription from remote realtime
        // telemetry server
        socket.send("subscribe " + key);
        return function unsubscribe() {
          // remove callback from array on unsubscribe
          var index = listener[key].indexOf(callback);
          if (index > -1) {
            listener[key].splice(index, 1);
          }

          // request unsubscribe from remote realtime
          // telemetry server
          socket.send("unsubscribe " + key);
        };
      },
    };

    openmct.telemetry.addProvider(provider);
  };
}
