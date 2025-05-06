import { CompositionProvider, DomainObject, OpenMCT, Identifier } from "openmct";
async function getDictionary() {
  return await fetch("/dictionary.json").then(async (result) => {
    return await result.json();
  });
}

var objectProvider = {
  get: async (
    identifier: any
  ): Promise<DomainObject> => {
    return getDictionary().then(function (dictionary) {
      if (identifier.key === "spacecraft") {
        return {
          identifier: identifier,
          name: dictionary.name,
          type: "folder",
          location: "ROOT",
        };
      } else {
        var measurement = dictionary.measurements.filter(function (m: any) {
          return m.key === identifier.key;
        })[0];
        return {
          identifier: identifier,
          name: measurement.name,
          type: "example.telemetry",
          telemetry: {
            values: measurement.values,
          },
          location: "example.taxonomy:spacecraft",
        };
      }
    });
  },
};

const compositionProvider = {
  appliesTo: function (domainObject: DomainObject): boolean {
    return (
      domainObject.identifier.namespace === "example.taxonomy" &&
      domainObject.type === "folder"
    );
  },
  load: async (
    domainObject: any
  ): Promise<Identifier[]> => {
    return getDictionary().then(function (dictionary) {
      return dictionary.measurements.map(function (m: any) {
        return {
          namespace: "example.taxonomy",
          key: m.key,
        };
      });
    });
  },
};

export const DictionaryPlugin = function (openmct: OpenMCT) {
  return function install(openmct: OpenMCT) {
    openmct.objects.addRoot({
      namespace: "example.taxonomy",
      key: "spacecraft",
    }, 0);

    openmct.objects.addProvider("example.taxonomy", objectProvider);

    openmct.composition.addProvider(compositionProvider as CompositionProvider);

    openmct.types.addType("example.telemetry", {
      label: "Example Telemetry Point",
        description: "Example telemetry point from our happy tutorial.",
      cssClass: "icon-telemetry",
    } as any);
  };
};
